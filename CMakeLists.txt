cmake_minimum_required (VERSION 3.5)
project (ChainBlock)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

add_subdirectory(tests)

include_directories("${PROJECT_SOURCE_DIR}/include")

#add_executable(test_db test.cpp)

#target_link_libraries(test_db /usr/local/lib/librocksdb.so)
