var SDL = require( '../sdl' );

SDL.init( SDL.INIT.VIDEO );
SDL.IMG.init( SDL.IMG.INIT.PNG );

var screen = SDL.setVideoMode( 640, 480, 32, SDL.SURFACE.SWSURFACE );

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
} );