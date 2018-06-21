# Core
Light-weight, C/C++ based, 3D rendering library

External dependencies:

- Assimp
- libpng
- libtiff
- libjpeg

Notes on dependencies:

Static version of each library are required. Each can typically be configured to produce a static library as the final build product, but are configured by default (for the most part) to produce shared libraries.

The libjpeg source bundle is the only library that is not straightforward to build. Each of the config* files in the source bundle had to be converted to unix format before successfully building on a unix system. Additionaly, the 'make' step was initially failing with errors like '.deps/jmemnobs.Plo: No such file or directory' after running 'configure'. To fix that it is nmecessary to run 'autoreconf -if' before running the 'configure' command.
