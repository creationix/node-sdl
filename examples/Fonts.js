
var colors = [
  [0xfce94f, 0xedd400, 0xc4a000],
  [0x8ae234, 0x73d216, 0x4e9a06],
  [0xe9b96e, 0xc17d11, 0x8f5902],
  [0xfcaf3e, 0xf57900, 0xce5c00],
  [0xad7fa8, 0x75507b, 0x5c3566],
  [0xef2929, 0xcc0000, 0xa40000],
  [0x729fcf, 0x3465a4, 0x204a87]
];


var SDL = require('../sdl');
var TTF = SDL.TTF;

SDL.init(SDL.INIT.VIDEO);
var screen = SDL.setVideoMode(1024,768,0,0);
TTF.init()
process.on('exit', function () { SDL.quit(); });

var font = TTF.openFont(__dirname + "/Jura-DemiBold.ttf", 32);

var y = 10;
var x = 10;

var now = Date.now();
setInterval(function () {
  var after = Date.now();
  var delta = after - now;
  now = after;
  SDL.fillRect(screen, null, 0);
  var message = (Math.floor(100000 / delta) / 100) + "fps";
  var s = TTF.renderTextBlended(font, message, 0xffffff);
  SDL.blitSurface(s, null, screen, [10, 10]);
  SDL.freeSurface(s);
  SDL.flip(screen);  
}, 15);

SDL.events.on("QUIT", function (evt) { process.exit(0); }); // Window close
SDL.events.on("KEYDOWN", function (evt) {
  if (evt.sym === 99 && evt.mod === 64) process.exit(0); // Control+C
  if (evt.sym === 27 && evt.mod === 0) process.exit(0);  // ESC
});

