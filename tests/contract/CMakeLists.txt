set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

aux_source_directory(. DIRSRCS)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(test_contract ${DIRSRCS})

target_link_libraries(test_contract bc_block bc_crypto /usr/local/openssl/lib/libcrypto.so /usr/local/lib/libsecp256k1.so)
