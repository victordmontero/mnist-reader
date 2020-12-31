#include <stdlib.h>
#include <stdio.h>
#include <mnist/mnist.h>

#define SIZE 4
#define LABEL_SIZE 4

void printASCIIImage(unsigned char* pixels)
{
	for (size_t j = 0; j < 28; j++)
	{
		for (size_t i = 0; i < 28; i++)
		{
			unsigned char num = pixels[j * 28 + i];

			if (num > 0 && num <= 50) {
				printf_s("+");
			}
			else if (num > 51 && num <= 100) {
				printf_s("x");
			}
			else if (num > 101 && num <= 200) {
				printf_s("*");
			}
			else if (num > 201 && num <= 255) {
				printf_s("#");
			}
			else {
				printf_s("0");
			}
		}
		printf_s("\n");
	}
}

int main(int args, char* argv[])
{
	mnist_file mfile;
	mnist_open(&mfile, argv[1]);

	printf("magic # = %d\n", mfile.magic_number);
	printf("# of items = %d\n", mfile.noitems);
	printf("rows = %d\n", mfile.rows);
	printf("columns = %d\n", mfile.columns);

	mnist_load_image(&mfile);

	mnist_load_label(mfile.images, argv[2]);

	for (size_t i = 0; i < 21; i++)
	{
		printf("Label = %d\n", mfile.images[i].label);
		printASCIIImage(mfile.images[i].pixels);
	}

	mnist_close(&mfile);

}