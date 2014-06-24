{
	'targets': [{
		'target_name': 'libnode_sdl',
		'sources': [
			'src/helpers.cc',
			'src/node_sdl.cc',
			'src/struct_wrappers.cc',
			'src/render.cc',
			'src/texture.cc',
			'src/window.cc',
			'src/surface.cc',
			'src/container.cc',
			'src/sdl_gl.cc',
			'src/event.cc',
			'src/key.cc',
			'src/mouse.cc',
			'src/font.cc',
			'src/joystick.cc',
			'src/controller.cc'
		],
		'conditions': [[
			'OS=="mac"', {
				'libraries': [
					'/usr/local/lib/libSDL2.dylib',
					'/usr/local/lib/libSDL2_ttf.dylib',
					'/usr/local/lib/libSDL2_image.dylib'
				],
				'include_dirs': [
					'/usr/local/include/SDL2'
				]
			}
		], [
			'OS=="linux" and target_arch=="ia32"', {
				'libraries': [
					'/usr/lib/libSDL2-2.0.so',
					'/usr/lib/libSDL2_image-2.0.so',
					'/usr/lib/libSDL2_ttf-2.0.so'
				],
				'include_dirs': [
					'/usr/include/SDL2'
				]
			}
		], [
			'OS=="linux" and target_arch=="x64"', {
				'libraries': [
					'/usr/lib/x86_64-linux-gnu/libSDL2-2.0.so',
					'/usr/lib/x86_64-linux-gnu/libSDL2_image-2.0.so',
					'/usr/lib/x86_64-linux-gnu/libSDL2_ttf-2.0.so'
				],
				'include_dirs': [
					'/usr/include/SDL2'
				]
			}
		]],
		'defines': [
			'_THREAD_SAFE',
			'ENABLE_ARG_CHECKING',
			'ENABLE_IMAGE',
			'ENABLE_TTF'
		]
	}]
}
