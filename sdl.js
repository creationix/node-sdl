function isFunction(functionToCheck) {
 var getType = {};
 return functionToCheck && getType.toString.call(functionToCheck) === '[object Function]';
}

console.log("About to load node_sdl.node");
var SDL = module.exports = require('./build/Release/node_sdl.node');
console.log("Finished loading node_sdl.node");

// console.log("Showing all keys for base SDL namespace:");
// for(key in SDL) {
//   console.log("SDLKEY: " + key + ", SDLVAL: " + SDL[key]);
// }
// console.log()

// console.log("Showing all keys for SDL.INIT:");
// for(key in SDL.INIT) {
//   console.log("INITKEY: " + key + ", INITVAL: " + SDL.INIT[key]);
// }

console.log("About to init SDL. (no args, default to EVERYTHING)");
SDL.init("FOO");

console.log("About to create a window.")
var window = new SDL.Window("Foo title")
console.log("Finished creating window.")
var render = new SDL.Renderer(window);
// for(key in window) {
//   console.log("WINDOWKEY: " + key + ", " + "WINDOWVAL: " + window[key]);
//   if(isFunction(window[key])) {
//     try {
//       console.log("WINDOWCALLRET: " + window[key]());
//     }
//     catch(err) {
//       console.log("WINDOWCALLERR: " + err);
//     }
//   }
// }

// var mode = window.getDisplayMode();
// for(key in mode) {
//   console.log("MODEKEY: " + key + ", " + "MODEVALUE: " + mode[key]);
// }

var p = new SDL.Point(1, 1);
// console.log(p.x + ", " + p.y);
console.log(p.toString());

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

