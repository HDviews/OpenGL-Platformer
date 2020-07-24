cmake_minimum_required(VERSION 3.0.0)
project(window VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/include)

find_package(OpenGL REQUIRED)

add_executable(main 
    main.cpp
    stb_image.cpp
    include/glad.c
    include/stb_image.h
    )

target_link_libraries(main glfw3.dll)
target_link_libraries(main OpenGL::GL)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)