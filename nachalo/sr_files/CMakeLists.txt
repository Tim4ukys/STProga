project("sr_files" C)

add_executable(${PROJECT_NAME} "main.c")

copy_files_to_target(${PROJECT_NAME} "a.txt")
copy_files_to_target(${PROJECT_NAME} "b.txt")
