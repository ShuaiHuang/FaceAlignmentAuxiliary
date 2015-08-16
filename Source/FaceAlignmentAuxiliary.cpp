// FaceAlignmentAuxiliary.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

Mat getMask(Mat &_inputImg)
{
	Mat retImg;
	Canny(_inputImg, retImg, 80, 90);
	return retImg;
}

Mat getColorMask(Mat &_mask, Scalar color)
{
	Mat colorMask = Mat(_mask.size(), CV_8UC3);
	if (_mask.channels() == 1)
	{
		colorMask.setTo(Scalar(0, 0, 0));
		vector<Mat> vecColorMask;
		split(colorMask, vecColorMask);
		for (int channel = 0; channel < 3; channel++)
		{
			_mask.copyTo(vecColorMask[channel]);
			vecColorMask[channel] = vecColorMask[channel] / 255 * color(channel);
		}
		merge(vecColorMask, colorMask);
	}
	return colorMask;
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
	Mat colorMask = getColorMask(faceMask, Scalar(255, 255, 255));

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

