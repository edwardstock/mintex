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

set(CMAKE_CXX_FLAGS_DEBUG -gdwarf-3)

execute_process(
	COMMAND bash -c "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
include(ConanInit)

add_conan_remote(bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
add_conan_remote(scatter https://api.bintray.com/conan/edwardstock/scatter)
conan_init()


set(DISABLE_JNI ON)
option(ENABLE_JNI OFF)
option(ENABLE_GO OFF)

if (APPLE)
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	set(CMAKE_MACOSX_RPATH 1)
endif ()

add_subdirectory(src/tx)
add_subdirectory(src/wallet)
add_subdirectory(src/resp_generator)

add_dependencies(mintex-wallet resp_generator)