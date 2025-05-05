include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(engine-6 app.cc camera_manager.cc camera.cc collider.cc circle_collider.cc input.cc node.cc physics.cc rectangle_collider.cc resource_manager.cc scene.cc sprite_sheet_animation.cc tile.cc tilemap.cc tileset.cc)
target_compile_features(engine-6 PRIVATE cxx_std_23)
set_target_properties(engine-6 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(engine-6 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(engine-6 PUBLIC SFML::Audio SFML::Graphics)
target_include_directories(engine-6 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")