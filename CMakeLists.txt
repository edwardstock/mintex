cmake_minimum_required(VERSION 3.13)
project(mintex
        VERSION 0.0.1
        DESCRIPTION "Minter Text Console"
        HOMEPAGE_URL "https://github.com/edwardstock/mintex"
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 14)
set(RP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(RP_BD ${CMAKE_BINARY_DIR})
set(MINTER_CHAIN "mainnet" CACHE STRING "Chain ID")

if (MINTER_CHAIN STREQUAL "mainnet")
	message(STATUS "Network: ${MINTER_CHAIN}")
elseif (MINTER_CHAIN STREQUAL "testnet")
	message(STATUS "Network: ${MINTER_CHAIN}")
else ()
	message(FATAL_ERROR "Unknown Network flavor: ${MINTER_CHAIN}")
endif ()
add_definitions(-DMINTER_CHAIN=\"${MINTER_CHAIN}\")

set(CMAKE_CXX_FLAGS_DEBUG -gdwarf-3)

execute_process(
	COMMAND bash -c "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
include(ConanInit)

add_conan_remote(bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
add_conan_remote(scatter https://api.bintray.com/conan/edwardstock/scatter)
add_conan_remote(edwardstock https://api.bintray.com/conan/edwardstock/conan-public)
conan_init()


set(DISABLE_JNI ON)
option(ENABLE_JNI OFF)
option(ENABLE_GO OFF)

# linux clang
if (NOT APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "-Wl,-rpath=${CMAKE_BINARY_DIR}/lib ${CMAKE_CXX_FLAGS} ")

	# MOJAVE
elseif (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	set(CMAKE_MACOSX_RPATH 1)
	set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -std=c++14 -stdlib=libc++ -undefined dynamic_lookup")

	# apple clang
elseif (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	set(CMAKE_MACOSX_RPATH 1)
	set(CMAKE_CXX_FLAGS "-stdlib=libc++ ${CMAKE_CXX_FLAGS}")
	set(CMAKE_CXX_LINKER_FLAGS "${CMAKE_CXX_LINKER_FLAGS} -std=c++14 -stdlib=libc++ -undefined dynamic_lookup")

else ()
	set(CMAKE_CXX_FLAGS "-rdynamic ${CMAKE_CXX_FLAGS}    ") #-Wl,-rpath=${CMAKE_INSTALL_PREFIX}/lib
	set(CMAKE_CXX_LINKER_FLAGS "")
endif ()

add_subdirectory(src/wallet)
add_subdirectory(src/resp_generator)

add_dependencies(mintex-wallet resp_generator)