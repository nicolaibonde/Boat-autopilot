cmake_minimum_required(VERSION 3.9)
project(exe.CAPTAIN)

FIND_PACKAGE( Boost 1.65 COMPONENTS system REQUIRED )
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB CAPTAIN_HEADERS
	./CAPTAIN/*.h
)

add_subdirectory(CAPTAIN)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(exe.CAPTAIN main.CAPTAIN/Source.cpp ${CAPTAIN_HEADERS})

target_link_libraries(exe.CAPTAIN lib.CAPTAIN)
target_link_libraries(exe.CAPTAIN ${Boost_LIBRARIES} -lpthread -lboost_filesystem -lboost_system -lpigpio -lrt)

install(TARGETS exe.CAPTAIN DESTINATION bin)
