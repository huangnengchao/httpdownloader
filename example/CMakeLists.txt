include(common_setting)


# 1. Add sources

include_directories(../src/)
include_directories(${curl_INCLUDE_PATH})
include_directories(${openssl_INCLUDE_PATH})
include_directories(${zlib_INCLUDE_PATH})

SET( EXAMPLE_SRC 
       example.cpp
       )


if(WIN)
add_definitions(-DUNICODE -D_UNICODE)
endif()


#add_executable(example ${EXAMPLE_SRC})

ADD_LIBRARY(imp_libcurl STATIC IMPORTED)
SET_TARGET_PROPERTIES(imp_libcurl PROPERTIES
    IMPORTED_LOCATION ${curl_LIB_PATH}/libcurl.a)
ADD_EXECUTABLE(example ${EXAMPLE_SRC})
TARGET_LINK_LIBRARIES(main imp_libcurl)


#target_link_libraries(example  ${CMAKE_INSTALL_PREFIX}/libhttpclient.a)
#target_link_libraries(example  ${curl_LIB_PATH}/libcurl.a)
#target_link_libraries(example  ${openssl_LIB_PATH}/libssl.a ${openssl_LIB_PATH}/libcrypto.a)
#target_link_libraries(example  ${zlib_LIB_PATH}/libz.a)

if(APPLE)
        target_link_libraries(example pthread dl ldap)
endif()


if(LINUX)
        target_link_libraries(example pthread dl)
endif()


INSTALL_TARGETS( /  example)

