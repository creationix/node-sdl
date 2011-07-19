var SDL = require('../sdl');
console.dir(SDL);

SDL.init(SDL.INIT.VIDEO);
var screen = SDL.setVideoMode(1024,768,32,0);
process.on('exit', function () { SDL.quit(); });

var img = SDL.createRGBSurface(0, 256, 256);
for (var y = 0; y < 256; y++) {
  for (var x = 0; x < 256; x++) {
    var a = Math.floor(Math.sin(x/256*Math.PI)*Math.sin(y/256*Math.PI)*256);
    SDL.fillRect(img, [x, y, 1,1], (0 << 0)+(y << 8)+(x << 16)+(a << 24));
  }
}
SDL.flip(img);
setInterval(function () {
  for (var i = 0; i < 10; i++) {
    var x = Math.floor(Math.random() * (screen.w - 256));
    var y = Math.floor(Math.random() * (screen.h - 256));
    SDL.blitSurface(img, null, screen, [x,y]);
  }
  SDL.flip(screen);
}, 10);
//SDL.freeSurface(img);

SDL.events.on("QUIT", function (evt) { process.exit(0); }); // Window close
SDL.events.on("KEYDOWN", function (evt) {
  if (evt.sym === 99 && evt.mod === 64) process.exit(0); // Control+C
  if (evt.sym === 27 && evt.mod === 0) process.exit(0);  // ESC
});

