cmake_minimum_required (VERSION 3.5)
project (ChainBlock)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

add_subdirectory(tests)

include_directories("${PROJECT_SOURCE_DIR}/include")

#INCLUDE_DIRECTORIES("/usr/local/openssl/include")
#add_library(demo SHARED sha.cpp)
add_executable(demo_exec sha.cpp)

target_link_libraries(demo_exec bc_crypto /usr/local/openssl/lib/libcrypto.so /usr/local/lib/libsecp256k1.so)
