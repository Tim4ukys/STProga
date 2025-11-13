project("rem_comments" C)

add_definitions(_CRT_SECURE_NO_WARNINGS)

file(GLOB SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/source/*.c")
file(GLOB PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCE} ${PUBLIC_HEADER})
add_library(ZVeZda::REM_COMMENTS ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(${PROJECT_NAME}_ex "main.c")
target_link_libraries(${PROJECT_NAME}_ex ZVeZda::REM_COMMENTS)
