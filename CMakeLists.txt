cmake_minimum_required(VERSION 2.8)
project(FRZ)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")

option(DEBUGWARN "DEBUGWARN" OFF)
if(DEBUGWARN)
  message("-- adding -Wall (removing -w)")
  set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11 -Wall")
else()
  set(CMAKE_CXX_FLAGS "-g -O2 -std=c++11 -Wall")
endif()

set(CMAKE_SHARED_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(ROOT REQUIRED)
include_directories("${ROOT_INCLUDE_DIR}")
link_directories("${ROOT_LIBRARY_DIR}")

option(ROOT6 "ROOT6" OFF)
if(ROOT6)
  message("-- ROOT 6 CMake option set ON")
else()
  message("-- ROOT 6 CMake option set OFF (default even for ROOT version 6 installation)")
endif()

include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/include")

ROOT_GENERATE_DICTIONARY(
  FRZDict
  include/FRZ/PhysicsObject.h
  include/FRZ/Lepton.h
  include/FRZ/Jet.h
  include/FRZ/MET.h
  include/FRZ/LeptonPair.h
  include/FRZ/FinalState.h
  include/FRZ/HistMaker.h
  include/FRZ/Sample.h
  include/FRZ/SampleHolder.h
  include/FRZ/Swizzler.h
  LINKDEF include/FRZ/LinkDef.h
  )

file(GLOB FRZ_src
  "src/*.cxx"
  )

add_library(
  FRZ SHARED
  ${FRZ_src}
  FRZDict
  )

target_link_libraries(FRZ ${ROOT_LIBRARIES})

#install(DIRECTORY include/FRZ DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
#install(TARGETS FRZ DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
#install(DIRECTORY config DESTINATION "${CMAKE_INSTALL_PREFIX}")
#install(DIRECTORY plotting DESTINATION "${CMAKE_INSTALL_PREFIX}")
#if(ROOT6)
#  install(FILES ${CMAKE_BINARY_DIR}/FRZDict_rdict.pcm DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
#endif()
