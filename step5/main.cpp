#include <stdlib.h>
#include <stdio.h>
#include "omp.h"
#include "parameters.h"

/* ------ Function Declaration ------ */
#pragma acc routine seq
unsigned int calculate_pixel(int r, int c);


/* ------ Main ------ */
int main(){

	// Allocate memory for image array
	size_t bytes_in_image = 3 * (image_width * image_height) * sizeof(unsigned char);
	unsigned char *image = (unsigned char*)malloc(bytes_in_image);
	#pragma acc enter data create(image[0:3*(image_width*image_height)])

	unsigned int iter;

	unsigned int num_blocks = 16;
	unsigned int block_height = image_height / num_blocks;

  double start_time = omp_get_wtime();

#pragma acc data present(image[0:3*(image_width*image_height)])
  // Calculate all pixels in the image array and set colors
	for(int block=0; block<num_blocks; block++){

		unsigned int y_start = block*block_height;
		unsigned int y_stop  = y_start + (block_height);

#pragma acc parallel loop async(block % 2 + 1)
		for(int row=y_start; row<y_stop; row++){
			for(int col=0; col<image_width; col++){

				iter = calculate_pixel(row, col);

				if(iter >= iteration_max){
					image[3 * (row * image_width + col) + 0] = 0;
					image[3 * (row * image_width + col) + 1] = 0;
					image[3 * (row * image_width + col) + 2] = 0;
				}
				else{
					image[3 * (row * image_width + col) + 0] = iter;
					image[3 * (row * image_width + col) + 1] = iter;
					image[3 * (row * image_width + col) + 2] = iter;
				}

			}
		}
#pragma acc update self(image[block*(3*block_height*image_width):block_height*(3*image_width)]) async(block % 2 + 1)
	}

#pragma acc wait

  double stop_time = omp_get_wtime();
  double elapsed_time = stop_time - start_time;

  printf("\n------------------------------------\n");
  printf("Elapsed Time (s): %f\n", elapsed_time);
  printf("------------------------------------\n\n");

	// Write image array to file in PPM format
	FILE *file = fopen("mandelbrot.ppm", "wb");
	fprintf(file, "P6\n%d %d\n%d\n", image_width, image_height, 255);
	fwrite(image, sizeof(unsigned char), 3 * (image_width * image_height), file);

	fclose(file);
#pragma acc exit data delete(image[0:3*(image_width*image_height)])
	free(image);

	return 0;

}


/* ------ Function Implementation ------ */
unsigned int calculate_pixel(int r, int c){

  unsigned int iteration = 0;

  double x = 0.0;
  double y = 0.0;
  double x_temp;

  // Calculate real values of the complex number C based on the x pixel
  double x0 = ( (x_pixel_size * c) + x_min ) + (x_pixel_size / 2.0);

  // Calculate imaginary values of the complex number C based on the y pixel
  double y0 = ( (y_pixel_size * r) + y_min ) + (y_pixel_size / 2.0);

	// Determine if complex number C = x0 + iy0 is in the set
  while( (iteration < iteration_max) && ( (x * x + y * y) < radius) ){

    x_temp = (x * x) - (y * y) + x0;
    y = (2.0 * x * y) + y0;
    x = x_temp;
    iteration = iteration + 1;

  }

  return iteration;

}
