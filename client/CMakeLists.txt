cmake_minimum_required(VERSION 3.10)
project(ChatRoom)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

# Include GTKMM and project-specific include directories
include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/client/include)

link_directories(${GTKMM_LIBRARY_DIRS})

add_definitions(${GTKMM_CFLAGS_OTHER})

# Add executables
add_executable(chat_client
    src/main.cpp
    src/LoginWindow.cpp
    src/ClientGUI.cpp
    src/MainWindow.cpp
)

target_link_libraries(chat_client ${GTKMM_LIBRARIES})

