/*
* DISCLAIMER
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING,
* BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
* OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* This file is part of mnist-reader which is released under GPLv3 License.
* See file LICENSE or go to https://github.com/victordmontero/mnist-reader/LICENSE for full license details.
*/

#include <mnist/mnist.h>
#include <libbmp.h>

int swapEndianess(int number)
{
	return (number & 0xFF000000) >> 24 |
		(number & 0x00FF0000) >> 8 |
		(number & 0x000000FF) << 24 |
		(number & 0x0000FF00) << 8;
}

int mnist_open(mnist_file* mfile, char* filename)
{
	int rows = 0;
	int columns = 0;
	FILE* f;

	if (mfile == NULL)
	{
		return -1;
	}

	int rc = fopen_s(&f, filename, "rb");
	if (rc != 0)
		return rc;

	fseek(f, 0, SEEK_SET);

	rc = fread_s(mfile, sizeof(mnist_file), sizeof(int), 4, f);

	if (rc <= 0)
		return rc;

	mfile->fd = f;

	mfile->magic_number = swapEndianess(mfile->magic_number);
	mfile->noitems = swapEndianess(mfile->noitems);
	mfile->rows = swapEndianess(mfile->rows);
	mfile->columns = swapEndianess(mfile->columns);

	return rc;
}

int mnist_load_image(mnist_file* mfile)
{
	char* pixels;
	size_t i = 0;
	int rc = 0;
	mfile->images = (mnist_image*)malloc(sizeof(mnist_image) * mfile->noitems);

	if (mfile->images == NULL)
	{
		errno = ENOMEM;
		return -1;
	}

	memset(mfile->images, 0, sizeof(mnist_image) * mfile->noitems);

	for (i = 0; i < mfile->noitems; i++)
	{
		pixels = (char*)malloc(mfile->rows * mfile->columns);

		if (pixels == NULL)
		{
			errno = ENOMEM;
			return -1;
		}

		rc = fread(pixels, sizeof(char), mfile->rows * mfile->columns, mfile->fd);

		if (rc <= 0) {
			free(pixels);
			return -1;
		}

		mfile->images[i].rows = mfile->rows;
		mfile->images[i].columns = mfile->columns;
		mfile->images[i].pixels = pixels;
	}
	return rc;
}

int mnist_load_label(mnist_image* images, char* lblfilename)
{
	FILE* lblfile;
	unsigned int nolabels = 0;
	int rc = 0;

	rc = fopen_s(&lblfile, lblfilename, "rb");

	if (rc != 0)
		return rc;

	fseek(lblfile, 4, SEEK_CUR);

	rc = fread_s(&nolabels, sizeof(unsigned int), sizeof(int), 1, lblfile);
	nolabels = swapEndianess(nolabels);

	//fseek(lblfile, 4, SEEK_CUR);

	for (size_t i = 0; i < nolabels; i++)
	{
		rc = fread_s(&images[i].label, sizeof(mnist_label), sizeof(char), 1, lblfile);

		if (rc <= 0)
		{
			errno = ENOMEM;
			return -1;
		}
	}

	fclose(lblfile);

	return rc;
}

void mnist_close(mnist_file* mfile)
{
	for (size_t i = 0; i < mfile->noitems; i++)
	{
		free(mfile->images[i].pixels);
	}
	free(mfile->images);
	fclose(mfile->fd);
}

