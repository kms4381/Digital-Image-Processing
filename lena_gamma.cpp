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

void PowImg(uchar** img, uchar** Result, int Row, int Col, double gamma)
{
	int i, j;
	double tmp;

	for (i = 0; i < Row; i++)//За
		for (j = 0; j < Col; j++)//ї­
		{
			tmp = pow(img[j][i] / 255., 1 / gamma);

			if (tmp * 255 > 255)tmp = 1;
			else if (tmp * 255 < 0) tmp = 0;

			tmp = tmp * 255;

			Result[j][i] = tmp;
		}
}

int main(int argc, char* argv[])
{

	int i, j;
	double gamma;
	IplImage* cvImg;
	CvSize imgSize;
	uchar** img;
	uchar** result_img;

	if (argc != 5)
	{
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}
	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);
	gamma = atof(argv[4]);

	img = uc_alloc(imgSize.width, imgSize.height);
	result_img = uc_alloc(imgSize.width, imgSize.height);

	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);
	cvImg = cvCreateImage(imgSize, 8, 1);

	PowImg(img, result_img, imgSize.width, imgSize.height, gamma);

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