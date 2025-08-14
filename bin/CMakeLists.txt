add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
    )
target_link_libraries(${PROJECT_NAME} PUBLIC application)