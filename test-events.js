var SDL = require('./build/default/node-sdl');
var Pattern = require('pattern');

SDL.init(SDL.INIT_VIDEO);
process.on('exit', SDL.quit);

SDL.setVideoMode(1024, 768, 0, 0);

function getEvent(type) {
  console.log("EVENT " + type);
  SDL.waitEvent(getEvent);
}
SDL.waitEvent(getEvent);

