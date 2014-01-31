# node-sdl ( Simple DirectMedia Layer bindings for node.js )

## 0. Installation

Currently, installation is finicky and largely depends upon my specific system for now. A future release is planned to make this process better. If you want to get it working, you need to have the dylibs mentioned in bindings.gyp under /usr/local/lib. (this includes the main SDL2 dylib, SDL2_ttf, and SDl2_image)

If you have those libraries, and clone node-sdl, you can build it with

<pre> node-gyp configure build </pre>

## 1. Usage

As a general rule, these bindings adhere to the following conventions.

* SDL structs are wrapped at the base level of the bindings. If you want a Window, it will be under sdl.Window.
* Structs that you would normally pass to various functions are instead wrapped as objects with prototype functions, and are created with the new keyword. As an example, instead of doing something such as <pre>sdl.GetWindowWidth(window)</pre>, you would instead do <pre>var window = new sdl.Window(...); window.getWidth()</pre>.
* Constants, enums, etc. are split up into various namespaces below the base namespace, based on where the underscores are in their name. This roughly translates <pre>SDL_WINDOWPOS_CENTERED</pre> into <pre>sdl.WINDOWPOS.CENTERED</pre>.
* Extensions to the base SDL API are under their own namespace. While you would find <pre>sdl.Window</pre> or <pre>sdl.Renderer</pre>, anything under the SDL_ttf library would be under <pre>sdl.TTF</pre> If you want to make a font, you would use <pre>new sdl.TTF.Font</pre>.

## 2. Specific usage

### 2.1 Initialization and Shutdown

Explaining the basics behind finding pieces of the SDL API is all well and good, but examples are still the best. So here is a quick and easy way to create a new window using node-sdl.

<pre>sdl.init(sdl.INIT.EVERYTHING); // Initialize all SDL subsystems.
var window = new sdl.Window("Test Window", sdl.WINDOWPOS.CENTERED, sdl.WINDOWPOS.CENTERED, 640, 480);
setTimeout(function() { sdl.quit(); }, 2000);</pre>

It's that easy. Though one thing to be aware of: in that example I declared a window in the global space. Because that is the only place it is referenced, once the script finishes (meaning after the call to setTimeout) there will be no more references to it. That means the window will get garbage collected sometime in the future. And because the bindings handle destroying SDL objects when the wrapping object gets destructed, that means the window will disappear, seemingly randomly. Make sure you keep a reference to all objects you want to persist somewhere, or you might find your window disappearing without warning.

### 2.2 Events

Currently, events are wrapped as a pure Javascript object. So trying to access properties of the event that don't exist for that event will give back undefined. You can determine the exact type of event by checking <pre>event.type</pre> just like in SDL. Same goes for all the other properties. If an event would have a key member, that will be in the Javascript object, etc.

## 3. License

Copyright (c) 2014 Tim "creationix" Caswell

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
