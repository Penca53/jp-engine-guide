add_executable(game-6 main.cc background.cc banana.cc default_scene.cc end.cc follow_player.cc game_manager.cc lose_canvas.cc mushroom.cc plant_bullet.cc plant.cc player.cc score_manager.cc win_canvas.cc)
target_compile_features(game-6 PRIVATE cxx_std_23)
set_target_properties(game-6 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(game-6 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (NOT MSVC) 
  target_compile_options(game-6 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
  target_link_options(game-6 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

target_link_libraries(game-6 PRIVATE engine-6)

add_custom_target(copy_resources-6
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:game-6>/resources"
)
add_dependencies(game-6 copy_resources-6)