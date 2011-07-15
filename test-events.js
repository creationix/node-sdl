var SDL = require('./build/default/node-sdl');

SDL.init(SDL.INIT_VIDEO | SDL.INIT_JOYSTICK);
process.on('exit', SDL.quit);

SDL.setVideoMode(1024, 768, 0, 0);

function getEvent(type) {
  console.log("EVENT " + type);
  if (type === SDL.QUIT) { process.exit(0); }
  SDL.waitEvent(getEvent);
}
SDL.waitEvent(getEvent);

