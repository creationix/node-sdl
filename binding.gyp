{
  "targets": [
    {
      # have to specify "liblib" here since gyp will remove the first one :\
      # Is the above still true?
      "target_name": "libnode_sdl",
      "type": "<(library)",
      "sources": [
        "src/helpers.cc",
        "src/node_sdl.cc",
        "src/struct_wrappers.cc",
        "src/render.cc",
        "src/texture.cc",
        "src/window.cc",
        "src/surface.cc",
        "src/container.cc",
        "src/sdl_gl.cc",
        "src/event.cc",
        "src/key.cc",
        "src/mouse.cc",
        "src/font.cc",
        "src/joystick.cc",
        "src/controller.cc"
      ],
      "libraries": [
        "/usr/local/lib/libSDL2.dylib",
        "/usr/local/lib/libSDL2_ttf.dylib",
        "/usr/local/lib/libSDL2_image.dylib",
      ],
      "include_dirs": [
        "/usr/local/include/SDL2"
      ],
      "defines": [
        "_THREAD_SAFE",
        "ENABLE_ARG_CHECKING",
        "ENABLE_IMAGE",
        "ENABLE_TTF"
      ]
    }
  ]
}
