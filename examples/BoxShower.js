var SDL = require( '../sdl' );

SDL.init( SDL.INIT.VIDEO );
SDL.IMG.init( 0 );

var doit = true;

var screen = SDL.setVideoMode( 640, 480, 32, SDL.SURFACE.SWSURFACE );
var rectangles = [];
var colors = [ 0xFFFF0000, 0xFF00FF00, 0xFF0000FF ];

for( var i = 0, il = colors.length; i < il; i++ ) {
    var surface = SDL.createRGBSurface( SDL.SURFACE.SWSURFACE | SDL.SURFACE.SRCALPHA, 20, 20 );
    SDL.fillRect( surface, [2, 2, 16, 16], colors[ i ] & 0x3FFFFFFF );
    SDL.fillRect( surface, [4, 4, 12, 12], colors[ i ] );
    rectangles.push( surface );
}

var rl = rectangles.length;
var count = 0;

SDL.WM.setCaption( 'Sample Application', 'Sample App' );
SDL.WM.setIcon( SDL.IMG.load( __dirname + '/eight.png' ) );

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

    if( ( evt.sym == 99 ) && ( evt.mod === 0 ) ) {
	if( doit ) {
	    doit = false;
	} else {
	    doit = true;
	}
    }
} );

SDL.events.on( 'tick', function ( d ) {
    if( doit ) {
	var x = Math.floor( Math.random() * (screen.w - 20 ) );
	var y = Math.floor( Math.random() * (screen.h - 20 ) );
	SDL.blitSurface( rectangles[ count % rl ], null, screen, [x, y] );
	SDL.flip( screen );
	count++;
    }
} );
