#!/bin/sh

cat <<EOT > builtin_config.c
#ifdef ENABLE_BUILTIN_CONFIG
#include "config.h"

ColorScheme default_colors = (ColorScheme){
	.window_background = {0xc0, 0xc0, 0xc0, 255},
    .top_panel_background = {0xc0, 0xc0, 0xc0, 255},
    .frame_top_left = {0x80, 0x80, 0x80, 255},
    .frame_bottom_right = {0xff, 0xff, 0xff, 255},
    .margin_top_left = {0xff, 0xff, 0xff, 255}
};

EOT

xxd -i textures/digits.bmp >> builtin_config.c
printf "\n" >> builtin_config.c
xxd -i textures/smileys.bmp >> builtin_config.c
printf "\n" >> builtin_config.c
xxd -i textures/tiles.bmp >> builtin_config.c

cat <<EOT >> builtin_config.c

Config builtin_config = (Config) {
	.color_scheme = default_colors,
	.allocated_buffers = false,
	.digits_bmp = textures_digits_bmp,
	.smileys_bmp = textures_smileys_bmp,
	.tiles_bmp = textures_tiles_bmp,
	.digits_bmp_len = textures_digits_bmp_len, 
	.smileys_bmp_len = textures_smileys_bmp_len, 
	.tiles_bmp_len = textures_tiles_bmp_len
};

EOT

printf "\n#endif" >> builtin_config.c
