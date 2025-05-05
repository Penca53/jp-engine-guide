add_executable(game-2 main.cc background.cc mushroom.cc player.cc)
target_compile_features(game-2 PRIVATE cxx_std_23)
set_target_properties(game-2 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(game-2 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (NOT MSVC) 
  target_compile_options(game-2 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
  target_link_options(game-2 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

target_link_libraries(game-2 PRIVATE engine-2)

add_custom_target(copy_resources-2
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:game-2>/resources"
)
add_dependencies(game-2 copy_resources-2)