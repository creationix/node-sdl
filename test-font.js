(function () {

  var SDL = require('./sdl');
  var TTF = SDL.TTF;

  SDL.init(SDL.INIT_VIDEO);
  SDL.setVideoMode(1024,768,0,SDL.FULLSCREEN);
  TTF.init()
  process.on('exit', SDL.quit);

  TTF.openFont("Jura-DemiBold.ttf", 20);

  var y = 10;
  var x = 10;
  arguments.callee.toString().split("\n").forEach(function (line) {
    if (line.length) TTF.renderTextBlended(line, x, y, 0xffffff);
    y += 20;
  });
  SDL.flip();

  function getEvent() {
    var data;
    while (data = SDL.pollEvent()) {
      if ((data.type === "KEYDOWN" && data.sym === 27) || data.type === "QUIT") process.exit(0);
    }
    SDL.waitEvent(getEvent);
  }
  SDL.waitEvent(getEvent);

}());
