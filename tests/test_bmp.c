#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <mnist/mnist.h>

int main(int args, char* argv[])
{
	size_t i;
	size_t j;
	if (args < 2)
	{
		fprintf(stdout, "test_bmp <mnist_image_file> <mnist_label_file>\n");
		return 0;
	}

	mnist_file mfile;
	if (mnist_open(&mfile, argv[1]) < 0)
	{
		fprintf(stderr, "error loading image file\n");
		return 0;
	}

	printf("magic # = %d\n", mfile.magic_number);
	printf("# of items = %d\n", mfile.noitems);
	printf("rows = %d\n", mfile.rows);
	printf("columns = %d\n", mfile.columns);

	mnist_load_image(&mfile);

	mnist_load_label(mfile.images, argv[2]);

	srand(time(NULL));

	for (i = 0; i < mfile.noitems; i++)
	{
		char filename[20] = { '\0' };
		printf("Saving image with Label = %d\n", mfile.images[i].label);
		sprintf(filename, "obj/%x.%d.bmp",i,mfile.images[i].label);
		assert(mnist_save_bmp(&mfile.images[i], filename) >= 0);
	}

	printf("i=%d\n", i);
	mnist_image image;
	mnist_image_create(&image, 5);
	mnist_load_bmp("obj/0.5.bmp", &image);

	mnist_save_bmp(&image, "5.bmp");

	assert(image.label == 5);

	mnist_close(&mfile);
}