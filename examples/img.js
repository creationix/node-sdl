var SDL = require('../sdl'),
    IMG = SDL.IMG;

var TILE_WIDTH = 99,
    TILE_HEIGHT = 82;

SDL.init(SDL.INIT.VIDEO);
var screen = SDL.setVideoMode(1024,768,32,0);

SDL.WM.setCaption("Node Explorer", "Node Exlorer");
SDL.WM.setIcon(IMG.load(__dirname + "/rock.png"));

process.on('exit', function () { SDL.quit(); });

IMG.init(IMG.INIT.PNG);

var tiles = IMG.load(__dirname + "/tiles.png");
var spriteData = require('./spriteData');


function draw(surface, name, x, y) {
  if (!spriteData.hasOwnProperty(name)) throw new Error("Invalid image name");
  var offsets = spriteData[name];
  SDL.blitSurface(tiles, [offsets[0], offsets[1], 101, 171], surface, [x, y + (offsets[2] || 0)]);
}


SDL.events.on("QUIT", function (evt) { process.exit(0); }); // Window close
SDL.events.on("KEYDOWN", function (evt) {
  if (evt.sym === 99 && evt.mod === 64) process.exit(0); // Control+C
  if (evt.sym === 27 && evt.mod === 0) process.exit(0);  // ESC
});

var map = [
  [
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","water-block","water-block","water-block","grass-block"],
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","water-block","water-block","water-block","grass-block"],
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","water-block","water-block","water-block","grass-block"],
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","water-block","water-block","water-block","grass-block"],
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","dirt-block","dirt-block","water-block","dirt-block","dirt-block"],
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","dirt-block","water-block","water-block","water-block","water-block"],
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","dirt-block","grass-block","water-block","water-block","water-block"],
    ["grass-block","grass-block","grass-block","grass-block","grass-block","grass-block","dirt-block","grass-block","grass-block","grass-block","grass-block"],
    ["grass-block","grass-block","grass-block","dirt-block","grass-block","dirt-block","dirt-block","dirt-block","dirt-block","dirt-block","dirt-block"],
    ["grass-block","grass-block","grass-block","dirt-block","dirt-block","dirt-block","grass-block","grass-block","grass-block","grass-block","grass-block"],
  ],
  [
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, "wood-block", "wood-block", "wood-block", "wood-block", "wood-block"],
    [null, "wood-block", null, null, null, "wood-block",null, "ramp-west","stone-block","ramp-east"],
    [null, "wood-block", null, null, null, "wood-block"],
    [null, "wood-block", null, null, null, "wood-block"],
    [null, "wood-block", "wood-block", "door-tall-closed", "wood-block", "wood-block"],
  ],
  [
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, "wood-block", "window-tall", "wood-block", "window-tall", "wood-block"],
    [null, "wood-block", null, null, null, "wood-block", null, null, "selector"],
    [null, "wood-block", null, null, null, "wood-block"],
    [null, "wood-block", null, null, null, "wood-block"],
    [null, "wood-block", "window-tall", null, "window-tall", "wood-block"],
  ],
  [
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, "wood-block", null, "wood-block", null, "wood-block"],
    [null, "wood-block", null, null, null, "wood-block"],
    [null, "wood-block", null, null, null, "wood-block"],
    [null, "wood-block", null, null, null, "wood-block"],
    [null, "wood-block", null, "wood-block", null, "wood-block"],
  ],
  [
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, "roof-north-west", "roof-north", "roof-north", "roof-north", "roof-north-east"],
    [null, "roof-west", "wood-block", "wood-block", "wood-block", "roof-east"],
    [null, "roof-west", "wood-block", null, "wood-block", "roof-east"],
    [null, "roof-west", "wood-block", "wood-block", "wood-block", "roof-east"],
    [null, "roof-south-west", "roof-south", "roof-south", "roof-south", "roof-south-east"],
  ],
  [
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, "wood-block", "window-tall", "wood-block", null],
    [null, null, "wood-block", null, "wood-block", null],
    [null, null, "wood-block", "window-tall", "wood-block", null],
    [null, null, null, null, null, null],
  ],
  [
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, "wood-block", null, "wood-block", null],
    [null, null, "wood-block", null, "wood-block", null],
    [null, null, "wood-block", null, "wood-block", null],
    [null, null, null, null, null, null],
  ],
  [
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, null, null, null, null],
    [null, null, "roof-north-west", "roof-north", "roof-north-east", null],
    [null, null, "roof-west", null, "roof-east", null],
    [null, null, "roof-south-west", "roof-south", "roof-south-east", null],
    [null, null, null, null, null, null],
  ]
];

map.forEach(function (layer, z) {
  layer.forEach(function (row, y) {
    row.forEach(function (cell, x) {
      if (cell) draw(screen, cell, x * 99 - 20, y * 82 - z * 41 - 70);
    });
  });
});

SDL.flip(screen);

