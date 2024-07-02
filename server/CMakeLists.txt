add_executable(server
    src/main.cpp
    src/Server.cpp
)

target_include_directories(server PRIVATE include)
