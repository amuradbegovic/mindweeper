#!/bin/sh

cat <<EOT > builtin_config.h
#pragma once
#include "config.h"

ColorScheme default_colors = (ColorScheme){
	.window_background = {0xc0, 0xc0, 0xc0, 255},
    .top_panel_background =  {0xc0, 0xc0, 0xc0, 255},
    .frame_top_left = {0x80, 0x80, 0x80, 255},
    .frame_bottom_right = {0xff, 0xff, 0xff, 255},
    .margin_top_left = {0xff, 0xff, 0xff, 255}
};

EOT

xxd -i textures/digits.bmp >> builtin_config.h
printf "\n" >> builtin_config.h
xxd -i textures/smileys.bmp >> builtin_config.h
printf "\n" >> builtin_config.h
xxd -i textures/tiles.bmp >> builtin_config.h
