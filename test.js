var SDL = require('./build/default/node-sdl');

console.dir(SDL);

SDL.init();
setInterval(function () {
  for (var i = 0; i < 100; i++) {
    var x = Math.floor(Math.random() * 1024);
    var y = Math.floor(Math.random() * 768);
    var w = 20;
    var h = 20;
    var color = Math.floor(Math.random() * 256 * 256 * 256);
    SDL.fillRect(x, y, w, h, color);
  }
  SDL.flip();
}, 16);

setTimeout(function () {
  SDL.quit();
  process.exit();
}, 60000);
