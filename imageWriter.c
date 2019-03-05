#include "imageWriter.h"

void print_img(float *img, CvMat* mat, int nx, int ny)
{
	int i, j, ii, jj;
	char img_name[500];
	FILE * f;
	double start, end;
	start = omp_get_wtime();
	sprintf(img_name, "img_%05d.png", nImage++);
	int sx=1, sy=1;
	if (nx<256 || ny<256) {
		sx = 256/nx;
		sy = 256/ny;
	}
	unsigned char* mat_dat=mat->data.ptr;
	for (j = 0; j < ny; ++j)
		for (i = 0; i < nx; ++i){
			unsigned char r,g,b;
			float t = img[i+(ny-1-j)*nx];
			b = (uchar)(255*(t<0.5?sin(2*M_PI*t):0));
			g = (uchar)(255*(t*t*t));
			r = (uchar)(255*sqrt(t));
			for (jj = 0; jj < sy; ++jj)
				for (ii = 0; ii < sx; ++ii){
					mat_dat[(i*sx+ii+(j*sy+jj)*nx*sx)*3] = b;
					mat_dat[(i*sx+ii+(j*sy+jj)*nx*sx)*3+1] = g;
					mat_dat[(i*sx+ii+(j*sy+jj)*nx*sx)*3+2] = r;
				}
		}
	cvSaveImage(img_name, mat, NULL);
	end = omp_get_wtime();
	printf("Écriture de l'image : %s (%f s)\n", img_name, end-start);
}