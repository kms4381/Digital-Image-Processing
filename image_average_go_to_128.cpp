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

void PowImg(uchar** img, uchar** Result, int Row, int Col, double gamma)
{
	int i, j;
	double tmp;

	for (i = 0; i < Row; i++)//행
		for (j = 0; j < Col; j++)//열
		{
			tmp = pow(img[j][i] / 255., 1 / gamma);

			if (tmp * 255 > 255)tmp = 1;
			else if (tmp * 255 < 0) tmp = 0;

			tmp = tmp * 255;

			Result[j][i] = tmp;
		}
}


int main(int argc, char* argv[]) {
	int i, j;
	double avg, gamma = 1;
	IplImage* cvImg;
	CvSize imgSize;
	uchar** img,** result_img;

	if (argc != 4) {
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}

	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);

	cvImg = cvCreateImage(imgSize, 8, 1);

	img = uc_alloc(imgSize.width, imgSize.height);
	result_img = uc_alloc(imgSize.width, imgSize.height);

	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);

	avg = average(img, imgSize.width, imgSize.height);

	if (avg < 128) {
		while (avg < 128) {
			gamma = gamma + 0.001;
			PowImg(img, result_img, imgSize.width, imgSize.height, gamma);
			avg = average(result_img, imgSize.width, imgSize.height);
			printf("Average of Image %lf \n", avg);
		}
	}
	else if (avg > 128) {
		while (avg > 128) {
			gamma = gamma - 0.001;
			PowImg(img, result_img, imgSize.width, imgSize.height, gamma);
			avg = average(result_img, imgSize.width, imgSize.height);
			printf("Average of Image %lf \n", avg);
		}
	}
	else
		PowImg(img, result_img, imgSize.width, imgSize.height, gamma);

	for (i = 0; i < imgSize.width; i++)
		for (j = 0; j < imgSize.height; j++) {
			((uchar*)(cvImg->imageData + cvImg->widthStep * j))[i] = img[j][i];
		}

	cvNamedWindow(argv[1], 0);
	cvShowImage(argv[1], cvImg);
	cvWaitKey(0);

	cvReleaseImage(&cvImg);


	return 0;
}