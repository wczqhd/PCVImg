
//设图象的漩涡中心为o,设p为图像任意点,只要对p做绕o的旋转,且旋转角随p-o距离的增加而减少,就是"漩涡"变换了.

//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
template<typename T> T sqr(T x){return x*x;}  
  
double Pi=3.14;  
  
//double Para=20;  
  
void Xuanwo(Mat& src,double Para)
{  
 //   Mat src = imread("D:/img/face01.jpg",1);  
//	Mat src(source);
	int width=src.cols;
	int heigh=src.rows;
    Point center(width/2, heigh/2);  
    Mat img;  
    src.copyTo(img);  
    Mat src1u[3];  
    try
    {
    split(src,src1u);  
  
    for (int y=0; y<heigh; y++)  
    {  
        uchar* imgP = img.ptr<uchar>(y);  
        uchar* srcP = src.ptr<uchar>(y);  
        for(int x=0; x<width; x++)  
        {  
            int R = norm(Point(x,y)-center);  
            double angle = atan2((double)(y-center.y),(double)(x-center.x));  
            double delta=Pi*Para/sqrtf(R+1);  
            int newX = R*cos(angle+delta) + center.x;  
            int newY = R*sin(angle+delta) + center.y;  
  
            if(newX<0) newX=0;  
            if(newX>width-1) newX=width-1;  
            if(newY<0) newY=0;  
            if(newY>heigh-1) newY=heigh-1;  
  
            imgP[3*x] = src1u[0].at<uchar>(newY,newX);  
            imgP[3*x+1] = src1u[1].at<uchar>(newY,newX);  
            imgP[3*x+2] = src1u[2].at<uchar>(newY,newX);  
        }  
    }  
//	IplImage  bb = img;
	//cvCopy(&bb,source);
    img.copyTo(src);
        img.release();
        for(int x=0;x<3;x++)
        	src1u[x].release();
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	    return;
        				string ss = "xuanwo  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
        			   }
/*     catch(Exception& e)
     {
    	 if(pPlat == NULL)
    	       return;
                    	string ss = "xuanwo ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
      }*/
   // imshow("vortex",img);  
  //  waitKey();  
  //  imwrite("D:/img/漩涡.jpg",img);  
}  
