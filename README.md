# node-sdl ( Simple DirectMedia Layer bindings for node.js )

## Installation

Installation of the node-sdl package is straight-forward: first clone the
package using git, then build the C++ portion of the package with the
node-waf command.

First, check out the source from github:

<pre>    git clone https://github.com/creationix/node-sdl.git</pre>

Second, build the package:

<pre>    cd node-sdl
    node-waf configure build</pre>

You can test if the package was properly built by running one or more of the
example programs:

<pre>    cd examples
    node img.js</pre>

This package depends on the SDL libraries being present on the target system.
The following command was required to install these libraries on a "stock"
Ubuntu 11.04 install:

<pre>    sudo apt-get install libsdl1.2-dev libghc6-sdl-ttf-dev libghc6-sdl-image-dev</pre>

## Usage

### Initialization and Shutdown

Begin by requiring the node-sdl package and calling the init() function:

<pre>    var SDL = require( 'sdl' );
    SDL.init( SDL.INIT.VIDEO )</pre>

The init() function takes a numeric parameter telling the library what
subsystems to initialize. The node-sdl package defines the following
constants:

<pre>    SDL.INIT.TIMER - initializes timers (not currently supported)
    SDL.INIT.AUDIO - initialize audio subsystem (not currently supported)
    SDL.INIT.VIDEO - initialize video subsystem
    SDL.INIT.CDROM - initialize CD playback subsystem (not currently supported)
    SDL.INIT.JOYSTICK - initialize joystick support
    SDL.INIT.EVERYTHING - all of the above
    SDL.INIT.NOPARACHUTE - don't catch fatal signals
</pre>

Two or more of these parameters may be selected by or-ing them together:

<pre>    SDL.init( SDL.INIT.VIDEO | SDL.INIT.JOYSTICK );</pre>

The QUIT event signals the closure of a SDL managed window, so adding a
function that exits the application when it is received may be useful:

<pre>    SDL.events.on( 'QUIT', function( evt ) { process.exit( 0 ); } );</pre>

Exiting the application when the user presses Control-C or the Escape key
can be achieved by adding a listener to the KEYDOWN event:

<pre>    SDL.events.on( 'KEYDOWN', function ( evt ) {
      if( ( ( evt.sym === 99 ) && ( evt.mod === 64 ) ) ||
          ( ( evt.sym === 27 ) && ( evt.mod === 0  ) ) ) {
        process.exit( 0 );
      }
    } );</pre>

### Video Functions

To create a window under SDL control, use the setVideoMode() function to 
create a "surface".

<pre>    var screen = SDL.setVideoMode( 640, 480, 32, SDL.SURFACE.SWSURFACE );</pre>

### Image Functions

This package uses a supplimentary image library intended to make it easy for
node-sdl applications to load and use JPG, PNG or TIFF images. Before using
Image functions, you should initalize them with the image init() function:

<pre>    SDL.IMG.init( SDL.IMG.INIT.PNG );</pre>

These three constants may be or'ed together to support more than one image
format:

<pre>    SDL.IMG.INIT.JPG - JPEG Support
SDL.IMG.INIT.PNG - PNG Support
SDL.IMG.INIT.TIF - TIFF Support</pre>

To load an image into memory, use the image load() function. It takes a file
path as a parameter and returns a reference to it. The following line loads
a PNG file called "foo.png".

<pre>    SDL.IMG.load( __dirname + '/foo.png' );</pre>

After you are finished using the image functions, be sure to use the image
quit() function:

<pre>    SDL.IMG.quit();</pre>

### Window Manager Functions

node-sdl is capable of setting window manager related info with the SDL.WM.*
functions.

To set the title of a SDL window, use the setCaption() function. This fragment
sets the window's title to "Window Title" and (if supported by your window
manager) sets the name of the minimized icon to "Icon Title"

<pre>    SDL.WM.setCaption( 'Window Title', 'Icon Title' );</pre>

To set the application's icon, use the setIcon() function. It expects an image
to be passed as it's parameter, so it's common practice to use the image load()
function. The following example loads an icon from the file 'eight.png' and
uses it as the app's icon:

<pre>    SDL.WM.setIcon( SDL.IMG.load( __dirname + '/eight.png' ) );</pre>