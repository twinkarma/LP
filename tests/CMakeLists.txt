find_package(Catch2 REQUIRED)
project(tests)

add_executable(${PROJECT_NAME} test.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "../include")
target_link_libraries(${PROJECT_NAME} Catch2::Catch2)