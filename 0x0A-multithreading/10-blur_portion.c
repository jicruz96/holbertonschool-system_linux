#include "multithreading.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define num_pixels(img) ((img)->w * (img)->h)
#define portion_end_index(p) (((p)->y + (p)->h) * (p)->img->w)
#define portion_start_index(p) ((p)->y * (p)->img->w + (p)->x)

/**
 * blur_portion - blurs a portion of an image using Gaussian Blur
 * @portion: pointer to data structure describing an image portion
 **/
void blur_portion(blur_portion_t const *portion)
{
	size_t row, col, i, end;

	end = min(portion_end_index(portion), num_pixels(portion->img));

	for (row = portion_start_index(portion); row < end; row += portion->img->w)
		for (col = 0; col < portion->w; col++)
		{
			i = row + col;
			if (col && i % portion->img->w == 0)
				break;
			blur_pixel(portion, i);
		}
}

/**
 * blur_pixel - applies Gaussian Blur to one pixel
 *
 * @portion: pointer to struct describing portion of image
 * @target_i: index of pixel to blur
 */
void blur_pixel(blur_portion_t const *portion, size_t target_i)
{
	float r = 0, g = 0, b = 0, running_sum = 0, weight;
	pixel_t *pixel;
	int neighbor_i;
	size_t i, j;

	neighbor_i = target_i - (portion->kernel->size / 2) * (1 + portion->img->w);

	for (i = 0; i < portion->kernel->size; i++)
	{
		for (j = 0; j < portion->kernel->size; j++)
			if (is_neighbor(portion, neighbor_i + j, target_i))
			{
				pixel = &(portion->img->pixels[neighbor_i + j]);
				weight = portion->kernel->matrix[i][j];
				r += pixel->r * weight;
				g += pixel->g * weight;
				b += pixel->b * weight;
				running_sum += weight;
			}

		neighbor_i += portion->img->w;
	}

	pixel = &(portion->img_blur->pixels[target_i]);
	pixel->r = (int)(r / running_sum);
	pixel->g = (int)(g / running_sum);
	pixel->b = (int)(b / running_sum);
}

/**
 * is_neighbor - validates that a pixel array index `neigbor i` is truly a
 *               neighbor of pixel array index `target_it`
 *
 * @portion: pointer to struct describing portion of image we are on
 * @neighbor_i: candidate for index of neighbor of target pixel
 * @target_i: index of target pixel
 * Return: 1 if true, 0 if false
 */
int is_neighbor(blur_portion_t const *portion, int neighbor_i, size_t target_i)
{
	int target_col, neighbor_col, kernel_size = (int)portion->kernel->size;
	int num_pixels = portion->img->h * portion->img->w;
	int row_size = (int)portion->img->w;

	/* return false if neighbor_i is not a valid index value */
	if (neighbor_i < 0 || neighbor_i >= num_pixels)
		return (0);

	/* cases where target_i lies near image boundary */
	target_col = (int)(target_i % row_size);
	neighbor_col = (int)(neighbor_i % row_size);

	if (target_col - (kernel_size / 2) < 0)
		return (neighbor_col + (kernel_size / 2) < row_size);

	if (target_col + (kernel_size / 2) >= row_size)
		return (neighbor_col - (kernel_size / 2) >= 0);

	return (1);
}
