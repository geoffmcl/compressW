Compression, based on the LZW compression algorithm.
----------------------------------------------------

This is my Windows port of the 1985 Revison 4.0 of the 'compress' application, now bumped
to Revision 4.1.

The first step was to add a CMakeLists.txt to do the building. This should allow 
the source to be built by a large number of tools, in a cross platform way.

Further I converted all functions from the original K&R style, to modern C 
style. Added more headers, and supplied forward reference such that is should
compile without error or warnings, at least when using the MSVC compiler/linker.

And some effort has been done to tidy up the indentation of the code, to make it 
more readable.

The building should be done in the 'build' folder. There is a build-me.bat 
there to facilitate this. Your system may be missing some of the tools used in 
that batch file, but if you have cmake and msvc installed, then it should 
complete its job.

But even if you do not use this batch file, the building is simple -

> cd build  
> cmake ..  
> cmake --build . --config release  

The project also produces btoa/atob sample applications, like uuencode/uudecode.
Running btoa, and inputing a string will produce an encoded character string,
plus an 'xbtoa End N ?? c ?....' sequences which can be passed to atob to decode 
back to the original string.

Enjoy.

Geoff.
20140826
