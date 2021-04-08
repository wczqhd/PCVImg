//使用 applyColorMap()

 
// #include "stdafx.h"
#include <opencv\cv.h> 
#include <math.h>  
//#include <opencv2\highgui\highgui.h>
#include "GifAppPlat.h"
#include <opencv2\opencv.hpp>
#include <opencv2/contrib/contrib.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
  
using namespace cv;  
using namespace std;  
  
	enum
     {

         COLORMAP_AUTUMN = 0,
         COLORMAP_BONE = 1,
         COLORMAP_JET = 2,
         COLORMAP_WINTER = 3,
         COLORMAP_RAINBOW = 4,
         COLORMAP_OCEAN = 5,
         COLORMAP_SUMMER = 6,
         COLORMAP_SPRING = 7,
         COLORMAP_COOL = 8,
         COLORMAP_HSV = 9,
         COLORMAP_PINK = 10,
         COLORMAP_HOT = 11,
         COLORMAP_EMPTY = 12
     };
void ColorMap(Mat& src,int nColor)
{  
  //  Mat src = imread("D:/img/face03.jpeg",1);  
//	int width = src.width;
	//int heigh = source->height;
//	Mat src(source);
    Mat gray;  //据说Mat比cvMat要新，不用自己处理内存
  //  Mat imgColor[12];
    try
    {
    Mat display(src.size(),CV_8UC3);
  
    cvtColor(src,gray,CV_BGR2GRAY);  
  //  for(int i=0; i<12; i++)  
    {  
        applyColorMap(gray,display,nColor);  
      //  int x=i%4;  
       // int y=i/4;  
     //   Mat displayROI = display(Rect(x*width,y*heigh,width,heigh));  
      //  resize(imgColor[nColor],display,display.size());  
    }  
    display.copyTo(src);
    display.release();
    gray.release();
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	      return;
        string ss = "ColorMap  cv";
        ss+= e.err;
        pPlat->SetInfo(ss);
     }

/*     catch(Exception& e)
     {
    	 if(pPlat == NULL)
    	         return;
            string ss = "ColorMap ";
            ss+= e.what();
            pPlat->SetInfo(ss);
      }*/
  //  imshow("colorImg",display);  
//	IplImage  bb = display;
	//cvCopy(&bb,source);
//	cvReleaseImage(*(*bb));
	
  //  waitKey();  
//    imwrite("D:/img/幻彩颜色02.jpg",display);  
}  
