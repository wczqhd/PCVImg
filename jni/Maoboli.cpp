//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
  
void Maoboli(Mat& src)
{  
  //  Mat src = imread("D:/scene03.jpg",1); 
//	Mat src(source);
    int width=src.cols;  
    int heigh=src.rows;  
    RNG rng;  
    Mat img(src.size(),CV_8UC3);  
    try
    {
    for (int y=1; y<heigh-1; y++)  
   {  
        uchar* P0  = src.ptr<uchar>(y);  
        uchar* P1  = img.ptr<uchar>(y);  
        for (int x=1; x<width-1; x++)  
        {  
            int tmp=rng.uniform(0,9);  
            P1[3*x]=src.at<uchar>(y-1+tmp/3,3*(x-1+tmp%3));  
            P1[3*x+1]=src.at<uchar>(y-1+tmp/3,3*(x-1+tmp%3)+1);  
            P1[3*x+2]=src.at<uchar>(y-1+tmp/3,3*(x-1+tmp%3)+2);  
        }  
  
    }  
  //  imshow("À©É¢",img);  
    //waitKey();
//	IplImage  bb = img;
	//    cvCopy(&bb,source);
    img.copyTo(src);
    img.release();
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	    return;
        				string ss = "Maoboli  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
     }

/*     catch(Exception& e)
     {
    	 if(pPlat == NULL)
    	     return;
                    	string ss = "Maoboli ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
     }*/
  //  imwrite("D:/À©É¢.jpg",img);  
}  
