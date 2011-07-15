var SDL = require('./build/default/node-sdl');

SDL.init(SDL.INIT_VIDEO | SDL.INIT_JOYSTICK);
process.on('exit', SDL.quit);

SDL.setVideoMode(320, 200, 0, 0);

SDL.joystickOpen(0);
SDL.joystickEventState(SDL.ENABLE);

console.log("joystickEventState", SDL.joystickEventState(SDL.QUERY));

function getEvent(data) {
  console.dir(data);
  if (data.type === "QUIT") process.exit(0);
  SDL.waitEvent(getEvent);
}
SDL.waitEvent(getEvent);

