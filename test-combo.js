var SDL = require('./build/default/node-sdl');

SDL.init(SDL.INIT_VIDEO | SDL.INIT_JOYSTICK);
process.on('exit', SDL.quit);

SDL.setVideoMode(0, 0, 0, SDL.SWSURFACE);
var width = SDL.getScreenWidth();
var height = SDL.getScreenHeight();

if (SDL.numJoysticks() < 1) {
  throw new Error("No joystick found!");
}
SDL.joystickOpen(0);
console.log("Found joystick " + SDL.joystickName(0));

setInterval(function () {
  SDL.joystickUpdate();
  var x = Math.floor(SDL.joystickGetAxis(0) / 256 + 128);
  var y = Math.floor(SDL.joystickGetAxis(1) / 256 + 128);
  var z = Math.floor(SDL.joystickGetAxis(2) / 256 + 128);
  SDL.fillRect(0, 0, width, height, SDL.mapRGB(x , y, z));
  SDL.flip();
}, 0);


