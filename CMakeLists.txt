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

execute_process(
	COMMAND bash -c "echo ${CMAKE_PROJECT_VERSION} > ${CMAKE_CURRENT_SOURCE_DIR}/version"
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
include(ConanInit)

add_conan_remote(bincrafters https://api.bintray.com/conan/bincrafters/public-conan)
add_conan_remote(scatter https://api.bintray.com/conan/edwardstock/scatter)
conan_init()


set(DISABLE_JNI ON)

if (APPLE)
	set(CMAKE_SKIP_BUILD_RPATH FALSE)
	set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
	set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
	set(CMAKE_MACOSX_RPATH 1)
endif ()



add_subdirectory(src/tx)
add_subdirectory(src/wallet)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)



#add_subdirectory(libs/eth)