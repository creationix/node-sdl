var sdl = require("./build/Release/node_sdl.node");

var entities = [];
var msPerFrame = 0;
var sPerFrame = 0;
var window, renderer, world;
var _quitFun;
var _going = true;

var tickerEvent = function() {
	var event;
	while((event = sdl.pollEvent()) !== undefined) {
		if(_quitFun(event, world) || sdl.quitRequested()) {
			_going = false;
		}
		// for(var i in entities) {
		// 	entities[i].processInput(event, world);
		// }
		// for(i = 0; i < entities.length; i++) {
		// 	entities[i].processInput(event, world);
		// }
		var i = entities.length;
		while(i--) {
			entities[i].processInput(event, world);
		}
		if(event.type === "KEYDOWN") {
			if(event.scancode == sdl.SCANCODE.Q) {
				_going = false;
			}
		}
	}
}
var tickerTick = function() {
	for(i = 0; i < entities.length; i++) {
		entities[i].tick(sPerFrame, world);
	}
}
var tickerRender = function() {
	var befc = process.hrtime();
	renderer.clear();
	// console.log("Clear took: " + hrtoms(process.hrtime(befc)) + "ms.");
	var befl = process.hrtime();
	for(i = 0; i < entities.length; i++) {
		entities[i].render(renderer);
	}
	// console.log("Render loop took: " + hrtoms(process.hrtime(befl)) + "ms.");
}
var ticker = function() {
	var start = process.hrtime();

	// Process things.
	var befev = process.hrtime();
	tickerEvent();
	// console.log("Event took: " + hrtoms(process.hrtime(befev)) + "ms.");
	var beft = process.hrtime();
	tickerTick();
	// console.log("Tick took: " + hrtoms(process.hrtime(beft)) + "ms.");
	var befr = process.hrtime();
	tickerRender();
	// console.log("Render took: " + hrtoms(process.hrtime(befr)) + "ms.");

	var delta = hrtoms(process.hrtime(start));
	// console.log("Ticker took: " + delta + "ms.");
	// var timeout = msPerFrame - delta;
	// if(timeout < 0) {
	// 	timeout = 0;
	// }
	// console.log("Setting timeout: " + timeout);
	// if(_going) {
	// 	setTimeout(ticker, timeout);
	// }
}
var baseline;
var last, lastpresent, lastpresentdelta;
var presenter = function() {
	while(_going) {
		if(last) {
			var calldelta = hrtoms(process.hrtime(last));
			// console.log("Time to have presenter called: " + calldelta + "ms.");
		}

		var befp = process.hrtime();
		// console.log("Calling present at: " + (new Date()).toISOString());
		var calledat = hrtoms(process.hrtime(baseline));
		renderer.present();
		var delta = hrtoms(process.hrtime(befp));
		if(delta > 20) {
			console.log("Present took too long: " + delta + "ms.");
			// console.log("Last present at: " + lastpresent);
			// console.log("Was called at: " + calledat);
			// console.log("Last delta: " + lastpresentdelta);
		}

		if(_going) {
			// var wait = 16.0 - (delta % 16.0) + 1;
			// var wait = 16.7 - (delta % 16.7) + 1;
			// if(delta < 16) {
			// 	wait = 16 - delta;
			// }
			// else {
			// 	wait = 32 - delta;
			// }
			// console.log("Setting presenter timeout: " + wait);
			// setTimeout(presenter, 1);
			last = process.hrtime();
			lastpresent = calledat;
			lastpresentdelta = delta;
			// setImmediate(presenter);
			var beft = process.hrtime();
			ticker();
			// console.log("Ticker took: " + hrtoms(process.hrtime(beft)) + "ms.");
		}
	}
}
var hrtoms = function(hr) {
	return (hr[0] * 1e9 + hr[1]) / 1e6;
}

