#include "imageWriter.h"



static int init_done = 0;
static png_byte ** row_pointers = NULL;
static png_structp png_ptr = NULL;
static png_infop info_ptr = NULL;

/* Given "bitmap", this returns the pixel of bitmap at the point
   ("x", "y"). */
/*
pixel_t * pixel_at (bitmap_t * bitmap, int x, int y)
{
	return bitmap->pixels + bitmap->width * y + x;
}
*/

void liberer_image(bitmap_t* bitmap)
{
	int y;
	for (y = 0; y < bitmap->height; y++) {
		png_free (png_ptr, row_pointers[y]);
	}
	png_free (png_ptr, row_pointers);
	free(bitmap->pixels);
}


/* Write "bitmap" to a PNG file specified by "path"; returns 0 on
   success, non-zero on error. */

int save_png_to_file (bitmap_t *bitmap, const char *path)
{
	FILE * fp;


	size_t x, y;
	png_byte *row;
	pixel_t* pixel;

			/* "status" contains the return value of this function. At first
	   it is set to a value which means 'failure'. When the routine
	   has finished its work, it is set to a value which means
	   'success'. */
	int status = -1;
	/* The following number is set by trial and error only. I cannot
	   see where it it is documented in the libpng manual.
	*/
	int pixel_size = 3;
	int depth = 8;

	fp = fopen (path, "wb");
	if (! fp) {
		goto fopen_failed;
	}

	if(!init_done)
	{
	png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct (png_ptr);
	if (info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	/* Set up error handling. */

	if (setjmp (png_jmpbuf (png_ptr))) {
		goto png_failure;
	}

	/* Set image attributes. */

	png_set_IHDR (png_ptr,
				  info_ptr,
				  bitmap->width,
				  bitmap->height,
				  depth,
				  PNG_COLOR_TYPE_RGB,
				  PNG_INTERLACE_NONE,
				  PNG_COMPRESSION_TYPE_DEFAULT,
				  PNG_FILTER_TYPE_DEFAULT);

	/* Initialize rows of PNG. */
	//printf("Début init\n"); fflush(stdout);

		row_pointers = png_malloc(png_ptr, bitmap->width * bitmap->height * sizeof(png_byte * ));
		for (y = 0; y < bitmap->height; y++)
		{
			row = png_malloc(png_ptr, sizeof(uint8_t) * bitmap->width * pixel_size);
			row_pointers[y] = row;
		}
		init_done = 1;
	}
	//printf("Entree for\n"); fflush(stdout);

#pragma omp parallel for private(row, pixel, x) shared(row_pointers, init_done) schedule(dynamic)
	for (y = 0; y < bitmap->height; y++)
	{
		row = row_pointers[y];
		for (x = 0; x < bitmap->width; x++)
		{
			pixel = pixel_at (bitmap, x, y);
			*row++ = pixel->red;
			*row++ = pixel->green;
			*row++ = pixel->blue;
		}
	}
	//printf("Fin init\n"); fflush(stdout);


	/* Write the image data to "fp". */

	png_init_io (png_ptr, fp);
	png_set_rows (png_ptr, info_ptr, row_pointers);
	png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	/* The routine has successfully written the file, so we set
	   "status" to a value which indicates success. */

	status = 0;
/*
	for (y = 0; y < bitmap->height; y++) {
		png_free (png_ptr, row_pointers[y]);
	}
	//png_free (png_ptr, row_pointers);
*/
	png_failure:
	png_create_info_struct_failed:
	png_destroy_write_struct (&png_ptr, &info_ptr);
	png_create_write_struct_failed:
	fclose (fp);
	fopen_failed:
	return status;
}

/* Given "value" and "max", the maximum value which we expect "value"
   to take, this returns an integer between 0 and 255 proportional to
   "value" divided by "max". */

/*
int pix (float value, float max)
{
	int res;
	if (value <= 0) {
		//printf("INF\n");
		return 0;
	}
	res = (int)((256.0 * value)/ max);
	if (res >= 256)
		res = 255;
	//printf("Valeur : %f, max : %f, pixel : %d\n", value, max, res);
	return res;
}
*/