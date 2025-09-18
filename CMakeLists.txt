cmake_minimum_required (VERSION 3.8)
project ("STProga" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

function(copy_files_to_target target files)
    foreach(file ${files})
        add_custom_command(
            TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/${file}
            $<TARGET_FILE_DIR:${target}>/${file}
        )
    endforeach()
endfunction()

add_compile_options(-Zi)

add_subdirectory("nachalo")

