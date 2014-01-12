console.log("About to load node_sdl.node");
var SDL = module.exports = require('./build/Release/node_sdl.node');
console.log("Finished loading node_sdl.node");

console.log("Showing all keys for base SDL namespace:");
for(key in SDL) {
  console.log("SDLKEY: " + key + ", SDLVAL: " + SDL[key]);
}
console.log()

console.log("Showing all keys for SDL.INIT:");
for(key in SDL.INIT) {
  console.log("INITKEY: " + key + ", INITVAL: " + SDL.INIT[key]);
}

console.log("About to init SDL. (no args, default to EVERYTHING");
SDL.init("FOO");
console.log("Finished SDL_Init.");

console.log("About to create a window.")
var window = new SDL.Window("Foo title")
console.log("Finished creating window.")

var timeout = 1000;
console.log("Setting timeout to: " + timeout + ", to SDL quit");
setTimeout(function() {SDL.quit()}, timeout);

// Easy event emitter based event loop.  Started automatically when the first
// listener is added.
// var events;
// Object.defineProperty(SDL, 'events', {
//   get: function () {
//     if (events) return events;
//     events = new (require('events').EventEmitter);
//     var now = Date.now();
//     setInterval(function () {
//       var after = Date.now();
//       var delta = after - now;
//       now = after;
//       var data;
//       while (data = SDL.pollEvent()) {
//         events.emit('event', data);
//         events.emit(data.type, data);
//       }
//       events.emit('tick', delta);
//     }, 16);
//     return events;
//   }
// });

