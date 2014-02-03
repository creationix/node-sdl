#!/usr/bin/env python

import os
import sys
import subprocess
import platform

# Create a list of directories to search sdl2-config for.
path = os.environ["PATH"].split(os.path.pathsep)
library_name = [
	"SDL2",
	"SDL2_ttf",
	"SDL2_image"
]
prepend_library = "lib" if os.name == "posix" or os.name == "mac" else ""
system = platform.system()
# posix is the name both on OS X and, assumedly, Linux distributions. So we need
# to check specifically for Darwin from platform.system() in order to determine
# if we need dylibs. Everthing else posix will probably be using a .so.
if os.name == "posix":
	if system == "Darwin":
		append_library = ".dylib"
	else:
		append_library = ".so"
# If it's not posix, it's Windows :)
else:
	append_library = ".dll"
# Actually build the full name of the library. Looks like:
#   libSDL2.[lib | dylib]
#   SDL2.dll
library_name = map(lambda s: prepend_library + s + append_library, library_name)


# Search a list of paths for sdl2-config.
def check_for_config(paths):
	for d in paths:
		if os.path.exists(d):
			files = os.listdir(d)
			for f in files:
				if f == "sdl2-config":
					if "~" in d:
						full_dir = os.path.expanduser(d)
					else:
						full_dir = os.path.abspath(d)
					full_dir = os.path.expandvars(full_dir)
					full_dir = os.path.normpath(full_dir)
					return os.path.join(full_dir, f)
	return None

def find_in(paths, to_find):
	if not isinstance(to_find, (list, tuple)):
		to_find = [to_find]
	ret = []
	for d in paths:
		if os.path.exists(d):
			files = os.listdir(d)
			for f in files:
				if f in to_find:
					if "~" in d:
						full_dir = os.path.expanduser(d)
					else:
						full_dir = os.path.abspath(d)
					full_dir = os.path.expandvars(full_dir)
					full_dir = os.path.normpath(full_dir)
					ret.append(os.path.join(full_dir, f))
	return ret

def strip_includes(flags):
	includeDirs = []
	while "-I" in flags:
		bigEye = flags.find("-I")
		nextEye = flags.find("-I", bigEye + 2)
		if nextEye == -1:
			nextEye = flags.find("-D")
			if nextEye == -1:
				nextEye = len(flags + 1)
		includeDirs.append(flags[bigEye + 2:nextEye - 1])
		flags = flags[nextEye:]
	return includeDirs, flags

if __name__ == "__main__":
	if len(sys.argv) == 1:
		print("Usage: findsdl.py [--prefix] [--includes] [--libs] [--compiler-args]")
		exit(1)
	verbose = False
	for arg in sys.argv[1:]:
		if arg == "--verbose":
			verbose = True
	if verbose:
		print("Checking for sdl-config binary in PATH.")
	location = check_for_config(path)
	if location is not None:
		if verbose:
			print("Found sdl2-config at: %s" % location)
		# Asked for the prefix for finding all the SDL2 stuff such as libraries or includes.
		# Can find specific bits by tacking on "lib" or "include".
		if sys.argv[1] == "--prefix":
			if verbose:
				print("About to run commmand: %s" % (location + " --prefix"))
			prefix, err = subprocess.Popen([location, "--prefix"], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
			if verbose and err:
				print("ERR: " + str(err.strip()))
			print(prefix.strip())
		# Asked specifically for the include directories provided from the config tool.
		# Note that sdl2-config is only coarse-grained to the point of ldflags and cflags, so we
		# need to pull specific bits out of the output. cflags includes both include directories
		# and possibly some defines, so we only grab the include directories here.
		elif sys.argv[1] == "--includes":
			if verbose:
				print("About to run commmand: %s" % (location + " --cflags"))
			cflags, err = subprocess.Popen([location, "--cflags"], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
			if verbose and err:
				print("ERR: " + str(err.strip()))
			cflags = cflags.strip()
			includeDirs, cflags = strip_includes(cflags)
			for d in includeDirs:
				print(d)
		elif sys.argv[1] == "--compiler-args":
			if verbose:
				print("About to run commmand: %s" % (location + " --cflags"))
			cflags, err = subprocess.Popen([location, "--cflags"], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
			if verbose and err:
				print("ERR: " + str(err.strip()))
			cflags = cflags.strip()
			_, cflags = strip_includes(cflags)
			compilerArgs = map(lambda s: s[2:], cflags.split())
			for arg in compilerArgs:
				print(arg)
		elif sys.argv[1] == "--libs":
			if verbose:
				print("About to run commmand: %s" % (location + " --libs"))
			ldflags, err = subprocess.Popen([location, "--libs"], stdout=subprocess.PIPE, stderr=subprocess.PIPE).communicate()
			if verbose and err:
				print("ERR: " + str(err))
			bigEl = ldflags.find("-L")
			littleEl = ldflags.find("-l")
			libLoc = ldflags[bigEl + 2:littleEl - 1]
			for lib in library_name:
				full = os.path.join(libLoc, lib)
				if os.path.exists(full):
					print(os.path.join(libLoc, lib))
	else:
		if verbose:
			print("Could not find sdl-config. Looking for libraries/include dirs directly.")
		found = find_in(path, library_name)
		print(found)
