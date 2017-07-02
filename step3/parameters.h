const unsigned int image_width  = 16384;
const unsigned int image_height = 16384;

// The x and y pixels in the image represent the real and complex components of
// of an imaginary number C = x + yi. The min and max values of the real and 
// complex components are defined as: 
const double y_min =  -2.0;
const double y_max =  2.0;
const double x_min =  -2.0;
const double x_max =  2.0;

const double radius = 2.0;

const unsigned int iteration_max = 1000;

const double x_range = x_max - x_min;
const double x_pixel_size = x_range / image_width;

const double y_range = y_max - y_min;
const double y_pixel_size = y_range / image_height;
