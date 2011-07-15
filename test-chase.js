var SDL = require('./build/default/node-sdl');
var Pattern = require('pattern');

SDL.init(SDL.INIT_VIDEO | SDL.INIT_JOYSTICK);
process.on('exit', SDL.quit);

//SDL.setVideoMode(1024, 768, 32, 0);
SDL.setVideoMode(1024, 600,0,SDL.DOUBLEBUFFER);
var width = SDL.getScreenWidth();
var height = SDL.getScreenHeight();

var numPlayers = SDL.numJoysticks();

if (!numPlayers) throw new Error("At least one joystick is required to play");

var colors = {
  butter: [0xfce94f, 0xedd400, 0xc4a000],
  chameleon: [0x8ae234, 0x73d216, 0x4e9a06],
  chocolate: [0xe9b96e, 0xc17d11, 0x8f5902],
  orange: [0xfcaf3e, 0xf57900, 0xce5c00],
  plum: [0xad7fa8, 0x75507b, 0x5c3566],
  scarlet: [0xef2929, 0xcc0000, 0xa40000],
  sky: [0x729fcf, 0x3465a4, 0x204a87]
}

function shuffle(array) {
  for (var i = 0, l = array.length; i < l; i++) {
    var j = Math.floor(Math.random() * l);
    var temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

var colorNames = Object.keys(colors);
shuffle(colorNames);

var Player = Pattern.extend({
  initialize: function (joystickIndex) {
    this.joy = joystickIndex;
    this.name = SDL.joystickName(this.joy);
    var colorName = this.colorName = colorNames.pop();
    var angle = 2 * Math.PI / numPlayers * joystickIndex;
    this.x = Math.floor(Math.sin(angle) * 50 * numPlayers) + width / 2;
    this.y = Math.floor(Math.cos(angle) * 50 * numPlayers) + height / 2;
    this.speed = 0.3;
    console.log("New %s player using %s", colorName[0].toUpperCase() + colorName.substr(1), this.name);
  },
  tick: function (delta) {
    SDL.joystickOpen(this.joy);
    var jx = SDL.joystickGetAxis(0) / 32768;
    var jy = SDL.joystickGetAxis(1) / 32768;
    this.x += jx * delta * this.speed;
    this.y += jy * delta * this.speed;
    if (this.x < 10) this.x = 10;
    if (this.y < 10) this.y = 10;
    if (this.x > width - 10) this.x = width - 10;
    if (this.y > height - 10) this.y = height - 10;
    
    var px = Math.floor(this.x);
    var py = Math.floor(this.y);
    SDL.fillRect(px - 10, py - 10, 20, 20, colors[this.colorName][0]);
    SDL.fillRect(px - 8, py - 8, 16, 16, colors[this.colorName][2]);
    SDL.fillRect(px - 4, py - 4, 8, 8, colors[this.colorName][1]);
  }
});

var rotate = 0;
var Spark = Pattern.extend({
  initialize: function (player) {
    rotate = (rotate + 7) % 360;
    this.colorName = player.colorName;
    this.color = colors[player.colorName][Math.floor(Math.random()*3)];
    this.x = player.x;
    this.y = player.y;
    this.r = rotate / 180 * Math.PI;
    this.d = 10;
    sparks.push(this);
  },
  tick: function (delta) {
    this.d += delta * 0.08 + this.d / 20;
    for (var a = 0; a < Math.PI * 2; a += Math.PI / 7) {
      var px = Math.floor(this.x + this.d * Math.sin(a + this.r));
      var py = Math.floor(this.y + this.d * Math.cos(a + this.r));
      SDL.fillRect(px - 3, py - 3, 6, 6, this.color);
    }
  },
  expire: function () {
    if (this.d > 400) {
      sparks.splice(sparks.indexOf(this), 1);
    }
  }
});

var sparks = [];
var players = new Array(numPlayers);
for (var i = 0; i < numPlayers; i++) {
  players[i] = Player.new(i);
}
console.dir(players);

var before = Date.now();
setInterval(function () {
  SDL.joystickUpdate();
  var after = Date.now();
  var delta = after - before;
  before = after;
  // Check for collisions
  var collision = false;
  for (var i = 0; i < numPlayers - 1; i++) {
    for (var j = i + 1; j < numPlayers; j++) {
      var dx = players[i].x - players[j].x;
      var dy = players[i].y - players[j].y;
      if (dx < 20 && dx > -20 && dy < 20 && dy > -20) {
//        collision = true;
        Spark.new(players[i]);
        Spark.new(players[j]);
      }
    }
  }
  if (collision) {
    SDL.fillRect(0,0,width,height,0xffffff);
  } else {
    SDL.fill();
  }
  // Run physics and controls
  for (var i = 0; i < numPlayers; i++) {
    players[i].tick(delta);
  }
  // Run physics and controls
  for (var i = 0, l = sparks.length; i < l; i++) {
    sparks[i].tick(delta);
  }
  sparks.forEach(function (spark) { spark.expire(); });  
  
  
  SDL.flip();
}, 10);


