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
* See file LICENSE or go to https://github.com/victordmontero/mnist-reader/blob/master/LICENSE for full license details.
*/

#ifndef MNIST_H_
#define MNIST_H_

#include <stdio.h>
#include <string.h>

#define MNIST_LBL_MAGIC_NUMBER 2049
#define MNIST_IMG_MAGIC_NUMBER 2051

typedef unsigned char mnist_label;

typedef struct _mnist_image
{
	unsigned char* pixels;
	int rows;
	int columns;
	mnist_label label;
}mnist_image;

typedef struct _mnist_file
{
	int magic_number;
	int noitems;
	int rows;
	int columns;
	int fd;
	mnist_image* images;
}mnist_file;

extern int swapEndianess(int number);
extern int mnist_open(mnist_file*, char*);
extern int mnist_load_image(mnist_file*);
extern int mnist_load_label(mnist_image*, char*);
extern void mnist_close(mnist_file*);
extern int mnist_save_file(mnist_file* from_file, const char* to_img_filename, const char* to_label_filename);
extern int mnist_add_image(mnist_image* this_image, mnist_file* to_file);
extern int mnist_image_create(mnist_image* image, mnist_label label);
extern void mnist_file_create(mnist_file* file);

extern int mnist_save_bmp(mnist_image* from_image, const char* to_filename);
extern int mnist_load_bmp(const char* from_filename, mnist_image* to_image);
#endif