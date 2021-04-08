//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
void SuMiao(Mat& src)
{  
  //  Mat src = imread("D:/arrow.jpg",1);  
//	Mat src(source);
    int width=src.cols;  
    int heigh=src.rows;  
    Mat gray0,gray1;  
    //去色  
    try
    {
    cvtColor(src,gray0,CV_BGR2GRAY);  
    //反色  
    addWeighted(gray0,-1,NULL,0,255,gray1);  
    //高斯模糊,高斯核的Size与最后的效果有关  
    GaussianBlur(gray1,gray1,Size(11,11),0);  
 
    //融合：颜色减淡  
    Mat img(gray1.size(),CV_8UC1);  
    for (int y=0; y<heigh; y++)  
    {  
  
        uchar* P0  = gray0.ptr<uchar>(y);  
        uchar* P1  = gray1.ptr<uchar>(y);  
        uchar* P  = img.ptr<uchar>(y);  
        for (int x=0; x<width; x++)  
        {  
            int tmp0=P0[x];  
            int tmp1=P1[x];  
            P[x] =(uchar) min((tmp0+(tmp0*tmp1)/(256-tmp1)),255);  
        }  
  
    }  
  //  img.copyTo(src);
    cvtColor(img,src,CV_GRAY2BGR);
    img.release();
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	     return;
        				string ss = "sumiao  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
        			   }

/*     catch(Exception& e)
     {
    	 if(pPlat == NULL)
    	      return;
                    	string ss = "sumiao ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
        			    }*/


   // imshow("素描",img);
   // waitKey();
	//IplImage  bb = img;
//	cvCopy(&bb,source);
 //   imwrite("D:/素描.jpg",img);  
}
