from os import popen

srcdir = '.'
blddir = 'build'
VERSION = '0.0.1'

def set_options(opt):
  opt.tool_options('compiler_cxx')

def configure(conf):
  conf.check_tool('compiler_cxx')
  conf.check_tool('node_addon')
  
  sdl_config = conf.find_program('sdl-config', var='SDL_CONFIG', mandatory=True)
  sdl_libs = popen("%s --libs" % sdl_config).readline().strip()
  sdl_cflags = popen("%s --cflags" % sdl_config).readline().strip()

  sdl_addpaths = []
  sdl_addlibs = []
  for item in sdl_libs.split(' '):
    # -L items are lib paths, -l are additional libraries
    if item.find("-L") == 0:
      sdl_addpaths.append(item[2:])
    if item.find("-l") == 0:
      sdl_addlibs.append(item[2:])

  conf.env.append_value("LIBPATH_SDL", sdl_addpaths)
  conf.env.append_value("LIB_SDL", sdl_addlibs)
   
  conf.env.append_value("CPPFLAGS_SDL", sdl_cflags.split(' '))

def build(bld):
  obj = bld.new_task_gen('cxx', 'shlib', 'node_addon')
  obj.target = "node-sdl"
  obj.cxxflags = ["-pthread", "-Wall"]
  obj.source = "src/node-sdl.cpp"
  obj.uselib = "SDL"
