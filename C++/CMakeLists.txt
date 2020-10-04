cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(Example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)
message(STATUS "-----------------------------------------")
message(STATUS "STX      =>  Downloading")
FetchContent_Declare(
  stx
  GIT_REPOSITORY https://github.com/lamarrr/STX.git
  GIT_TAG        master
)
message(STATUS "CXXOPTS  =>  Downloading")
FetchContent_Declare(
  opts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        master
)
message(STATUS "ASIO     =>  Downloading")
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG        master
)
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()
set(ASIO_PATH ${asio_SOURCE_DIR})

FetchContent_GetProperties(opts)
if(NOT opts_POPULATED)
    FetchContent_Populate(opts)
endif()
set(CXXOPTS_PATH ${opts_SOURCE_DIR})

message(STATUS "CXXOPTS =>  Downloaded")
FetchContent_MakeAvailable(opts)
message(STATUS "ASIO    =>  Downloaded")
FetchContent_MakeAvailable(asio)
message(STATUS "STX     =>  Downloaded")
FetchContent_MakeAvailable(stx)
message(STATUS "-----------------------------------------")

find_program(IWYU_PATH NAMES include-what-you-use iwyu)
if(NOT IWYU_PATH)
  message(FATAL_ERROR "Could not find the program include-what-you-use")
endif()
set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})
set(CMAKE_C_INCLUDE_WHAT_YOU_USE ${IWYU_PATH})

find_package(Threads REQUIRED)
if(WIN32)
    link_libraries(ws2_32 wmsock)
endif()
include_directories(${ASIO_PATH}/asio/include)
include_directories(${CXXOPTS_PATH}/include)
include_directories(headers)

add_executable(netsafer
    headers/networking.hpp
    src/networking.cpp
    src/main.cpp
)
target_link_libraries(netsafer
    PUBLIC stx
    PRIVATE $<TARGET_NAME_IF_EXISTS:Threads::Threads>
)
