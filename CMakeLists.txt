cmake_minimum_required(VERSION 3.10)
project(extfix-gui)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(wxWidgets REQUIRED core base)
include("${wxWidgets_USE_FILE}")

file(GLOB_RECURSE EFIXGUI_SOURCES "src/*.*")

add_executable(extfix-gui ${EFIXGUI_SOURCES})
target_include_directories(extfix-gui PUBLIC ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(extfix-gui ${wxWidgets_LIBRARIES})
set_target_properties(extfix-gui PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)