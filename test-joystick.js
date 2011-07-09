var SDL = require('./build/default/node-sdl');

SDL.init(SDL.INIT_VIDEO | SDL.INIT_JOYSTICK);
SDL.setVideoMode(0, 0, 0, 0);

process.on('exit', SDL.quit);

console.log("%s joysticks were found.\n", SDL.numJoysticks());

for (var i = 0, l = SDL.numJoysticks(); i < l; i++) {
  SDL.joystickOpen(i);
  console.log("\nOpened Joystick " + i);
  console.log("Name: %s", SDL.joystickName(i));
  console.log("Number of Axes: %s", SDL.joystickNumAxes());
  console.log("Number of Buttons: %s", SDL.joystickNumButtons());
  console.log("Number of Balls: %s", SDL.joystickNumBalls());
  console.log("Number of Hats: %s", SDL.joystickNumHats());
}

setInterval(function () {
  SDL.joystickUpdate();
  var sticks = new Array(SDL.numJoysticks());
  for (var i = 0, l = sticks.length; i < l; i++) {
    SDL.joystickOpen(i);
    var data = sticks[i] = {
      id: i,
      name: SDL.joystickName(i),
      axes: new Array(SDL.joystickNumAxes()),
      buttons: new Array(SDL.joystickNumButtons()),
      hats: new Array(SDL.joystickNumHats())
    };
    for (var a = 0; a < data.axes.length; a++) {
      data.axes[a] = SDL.joystickGetAxis(a);
    }
    for (var a = 0; a < data.buttons.length; a++) {
      data.buttons[a] = SDL.joystickGetButton(a);
    }
    for (var a = 0; a < data.hats.length; a++) {
      data.hats[a] = SDL.joystickGetHat(a);
    }
  }
  console.dir(sticks);
}, 0)

