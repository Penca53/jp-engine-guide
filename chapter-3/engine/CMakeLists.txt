include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(engine-3 app.cc input.cc node.cc resource_manager.cc scene.cc tile.cc tilemap.cc tileset.cc)
target_compile_features(engine-3 PRIVATE cxx_std_23)
set_target_properties(engine-3 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(engine-3 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(engine-3 PUBLIC SFML::Audio SFML::Graphics)
target_include_directories(engine-3 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")