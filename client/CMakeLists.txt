find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/client/include)  # Explicitly include the full path

link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

add_executable(client
    src/main.cpp
    src/LoginWindow.cpp
)

target_link_libraries(client ${GTKMM_LIBRARIES})
