cmake_minimum_required(VERSION 3.10)
project(mintex-wallet
        VERSION 0.0.1
        DESCRIPTION "Minter Text Console"
        HOMEPAGE_URL "https://github.com/edwardstock/mintex"
        LANGUAGES CXX
        )

set(CMAKE_CXX_STANDARD 14)

set(INCLUDE_FILES
        include/wallet/base_repository.h
        include/wallet/explorer/repository.h
        include/wallet/explorer/base.h
        include/wallet/resp_gen.h
        include/wallet/explorer/results.hpp
        include/wallet/app/app.h
        include/wallet/app/init_controller.h
        include/wallet/app/secret_controller.h
        include/wallet/app/account_controller.h
        include/wallet/app/tx_controller.h
        include/wallet/app/settings_controller.h
        include/wallet/term.h)

set(SOURCE_FILES
        ${INCLUDE_FILES}
        main.cpp
        src/base/base_repository.cpp
        src/explorer/results.cpp
        src/app/app.cpp
        src/app/account_controller.cpp
        src/app/init_controller.cpp
        src/app/secret_controller.cpp
        src/app/settings_controller.cpp
        src/app/tx_controller.cpp
        src/term.cpp)

option(JSON_BuildTests "" OFF)
option(JSON_MultipleHeaders "" OFF)
option(ENABLE_SHARED OFF)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PUBLIC MINTEX_VERSION=\"${CMAKE_PROJECT_VERSION}\")
target_compile_options(${PROJECT_NAME} PUBLIC -fexceptions)

add_subdirectory(${RP_DIR}/libs/json ${CMAKE_BINARY_DIR}/json)
add_subdirectory(${RP_DIR}/libs/leveldb ${CMAKE_BINARY_DIR}/leveldb EXCLUDE_FROM_ALL)
add_subdirectory(${RP_DIR}/libs/rang ${CMAKE_BINARY_DIR}/rang EXCLUDE_FROM_ALL)

target_link_libraries(${PROJECT_NAME} nlohmann_json)
target_link_libraries(${PROJECT_NAME} mintex_tx)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::httb)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::boost_program_options)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::boost_filesystem)
target_link_libraries(${PROJECT_NAME} CONAN_PKG::fmt)
target_link_libraries(${PROJECT_NAME} leveldb)
target_link_libraries(${PROJECT_NAME} devcore)
target_link_libraries(${PROJECT_NAME} rang)

target_include_directories(${PROJECT_NAME} PUBLIC ${RP_DIR}/libs/json/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${RP_DIR}/libs/leveldb/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


if (WITH_TEST)
    add_definitions(-DTEST_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/tests")
    set(PROJECT_NAME_TEST ${PROJECT_NAME}-test)

    if (WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    elseif (APPLE)
        add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
        add_definitions(-D__GLIBCXX__)
    endif ()

    set(TEST_SOURCES
            ${SOURCE_FILES}
            tests/transaction_item_deser_test.cpp
            include/wallet/term.h)

    add_executable(${PROJECT_NAME_TEST} ${TEST_SOURCES})
    target_compile_options(${PROJECT_NAME_TEST} PUBLIC -Wno-unused-parameter)

    target_link_libraries(${PROJECT_NAME_TEST} nlohmann_json)
    target_link_libraries(${PROJECT_NAME_TEST} mintex_tx)
    target_link_libraries(${PROJECT_NAME_TEST} CONAN_PKG::httb)
    target_link_libraries(${PROJECT_NAME_TEST} CONAN_PKG::boost_program_options)
    target_link_libraries(${PROJECT_NAME_TEST} leveldb)
    target_link_libraries(${PROJECT_NAME_TEST} devcore)

    target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${RP_DIR}/libs/json/include)
    target_include_directories(${PROJECT_NAME_TEST} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs/leveldb/include)
    target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    if (WITH_COVERAGE)
        set(COVERAGE_LCOV_EXCLUDES
                '${CMAKE_CURRENT_SOURCE_DIR}/libs/*'
                '${CMAKE_CURRENT_SOURCE_DIR}/tests/*'
                'boost/*'
                '/usr/include/*'
                '/usr/local/*'
                'v1'
                )
        set(COVERAGE_TARGET_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/)
        include(cmake-modules/CodeCoverage.cmake)
        append_coverage_compiler_flags()
        setup_target_for_coverage_lcov(
                NAME ${PROJECT_NAME}-coverage
                EXECUTABLE ${PROJECT_NAME_TEST}
                DEPENDENCIES ${PROJECT_NAME_TEST}
        )
    endif ()
endif ()