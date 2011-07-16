var SDL = require('./sdl');
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

var events = new (require('events').EventEmitter);
function getEvent(err) {
  if (err) events.emit('error', err);
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
events.on("event", console.dir);
events.on("QUIT", function (evt) { process.exit(0); }); // Window close
events.on("KEYDOWN", function (evt) {
  if (evt.sym === 99 && evt.mod === 64) process.exit(0); // Control+C
  if (evt.sym === 27 && evt.mod === 0) process.exit(0);  // ESC
});
