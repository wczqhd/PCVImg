//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
  
void HuaiJiu(Mat& src)
{  
   // Mat src = imread("D:/scene04.jpg",1);  
//	Mat src(source);
    int width=src.cols;  
    int heigh=src.rows;  
    RNG rng;  
    try
    {
    Mat img(src.size(),CV_8UC3);  
    for (int y=0; y<heigh; y++)  
    {  
        uchar* P0  = src.ptr<uchar>(y);  
        uchar* P1  = img.ptr<uchar>(y);  
        for (int x=0; x<width; x++)  
        {  
            float B=P0[3*x];  
            float G=P0[3*x+1];  
            float R=P0[3*x+2];  
            float newB=0.272*R+0.534*G+0.131*B;  
            float newG=0.349*R+0.686*G+0.168*B;  
            float newR=0.393*R+0.769*G+0.189*B;  
            if(newB<0)newB=0;  
            if(newB>255)newB=255;  
            if(newG<0)newG=0;  
            if(newG>255)newG=255;  
            if(newR<0)newR=0;  
            if(newR>255)newR=255;  
            P1[3*x] = (uchar)newB;  
            P1[3*x+1] = (uchar)newG;  
            P1[3*x+2] = (uchar)newR;

         //   P0[3*x] = P1[3*x];
            //P0[3*x+1] = P1[3*x+1];
           // P0[3*x+2] = P1[3*x+2];
        }  
  
    }  
    img.copyTo(src);
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	    return;
    	string ss = "huaijiu  cv";
    	ss+= e.err;
    	pPlat->SetInfo(ss);
    }

/*    catch(Exception& e)
    {
    	if(pPlat == NULL)
    	     return;
                	string ss = "huaijiu ";
                	ss+= e.what();
                	pPlat->SetInfo(ss);
    			    }*/
  //  src
 //   imshow("»³¾ÉÉ«",img);  
   // waitKey();
//	IplImage  bb = img;
	//    cvCopy(&bb,source);
 //   imwrite("D:/»³¾ÉÉ«.jpg",img);  
}
