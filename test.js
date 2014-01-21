var sdl = require("./build/Release/node_sdl.node");
var eng = require("./engine.js");

var width = 640;
var height = 480;
var title = "Pong";
var fps = 60;

var paddleSize = {x: width / 10, y: width / 20};
var paddleSpeed = 300;

var paddleInput = function(event, world) {
	if(event.repeat !== undefined && event.repeat) {
		return;
	}

	if(event.type === "KEYDOWN") {
		if(event.scancode === sdl.SCANCODE.LEFT) {
			this.vel.x += -paddleSpeed;
		}
		else if(event.scancode === sdl.SCANCODE.RIGHT) {
			this.vel.x += paddleSpeed;
		}
	}
	else if(event.type === "KEYUP") {
		if(event.scancode === sdl.SCANCODE.LEFT) {
			this.vel.x += paddleSpeed;
		}
		else if(event.scancode === sdl.SCANCODE.RIGHT) {
			this.vel.x += -paddleSpeed;
		}
	}
}
var paddleTick = function(dt, world) {
	eng.Entity.prototype.tick.apply(this, [dt, world]);

	if(this.x < world.leftWall.x + world.leftWall.w) {
		this.x = world.leftWall.x + world.leftWall.w - 1;
	}
	else if(this.x + this.w > world.rightWall.x) {
		this.x = world.rightWall.x - this.w;
	}
}

var ballTick = function(dt, world) {
	eng.Entity.prototype.tick.apply(this, [dt, world]);

	if(this.x < world.leftWall.x + world.leftWall.w) {
		this.vel.x = -this.vel.x;
	}
	else if(this.x + this.w > world.rightWall.x) {
		this.vel.x = -this.vel.x;
	}
	else if(this.y < 0) {
		this.vel.y = -this.vel.y;
	}
	else if(this.y + this.h > height) {
		this.vel.y = -this.vel.y;
	}
}

var shouldQuit = function(event, world) {
	if(event.type === "KEYDOWN") {
		if(event.scancode === sdl.SCANCODE.Q) {
			return true;
		}
	}
	return false;
}

var setup = function(window, renderer) {
	var world = {};

	var surface = new sdl.Surface(paddleSize.x, paddleSize.y);
	surface.fillRect(new sdl.Color(255, 255, 255).getColor(surface.getPixelFormat()));
	world.paddleTex = new sdl.Texture(renderer, surface);

	surface = new sdl.Surface(width / 30, width / 30);
	surface.fillRect(new sdl.Color(255, 255, 255).getColor(surface.getPixelFormat()));
	world.ballTex = new sdl.Texture(renderer, surface);

	surface = new sdl.Surface(width / 100, height);
	surface.fillRect(new sdl.Color(255, 255, 255).getColor(surface.getPixelFormat()));
	world.wallTex = new sdl.Texture(renderer, surface);

	world.paddle = new eng.Entity({
		texture: world.paddleTex,
		processInput: paddleInput,
		tick: paddleTick,
		pos: {y: height - paddleSize.y * 2, x: width / 2 - paddleSize.x},
		size: paddleSize
	});

	world.leftWall = new eng.Entity({
		texture: world.wallTex,
		pos: {y: 0, x: (width / 100) * 2},
		size: {x: width / 100, y: height}
	});
	world.rightWall = new eng.Entity({
		texture: world.wallTex,
		pos: {y: 0, x: width - (width / 100) * 2},
		size: {x: width / 100, y: height}
	});

	world.ball = new eng.Entity({
		texture: world.ballTex,
		tick: ballTick,
		pos: {y: height / 2, x: width / 2},
		size: {x: width / 30, y: width / 30},
		vel: {x: 100, y: 100}
	});

	return world;
}

eng.gameLoop(fps, setup, shouldQuit, {width: width, height: height, title: title});
