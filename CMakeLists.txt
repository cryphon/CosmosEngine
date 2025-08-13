cmake_minimum_required(VERSION 3.16)
project(CosmosEngine LANGUAGES C CXX)

# ---------- Toolchain / language ----------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- Paths ----------
set(ROOT_DIR        ${CMAKE_SOURCE_DIR})
set(SRC_DIR         ${ROOT_DIR}/src)
set(INCLUDE_DIR     ${ROOT_DIR}/include)      # public headers: include/cosmos/...
set(IMGUI_DIR       ${SRC_DIR}/imgui)         
set(IMGUI_BACKENDS  ${IMGUI_DIR}/backends)

# ---------- Dependencies ----------
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)   # system glfw via pkg-config
pkg_check_modules(GLM   REQUIRED glm)     # system glm via pkg-config
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)            # for std::thread if needed

# ---------- glad ----------
add_library(glad STATIC ${ROOT_DIR}/libs/glad/glad.c)
target_include_directories(glad PUBLIC ${ROOT_DIR}/libs)

# ---------- imgui ----------
add_library(imgui STATIC
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_BACKENDS}/imgui_impl_glfw.cpp
  ${IMGUI_BACKENDS}/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  ${IMGUI_DIR}
  ${IMGUI_BACKENDS}
  ${GLFW3_INCLUDE_DIRS}
)
target_link_libraries(imgui PUBLIC glad ${GLFW3_LIBRARIES} OpenGL::GL ${CMAKE_DL_LIBS})
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)




# ---------- stb (single TU that defines STB_IMAGE_IMPLEMENTATION) ----------
if(EXISTS ${SRC_DIR}/stb.cpp)
  add_library(stb_image STATIC ${SRC_DIR}/stb.cpp)
  target_include_directories(stb_image PUBLIC ${ROOT_DIR})
else()
  add_library(stb_image INTERFACE)
endif()

# ---------- Engine libraries (folders -> targets) ----------
file(GLOB_RECURSE COSMOS_RENDER_SRC   CONFIGURE_DEPENDS ${SRC_DIR}/render/*.cpp)
file(GLOB_RECURSE COSMOS_ASSETS_SRC   CONFIGURE_DEPENDS ${SRC_DIR}/assets/*.cpp)
file(GLOB_RECURSE COSMOS_SCENE_SRC    CONFIGURE_DEPENDS ${SRC_DIR}/scene/*.cpp)
file(GLOB_RECURSE COSMOS_INPUT_SRC    CONFIGURE_DEPENDS ${SRC_DIR}/input/*.cpp)
file(GLOB_RECURSE COSMOS_UI_SRC       CONFIGURE_DEPENDS ${SRC_DIR}/ui/*.cpp)
file(GLOB_RECURSE COSMOS_CORE_SRC     CONFIGURE_DEPENDS ${SRC_DIR}/core/*.cpp)



# --- render ---
add_library(cosmos_render STATIC ${COSMOS_RENDER_SRC})
target_include_directories(cosmos_render
  PUBLIC  ${INCLUDE_DIR}
  PRIVATE ${INTERNAL_DIR}
)
target_link_libraries(cosmos_render
  PUBLIC  glad OpenGL::GL ${GLM_LIBRARIES} stb_image imgui
)
target_include_directories(cosmos_render PUBLIC ${GLM_INCLUDE_DIRS})

# --- assets ---
add_library(cosmos_assets STATIC ${COSMOS_ASSETS_SRC})
target_include_directories(cosmos_assets PUBLIC ${INCLUDE_DIR} PRIVATE ${INTERNAL_DIR})
target_link_libraries(cosmos_assets PUBLIC cosmos_render)  # assets build materials/textures, so they depend on render

# --- scene ---
add_library(cosmos_scene STATIC ${COSMOS_SCENE_SRC})
target_include_directories(cosmos_scene PUBLIC ${INCLUDE_DIR} PRIVATE ${INTERNAL_DIR})
target_link_libraries(cosmos_scene PUBLIC cosmos_render imgui)

# --- input ---
add_library(cosmos_input STATIC ${COSMOS_INPUT_SRC})
target_include_directories(cosmos_input PUBLIC ${INCLUDE_DIR})
target_link_libraries(cosmos_input PUBLIC ${GLFW3_LIBRARIES})

# --- ui ---
add_library(cosmos_ui STATIC ${COSMOS_UI_SRC})
target_include_directories(cosmos_ui PUBLIC ${INCLUDE_DIR})
target_include_directories(cosmos_ui PUBLIC ${SRC_DIR}/imgui)
target_link_libraries(cosmos_ui PUBLIC imgui)


add_subdirectory(scenes/MainScene)
add_subdirectory(scenes/SecondScene)


# --- core (Window/Engine/Application) ---
add_library(cosmos_core STATIC ${COSMOS_CORE_SRC})
target_include_directories(cosmos_core PUBLIC ${INCLUDE_DIR} PRIVATE ${INTERNAL_DIR})
target_link_libraries(cosmos_core
  PUBLIC cosmos_render cosmos_scene cosmos_assets cosmos_input cosmos_ui main_scene second_scene
         ${GLFW3_LIBRARIES} glad OpenGL::GL Threads::Threads
)
target_include_directories(cosmos_core PUBLIC ${GLFW3_INCLUDE_DIRS})

# ---------- App executable ----------
file(GLOB APP_MAIN CONFIGURE_DEPENDS ${SRC_DIR}/main.cpp ${SRC_DIR}/app/*.cpp)
add_executable(${PROJECT_NAME} ${APP_MAIN})
target_link_libraries(${PROJECT_NAME} PRIVATE cosmos_core)



# ---------- Assets to binary dir (adjust to your actual asset folders) ----------
foreach(asset_dir shaders textures models assets)
  if(EXISTS ${PROJECT_SOURCE_DIR}/${asset_dir})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${PROJECT_SOURCE_DIR}/${asset_dir} ${CMAKE_CURRENT_BINARY_DIR}/${asset_dir})
  endif()
endforeach()

# ---------- Convenience targets ----------
add_custom_target(run
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Build and run ${PROJECT_NAME}"
)

add_custom_target(debug
  COMMAND gdb ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Run ${PROJECT_NAME} in GDB"
)

