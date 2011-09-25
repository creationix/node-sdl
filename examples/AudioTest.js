// Extremely simple demo of the SDL_mixer functions.
// Needs a lot of cleanup and doesn't demo mixing capabilities.
// However, if you put a wav file in ./sample.wav, it will actually play it.

var SDL = require( '../sdl' );

SDL.init( SDL.INIT.AUDIO );
SDL.MIXER.openAudio( 44100, SDL.MIXER.AUDIO_S16SYS, 2, 1024 );

var chunk;

function exuentOmnis ( exitCode ) {
    if( chunk ) {
        SDL.MIXER.freeChunk( chunk );
    }

    SDL.MIXER.closeAudio();
    process.exit( exitCode );
}

SDL.events.on( 'QUIT', function( evt ) { exuentOmnis( 0 ); } );

try {
    chunk = SDL.MIXER.loadWAV( __dirname + "/sample.wav" );
} catch( e ) {
    var message = "Uh oh; can't find sample.wav.\n\n" +
        "I'm using one from freesoundeffectsandloops.com. You can download it\n" +
        "with the command:\n\n" +
        "wget -O sample.wav http://www.freesoundeffectsandloops.com/SFX/freesoundeffectsandloops.comFREELFNKYDR105-1.wav"
    console.log( message );
    exuentOmnis( 2 );
}

var currentVolume = SDL.MIXER.volumeChunk( chunk, SDL.MIXER.MIX_MAX_VOLUME / 2 );
console.log( "chunk's initial volume was " + currentVolume );

var channels = SDL.MIXER.allocateChannels( 2 );
console.log( "allocated " + channels + " channel(s)" );

currentVolume = SDL.MIXER.volume( 0, SDL.MIXER.MIX_MAX_VOLUME );
console.log( "initial volume was " + currentVolume );

var channel = SDL.MIXER.playChannel( 0, chunk, -1 );
console.log( "will play on channel " + channel + ". Hit Ctrl-C to quit." );