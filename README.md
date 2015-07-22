DepthView
=========
is a basic stereoscopic 3D image viewer for .jps and .pns files. 

Render Modes
------------
* Red/Cyan Anglaph (full-color, half-color, and greyscale)
* Side by Side (with mirror)
* Top/Bottom (also with mirror)
* Interlaced (horizontal and vertical, fullscreen only)
* Checkerboard (fullscreen only)
* Single Image (Mono)

Dependencies
------------
* [CMake]
* [Qt]

Building
--------
* Clone or download the source code.
* In the source folder, create a `build` folder.
* In the build folder, run `cmake ..`. Add any options listed below to enable them.
* To use a different generator than your platform default, add `-G <generator>` to the cmake command with your desired generator. A list of generators can be found by running `cmake -h`.
* The project files should now be generated in `build`.

Build options
-------------
* -DDEPTHVIEW_PORTABLE=On	- Builds a portable version that stores settings in the same directory as the executable.

License
-------
[MIT License]


[CMake]:http://www.cmake.org
[Qt]:http://qt-project.org
[MIT License]:LICENSE
