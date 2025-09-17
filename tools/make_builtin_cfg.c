#include <stdio.h>

#define INI2C
#include "../src/config.h"

static inline void ByteToC(FILE *stream, uint8_t byte) {
	fprintf(stream, "0x%.2x", byte);
}

void ColorToC(FILE *stream, SDL_Color color) {
	fputc('{', stream);
	ByteToC(stream, color.r);
	fprintf(stream, ", ");
	ByteToC(stream, color.g);
	fprintf(stream, ", ");
	ByteToC(stream, color.b);
	fprintf(stream, ", ");
	ByteToC(stream, color.a);
	fputc('}', stream);	
}

void BinToC(FILE *stream, const char *array_name, unsigned char *data, unsigned int len) {
	fprintf(stream, "\nconst unsigned char %s[] = {", array_name);
	for (unsigned int i = 0; i < len; ++i) {
		if (i % 12 == 0) fprintf(stream, "\n    ");
		ByteToC(stream, (uint8_t)data[i]);
		fprintf(stream, "%s", i < len - 1 ? ", " : "\n");
	}
	fprintf(stream, "};\nconst unsigned int %s_len = %d;\n", array_name, len);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: make_builtin_cfg ini_file c_file\n");
		return 1;
	}

	FILE *ini_file = fopen(argv[1], "r");
	if (ini_file == NULL) {
		fprintf(stderr, "error: can't open %s for reading.\n", argv[1]);
		return 1;
	}
	FILE *c_file = fopen(argv[2], "w");
	if (c_file == NULL) {
		fprintf(stderr, "error: can't open %s for writing.\n", argv[2]);
		return 1;
	}

	Config cfg;
	if (!LoadConfig(&cfg, argv[1])) {
		fprintf(stderr, "error loading config from %s\n", argv[1]);
		return 1;
	}
	fclose(ini_file);

	fprintf(c_file, "#ifdef ENABLE_BUILTIN_CONFIG\n"
					"#include \"config.h\"\n\n"
					"const ColorScheme builtin_colors = (ColorScheme){\n");
	fprintf(c_file, "    .window_background = ");
	ColorToC(c_file, cfg.color_scheme.window_background);
	fprintf(c_file, ",\n    .top_panel_background = ");
	ColorToC(c_file, cfg.color_scheme.top_panel_background);
	fprintf(c_file, ",\n    .frame_top_left = ");
	ColorToC(c_file, cfg.color_scheme.frame_top_left);
	fprintf(c_file, ",\n    .frame_bottom_right= ");
	ColorToC(c_file, cfg.color_scheme.frame_bottom_right);
	fprintf(c_file, ",\n    .margin_top_left = ");
	ColorToC(c_file, cfg.color_scheme.margin_top_left);
	fprintf(c_file, "\n};\n\n");

	BinToC(c_file, "builtin_digits_bmp", cfg.digits_bmp, cfg.digits_bmp_len);
	BinToC(c_file, "builtin_smileys_bmp", cfg.smileys_bmp, cfg.smileys_bmp_len);
	BinToC(c_file, "builtin_tiles_bmp", cfg.tiles_bmp, cfg.tiles_bmp_len);

	fprintf(c_file, "\nConfig builtin_config = (Config) {\n"
					"    .color_scheme = builtin_colors,\n"
					"    .allocated_buffers = false,\n"
					"    .digits_bmp = builtin_digits_bmp,\n"
					"    .smileys_bmp = builtin_smileys_bmp,\n"
					"    .tiles_bmp = builtin_tiles_bmp,\n"
					"    .digits_bmp_len = builtin_digits_bmp_len,\n"
					"    .smileys_bmp_len = builtin_smileys_bmp_len,\n"
					"    .tiles_bmp_len = builtin_tiles_bmp_len\n};\n#endif\n");

	return 0;
}
