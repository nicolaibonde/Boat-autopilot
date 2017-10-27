cmake_minimum_required(VERSION 3.9)

project(lib.CAPTAIN)

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)

add_library(lib.CAPTAIN STATIC ${SOURCES})

install(TARGET lib.CAPTAIN DESTINATION lib)
install(FILES ${HEADERS} DESTINATION include)