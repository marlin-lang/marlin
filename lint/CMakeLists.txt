add_library(${PROJECT_NAME}.lint INTERFACE)
target_compile_features(${PROJECT_NAME}.lint INTERFACE cxx_std_17)

target_include_directories(${PROJECT_NAME}.lint INTERFACE include)
