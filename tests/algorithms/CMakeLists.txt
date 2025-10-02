file(GLOB TEST_SOURCES "*.cpp")

add_compile_options(-Zi)


foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    
    add_executable(${test_name} ${test_file})
    target_link_libraries(${test_name} GTest::gtest GTest::gtest_main ALGORITHMS::BUBBLE_SORT)
    add_test(NAME ${test_name} COMMAND ${test_name})
    
    message(STATUS "Added test: ${test_name}")
endforeach()