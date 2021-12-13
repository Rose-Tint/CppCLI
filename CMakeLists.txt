cmake_minimum_required(VERSION 3.10.2)
project(cli VERSION 1.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./)

add_executable(rose
    main.cpp
    CLI.cpp
    CliError.cpp
    ColorCode.cpp
    ThreadPool.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(rose Threads::Threads)


target_include_directories(rose
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(rose
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)


target_compile_options(rose
    PUBLIC
      -O2
      -Wall
      -Wextra
      -Werror
      -pedantic-errors
)
