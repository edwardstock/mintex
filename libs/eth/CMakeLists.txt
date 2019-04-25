project(ethdevcore
        VERSION 0.0.1
        DESCRIPTION "devcore"
        HOMEPAGE_URL "https://github.com/ethereum/aleth"
        LANGUAGES CXX)
#file(GLOB sources "*.cpp")
#file(GLOB headers "*.h")

set(ETH_HEADERS
    RLP.h
    Common.h
    CommonData.h
    FixedHash.h
    vector_ref.h
    )
#
set(ETH_SOURCES
    ${ETH_HEADERS}
    RLP.cpp
    Common.cpp
    CommonData.cpp
    FixedHash.cpp
    )

add_library(devcore STATIC ${ETH_SOURCES})

# Needed to prevent including system-level boost headers:
#target_include_directories(devcore SYSTEM PUBLIC ${Boost_INCLUDE_DIR} PRIVATE ../utils)

find_package(Threads)

target_link_libraries(devcore PUBLIC
                      CONAN_PKG::boost_filesystem
                      CONAN_PKG::boost_system
                      CONAN_PKG::boost_log
                      CONAN_PKG::boost_thread
                      Threads::Threads
                      #PRIVATE ethash::ethash
                      )
#
#if (MSVC)
#    find_package(LevelDB)
#    target_include_directories(devcore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
#    target_link_libraries(devcore PRIVATE ${LEVELDB_LIBRARIES})
#else()
#    hunter_add_package(leveldb)
#    find_package(leveldb CONFIG REQUIRED)
#    target_link_libraries(devcore PRIVATE leveldb::leveldb)
#endif()

#hunter_add_package(rocksdb)
#find_package(RocksDB CONFIG REQUIRED)
#target_link_libraries(devcore PRIVATE RocksDB::rocksdb)
