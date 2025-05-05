add_executable(game-5 main.cc background.cc banana.cc default_scene.cc follow_player.cc game_manager.cc lose_canvas.cc mushroom.cc player.cc score_manager.cc)
target_compile_features(game-5 PRIVATE cxx_std_23)
set_target_properties(game-5 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(game-5 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (NOT MSVC) 
  target_compile_options(game-5 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
  target_link_options(game-5 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

target_link_libraries(game-5 PRIVATE engine-5)

add_custom_target(copy_resources-5
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:game-5>/resources"
)
add_dependencies(game-5 copy_resources-5)