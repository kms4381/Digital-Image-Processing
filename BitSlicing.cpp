#include <opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>

#include <stdio.h>

#define unsigned char uchar


uchar** uc_alloc(int size_x, int size_y)
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



double average(uchar** img, int size_x, int size_y)
{
	double sum = 0, avg;
	int i, j;

	for (i = 0; i < size_x; i++)
	{
		for (j = 0; j < size_y; j++)
		{
			sum += img[i][j];
		}
	}
	avg = sum / ((double)size_x * size_y);

	return avg;
}

void BitSlicing(uchar** img, uchar** Result, int Row, int Col, int position)
{
	int i, j;
	uchar mask = 0x01;
	mask <<= position;

	for (i = 0; i < Row; i++)
		for (j = 0; j < Col; j++)
		{
			if ((mask & img[j][i]) == pow(2, position))
			{
				Result[j][i] = 255;
			}
			else
			{
				Result[j][i] = 0;
			}

		}
}

int main(int argc, char* argv[])
{

	int i, j, count;
	double sum;
	IplImage* cvImg;
	CvSize imgSize;
	uchar** img, ** result_img;
	uchar** simg4, ** simg5, ** simg6, ** simg7;

	if (argc != 5)
	{
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}
	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);
	count = atoi(argv[4]);
	img = uc_alloc(imgSize.width, imgSize.height);
	result_img = uc_alloc(imgSize.width, imgSize.height);

	simg5 = uc_alloc(imgSize.width, imgSize.height);
	simg6 = uc_alloc(imgSize.width, imgSize.height);
	simg7 = uc_alloc(imgSize.width, imgSize.height);
	simg4 = uc_alloc(imgSize.width, imgSize.height);

	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);

	cvImg = cvCreateImage(imgSize, 8, 1);

	BitSlicing(img, simg7, imgSize.width, imgSize.height, 7);
	BitSlicing(img, simg6, imgSize.width, imgSize.height, 6);
	BitSlicing(img, simg5, imgSize.width, imgSize.height, 5);
	BitSlicing(img, simg4, imgSize.width, imgSize.height, 4);


	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++)
		{
			if (count == 4) {
				result_img[i][j] = simg7[i][j] / 2 + simg6[i][j] / 4 + simg5[i][j] / 8 + simg4[i][j] / 16;

			}
			if (count == 3) {
				result_img[i][j] = simg7[i][j] / 2 + simg6[i][j] / 4 + simg5[i][j] / 8;
			}


			((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = result_img[i][j];

		}

	sum = average(result_img, imgSize.width, imgSize.height);
	printf("Average is %f\n", sum);


	cvNamedWindow(argv[1], 1);
	cvShowImage(argv[1], cvImg);

	cvWaitKey(0);

	cvDestroyWindow("image");
	cvReleaseImage(&cvImg);

	getchar();

	return 0;

}