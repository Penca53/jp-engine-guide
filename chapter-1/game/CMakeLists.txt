add_executable(game-1 main.cc player.cc)
target_compile_features(game-1 PRIVATE cxx_std_23)
set_target_properties(game-1 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(game-1 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (NOT MSVC) 
  target_compile_options(game-1 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
  target_link_options(game-1 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

target_link_libraries(game-1 PRIVATE engine-1)

add_custom_target(copy_resources-1
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:game-1>/resources"
)
add_dependencies(game-1 copy_resources-1)