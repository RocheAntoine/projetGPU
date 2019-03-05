#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H


#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>


/* A coloured pixel. */

typedef struct
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}
		pixel_t;

/* A picture. */

typedef struct
{
	pixel_t *pixels;
	size_t width;
	size_t height;
}
		bitmap_t;

#define pixel_at(bitmap, x, y) (((bitmap)->pixels) + ((bitmap)->width) * (y) + (x))
#define pix(value, max) ((value) <= 0 ? 0 : (int)(256.0 * (float)(value) / (float)(max)) - 1)
//#define pix(value, max) (tmp = )
//pixel_t * pixel_at (bitmap_t * bitmap, int x, int y);
int save_png_to_file (bitmap_t *bitmap, const char *path);
void liberer_image(bitmap_t*);
//int pix (float value, float max);

#endif
