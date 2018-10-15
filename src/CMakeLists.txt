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


ADD_LIBRARY(httpclient STATIC ${HTTP_DOWNLOADER_SRC})

target_link_libraries(httpclient  ${curl_LIB_PATH}/libcurl.a)
target_link_libraries(httpclient  ${openssl_LIB_PATH}/libssl.a ${openssl_LIB_PATH}/libcrypto.a)
target_link_libraries(httpclient  ${zlib_LIB_PATH}/libz.a)


if(APPLE)
        target_link_libraries(httpclient pthread dl)
endif()


if(LINUX)
        target_link_libraries(httpclient pthread dl)
endif()


INSTALL_TARGETS( /  httpclient)

