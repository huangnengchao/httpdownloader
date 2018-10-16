include(common_setting)


# 1. Add sources

include_directories(./)
include_directories(${curl_INCLUDE_PATH})


SET( HTTP_DOWNLOADER_SRC 
       http_client.cpp
       http_downloader.cpp
       )


if(WIN)
add_definitions(-DUNICODE -D_UNICODE)
endif()


ADD_LIBRARY(httpdownloader STATIC ${HTTP_DOWNLOADER_SRC})

target_link_libraries(httpdownloader  ${curl_LIB_PATH}/libcurl.a)
target_link_libraries(httpdownloader  ${openssl_LIB_PATH}/libssl.a ${openssl_LIB_PATH}/libcrypto.a)
target_link_libraries(httpdownloader  ${zlib_LIB_PATH}/libz.a)


if(APPLE)
        target_link_libraries(httpdownloader pthread dl)
endif()


if(LINUX)
        target_link_libraries(httpdownloader pthread dl)
endif()


INSTALL_TARGETS( /lib  httpdownloader)

INSTALL(FILES http_downloader.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

INSTALL(FILES ${curl_LIB_PATH}/libcurl.a ${zlib_LIB_PATH}/libz.a ${openssl_LIB_PATH}/libssl.a ${openssl_LIB_PATH}/libcrypto.a DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)


INSTALL(DIRECTORY ../example DESTINATION ${CMAKE_INSTALL_PREFIX})
