cmake_minimum_required(VERSION 3.10)
project(mintex-wallet
        VERSION 0.0.1
        DESCRIPTION "Minter Text Console"
        HOMEPAGE_URL "https://github.com/edwardstock/mintex"
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 14)


set(INCLUDE_FILES
    src/base/base_repository.h
    src/repository/explorer_repository.h)

set(SOURCE_FILES
    ${INCLUDE_FILES}
    main.cpp
    src/base/base_repository.cpp)

option(JSON_BuildTests "" OFF)
option(JSON_MultipleHeaders "" OFF)

add_subdirectory(${RP_DIR}/libs/json ${CMAKE_BINARY_DIR}/json EXCLUDE_FROM_ALL)
add_subdirectory(${RP_DIR}/libs/leveldb ${CMAKE_BINARY_DIR}/leveldb EXCLUDE_FROM_ALL)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_compile_options(${PROJECT_NAME} PUBLIC -fexceptions)

target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_include_directories(${PROJECT_NAME} PRIVATE ${RP_DIR}/libs/json/include)


target_link_libraries(${PROJECT_NAME} CONAN_PKG::httb)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::boost_program_options)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::boost_log)
target_link_libraries(${PROJECT_NAME} bip39_core)
target_link_libraries(${PROJECT_NAME} secp256k1_core)
target_link_libraries(${PROJECT_NAME} leveldb)
target_link_libraries(${PROJECT_NAME} devcore)
target_link_libraries(${PROJECT_NAME} mintex_uint256)
target_link_libraries(${PROJECT_NAME} mintex_tx)


target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/leveldb/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CONAN_INCLUDE_DIRS_HTTB})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#if (WITH_TEST)
#	add_definitions(-DTEST_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/tests")
#	set(PROJECT_NAME_TEST ${PROJECT_NAME}-test)
#
#	if (WIN32)
#		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
#	elseif (APPLE)
#		add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
#		add_definitions(-D__GLIBCXX__)
#	endif ()
#
#	set(TEST_SOURCES
#	    ${ETH_SOURCES}
#	    tests/RLPTest.cpp
#	    )
#
#	add_executable(${PROJECT_NAME_TEST} ${TEST_SOURCES})
#	target_compile_options(${PROJECT_NAME_TEST} PUBLIC -Wno-unused-parameter)
#
#	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
#	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)
#	target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
#
#	target_link_libraries(${PROJECT_NAME_TEST} CONAN_PKG::gtest)
#
#	if (WITH_COVERAGE)
#		set(COVERAGE_LCOV_EXCLUDES
#		    '${CMAKE_CURRENT_SOURCE_DIR}/libs/*'
#		    '${CMAKE_CURRENT_SOURCE_DIR}/tests/*'
#		    'boost/*'
#		    '/usr/include/*'
#		    '/usr/local/*'
#		    'v1'
#		    )
#		set(COVERAGE_TARGET_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/)
#		include(cmake-modules/CodeCoverage.cmake)
#		append_coverage_compiler_flags()
#		setup_target_for_coverage_lcov(
#			NAME ${PROJECT_NAME}-coverage
#			EXECUTABLE ${PROJECT_NAME_TEST}
#			DEPENDENCIES ${PROJECT_NAME_TEST}
#		)
#	endif ()
#endif ()