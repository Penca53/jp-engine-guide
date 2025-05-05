add_executable(game-4 main.cc background.cc banana.cc default_scene.cc game_manager.cc mushroom.cc player.cc)
target_compile_features(game-4 PRIVATE cxx_std_23)
set_target_properties(game-4 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(game-4 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (NOT MSVC) 
  target_compile_options(game-4 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
  target_link_options(game-4 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

target_link_libraries(game-4 PRIVATE engine-4)

add_custom_target(copy_resources-4
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:game-4>/resources"
)
add_dependencies(game-4 copy_resources-4)