int mnist_save_file(mnist_file* from_file, const char* to_img_filename, const char* to_label_filename)
{
	FILE* to_image_file = NULL;
	FILE* to_label_file = NULL;

	int header[4] = { 0 };


	if ((!fopen_s(&to_image_file, to_img_filename, "a+b")) &&
		(!fopen_s(&to_label_file, to_label_filename, "a+b")))
	{
		header[0] = swapEndianess(MNIST_IMG_MAGIC_NUMBER);
		header[1] = swapEndianess(from_file->noitems);
		header[2] = swapEndianess(from_file->rows);
		header[3] = swapEndianess(from_file->columns);

		fwrite(header, sizeof(int), 4, to_image_file);

		if (ferror(to_image_file))
		{
			fclose(to_image_file);
			return -1;
		}

		header[0] = swapEndianess(MNIST_LBL_MAGIC_NUMBER);

		fwrite(header, sizeof(int), 2, to_label_file);

		if (ferror(to_label_file))
		{
			fclose(to_label_file);
			return -1;
		}

		for (size_t i = 0; i < from_file->noitems; i++)
		{
			mnist_image img = from_file->images[i];

			fwrite(img.pixels, sizeof(char), img.rows * img.columns, to_image_file);
			if (ferror(to_image_file))
			{
				fclose(to_image_file);
				return -1;
			}

			fwrite(&img.label, sizeof(char), 1, to_label_file);
			if (ferror(to_label_file))
			{
				fclose(to_label_file);
				return -1;
			}

		}
		fclose(to_label_file);
		fclose(to_image_file);
		return 0;
	}
	return -1;
}

int mnist_add_image(mnist_image* this_image, mnist_file* to_file)
{
	/*This is really unefficient. but later on i'll change
	the data structure for list or somthing. Please bear with me ;)*/
	if (to_file->images)
	{
		mnist_image* images_copies = (mnist_image*)malloc(sizeof(mnist_image) * (to_file->noitems + 1));

		if (!images_copies)
		{
			errno = ENOMEM;
			return -1;
		}

		memset(images_copies, 0, sizeof(mnist_image) * to_file->noitems);

		memcpy(images_copies, to_file->images, sizeof(mnist_image) * to_file->noitems);
		free(to_file->images);
		images_copies[to_file->noitems] = *this_image;
		to_file->images = images_copies;
		to_file->noitems++;
		return 0;
	}
	to_file->images = (mnist_image*)malloc(sizeof(mnist_image));

	if (!to_file->images)
	{
		errno = ENOMEM;
		return -1;
	}

	to_file->images[0] = *this_image;
	to_file->noitems = 1;

	return 0;

}

int mnist_image_create(mnist_image* image, mnist_label label)
{
	memset(image, 0, sizeof(mnist_image));
	image->label = label;
}

void mnist_file_create(mnist_file* file)
{
	memset(file, 0, sizeof(mnist_file));
}

extern int mnist_save_bmp(mnist_image* from_image, const char* to_filename)
{
	bmp_img img;
	bmp_img_init_df(&img, from_image->rows, from_image->columns);
	img.img_header.bfReserved = from_image->label;

	for (size_t i = 0; i < from_image->rows; i++)
	{
		for (size_t j = 0; j < from_image->columns; j++)
		{
			int mono = 255 - from_image->pixels[j * from_image->rows + i];
			bmp_pixel_init(&img.img_pixels[j][i], mono, mono, mono);
		}
	}

	enum bmp_error error = bmp_img_write(&img, to_filename);

	bmp_img_free(&img);
	return error == BMP_OK ? 0 : -1;
}
extern int mnist_load_bmp(const char* from_filename, mnist_image* to_image)
{
	bmp_img img;

	if (!from_filename)
		return -1;

	enum bmp_error error = bmp_img_read(&img, from_filename);

	if (error != BMP_OK)
		return 0; // 0 means failure

	to_image->rows = img.img_header.biWidth;
	to_image->columns = img.img_header.biHeight;

	if (!to_image->pixels)
		to_image->pixels = (unsigned char*)malloc(to_image->rows * to_image->columns);

	for (size_t i = 0; i < to_image->rows; i++)
	{
		for (size_t j = 0; j < to_image->columns; j++)
		{
			int hex = (img.img_pixels[i][j].red + img.img_pixels[i][j].green + img.img_pixels[i][j].blue) / 3;
			to_image->pixels[i * to_image->rows + j] = 255 - hex;
		}
	}

	bmp_img_free(&img);

}