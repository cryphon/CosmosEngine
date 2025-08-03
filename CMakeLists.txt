cmake_minimum_required(VERSION 3.10)
project(CosmosEngine)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(IMGUI_DIR ${SRC_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_DIR}/backends)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 🔧 Include path for project-specific headers
include_directories(${INCLUDE_DIR})

# 🔧 All source files including ImGui and your engine
set(MAIN_SOURCE_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Shader.cpp
    ${SRC_DIR}/VAO.cpp
    ${SRC_DIR}/VBO.cpp
    ${SRC_DIR}/EBO.cpp
    ${SRC_DIR}/stb.cpp
    ${SRC_DIR}/Texture.cpp
    ${SRC_DIR}/TextureHDR.cpp
    ${SRC_DIR}/Material.cpp
    ${SRC_DIR}/Mesh.cpp
    ${SRC_DIR}/Skybox.cpp

    ${SRC_DIR}/Application.cpp
    ${SRC_DIR}/Window.cpp
    ${SRC_DIR}/Engine.cpp
    ${SRC_DIR}/Renderer.cpp
    ${SRC_DIR}/Camera.cpp
    ${SRC_DIR}/InputManager.cpp
    ${SRC_DIR}/InputListener.cpp
    ${SRC_DIR}/ObjLoader.cpp
    ${SRC_DIR}/ShaderLibrary.cpp
    ${SRC_DIR}/SkyboxUtils.cpp
    ${SRC_DIR}/Ui.cpp

    # ImGui core
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp

    # ImGui backend
    ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
)

# 🔧 Find external dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(GLM REQUIRED glm)

# 🔧 Add GLAD as static library
add_library(glad STATIC libs/glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/libs)

# 🔧 Main executable
add_executable(${PROJECT_NAME} ${MAIN_SOURCE_FILES})

# 🔧 Include directories for ImGui, GLAD, GLFW, GLM
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${GLFW3_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/libs           # for glad
    ${IMGUI_DIR}                       # for imgui.h and imconfig.h
    ${IMGUI_BACKENDS_DIR}             # for backend headers
)

# Add Scene directories
add_subdirectory(scenes/MainScene)
add_subdirectory(scenes/SecondScene)

# 🔧 Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${GLFW3_LIBRARIES} 
    ${GLM_LIBRARIES}
    glad 
    GL 
    GLU
    dl
    pthread
)

# 🔧 Define that we're using GLAD as the OpenGL loader for ImGui
target_compile_definitions(${PROJECT_NAME} PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)

# 🔧 Convenience run target
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Build and run 3DSpace"
)


file(COPY ${PROJECT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/textures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/models DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# Custom GDB run target
add_custom_target(debug
  COMMAND gdb ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${PROJECT_NAME}
  COMMENT "Run ${PROJECT_NAME} in GDB"
)


