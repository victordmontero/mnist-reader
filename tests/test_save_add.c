#include <stdio.h>
#include <assert.h>
#include <mnist/mnist.h>
#include <Windows.h>
#include <Shlwapi.h>

#pragma comment( lib, "shlwapi.lib")

void check_image(mnist_image* first_image, mnist_image* second_image)
{
	for (size_t i = 0; i < first_image->columns; i++)
	{
		for (size_t j = 0; j < first_image->rows; j++)
		{
			assert(first_image->pixels[j * first_image->rows + i] == second_image->pixels[j * second_image->rows + i]);
		}
	}
}

int main(int args, char* argv[])
{
	mnist_file file;
	mnist_image image;
	mnist_image image2;

	mnist_file_create(&file);
	mnist_image_create(&image, 1);
	mnist_image_create(&image2, 2);

	assert(!file.images);
	assert(!image.pixels);
	assert(!image2.pixels);

	mnist_load_bmp("obj/0.5.bmp", &image);

	file.rows = file.columns = image.rows;

	assert(mnist_add_image(&image, &file) == 0);

	assert(file.noitems == 1);
	//check_image(&image, &file.images[0]);

	/*------------------------------------------------------------*/

	mnist_load_bmp("obj/1.0.bmp", &image2);

	file.rows = file.columns = image.rows;

	assert(mnist_add_image(&image2, &file) == 0);

	assert(file.noitems == 2);
	//check_image(&image2, &file.images[1]);

	mnist_save_file(&file, "image_test\0", "label_test\0");
	assert(PathFileExistsA("./image_test") && PathFileExistsA("./label_test"));

}