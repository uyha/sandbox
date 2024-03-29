include(CompileOptions)
include(Conan)

find_package(cppzmq REQUIRED)
find_package(fmt REQUIRED)

if(PRECOMPILE)
  add_custom_command(
    OUTPUT dummy.cpp
    COMMAND "${CMAKE_COMMAND}" -E touch dummy.cpp
  )
  add_library(sandbox_pch dummy.cpp)
  target_precompile_headers(sandbox_pch PUBLIC
    <chrono>
    <fmt/printf.h>
    <thread>
    <zmq.hpp>
    )
  target_link_libraries(sandbox_pch PRIVATE cppzmq fmt::fmt)
  target_compile_features(sandbox_pch PRIVATE cxx_std_20)
  target_compile_options(sandbox_pch PRIVATE ${warnings} ${wae})
endif()

macro(sandbox_add_executable name)
  add_executable("${name}" "${name}.cpp" ${ARGN})
  target_link_libraries("${name}" PRIVATE cppzmq fmt::fmt)
  target_compile_features("${name}" PRIVATE cxx_std_20)
  target_compile_options("${name}" PRIVATE ${warnings} ${wae})
  if(PRECOMPILE)
    target_precompile_headers("${name}" REUSE_FROM sandbox_pch)
  endif()
endmacro()

foreach(dir RANGE 1 4)
  add_subdirectory(chapter${dir})
endforeach()
