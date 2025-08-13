add_library(
    arg-parser STATIC 
    ArgParser.cpp ArgParser.hpp
    Handlers.cpp Handlers.hpp
)
target_include_directories(arg-parser PUBLIC ${CMAKE_SOURCE_DIR})