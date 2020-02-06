cmake_minimum_required(VERSION 3.10)
project(extfix-gui)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(wxBUILD_SHARED OFF)
set(wxBUILD_PRECOMP ON)
set(wxBUILD_INSTALL OFF)
set(wxUSE_REGEX OFF)
set(wxUSE_ZLIB OFF)
set(wxUSE_EXPAT OFF)
set(wxUSE_LIBJPEG OFF)
set(wxUSE_LIBPNG OFF)
set(wxUSE_LIBTIFF OFF)
set(wxUSE_LIBLZMA OFF)
set(wxUSE_LIBSDL OFF)
set(wxUSE_LIBMSPACK OFF)
set(wxUSE_SVG OFF)
set(wxUSE_ARTPROVIDER_TANGO OFF)

add_subdirectory(external/wxWidgets)


set(WEIRDLIB_ENABLE_MULTITHREADING OFF)
set(WEIRDLIB_BUILD_NATIVE OFF)
set(WEIRDLIB_BUILD_DOCS OFF)
set(WEIRDLIB_ENABLE_TESTING OFF)
set(WEIRDLIB_ENABLE_BUILD_STRINGOPS OFF)
set(WEIRDLIB_ENABLE_BUILD_CRYPTO OFF)
set(WERIDLIB_ENABLE_BUILD_IMAGEOPS OFF)
set(WERIDLIB_ENABLE_BUILD_PARSERS OFF)
set(WERIDLIB_ENABLE_BUILD_VECMATH OFF)

add_subdirectory("external/weirdlib")


file(GLOB_RECURSE EFIXGUI_SOURCES "src/*.*")

add_executable(extfix-gui ${EFIXGUI_SOURCES})
target_include_directories(extfix-gui PUBLIC ${wxWidgets_INCLUDE_DIRS} "external/weirdlib/include")
target_link_libraries(extfix-gui core base ${WEIRDLIB_LINKER_LIBRARIES})
set_target_properties(extfix-gui PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)