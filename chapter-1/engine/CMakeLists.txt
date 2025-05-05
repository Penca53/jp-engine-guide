include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(engine-1 app.cc input.cc resource_manager.cc)
target_compile_features(engine-1 PRIVATE cxx_std_23)
set_target_properties(engine-1 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(engine-1 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(engine-1 PUBLIC SFML::Audio SFML::Graphics)
target_include_directories(engine-1 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")