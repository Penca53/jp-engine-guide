add_executable(game-0 main.cc)
target_compile_features(game-0 PRIVATE cxx_std_23)
set_target_properties(game-0 PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(game-0 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (NOT MSVC) 
  target_compile_options(game-0 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
  target_link_options(game-0 PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

target_link_libraries(game-0 PRIVATE engine-0)