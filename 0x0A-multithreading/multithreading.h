#ifndef MULTITHREADING_H
#define MULTITHREADING_H
#include <stddef.h>
#include <stdint.h>

/**
 * struct pixel_s - RGB pixel
 *
 * @r: Red component
 * @g: Green component
 * @b: Blue component
 */
typedef struct pixel_s
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel_t;

/**
 * struct img_s - Image
 *
 * @w:      Image width
 * @h:      Image height
 * @pixels: Array of pixels
 */
typedef struct img_s
{
	size_t w;
	size_t h;
	pixel_t *pixels;
} img_t;

/**
 * struct kernel_s - Convolution kernel
 *
 * @size:   Size of the matrix (both width and height)
 * @matrix: Kernel matrix
 */
typedef struct kernel_s
{
	size_t size;
	float **matrix;
} kernel_t;

/**
 * struct blur_portion_s - Information needed to blur a portion of an image
 *
 * @img:      Source image
 * @img_blur: Destination image
 * @x:        X position of the portion in the image
 * @y:        Y position of the portion in the image
 * @w:        Width of the portion
 * @h:        Height of the portion
 * @kernel:   Convolution kernel to use
 */
typedef struct blur_portion_s
{
	img_t const *img;
	img_t *img_blur;
	size_t x;
	size_t y;
	size_t w;
	size_t h;
	kernel_t const *kernel;
} blur_portion_t;

/* FUNCTION PROTOTYPES */

/* 0-thread_entry.c */
void *thread_entry(void *arg);

/* 1-tprintf.c AND 20-tprintf.c */
int tprintf(char const *format, ...);

/* 10-blur_portion.c */
void blur_portion(blur_portion_t const *portion);
void blur_pixel(blur_portion_t const *portion, size_t pixel_index);
int is_neighbor(blur_portion_t const *, int, size_t);

/* 11-blur_image.c */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel);
void portion_init(blur_portion_t *portion, img_t *img_blur, img_t const *img,
				kernel_t const *kernel, size_t x, size_t y, size_t w, size_t h);
size_t get_num_slices(size_t max_threads);
void *blur_portion_mt(void *portion);


#endif /* MULTITHREADING_H */
