cmake_minimum_required(VERSION 3.1)

set(CMAKE_VERBOSE_MAKEFILE ON)

include( ./CMake/projectCompilerSetting.cmake )


PROJECT( httpclient )


if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
   set(LINUX True)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
   set(WIN True)
   
   # handle bigger than 2G file in 32 or 64 bit system
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_FILE_OFFSET_BITS=64  ")  
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64  ")
   
   if(USE_32BITS)  
      message(STATUS "*****************using 32bits*************************")  
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32  ")  
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32  ")  
   else()  
      message(STATUS "*****************using 64bits*************************")  
   endif(USE_32BITS) 
   
endif()

# Add custom modules path
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMake )

message("CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message("CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")


ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(example)

add_custom_target(httpclient-all ALL )
add_dependencies(httpclient-all httpclient)

