add_library(IMGUI STATIC)

# set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
# set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
# add_subdirectory(../glfw3.3)

target_sources(
	IMGUI
	PRIVATE
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
	imgui/imgui_tables.cpp
	imgui/imgui_widgets.cpp
	imgui/imgui.cpp

	PRIVATE
	imgui/backends/imgui_impl_opengl3.cpp
	imgui/backends/imgui_impl_glfw.cpp
)

target_include_directories(
	IMGUI
	PUBLIC imgui
	PUBLIC imgui/backends
)

target_link_libraries(IMGUI PUBLIC glfw ${CMAKE_DL_LIBS})