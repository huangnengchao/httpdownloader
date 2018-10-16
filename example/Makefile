OS_NAME = $(shell uname -o)
LC_OS_NAME = $(shell echo $(OS_NAME) | tr '[A-Z]' '[a-z]')
ifeq ($(LC_OS_NAME), gnu/linux)
CUR_OS = "current os is linux"
LD_LIB = -lpthread -ldl
$(warning $(CUR_OS))
else
CUR_OS = "current os is mac"
LD_LIB = -lpthread -ldl -lldap
$(warning $(CUR_OS))
endif

example: example.cpp ../lib/libhttpdownloader.a ../lib/libcurl.a ../lib/libssl.a ../lib/libcrypto.a ../lib/libz.a
	g++ -std=c++11 -o example example.cpp ../lib/libhttpdownloader.a ../lib/libcurl.a ../lib/libssl.a ../lib/libcrypto.a ../lib/libz.a $(LD_LIB)