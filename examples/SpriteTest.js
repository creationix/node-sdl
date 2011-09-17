var SDL = require( '../sdl' );

var velocity = [0,0];

function SpriteSheet( path, mapFunction ) {
    this.path = path;
    this.mapFunction = mapFunction;
}

SpriteSheet.prototype.setAlpha = function ( alpha ) {
    this.alpha = alpha;
};

SpriteSheet.prototype.setColorKey = function ( colorKey ) {
    this.colorKey = colorKey;
};

SpriteSheet.prototype.load = function ( ) {
    var t = SDL.IMG.load( this.path );
    this.sheet = SDL.displayFormat( t );

    if( this.alpha )
	SDL.setAlpha( this.sheet, SDL.SURFACE.SRCALPHA|SDL.SURFACE.RLEACCEL, this.alpha );

    if( this.colorKey )
	SDL.setColorKey( this.sheet,
			 SDL.SURFACE.SRCCOLORKEY|SDL.SURFACE.RLEACCEL,
			 SDL.mapRGB( this.sheet.format, this.colorKey[0], this.colorKey[1], this.colorKey[2] ) );

    SDL.freeSurface( t );
};

SpriteSheet.prototype.draw = function ( screen, number, location ) {
    SDL.blitSurface( this.sheet, this.mapFunction( number ), screen, location );
};

SpriteSheet.prototype.background = function( x, y, tiles ) {
    var bg = SDL.createRGBSurface( SDL.SURFACE.SWSURFACE | SDL.SURFACE.SRCALPHA, x, y );

    for( var i = 0, il = tiles.length; i < il; i++ ) {
	this.draw( bg, tiles[i][0], tiles[i][1] );
    }

    return( bg );
};

SpriteSheet.prototype.evenArray = function( width, height, x, y, tiles ) {
    var rv = [];
    var columns = Math.floor( width / x );
    var rows = Math.floor( height / y );

    for( var i = 0, il = tiles.length; i < il; i++ ) {
	var current = [ tiles[ i ] ];
	var row = Math.floor( i / columns );
	var col = i - ( row * columns );
	current.push( [ col * y, row * x ] );
	rv.push( current );
    }

    return( rv );
};

SDL.init( SDL.INIT.VIDEO );
SDL.IMG.init( 0 );

var screen = SDL.setVideoMode( 640, 480, 32, SDL.SURFACE.SWSURFACE );

SDL.WM.setCaption( 'Sample Application', 'Sample App' );
SDL.WM.setIcon( SDL.IMG.load( __dirname + '/eight.png' ) );

var bg = SDL.createRGBSurface( SDL.SURFACE.SWSURFACE | SDL.SURFACE.SRCALPHA, 128, 128 );

function tbMap ( n ) {
    var row = Math.floor( n / 22 );
    var col = n % 22;
    return( [ 33 * (col+1), 33 * (row+1), 32, 32 ] );
}

var flip = true;
var y = 352;
var x = 128;
var i = 14;
var sheet = new SpriteSheet( __dirname + '/tankbrigade.png', tbMap );
sheet.setColorKey( [1,1,1] );
sheet.load();

var bg_array = [
    71, 71, 71, 71, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 71, 72,
    71, 72, 72, 71, 70, 96, 96, 96, 96, 96, 96, 96, 70, 96, 96, 96, 96, 70, 71, 72,
    71, 72, 72, 71, 90, 71, 71, 71, 71, 71, 71, 71, 70, 71, 71, 71, 71, 70, 71, 72,
    71, 71, 72, 72, 90, 72, 72, 72, 71, 71, 71, 71, 70, 71, 71, 71, 71, 70, 71, 72,
    71, 71, 71, 71, 90, 71, 71, 72, 71, 71, 71, 71, 70, 71, 71, 71, 71, 70, 71, 72,
    71, 71, 71, 71, 70, 71, 71, 72, 72, 72, 71, 71, 70, 97, 97, 97, 97, 70, 71, 72,
    71, 71, 71, 71, 70, 71, 71, 72, 71, 71, 71, 71, 96, 96, 70, 96, 96, 96, 71, 72,
    71, 71, 71, 71, 70, 71, 71, 72, 71, 71, 71, 71, 71, 71, 90, 71, 71, 71, 71, 72,
    71, 71, 71, 71, 70, 71, 71, 72, 72, 72, 72, 72, 72, 72, 90, 72, 72, 72, 72, 72,
    71, 71, 71, 71, 70, 71, 71, 71, 71, 71, 71, 71, 71, 71, 90, 71, 71, 71, 71, 72,
    71, 71, 71, 71, 70, 71, 71, 71, 71, 71, 71, 71, 71, 71, 70, 71, 71, 71, 71, 72,
    71, 71, 71, 71, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 71, 71, 71, 71, 72,
    71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 72,
    71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 72,
    71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 71, 72
];

var bg = sheet.background( 640, 480, sheet.evenArray( 640, 480, 32, 32, bg_array ) );

setInterval( function () { flip = true; }, 50 );

function exuentOmnis ( exitCode ) {
    SDL.IMG.quit();
    process.exit( exitCode );
}

SDL.events.on( 'QUIT', function( evt ) { exuentOmnis( 0 ); } );
SDL.events.on( 'KEYDOWN', function ( evt ) {
    if( ( ( evt.sym === 99 ) && ( evt.mod === 64 ) ) ||
        ( ( evt.sym === 27 ) && ( evt.mod === 0  ) ) ) {
        exuentOmnis( 0 );
    }
    
    switch( evt.sym ) {
    case 273:
	velocity[0] -= 1;
	if( velocity[0] < -3 ) velocity[0] = -3;
	velocity[1] = 0;
	break;
    case 274:
	velocity[0] += 1;
	if( velocity[0] > 3 ) velocity[0] = 3;
	velocity[1] = 0;
	break;
    case 275:
	velocity[1] += 1;
	if( velocity[1] > 3 ) velocity[1] = 3;
	velocity[0] = 0;
	break;
    case 276:
	velocity[1] -= 1;
	if( velocity[1] < -3 ) velocity[1] = -3;
	velocity[0] = 0;
	break;
    }
} );

SDL.events.on( 'tick', function ( d ) {
    if( flip ) {
	SDL.blitSurface( bg, [0,0,640,480], screen, [0,0] );
	sheet.draw( screen, i, [ x, y ] );
	SDL.flip( screen );

	i++; if( i > 21 ) i = 14;

	y = y + velocity[0];
	if( y < 0 ) y = 0;
	if( y > 448 ) y = 448;

	x = x + velocity[1];
	if( x < 0 ) x = 0;
	if( x > 602 ) x = 602;

	flip = false;
    }
} );
