include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(engine-2 app.cc input.cc resource_manager.cc tile.cc tilemap.cc tileset.cc)
target_compile_features(engine-2 PRIVATE cxx_std_23)
set_target_properties(engine-2 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(engine-2 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(engine-2 PUBLIC SFML::Audio SFML::Graphics)
target_include_directories(engine-2 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")