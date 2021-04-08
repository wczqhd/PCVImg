//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp> 
#include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
//int num2=10;//   num：风线密度  
//int num1=20;//  num1：风线长度  
  
void Wind(Mat& src,int num2,int num1)
{  
  //  Mat src = imread("D:/test3.jpg",1);  
//	Mat src(source);
    Mat src1u[3];  

    try
    {
    split(src,src1u);  
  
    int width=src.cols;  
    int heigh=src.rows;  
    Mat img;  
    src.copyTo(img);  
  
    Point center(width/2,heigh/2);  
  
   RNG rng;  
  
    for (int y=0; y<heigh; y++)  
    {  
  
        uchar *imgP  = img.ptr<uchar>(y);  
  
//      for (int x=0; x<width; x++)  
        {  
  
            for (int i=0;i<num2;i++)      //  num：风线密度  
            {  
                int newX=rng.uniform(i*width/num2,(i+1)*width/num2);  
                int newY = y;  
  
                if(newX<0)newX=0;  
                if(newX>width-1)newX=width-1;  
  
                uchar tmp0 = src1u[0].at<uchar>(newY,newX);  
                uchar tmp1 = src1u[1].at<uchar>(newY,newX);  
                uchar tmp2 = src1u[2].at<uchar>(newY,newX);  
  
                for (int j=0; j<num1; j++)   //num1：风线长度  
                {  
                    int tmpX=newX-j;//减：风向左；加：风向右  
  
                    if(tmpX<0)tmpX=0;  
                    if(tmpX>width-1)tmpX=width-1;  
                      
                    imgP[tmpX*3]=tmp0;  
                    imgP[tmpX*3+1]=tmp1;  
                    imgP[tmpX*3+2]=tmp2;  
                }  
            }  
  
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
        				string ss = "wind  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
        }
/*        catch(Exception& e)
        {
        	if(pPlat == NULL)
        	    return;
                    	string ss = "wind ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
        }*/
 //   imshow("径向模糊",img);  
   // waitKey();  
  //  imwrite("D:/风.jpg",img);  
}  
