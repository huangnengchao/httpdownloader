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



ADD_LIBRARY(httpdownloader  STATIC ${HTTP_DOWNLOADER_SRC})

link_libraries("-Wl,--whole-archive" ${curl_LIB_PATH}/libcurl.a)

add_library(imp_libcurl STATIC IMPORTED)
set_property(TARGET imp_libcurl PROPERTY IMPORTED_LOCATION "${curl_LIB_PATH}/libcurl.a")
TARGET_LINK_LIBRARIES(httpdownloader imp_libcurl)


#TARGET_LINK_LIBRARIES(httpdownloader   "-Wl,--whole-archive" ${curl_LIB_PATH}/libcurl.a)
#TARGET_LINK_LIBRARIES(httpdownloader  ${openssl_LIB_PATH}/libssl.a ${openssl_LIB_PATH}/libcrypto.a)
#TARGET_LINK_LIBRARIES(httpdownloader  ${zlib_LIB_PATH}/libz.a)

if(APPLE)
        target_link_libraries(httpdownloader pthread dl)
endif()


if(LINUX)
        target_link_libraries(httpdownloader pthread dl)
endif()


INSTALL (TARGETS httpdownloader LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
#INSTALL_TARGETS( /lib  httpdownloader)

INSTALL(FILES http_downloader.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)

if(APPLE)
	INSTALL(FILES ${curl_LIB_PATH}/libcurl.a ${zlib_LIB_PATH}/libz.a ${openssl_LIB_PATH}/libssl.a ${openssl_LIB_PATH}/libcrypto.a DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()

if(LINUX)
	INSTALL(FILES ${curl_LIB_PATH}/libcurl.a ${zlib_LIB_PATH}/libz.a ${openssl_LIB_PATH}/libssl.a ${openssl_LIB_PATH}/libcrypto.a DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()

if(WIN)
	INSTALL(FILES ${curl_LIB_PATH}/libcurl.a DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
endif()

INSTALL(DIRECTORY ../example DESTINATION ${CMAKE_INSTALL_PREFIX})
