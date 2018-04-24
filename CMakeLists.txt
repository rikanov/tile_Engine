cmake_minimum_required(VERSION 2.6)
project(engine)

add_executable(engine main.cpp engine.cpp tile.cpp board.cpp node.cpp)
target_link_libraries(engine ${CMAKE_SOURCE_DIR}/../View2D/build/libview2d.a ${CMAKE_SOURCE_DIR}/../BoardView/build/libboardview.a)

install(TARGETS engine RUNTIME DESTINATION bin)
