var SDL = require('./build/default/node-sdl');

SDL.init(SDL.INIT_VIDEO | SDL.INIT_JOYSTICK);
process.on('exit', SDL.quit);

SDL.setVideoMode(1024, 768, 0, 0);

SDL.joystickEventState(SDL.ENABLE);
console.log("joystickEventState", SDL.joystickEventState(SDL.QUERY));

function getEvent(type, data) {
  console.dir(arguments);
//  switch (type) {
//    case SDL.QUIT: process.exit(0);
//    case SDL.ACTIVEEVENT:
//      console.dir(SDL.getAppState());
//      break;
//    case SDL.MOUSEMOTION:
//    case SDL.MOUSEBUTTONDOWN:
//    case SDL.MOUSEBUTTONUP:
//      console.dir(SDL.getMouseState());
//      break;
//    case SDL.KEYDOWN:
//      console.dir("KEYDOWN");
//      break;
//    case SDL.KEYUP:
//      console.dir("KEYUP");
//      break;
//    default:
//      console.dir(SDL);
//      throw new Error("UNKNOWN EVENT TYPE " + type);
//  }
  SDL.waitEvent(getEvent);
}
SDL.waitEvent(getEvent);

