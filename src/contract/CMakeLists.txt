set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. DIRSRCS)

include_directories("${PROJECT_SOURCE_DIR}/include")

add_library(bc_contract SHARED ${DIRSRCS})

link_libraries(/usr/local/openssl/lib/libcrypto.so /usr/local/lib/libsecp256k1.so)
