var SDL = require('../sdl'),
    IMG = SDL.IMG;
    
console.dir(IMG);

SDL.init(SDL.INIT.VIDEO);
var screen = SDL.setVideoMode(1024,768,32,0);
process.on('exit', function () { SDL.quit(); });

IMG.init(IMG.INIT.PNG);

SDL.events.on("QUIT", function (evt) { process.exit(0); }); // Window close
SDL.events.on("KEYDOWN", function (evt) {
  if (evt.sym === 99 && evt.mod === 64) process.exit(0); // Control+C
  if (evt.sym === 27 && evt.mod === 0) process.exit(0);  // ESC
});

var img = IMG.load(__dirname + "/imgs/Rock.png");
console.dir(img);
SDL.blitSurface(img, null, screen, [100,100]);
SDL.flip(screen);
SDL.freeSurface(img);
