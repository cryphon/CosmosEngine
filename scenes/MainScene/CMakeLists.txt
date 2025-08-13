add_library(main_scene STATIC MainScene.cpp)
target_include_directories(main_scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(main_scene PUBLIC cosmos_scene cosmos_render cosmos_assets)

