var SDL = require('../sdl');
//console.dir(SDL);

SDL.init(SDL.INIT.VIDEO | SDL.INIT.JOYSTICK);
process.on('exit', function () { SDL.quit(); });

var screen = SDL.setVideoMode(320, 200, 0, 0);
console.dir(screen);

var joys = []
for (var i = 0, l = SDL.numJoysticks(); i < l; i++) {
  joys[i] = SDL.joystickOpen(i);
}
console.dir(joys);

SDL.events.on("event", console.dir);
SDL.events.on("QUIT", function (evt) { process.exit(0); }); // Window close
SDL.events.on("KEYDOWN", function (evt) {
  if (evt.sym === 99 && evt.mod === 64) process.exit(0); // Control+C
  if (evt.sym === 27 && evt.mod === 0) process.exit(0);  // ESC
});
