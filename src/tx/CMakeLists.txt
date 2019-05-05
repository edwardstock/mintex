cmake_minimum_required(VERSION 3.10)
project(mintex_tx
        VERSION 0.0.1
        DESCRIPTION "Minter Transaction Maker"
        LANGUAGES CXX
        )

set(HEADERS
    include/mintex-tx/tx.h
    include/mintex-tx/rlp_encodable.h
    include/mintex-tx/tx_send_coin.h
    include/mintex-tx/tx_fwd.h
    include/mintex-tx/tx_data.h
    include/mintex-tx/tx_sell_coin.h
    include/mintex-tx/tx_sell_all_coins.h
    include/mintex-tx/tx_buy_coin.h
    include/mintex-tx/tx_create_coin.h
    include/mintex-tx/tx_declare_candidacy.h
    include/mintex-tx/tx_delegate.h
    include/mintex-tx/tx_unbond.h
    include/mintex-tx/tx_redeem_check.h
    include/mintex-tx/tx_set_candidate_on_off.h
    include/mintex-tx/tx_create_multisig_address.h
    include/mintex-tx/tx_multisend.h
    include/mintex-tx/tx_edit_candidate.h
    include/mintex-tx/tx_type.h
    include/mintex-tx/signature_data.h
    include/mintex-tx/minter_address.h
    include/mintex-tx/minter_private_key.h
    include/mintex-tx/signature.h
    include/mintex-tx/utils.h
    include/mintex-tx/secp256k1_raii.h
    include/mintex-tx/tx_builder.h include/mintex-tx/minter_public_key.h include/mintex-tx/minter_hash.h)

set(SOURCES
    ${HEADERS}
    src/tx/tx.cpp
    src/tx/tx_send_coin.cpp
    src/tx/tx_sell_coin.cpp
    src/tx/tx_sell_all_coins.cpp
    src/tx/tx_buy_coin.cpp
    src/tx/tx_create_coin.cpp
    src/tx/tx_declare_candidacy.cpp
    src/tx/tx_delegate.cpp
    src/tx/tx_unbond.cpp
    src/tx/tx_redeem_check.cpp
    src/tx/tx_set_candidate_on_off.cpp
    src/tx/tx_create_multisig_address.cpp
    src/tx/tx_multisend.cpp
    src/tx/tx_edit_candidate.cpp
    src/data/minter_address.cpp
    src/tx/signature_data.cpp
    src/utils.cpp
    src/tx/tx_type.cpp
    src/tx/tx_builder.cpp
    src/data/minter_public_key.cpp
    src/data/minter_hash.cpp)

add_library(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PUBLIC -Wno-shift-count-overflow -std=c++14 -fPIC) #-static-libstdc++
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#uint256
add_subdirectory(${RP_DIR}/src/uint256 ${CMAKE_BINARY_DIR}/uint256)

# eth
add_subdirectory(${RP_DIR}/libs/eth ${CMAKE_BINARY_DIR}/devcore)

# bip39
set(ENABLE_JNI OFF)
set(ENABLE_GO OFF)
set(ENABLE_SHARED OFF)
add_subdirectory(${RP_DIR}/libs/bip39 ${CMAKE_BINARY_DIR}/bip39)

#secp256k1
add_subdirectory(${RP_DIR}/libs/secp256k1 ${CMAKE_BINARY_DIR}/secp256k1)
target_include_directories(secp256k1_core PRIVATE ${RP_DIR}/libs/secp256k1)


target_link_libraries(${PROJECT_NAME} CONAN_PKG::boost_multiprecision)
target_link_libraries(${PROJECT_NAME} devcore)
target_link_libraries(${PROJECT_NAME} bip39_core)
target_link_libraries(${PROJECT_NAME} secp256k1_core)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

target_include_directories(${PROJECT_NAME} PUBLIC ${RP_DIR}/libs)


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
	    tests/tx_send_test.cpp
	    tests/tx_buy_coin_test.cpp
	    tests/tx_create_coin_test.cpp
	    tests/utils_test.cpp
	    tests/tx_sell_coin_test.cpp
	    tests/tx_delegate_test.cpp
	    tests/tx_set_candidate_on_off.cpp
	    tests/tx_declare_candidacy_test.cpp
	    tests/tx_edit_candidate_test.cpp
	    tests/priv_key_test.cpp
	    tests/tx_multisend_test.cpp
	    tests/tx_multisig_test.cpp
	    tests/tx_check_test.cpp
	    )

	add_executable(${PROJECT_NAME_TEST} ${TEST_SOURCES})
	target_compile_options(${PROJECT_NAME_TEST} PUBLIC -Wno-unused-parameter)

	#	target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
	#	target_include_directories(${PROJECT_NAME_TEST} PUBLIC ${RP_DIR}/src)

	target_link_libraries(${PROJECT_NAME_TEST} CONAN_PKG::gtest)
	target_link_libraries(${PROJECT_NAME_TEST} ${PROJECT_NAME})

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