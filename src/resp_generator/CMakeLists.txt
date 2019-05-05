cmake_minimum_required(VERSION 3.10)
project(response-generator)

set(CMAKE_CXX_STANDARD 14)

#add_subdirectory(${RP_DIR}/libs/json ${CMAKE_BINARY_DIR}/json EXCLUDE_FROM_ALL)

add_executable(resp_generator main.cpp)
target_compile_definitions(resp_generator PUBLIC RES_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\")
target_link_libraries(resp_generator nlohmann_json)
target_link_libraries(resp_generator CONAN_PKG::fmt)
target_link_libraries(resp_generator CONAN_PKG::toolboxpp)
target_link_libraries(resp_generator CONAN_PKG::boost_filesystem)
target_link_libraries(resp_generator CONAN_PKG::boost_program_options)
target_include_directories(resp_generator PUBLIC ${RP_DIR}/libs/json/include)