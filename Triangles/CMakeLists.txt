cmake_minimum_required(VERSION 2.8)
#Déclaration du projet
cmake_policy(SET CMP0004 OLD)
project(Inside)
add_executable(Inside Inside.c)

project(Outside)
add_executable(Outside Outside.c)


#Déclaration de l'exécutable
include(FindPkgConfig)

target_link_libraries (Inside m)
target_link_libraries (Outside m)
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
  include_directories(${SDL2_INCLUDE_DIR})
  target_link_libraries (Inside ${SDL2_LIBRARIES})
  target_link_libraries (Outside ${SDL2_LIBRARIES})
else (SDL2_FOUND)
  MESSAGE(FATAL_ERROR "SDL2 lib required")
endif (SDL2_FOUND)
include(FindPkgConfig)
pkg_search_module(SDL2GFX REQUIRED SDL2_gfx)
if (SDL2GFX_FOUND)
  include_directories(${SDL2GFX_INCLUDE_DIRS})
  target_link_libraries (Inside ${SDL2GFX_LIBRARIES})
  target_link_libraries (Outside ${SDL2GFX_LIBRARIES})
else (SDL2GFX_FOUND)
	MESSAGE(FATAL_ERROR "SDL2 GFX lib required")
endif (SDL2GFX_FOUND)

if( WITH_COREDEBUG )
  add_definitions(-ggdb3)
  message(STATUS "GCC: Debug-flags set (-ggdb3)")
endif()

