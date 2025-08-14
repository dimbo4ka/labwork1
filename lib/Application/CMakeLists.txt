add_library(application STATIC Application.hpp Application.cpp)

target_link_libraries(application PRIVATE arg-parser log-analyzer)
target_include_directories(application PUBLIC ${CMAKE_SOURCE_DIR})