module.exports = {
	Entity: function (args) {
		this._pos = {x: 0, y: 0};
		this._size = {x: 0, y: 0};
		this._vel = {x: 0, y: 0};
		this._accel = {x: 0, y: 0};

		this.pos = args.pos;
		this.size = args.size;
		this.vel = args.vel;
		this.accel = args.accel;
		this._texture = args.texture === undefined ? testTex : args.texture;
		this._id = args.id === undefined ? "BasicID" : args.id;

		this.render = args.render || this.render;
		this.processInput = args.processInput || this.processInput;
		this.tick = args.tick || this.tick;

		entities.push(this);
	},
	choice: function (l) {
		var chosen = Math.floor(Math.random() * l.length);
		return l[chosen];
	},
	gameLoop: function(fps, setupFun, quitFun, windowArgs) {
		msPerFrame = 1000 / fps;
		sPerFrame = msPerFrame / 1000;
		_quitFun = quitFun;

		window = new sdl.Window(!!windowArgs.title ? "Default" : windowArgs.title,
			!!windowArgs.x ? sdl.WINDOWPOS.CENTERED : windowArgs.x,
			!!windowArgs.y ? sdl.WINDOWPOS.CENTERED : windowArgs.y,
			!!windowArgs.w ? 640 : windowArgs.w,
			!!windowArgs.h ? 480 : windowArgs.h);
		// msPerFrame = 1000 / window.getDisplayMode().refreshRate;
		// sPerFrame = msPerFrame / 1000;
		renderer = new sdl.Renderer(window, -1, sdl.RENDERER.ACCELERATED | sdl.RENDERER.PRESENTVSYNC);
		// renderer = new sdl.Renderer(window, -1, sdl.RENDERER.ACCELERATED);
		world = setupFun(window, renderer);
		baseline = process.hrtime();
		renderer.clear();
		presenter();
		// setTimeout(presenter, 0);
		// setTimeout(ticker, 0);
		// ticker();
	}
}

module.exports.Entity.prototype = {
	move: function(delta) {
		this.x = this.x + delta.x;
		this.y = this.y + delta.y;
	},

	get x() {
		return this._pos.x;
	},
	get y() {
		return this._pos.y;
	},
	set x(x) {
		this._pos.x = x;
	},
	set y(y) {
		this._pos.y = y;
	},
	get pos() {
		return {x: this._pos.x, y: this._pos.y};
	},
	set pos(pos) {
		this._pos.x = pos.x === undefined ? this._pos.x : pos.x;
		this._pos.y = pos.y === undefined ? this._pos.y : pos.y;
	},

	get w() {
		return this._size.x;
	},
	get h() {
		return this._size.y;
	},
	set w(w) {
		this._size.x = w;
	},
	set h(h) {
		this._size.y = h;
	},
	get size() {
		return {x: this._size.x, y: this._size.y};
	},
	set size(size) {
		if(size) {
			this._size.x = size.x === undefined ? this._size.x : size.x;
			this._size.y = size.y === undefined ? this._size.y : size.y;
		}
	},

	get vel() {
		return this._vel;
	},
	set vel(vel) {
		if(vel) {
			this._vel.x = vel.x === undefined ? this._vel.x : vel.x;
			this._vel.y = vel.y === undefined ? this._vel.y : vel.y;
		}
	},

	get accel() {
		return {x: this._accel.x, y: this._accel.y};
	},
	set accel(accel) {
		if(accel) {
			this._accel.x = accel.x === undefined ? this._accel.x : accel.x;
			this._accel.y = accel.y === undefined ? this._accel.y : accel.y;
		}
	},

	processInput: function(event, world) {
		// console.log("Entity basic processInput: " + event.type);
	},
	render: function(renderer) {
		renderer.copy(this._texture, undefined, new sdl.Rect(this.x, this.y, this.w, this.h));
	},
	tick: function(dt, world) {
		// console.log("accel: " + JSON.stringify(this._accel));
		this._vel.x += this._accel.x * dt;
		this._vel.y += this._accel.y * dt;
		// console.log("vel: " + JSON.stringify(this._vel));
		this._pos.x += this._vel.x * dt;
		this._pos.y += this._vel.y * dt;
		// console.log("pos: " + JSON.stringify(this._pos));
	}
};
