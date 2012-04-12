{
  'targets': [
    {
      # have to specify 'liblib' here since gyp will remove the first one :\
      'target_name': 'libnode-sdl',
      'sources': [
        'src/helpers.cc',
        'src/sdl.cc',
      ],
      'ldflags': [
        '<!@(sdl-config --libs)',
        "-lSDL_ttf",
        "-lSDL_image"
      ],
      'cflags': [
        '<!@(sdl-config --cflags)'
      ],
    }
  ]
}
