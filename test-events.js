var SDL = require('./build/default/node-sdl');

var EventEmitter = require('events').EventEmitter;

SDL.init(SDL.INIT_VIDEO | SDL.INIT_JOYSTICK);
process.on('exit', SDL.quit);

SDL.setVideoMode(320, 200, 0, 0);

for (var i = 0, l = SDL.numJoysticks(); i < l; i++) {
  SDL.joystickOpen(i);
}

var events = new EventEmitter();

function getEvent() {
  var data;
  while (data = SDL.pollEvent()) {
    events.emit(data.type, data);
    events.emit("event", data);
  }
  SDL.waitEvent(getEvent);
}
SDL.waitEvent(getEvent);



///////////
// USAGE //
///////////
events.on("QUIT", process.exit);
events.on("event", console.dir);

