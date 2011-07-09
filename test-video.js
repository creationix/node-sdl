var SDL = require('./build/default/node-sdl');

console.dir(SDL);

SDL.init(SDL.INIT_VIDEO);
process.on('exit', SDL.quit);

//console.log("Checking mode 640x480@16bpp.");
//var bpp = SDL.videoModeOK(640, 480, 16, SDL.SWSURFACE | SDL.FULLSCREEN);
//if (!bpp) throw new Error("Mode not available.");

//console.log("SDL Recommends 640x480@%sbpp.", bpp);

SDL.setVideoMode(0, 0, 0, SDL.SWSURFACE);

var width = SDL.getScreenWidth();
var height = SDL.getScreenHeight();

for (var z = 0; z < 256; z++) {
  SDL.lockSurface();
  var ox = (width - 256) / 2;
  var oy = (height - 256) / 2;
  for (x = 0; x < 256; x++) {
    for (y = 0; y < 256; y++) {
      var color = SDL.mapRGB(x, y, z);
      SDL.putPixel(ox + x, oy + y, color);
    }
  }
  SDL.unlockSurface();
  /* Update just the part of the display that we've changed */
  SDL.updateRect(ox, oy, 256, 256);

//  for (var i = 0; i < 100; i++) {
//    var x = Math.floor(Math.random() * width);
//    var y = Math.floor(Math.random() * height);
//    var size = (Math.floor(Math.random() * 10) + 5) * 2;
//    var color = Math.floor(Math.random()*256*256*256);
//    SDL.fillRect(x - size / 2, y - size / 2, size, size, color);
//    SDL.updateRect(Math.max(x - size / 2, 0), Math.max(y - size / 2, 0), size, size);
//  }


}


