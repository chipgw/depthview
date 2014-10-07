DepthView
=========
is a basic stereoscopic 3D image viewer for .jps and .pns files. 

Render Modes
------------
* Red/Cyan Anglaph (full-color, half-color, and greyscale)
* Side by Side (with mirror)
* Top/Tottom (also with mirror)
* Interlaced (horizontal and vertical, not fully tested)
* Checkerboard (also not fully tested)
* Single Image (Mono)

Dependencies
------------
* [CMake]
* [Qt]
* [GLM]
* [TinyXml]

Building
--------
* Clone or download the source code.
* In the source folder, create a `build` folder.
* In the build folder, run `cmake ..`, Add -D<option>=ON with any option listed below in place of <option> to enable that option.
* If you want to use a different generator than your platform default, add `-G <generator>` to the cmake command, with your desired generator. A list of generators can be found by running `cmake -h`.
* The project files should now be generated in `build`.

Build options
-------------
* DEPTHVIEW_PORTABLE	- Builds a portable version that stores settings in the same directory as the executable.
* DEPTHVIEW_USE_QT4	- Builds against Qt 4 instead of 5.

License
-------
[MIT License]


[CMake]:http://www.cmake.org
[Qt]:http://qt-project.org
[MIT License]:LICENSE
