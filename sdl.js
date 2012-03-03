var SDL = module.exports = require('./build/Release/node-sdl.node');

// Easy event emitter based event loop.  Started automatically when the first
// listener is added.
var events;
Object.defineProperty(SDL, 'events', {
  get: function () {
    if (events) return events;
    events = new (require('events').EventEmitter);
    var now = Date.now();
    setInterval(function () {
      var after = Date.now();
      var delta = after - now;
      now = after;
      var data;
      while (data = SDL.pollEvent()) {
        events.emit('event', data);
        events.emit(data.type, data);
      }
      events.emit('tick', delta);
    }, 16);
    return events;
  }
});

