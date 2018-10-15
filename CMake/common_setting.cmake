if (APPLE)
    set(BUILD_SYSTEM "mac")
elseif (LINUX)
    set(BUILD_SYSTEM "linux")
elseif (WIN)
    set(BUILD_SYSTEM "win")
else()
    message(FATAL_ERROR "Platform is not support!")
endif()

set(OS ${BUILD_SYSTEM})

if (LINUX)
  set(GT_LIB_POSTFIX "_linux")
elseif(APPLE)
  set(GT_LIB_POSTFIX "_mac")
elseif (WIN)
  set(GT_LIB_POSTFIX "_win")
else()
    message(FATAL_ERROR "System is not supported!")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "SuperRelease")
    set(GT_LIB_POSTFIX "${GT_LIB_POSTFIX}_release")
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GT_LIB_POSTFIX "${GT_LIB_POSTFIX}_debug")
elseif(CMAKE_BUILD_TYPE STREQUAL "FastDebug")
    set(GT_LIB_POSTFIX "${GT_LIB_POSTFIX}_debug")
else()
    message(FATAL_ERROR "Build Mode ${CMAKE_BUILD_TYPE} is not support!")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "SuperRelease")
  set(IMPORT_DIR ${BUILD_SYSTEM}/${COMPILER_CHOOSE}/release)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(IMPORT_DIR ${BUILD_SYSTEM}/${COMPILER_CHOOSE}/debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "FastDebug")
  set(IMPORT_DIR ${BUILD_SYSTEM}/${COMPILER_CHOOSE}/debug)
else()
  message(FATAL_ERROR "Build Mode ${CMAKE_BUILD_TYPE} is not support!")
endif()



set (curl_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/lib/${IMPORT_DIR}/curl/include)
set (curl_LIB_PATH ${PROJECT_SOURCE_DIR}/lib/${IMPORT_DIR}/curl/lib)


set (openssl_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/lib/${IMPORT_DIR}/openssl/include)
set (openssl_LIB_PATH ${PROJECT_SOURCE_DIR}/lib/${IMPORT_DIR}/openssl/lib)


set (zlib_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/lib/${IMPORT_DIR}/zlib/include)
set (zlib_LIB_PATH ${PROJECT_SOURCE_DIR}/lib/${IMPORT_DIR}/zlib/lib)


if(APPLE)
	find_path(qt_LIB_PATH libz.dylib
	         HINTS /usr/lib)
endif()
