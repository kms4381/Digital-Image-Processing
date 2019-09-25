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
	printf("Average of Image %lf \n", avg);

	return avg;
}

void makeBinary(uchar** img, uchar** out, int size_x, int size_y, double avg)
{
	int i, j;

	for (i = 0; i < size_x; i++)
	{
		for (j = 0; j < size_y; j++)
		{
			if (img[i][j] > avg)
				out[i][j] = 255;      // chang value for book with avg-30
			else
				out[i][j] = 0;
		}
	}
}

int main(int argc, char* argv[])
{

	int i, j;
	IplImage* cvImg;
	CvSize imgSize;
	uchar** img;

	if (argc != 4)
	{
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}
	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);
	img = uc_alloc(imgSize.width, imgSize.height);
	result_img = uc_alloc(imgSize.width, imgSize.height);

	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);
	cvImg = cvCreateImage(imgSize, 8, 1);

	average(img, imgSize.width, imgSize.height);
	makeBinary(img, result_img, imgSize.width, imgSize.height, avg);

	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++)
		{

			((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = result_img[i][j];

		}
	cvNamedWindow(argv[1], 1);
	cvShowImage(argv[1], cvImg);

	cvWaitKey(0);

	cvDestroyWindow("image");
	cvReleaseImage(&cvImg);

	getchar();

	return 0;

}