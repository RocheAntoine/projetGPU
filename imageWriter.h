#ifndef IMAGEWRITER_H
#define IMAGEWRITER_H


#include <opencv2/core/core_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>

static int nImage = 0;

void print_img(float *img, CvMat*, int nx, int ny);
#endif
