#include <opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>

#include <stdio.h>

#define unsigned char uchar


uchar** uc_alloc(int size_x, int size_y) // 행렬 메모리 할당 x,y
{

	uchar** m;
	int i;

	if ((m = (uchar * *)calloc(size_y, sizeof(uchar*))) == NULL)
	{
		printf("uc_alloc error 1\7\n");
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL) {
			printf("uc_alloc error 2\7\n");
			exit(0);

		}
	return m;
}

void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, char* filename)
{
	int i;
	FILE* f;
	if ((fopen_s(&f, filename, "rb")) != NULL)
	{
		printf("%s File open Error! \n", filename);
		exit(0);

	}
	for (i = 0; i < size_y; i++)
		if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Read Error! \n");
			exit(0);

		}
	fclose(f);
}

void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, char* filename)
{
	int i;
	FILE* f;

	if ((fopen_s(&f, filename, "wb")) != NULL)
	{
		printf("%s File open Error! \n", filename);
		exit(0);
	}

	for (i = 0; i < size_y; i++)
		if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Write Error! \n");
			exit(0);
		}
	fclose(f);
}

void Negative(int size_x, int size_y, uchar** img, uchar** Result)
{
	int i, j;

	for (i = 0; i < size_y; i++)
		for (j = 0; j < size_x; j++)
			Result[i][j] = 255 - img[i][j]
}

void mosaic(uchar** img, uchar** out, int Row, int Col, char Block) {
	int i, j, x, y, tmp, count;

	for (i = 0; i < Row; i += Block) {
		for (j = 0; j < Col; j += Block) {
			tmp = 0;
			count = 0;
			for (y = 0; y < Block; y++) {
				for (x = 0; x < Block; x++) {
					tmp += img[j + x][i + y];
					count++;
				}
			}
			tmp /= count;
			for (y = 0; y < Block; y++) {
				for (x = 0; x < Block; x++) {
					out[j + x][i + y] = tmp;
				}
			}
		}
	}
}


int main(int argc, char* argv[])
{

	int i, j;
	IplImage* cvImg;
	CvSize imgSize;
	uchar** img;

	if (argc != 6)
	{
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}
	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);
	img = uc_alloc(imgSize.width, imgSize.height);
	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);
	
	cvImg = cvCreateImage(imgSize, 8, 1);
	//Negative(imgSize.width, imgSize.height, img, img);
	mosaic(img, img, imgSize.width, imgSize.height, atoi(argv[4]));
	
	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++)
		{

			((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];

		}
	cvNamedWindow(argv[1], 1);
	cvShowImage(argv[1], cvImg);

	write_ucmatrix(imgSize.width, imgSize.height, img, argv[5]);
	cvWaitKey(0);

	cvDestroyWindow("image");
	cvReleaseImage(&cvImg);

	getchar();

	return 0;

}