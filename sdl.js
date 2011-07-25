var SDL = module.exports = require('./build/default/node-sdl.node');

// Easy event emitter based event loop.  Started automatically when the first
// listener is added.
var events;
Object.defineProperty(SDL, 'events', {
  get: function () {
    if (events) return events;
    events = new (require('events').EventEmitter);
    setInterval(function () {
      var data;
      var motion = {};
      while (data = SDL.pollEvent()) {
        if (data.type.substr(data.type.length - 6) === "MOTION") {
          motion[data.type] = data;
          continue;
        }
        events.emit(data.type, data);
        events.emit("event", data);
      }
      Object.keys(motion).forEach(function (name) {
        var event = motion[name];
        events.emit(event.type, event);
        events.emit("event", event);
      });
    }, 13);
    return events;
  }
});

