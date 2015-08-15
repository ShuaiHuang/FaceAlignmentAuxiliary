// FaceAlignmentAuxiliary.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

Mat getMask(Mat &_inputImg)
{
	Mat retImg;
	Canny(_inputImg, retImg, 80, 90);
	return retImg;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		return -1;
	}

	string faceFile(argv[1]);
	Mat faceImg = imread(faceFile);
	Mat faceMask = getMask(faceImg);
	Mat colorMask = Mat(faceMask.size(), CV_8UC3);
	colorMask.setTo(Scalar(0, 0, 0));
	vector<Mat> vecColorMask;
	split(colorMask, vecColorMask);
	faceMask.copyTo(vecColorMask[1]);
	merge(vecColorMask, colorMask);

	VideoCapture capture(0); // open the default camera
	if (!capture.isOpened())  // check if we succeeded
	{
		return -1;
	}

	namedWindow("Mask");

	while (true)
	{
		Mat frame;
		capture >> frame;
		
		Rect maskRect(100, 100, faceMask.cols, faceMask.rows);
		colorMask.copyTo(frame(maskRect), faceMask);
		imshow("Mask", frame);

		if (waitKey(30) >= 0)
		{
			break;
		}
	}

	return 0;
}

