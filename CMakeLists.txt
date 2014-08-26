# CMakeLists.txt for compress - 20140825

cmake_minimum_required( VERSION 2.6 )

include (CheckFunctionExists)

project( compress )

if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    set(IS_64BIT_BUILD 1)
endif ()

if(CMAKE_COMPILER_IS_GNUCXX)
    set( WARNING_FLAGS -Wall )
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set( WARNING_FLAGS "-Wall -Wno-overloaded-virtual" )
endif() 

if(WIN32 AND MSVC)
    # turn off various warnings
    set(WARNING_FLAGS "${WARNING_FLAGS} /wd4996")
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305)
    # C4267: 'function' : conversion from 'size_t' to 'long', possible loss of data
    foreach(warning 4267)
        set(WARNING_FLAGS "${WARNING_FLAGS} /wd${warning}")
    endforeach()
    set( MSVC_FLAGS "-DNOMINMAX -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D__CRT_NONSTDC_NO_WARNINGS" )
    #set( NOMINMAX 1 )
    #if(MINGW)
        # need to specifically handle rc files, like resource compilation for mingw
        #ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_rc.o
        #                   COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
        #                                       -i${CMAKE_CURRENT_SOURCE_DIR}/test.rc
        #                                       -o ${CMAKE_CURRENT_BINARY_DIR}/test_rc.o)
        #SET(test_EXE_SRCS ${test_EXE_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/test_rc.o)
    #else(MINGW)
        #SET(test_EXE_SRCS ${test_EXE_SRCS} test.rc)
    #endif(MINGW)
else()
    # add any gcc flags
endif()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}" )

check_function_exists(rindex HAVE_RINDEX_FUNC)
if (HAVE_RINDEX_FUNC)
    add_definitions( -DHAVE_RINDEX_FUNC )
endif ()
add_definitions( -DBITS=16 )

if (MSVC)
    if (IS_64BIT_BUILD)
        set(name compress64)
    else ()
        set(name compress32)
    endif ()    
else ()
    set(name compress)
endif ()
set(dir src)
add_executable( ${name} ${dir}/compress.c ${dir}/compress.h )
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()

set(name btoa)
add_executable( ${name} ${dir}/btoa.c  )
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()

set(name atob)
add_executable( ${name} ${dir}/atob.c  )
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()

# eof
