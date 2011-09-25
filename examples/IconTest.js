#!/usr/local/bin/node

var icons = require( './Icons' );
var SDL   = require( '../sdl' );
var screen;

init();

var iconNames = [ 'saturn', 'gear', 'world' ];
var x = 16;
var y = 16;

for( var i = 0, il = iconNames.length; i < il; i++ ) {
    var temp = SDL.IMG.loadbuf( icons.getIcon( iconNames[ i ] ) );
    var currentIcon = SDL.displayFormat( temp );
    SDL.freeSurface( temp );
    SDL.setAlpha( currentIcon, SDL.SURFACE.SRCALPHA | SDL.SURFACE.RLEACCEL, 192 );
    SDL.setColorKey( currentIcon, SDL.SURFACE.SRCCOLORKEY, 0x00000000 );
    SDL.fillRect( screen, [x - 8, y - 8, 144, 144], 0x5F5F5FAF );
    SDL.blitSurface( currentIcon, [0, 0, 128, 128], screen, [x, y] );
    y += 156;
}

SDL.flip( screen );

function init ( ) {
    SDL.init( SDL.INIT.VIDEO );
    SDL.IMG.init( 0 );

    SDL.WM.setCaption( 'Icon Test', 'Icon Test' );
    SDL.WM.setIcon( SDL.IMG.loadbuf( icons.getIcon( 'gear' ) ) );

    SDL.events.on( 'QUIT', exuentOmnis );
    SDL.events.on( 'KEYDOWN', keyHandler );

    screen = SDL.setVideoMode( 160, 472, 32, SDL.SURFACE.SWSURFACE );
}

function exuentOmnis ( exitCode ) {
    if( 'undefined' === typeof( exitCode ) ) {
	exitCode = 0;
    }

    SDL.IMG.quit();
    process.exit( exitCode );
}

function keyHandler ( evt ) {
    if( ( ( evt.sym === 99 ) && ( evt.mod === 64 ) ) ||
        ( ( evt.sym === 27 ) && ( evt.mod === 0  ) ) ) {
        exuentOmnis( 0 );
    }
}