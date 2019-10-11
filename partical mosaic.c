
#include <iostream>

#include<opencv\cv.h>
#include<opencv\highgui.h>
#include<opencv\cxcore.h>
#include<stdio.h>

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

void mosaic(uchar** img, uchar** out, int Row, int Col, int Block) {
	int i, j, x, y, tmp, count;
	for (i = 0; i < Row; i += Block) {
		for (j = 0; j < Col; j += Block) {
			tmp = 0;
			count = 0;
			for (y = 0; y < Block; y++) {
				for (x = 0; x < Block; x++) {
					tmp += img[i + y][j + x];
					count++;
				}


				tmp /= count;
				for (y = 0; y < Block; y++) {
					for (x = 0; x < Block; x++) {
						out[i + y][j + x] = tmp;
					}

				}
			}
		}
	}
}

void Circle(uchar** img, uchar** Result, int Row, int Col, double diameter) {
	int i, j;
	double tmp, xSqure, ySqure;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			ySqure = (abs(Row / 2 - i - 10)) * (abs(Row / 2 - i - 10));
			xSqure = (abs(Col / 2 - j)) * (abs(Col / 2 - j));

			tmp = sqrt(ySqure + xSqure);

			if (tmp < diameter) Result[i][j] = img[i][j]; //255 white
			else Result[i][j] = 255;
		}
	}

}

void Circle2(uchar** img, uchar** Result, int Row, int Col, double diameter) {
	int i, j;
	double tmp, xSqure, ySqure;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			ySqure = (abs(Row / 2 - i - 10)) * (abs(Row / 2 - i - 10));
			xSqure = (abs(Col / 2 - j)) * (abs(Col / 2 - j));

			tmp = sqrt(ySqure + xSqure);

			if (tmp > diameter) Result[i][j] = img[i][j]; //255 white
			else Result[i][j] = 255;
		}
	}

}

void MaskAnd(uchar** in1img, uchar** in2img, uchar** outimg, int Row, int Col) {
	int i, j;

	for (i = 0; i < Row; i++) {
		for (j = 0; j < Col; j++) {
			outimg[i][j] = in1img[i][j] & in2img[i][j];
		}
	}
}


int main(int argc, char* argv[])
{

	int i, j, position;
	IplImage* cvImg;
	CvSize imgSize;
	uchar** img, ** result_img, ** result2_img, ** circle_img, ** circle2_img;

	if (argc != 4)
	{
		printf("Exe imgData x_size y_size \n");
		exit(0);
	}

	imgSize.width = atoi(argv[2]);
	imgSize.height = atoi(argv[3]);
	//position = atoi(argv[4]);

	img = uc_alloc(imgSize.width, imgSize.height);
	circle_img = uc_alloc(imgSize.width, imgSize.height);
	circle2_img = uc_alloc(imgSize.width, imgSize.height);
	result_img = uc_alloc(imgSize.width, imgSize.height);
	result2_img = uc_alloc(imgSize.width, imgSize.height);
	read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);

	cvImg = cvCreateImage(imgSize, 8, 1);

	mosaic(img, result_img, imgSize.width, imgSize.height, 16);
	Circle(result_img, circle_img, imgSize.width, imgSize.height, 200);
	Circle2(img, circle2_img, imgSize.width, imgSize.height, 200);

	MaskAnd(circle_img, circle2_img, result2_img, imgSize.width, imgSize.height);


	for (i = 0; i < imgSize.height; i++)
		for (j = 0; j < imgSize.width; j++)
		{
			((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = result2_img[i][j];
		}

	cvNamedWindow(argv[1], 1);
	cvShowImage(argv[1], cvImg);

	cvWaitKey(0);

	cvDestroyWindow("image");
	cvReleaseImage(&cvImg);

	getchar();

	return 0;

}