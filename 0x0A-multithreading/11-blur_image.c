#include "multithreading.h"
#include <pthread.h>
#include <stdlib.h>
#include "10-blur_portion.c"

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define num_pixels(img) ((img)->w * (img)->h)
#define portion_end_index(p) (((p)->y + (p)->h) * (p)->img->w)
#define portion_start_index(p) ((p)->y * (p)->img->w + (p)->x)
#define MAX_NUM_THREADS 16

/**
 * blur_image - blurs an entire image using Gaussian Blur
 * @img_blur: address where blurred image is to be stored
 * @img: source image
 * @kernel: points to the convolution kernel to use
 **/
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	size_t num_slices = get_num_slices(MAX_NUM_THREADS);
	size_t num_threads = num_slices * num_slices;
	blur_portion_t *portions = malloc(sizeof(blur_portion_t) * num_threads);
	pthread_t *threads = malloc(sizeof(pthread_t) * num_threads);
	size_t i, x, y, w, h, remnant;

	w = max(img->w / num_slices, 1);
	for (i = 0, x = 0; x < img->w; x += w)
	{
		h = max(img->h / num_slices, 1);
		remnant = img->w - (x + w);
		if (remnant && remnant < w)
			w += remnant;

		for (y = 0; y < img->h; y += h)
		{
			remnant = img->h - (y + h);
			if (remnant && remnant < h)
				h += remnant;

			portion_init(&portions[i], img_blur, img, kernel, x, y, w, h);
			pthread_create(&threads[i], NULL, &blur_portion_mt, &portions[i]);
			i += 1;
		}
	}

	for (i = 0; i < num_threads; i++)
		pthread_join(threads[i], NULL);

	free(portions);
	free(threads);
}

/**
 * get_num_slices - given a max thread count, returns number of image
 *					subdivisions to use for blurring algorithm
 * @max_threads: maximum amount of threads allowed
 * Return: num_slices
 */
size_t get_num_slices(size_t max_threads)
{
	size_t n = 1;

	while (n * n <= max_threads)
		n++;

	return (n - 1);
}

/**
 * portion_init - initializes a new portion
 * @portion: pointer to portion to initialize
 * @img_blur: pointer to img_t struct representing copy of image to be blurred
 * @img: pointer to img_t struct representing original image
 * @kernel: pointer to kernel_t struct representing blur kernel
 * @x: x-coordinate of portion
 * @y: y-coordinate of portion
 * @w: width of portion
 * @h: height of portion
 */
void portion_init(blur_portion_t *portion, img_t *img_blur, img_t const *img,
				kernel_t const *kernel, size_t x, size_t y, size_t w, size_t h)
{
	if (portion)
	{
		portion->img = img;
		portion->img_blur = img_blur;
		portion->kernel = kernel;
		portion->x = x;
		portion->y = y;
		portion->w = w;
		portion->h = h;
	}
}

/**
 * blur_portion_mt - multithreading-ready wrapper for blur_portion
 * @portion: pointer to portion struct describing portion of image to blur
 * Return: N/A basically
 */
void *blur_portion_mt(void *portion)
{
	blur_portion((blur_portion_t const *)portion);
	pthread_exit(NULL);
}
