{
  'targets': [
    {
      # have to specify 'liblib' here since gyp will remove the first one :\
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
        'src/event.cc'
      ],
      'libraries': [
        '/Library/Frameworks/SDL2.framework/SDL2',
        '/Library/Frameworks/SDL2_ttf.framework/SDL2_ttf',
        '/Library/Frameworks/SDL2_image.framework/SDL2_image',
      ],
      'include_dirs': [
        "/Library/Frameworks/SDL2.framework/Headers",
        "/Library/Frameworks/SDL2_ttf.framework/Headers",
        "/Library/Frameworks/SDL2_image.framework/Headers",
      ],
      'cflags': [
        "-D_THREAD_SAFE"
      ]
    }
  ]
}
