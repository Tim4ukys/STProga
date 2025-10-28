project("tests" CXX)

find_package(GTest CONFIG REQUIRED)

add_subdirectory(algorithms)
add_subdirectory(st_data)
add_subdirectory(nachalo)
