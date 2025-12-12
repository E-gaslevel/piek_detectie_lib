add_executable(peak_example main.c)
target_link_libraries(peak_example PRIVATE peakdetect)
target_include_directories(peak_example PRIVATE ${PROJECT_SOURCE_DIR}/include)
