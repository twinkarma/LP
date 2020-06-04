find_package(Catch2 REQUIRED)
project(tests C CXX CUDA)

file(GLOB test_files
    "*.cpp"
)

# Creating individual target per test .cpp file
foreach(file ${test_files})
    get_filename_component(file_name ${file} NAME_WE)
    set(target_name "test_${file_name}")

    if(NOT ${file_name} MATCHES "main")
        message(STATUS "Adding test case ${target_name} from  ${file} ")
        add_executable(${target_name} ${file} "main.cpp")
        target_include_directories(${target_name} PUBLIC "../include" "../LP")
        target_link_libraries(${target_name} Catch2::Catch2 "LP")
        add_dependencies(${target_name} "LP")
        include(CTest)
        include(Catch)
        catch_discover_tests(${target_name})
    endif()
endforeach()

# Create target to test all cases
add_executable("tests_all" ${test_files})
target_include_directories("tests_all" PUBLIC "../include")
target_link_libraries("tests_all" Catch2::Catch2 "LP")
add_dependencies("tests_all" "LP")
include(CTest)
include(Catch)
catch_discover_tests("tests_all")

