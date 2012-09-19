cmake_minimum_required(VERSION 2.8)
project(xkcd-viewer)
find_package(Qt4 REQUIRED)

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_subdirectory(src)
