include(build_options)
include(use_conan)

find_package(fmt)

add_executable(main "main.cpp")
target_link_libraries(main PRIVATE fmt::fmt rt)
target_compile_features(main PRIVATE cxx_std_20)
target_compile_options(main PRIVATE ${warnings} ${wae})
