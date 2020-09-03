# asadharon-ascii-renderer
scanline based ascii renderer. runs in terminal using ncurses.


nintendo 64 link and mario rendered in the terminal using vertex data downloaded from a questionable website:
![wait](https://github.com/narutouzumaki798/tttttt/blob/master/64.gif)
movements are hardcoded. no input handling for now.

edge/scanline intersection done incrementally, integer-only calculation.  
depth calculations have to be fixed, currently ray triangle intersection.

obj files can have only triangles, for now. only vertex coordinates and face data is used.

requires ncurses

build: g++ engine.cpp -lncurses

better ascii renderers:  
https://github.com/interl0per/Console-Graphics  
https://github.com/DinoZ1729/Ray
