add_library(second_scene STATIC SecondScene.cpp)
target_include_directories(second_scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(second_scene PUBLIC cosmos_render cosmos_scene)
