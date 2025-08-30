cmake_minimum_required(VERSION 3.10)
project(mindweeper)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)

add_executable(mindweeper main.c game_logic.c sprite_sheet.c ui.c game_context.c)

set_property(TARGET mindweeper PROPERTY C_STANDARD 11)

target_link_libraries(mindweeper SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)