cmake_minimum_required(VERSION 3.10)
project(3DSpace)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${INCLUDE_DIR})

set(MAIN_SOURCE_FILES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/VAO.cpp
    ${SRC_DIR}/VBO.cpp
    ${SRC_DIR}/EBO.cpp
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
pkg_check_modules(GLM REQUIRED glm)

add_library(glad STATIC libs/glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/libs)

# main executable
add_executable(${PROJECT_NAME} ${MAIN_SOURCE_FILES})

# Include directories for main target
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${GLFW3_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    ${GLFW3_LIBRARIES} 
    ${GLM_LIBRARIES}
    glad 
    GL 
    GLU
    dl
)

# main
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "Build and run 3DSpace"
)
