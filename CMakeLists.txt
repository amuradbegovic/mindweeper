cmake_minimum_required(VERSION 3.10)
project(mindweeper)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)

#add_executable(make_builtin_cfg tools/make_builtin_cfg.c)

# Build vendored libinih
add_subdirectory(extern/inih)

add_executable(mindweeper src/main.c src/game_logic.c src/sprite_sheet.c src/ui.c src/game_context.c src/config.c src/builtin_config.c)

set_property(TARGET mindweeper PROPERTY C_STANDARD 11)

add_compile_definitions(ENABLE_BUILTIN_CONFIG)

target_include_directories(mindweeper PUBLIC
                          "${PROJECT_BINARY_DIR}"
						  "src"
                          "${PROJECT_SOURCE_DIR}/extern/inih"
                          )

target_link_libraries(mindweeper SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf inih)
