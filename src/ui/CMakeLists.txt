find_package(Qt5 5.9 REQUIRED COMPONENTS Gui Widgets)
message(STATUS "Using Qt ${Qt5Core_VERSION}")

set_property(TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_FEATURES "")
set(CMAKE_AUTOMOC ON)

if(WIN32)
  set(GUI_TYPE WIN32)
elseif(APPLE)
  set(GUI_TYPE MACOSX_BUNDLE)
endif()

add_executable(schroedinger
			${GUI_TYPE}
			main.cpp
			MainWindow.cpp
			MainWindow.h)

target_link_libraries(schroedinger
                      PRIVATE g_options
                              g_warnings
                              schroedinger_core
                              Qt5::Widgets)
