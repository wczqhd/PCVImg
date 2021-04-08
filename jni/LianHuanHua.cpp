 
//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
  #include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
  
void LianHuanHua(Mat& src)
{  
  //  Mat src = imread("D:/scene04.jpg",1); 
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
            float newB=abs(B-G+B+R)*G/256;  
            float newG=abs(B-G+B+R)*R/256;  
            float newR=abs(G-B+G+R)*R/256;  
            if(newB<0)newB=0;  
            if(newB>255)newB=255;  
            if(newG<0)newG=0;  
            if(newG>255)newG=255;  
            if(newR<0)newR=0;  
            if(newR>255)newR=255;  
            P1[3*x] = (uchar)newB;  
            P1[3*x+1] = (uchar)newG;  
            P1[3*x+2] = (uchar)newR;  
        }  
  
    }  
    Mat gray;  
    cvtColor(img,gray,CV_BGR2GRAY);  
    normalize(gray,gray,255,0,CV_MINMAX); 
    img.copyTo(src);//
    img.release();
    gray.release();
    }
     catch(cv::Exception& e)
      {
    	 if(pPlat == NULL)
    	     return;
        				string ss = "lianhuanhua  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
      }

/*        catch(Exception& e)
        {
        	if(pPlat == NULL)
        	       return;
                    	string ss = "lianhuanhua ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
        }*/

	//IplImage  bb = img;
	  //  cvCopy(&bb,source);
   // imshow("连环画",gray);
   // waitKey();
  //  imwrite("D:/连环画.jpg",gray);  
}
