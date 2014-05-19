#include <stdio.h>
#include <inttypes.h>

#define LINES 9

int main(int argc, char *argv[])
{
	uint16_t out[256][LINES] = {{0}};
	char buf[256];
	FILE *fp;
	int i;

	fp = fopen(argv[1], "rb");
	if (!fp) {
		fprintf(stderr, "could not open %s\n", argv[1]);
		return -1;
	}

	while (fgets(buf, sizeof(buf), fp)) {
		uint8_t idx = buf[3];
		if (buf[0] != '#' && buf[1] != '#')
			continue;
		fgets(buf, sizeof(buf), fp); /* skip line */
		for (i = 0; i < LINES; i++) {
			uint16_t val = 0;
			fgets(buf, sizeof(buf), fp);
			if (buf[ 1] != '_') val |= 0x0001;
			if (buf[ 3] != '_') val |= 0x0002;
			if (buf[ 5] != '_') val |= 0x0004;
			if (buf[ 7] != '_') val |= 0x0008;
			if (buf[ 9] != '_') val |= 0x0010;
			if (buf[11] != '_') val |= 0x0020;
			if (buf[13] != '_') val |= 0x0040;
			if (buf[15] != '_') val |= 0x0080;
			if (buf[17] != '_') val |= 0x0100;
			if (buf[19] != '_') val |= 0x0200;
			out[idx][i] = val;
		}
	}

	printf("static uint16_t bitfont[0x100][%d] = {\n", LINES);
	for (i = 0; i < 0x100; i++) {
		int j;
		printf("\t{");
		for (j = 0; j < LINES; j++) {
			printf(" 0x%04x,", out[i][j]);
		}
		printf("},");
		if (i >= 0x20 && i < 0x7F) {
			printf(" /* %c */", i);
		}
		printf("\n");
	}
	printf("};\n");

	fclose(fp);

	return 0;
}
