cmake_minimum_required(VERSION 3.15)
project(OpenGlGeoEx VERSION 0.1.0)

set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )

add_definitions(-DGLEW_STATIC)
add_subdirectory(external/glew-cmake EXCLUDE_FROM_ALL)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw3.3)

set(GLM_TEST_ENABLE OFF)
add_subdirectory(external/glm)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_USE_CPP14 ON CACHE INTERNAL "" FORCE)
add_subdirectory(external/tinygltf)

add_subdirectory(external/imgui-cmake)

# add_subdirectory( src )

file( GLOB SOURCEFILES src/*.cpp )

add_executable(OpenGlGeoEx ${SOURCEFILES})

# target_link_libraries(OpenGLTraining game engine maths input ${GLEW_LIBRARIES} ${SDL2_LIBRARIES} OpenGL::GL OpenGL::GLU)
target_link_libraries(OpenGlGeoEx ${OPENGL_LIBRARIES} glm libglew_static glfw IMGUI tinygltf ${CMAKE_DL_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)