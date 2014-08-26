Compression, based on the LZW compression algorithm.
----------------------------------------------------

This is my Windows port of the 1985 Revison 4.0 of the 'compress' application, now bumped
to Revision 4.1.

The first step was to add a CMakeLists.txt to do the building. This should allow 
the source to be built by a large number of tools, in a cross platform way.

Further I converted all functions from the original K&R style, to modern C 
style. Added more headers, and supplied forward reference such that is should
compile without error or warnings, at least when using the MSVC compiler/linker,
and in Ubuntu 14.04 LTS linux.

And some effort has been done to tidy up the indentation of the code, to make it 
more readable.

The building should be done in the 'build' folder. There is a build-me.bat / build-me.sh
there to facilitate this. Your system may be missing some of the tools used in 
that batch file, but if you have cmake and msvc installed, then it should 
complete its job.

For those with building WIN64 capability, there is a build.x64 folder for this,
again with a build-me.bat that first sets up the 64-bit build environment, then does 
the build. If you are not using MSVC10, or it is installed diffently, then this 
batch file may need some TLC.

But even if you do not use these batch files, the building is simple -

> cd build  
> cmake ..  
> cmake --build . --config release  

In *nix the 3rd line can just be 'make' if a unix makefile is the generator of your 
choice. Alternatively you can use the cmake GUI, setting the source folder, and the 
binary directory to the 'build' folder.

The project also produces btoa/atob sample applications, like uuencode/uudecode.
Running btoa, and inputing a string will produce an encoded character string,
plus an 'xbtoa End N ?? c ?....' sequences which can be passed to atob to decode 
back to the original string.

This <a href="http://en.wikipedia.org/wiki/Compress" target="_blank">wiki</a> page gives
more information, especially the history of using this algorithm. Of course during the 
years that unisys held the patent, <a href="http://www.gzip.org/" target="_blank">gzip</a>
and <a href="http://bzip.org/" target="_blank">bzip2</a> became the preferred compression 
tools, and they usually achieve a sometimes significant increased compression ratio.

But compress still works, and on a simple ascii file usually achieves a 40-50% compression
ratio.

Enjoy.

Geoff.
20140826
