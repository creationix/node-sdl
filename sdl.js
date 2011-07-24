var SDL = module.exports = require('./build/default/node-sdl.node');

// Easy event emitter based event loop.  Started automatically when the first
// listener is added.
var events;
Object.defineProperty(SDL, 'events', {
  get: function () {
    if (events) return events;
    events = new (require('events').EventEmitter);
    function getEvent(err) {
      if (err) events.emit('error', err);
      var data;
      var mousemotion;
      while (data = SDL.pollEvent()) {
        if (data.type === "MOUSEMOTION") {
          // Collapse motion events to just the last position
          // TODO: collapse other high frequency events
          mousemotion = data;
          continue;
        }
        events.emit(data.type, data);
        events.emit("event", data);
      }
      if (mousemotion) {
        events.emit(mousemotion.type, mousemotion);
        events.emit("event", mousemotion);
      }
      SDL.waitEvent(getEvent);
    }
    SDL.waitEvent(getEvent);
    return events;
  }
});

