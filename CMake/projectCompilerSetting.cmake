message("run ${COMPILER_CHOOSE} setting ...") 
    
# We use intel compiler to generate codes
#include(CMakeForceCompiler)


IF ((COMPILER_CHOOSE STREQUAL "gcc") OR (COMPILER_CHOOSE STREQUAL "clang"))
    ##################### DEFAULT GCC Compiler Setting #####################################

    #Define our configuration types
    if(CMAKE_CONFIGURATION_TYPES)
       list(APPEND CMAKE_CONFIGURATION_TYPES SuperRelease Debug FastDebug)
       list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)
       set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
         "Add the configurations that we need"
         FORCE)
    endif()
    
    #Following Compiler option is used for linux x64
    if (APPLE)
        set(CMAKE_CXX_COMPILER "clang++" ) 
        set(CMAKE_C_COMPILER "clang" )  
    else()
        set(CMAKE_CXX_COMPILER "g++") 
        set(CMAKE_C_COMPILER "gcc" )  
    endif()


    if (APPLE)
       set(OS_DEP_DEFINE  " -DMACOSX -DQT_QML_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB")
       set(OS_LFLAGS_DEFINE "")
    else()
       set(OS_DEP_DEFINE  "")
       set(OS_LFLAGS_DEFINE " -static-libstdc++  -static-libgcc ") # -static-libstdc++ 
    endif()
   
    if (APPLE)
    	set( CMAKE_EXE_LINKER_FLAGS " " )
    else()
        set( CMAKE_EXE_LINKER_FLAGS " -static-libstdc++  -static-libgcc ")
    endif()
	
	if (USE_32BITS)
		 set(OS_DEP_DEFINE  "  ${OS_DEP_DEFINE} -DUSE_32BITS ")
    else()
		 set(OS_DEP_DEFINE  "  ${OS_DEP_DEFINE}  -m64 ")
	endif()
	
    set( COMMON_COMPILER_FLAGS "-Wall  -Wno-unused-local-typedefs  -fexceptions  -DTBB_USE_EXCEPTIONS -fPIC") 
    set( COMMON_CPP_COMPILER_FLAGS "  -Wno-reorder ${COMMON_COMPILER_FLAGS}  ${OS_DEP_DEFINE} -std=c++11 ") 
    set( COMMON_C_COMPILER_FLAGS "-Wall ${COMMON_COMPILER_FLAGS} ${OS_DEP_DEFINE} -std=gnu89 -Wall -Wextra -fPIC ")
    set( COMMON_LINKER_FLAGS  "   ${OS_LFLAGS_DEFINE}    " ) #   
    
        # SuperRelease mode
           set( CMAKE_CXX_FLAGS_SUPERRELEASE " -O3  ${COMMON_CPP_COMPILER_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -DBOOST_DISABLE_ASSERTS " CACHE STRING #  -ftracer  -fivopts -ftree-loop-linear -fforce-addr -fprefetch-loop-arrays -funroll-loops" CACHE STRING
            "Flags used by the C++ compiler during superrelease builds."
            FORCE )
           set( CMAKE_C_FLAGS_SUPERRELEASE "-O3   ${COMMON_C_COMPILER_FLAGS} ${CMAKE_C_FLAGS_RELEASE}  -DNDEBUG -DBOOST_DISABLE_ASSERTS " CACHE STRING #  -ftracer  -fivopts -ftree-loop-linear -fforce-addr -fprefetch-loop-arrays  -funroll-loops" CACHE STRING
            "Flags used by the C++ compiler during superrelease builds."
            FORCE )
           set( CMAKE_SHARED_LINKER_FLAGS_SUPERRELEASE 
            " ${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${COMMON_LINKER_FLAGS} " CACHE STRING
            "Flags used by the shared libraries linker during superrelease builds."
            FORCE )
           set( CMAKE_EXE_LINKER_FLAGS_SUPERRELEASE  
            " ${CMAKE_EXE_LINKER_FLAGS_RELEASE}  ${COMMON_LINKER_FLAGS}" CACHE STRING #
            "Flags used by the shared libraries linker during superrelease builds."
            FORCE )
        
    
        # Debug Mode
        SET( CMAKE_CXX_FLAGS_DEBUG "-O0 ${COMMON_CPP_COMPILER_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG_INIT}   -DDEBUG -g  -fno-omit-frame-pointer "  CACHE STRING
            "Flags used by the C++ compiler during debug builds."
            FORCE )
        SET( CMAKE_C_FLAGS_DEBUG " -g -O0 ${COMMON_C_COMPILER_FLAGS} ${CMAKE_C_FLAGS_DEBUG_INIT} -DDEBUG  -fno-omit-frame-pointer "   CACHE STRING
            "Flags used by the C++ compiler during debug builds."
            FORCE )
        SET( CMAKE_SHARED_LINKER_FLAGS_DEBUG 
            "${CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT}  ${COMMON_LINKER_FLAGS}  -fno-omit-frame-pointer "  CACHE STRING # ${COMMON_LINKER_FLAGS}
            "Flags used by the shared libraries linker during debug builds."
            FORCE )
        SET( CMAKE_EXE_LINKER_FLAGS_DEBUG 
            "${CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT} ${COMMON_LINKER_FLAGS}  -fno-omit-frame-pointer " CACHE STRING
            "Flags used by the exe libraries linker during debug builds."
            FORCE )
    

        # Fast Debug Mode
        SET( CMAKE_CXX_FLAGS_FASTDEBUG "-O0 -g ${COMMON_CPP_COMPILER_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG_INIT}  -DNDEBUG -DBOOST_DISABLE_ASSERTS -g "  CACHE STRING
            "Flags used by the C++ compiler during fast debug builds."
            FORCE )
        SET( CMAKE_C_FLAGS_FASTDEBUG "-O0 -g ${COMMON_C_COMPILER_FLAGS} ${CMAKE_C_FLAGS_DEBUG_INIT}  -DNDEBUG -DBOOST_DISABLE_ASSERTS -g   "   CACHE STRING
            "Flags used by the C++ compiler during fast debug builds."
            FORCE )
        SET( CMAKE_SHARED_LINKER_FLAGS_FASTDEBUG 
            "${CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT}   ${COMMON_LINKER_FLAGS} "  CACHE STRING # ${COMMON_LINKER_FLAGS}
            "Flags used by the shared libraries linker during fast debug builds."
            FORCE )
        SET( CMAKE_EXE_LINKER_FLAGS_FASTDEBUG 
            "${CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT} ${COMMON_LINKER_FLAGS}  " CACHE STRING
            "Flags used by the exe libraries linker during fast debug builds."
            FORCE )

        MARK_AS_ADVANCED(
            CMAKE_CXX_FLAGS_SUPERRELEASE
            CMAKE_C_FLAGS_SUPERRELEASE
            CMAKE_EXE_LINKER_FLAGS_SUPERRELEASE
            CMAKE_SHARED_LINKER_FLAGS_SUPERRELEASE
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_C_FLAGS_DEBUG
            CMAKE_EXE_LINKER_FLAGS_DEBUG
            CMAKE_SHARED_LINKER_FLAGS_DEBUG
	    CMAKE_CXX_FLAGS_FASTDEBUG
	    CMAKE_C_FLAGS_FASTDEBUG
	    CMAKE_SHARED_LINKER_FLAGS_FASTDEBUG
	    CMAKE_EXE_LINKER_FLAGS_FASTDEBUG
             )

endif()
