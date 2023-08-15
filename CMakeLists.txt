cmake_minimum_required(VERSION 3.14)
include(FetchContent)

project(monitor)

# Include ncurses lib from system compiler
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIRS})

# Download fmt from git repo
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(monitor ${SOURCES})

set_property(TARGET monitor PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

target_link_libraries(monitor ${CURSES_LIBRARIES} fmt::fmt)
# TODO: Run -Werror in CI.
target_compile_options(monitor PRIVATE -Wall -Wextra)
