#include<iostream>
#include<opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main()
{    
	float wh_ratio_high = 1.1;
	float wh_ratio_low = 0.9;
	float area_th = 100;


	cv::Mat srcMat = cv::imread("C:/Users/DELL/Desktop/9.jpg");
	if (srcMat.empty())
	{
		cout << "fai to read pic" << endl;
		return-1;
	}
	Mat bnyMat;
	Mat disMat;
	Mat sttMat;
	Mat cntMat;
	Mat lblMat;

	srcMat.copyTo(disMat);
	cvtColor(srcMat, srcMat, COLOR_BGR2GRAY);
	threshold(srcMat, bnyMat, 100, 255, THRESH_OTSU);
	bnyMat = 255 - bnyMat;

	int nComp = connectedComponentsWithStats(bnyMat, lblMat, sttMat, cntMat);

	int * flag = new int[nComp];
	memset(flag, 0, sizeof(int)*nComp);


	for (int i = 1; i < nComp; i++) {
		float width = (float)sttMat.at<int>(i, CC_STAT_WIDTH);
		float height = (float)sttMat.at<int>(i, CC_STAT_HEIGHT);
		float ratio = width / height;

		if (
			(ratio > wh_ratio_low)
			&& (ratio < wh_ratio_high)
			&& (sttMat.at<int>(i, CC_STAT_AREA) > area_th)
			)
		{
			
			Rect bbox;
			bbox.x = sttMat.at<int>(i, 0);
			bbox.y = sttMat.at<int>(i, 1);
	
			bbox.width = sttMat.at<int>(i, 2);
			bbox.height = sttMat.at<int>(i, 3);
			
			rectangle(disMat, bbox, CV_RGB(255, 255, 0), 2, 8, 0);

			
			flag[i] = 1;
		}
	}


	for (int i = 0; i < srcMat.rows; i++)
	{
		for (int j = 0; j < srcMat.cols; j++)
		{
			int no = lblMat.at<int>(i, j);

			if (flag[no] == 1) {
				disMat.at<Vec3b>(i, j)[0] = 0;
				disMat.at<Vec3b>(i, j)[1] = 255;
				disMat.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}

	delete[] flag;

	imshow("source image", srcMat);
	imshow("binary image", bnyMat);
	imshow("hole image", disMat);

	waitKey(0);

	destroyAllWindows();


}