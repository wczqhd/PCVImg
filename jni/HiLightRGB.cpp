//R(��)>127.5
//
//R=R(��)+(255-R(��))*(R(��)-127.5)/127.5;
//
//R(��)<127.5
//
//
//R=R(��)-R(��)*(127.5-R(��))/127.5=(R(��)*R(��))/127.5;
//
//
//
//
//
//
//
//
//
//[cpp] view plaincopyprint?
//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
 #include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
//int R=11;  
  
void HiLightRGB(Mat& src)  //��ͨ��ǿ��
{  
   // Mat src = imread("D:/img/liushishi02.jpg",1); 
//	Mat src(source);
 //   imshow("src",src);
    int width=src.cols;  
    int heigh=src.rows;  
    Mat img;  
    src.copyTo(img);  
  
    Mat dst(img.size(),CV_8UC3);  
    Mat dst1u[3];  
  
  
    float tmp,r;  
    try
    {
    for (int y=0;y<heigh;y++)  
    {  
        uchar* imgP=img.ptr<uchar>(y);  
        uchar* dstP=dst.ptr<uchar>(y);  
        for (int x=0;x<width;x++)  
        {  
            r = (float)imgP[3*x];  
            if(r>127.5)  
                tmp = r+(255-r)*(r-127.5)/127.5;  
            else  
                tmp = r*r/127.5;  
            tmp=tmp>255?255:tmp;  
            tmp=tmp<0?0:tmp;  
            dstP[3*x]=(uchar)(tmp);  
  
            r = (float)imgP[3*x+1];  
            if(r>127.5)  
                tmp = r+(255-r)*(r-127.5)/127.5;  
            else  
                tmp = r*r/127.5;  
            tmp=tmp>255?255:tmp;  
            tmp=tmp<0?0:tmp;  
            dstP[3*x+1]=(uchar)(tmp);  
  
            r = (float)imgP[3*x+2];  
            if(r>127.5)  
                tmp = r+(255-r)*(r-127.5)/127.5;  
            else  
                tmp = r*r/127.5;  
            tmp=tmp>255?255:tmp;  
            tmp=tmp<0?0:tmp;  
            dstP[3*x+2]=(uchar)(tmp);  
        }  
    }  
  //  imshow("ǿ��",dst);  
  //    IplImage  bb = dst;
	//    cvCopy(&bb,source);
    dst.copyTo(src);
    dst.release();
    img.release();
    for(int g=0;g<3;g++)
    	dst1u[g].release();
    }
    catch(cv::Exception& e)
     {

    	  if(pPlat == NULL)
    	     return;
          string ss = "HiLightRGB  cv";
          ss+= e.err;
          pPlat->SetInfo(ss);
     }
/*     catch(Exception& e)
     {
    	 if(pPlat == NULL)
    	     return;
         string ss = "HiLightRGB ";
         ss+= e.what();
         pPlat->SetInfo(ss);
      }*/
  //  split(dst,dst1u);  
 //   imshow("��ͨ��ǿ��",dst1u[1]);  
  
   // waitKey();  
   /* imwrite("D:/img/ǿ��.jpg",dst);  
    imwrite("D:/img/ǿ��_��ͨ��.jpg",dst1u[0]);  
    imwrite("D:/img/ǿ��_��ͨ��.jpg",dst1u[1]);  
    imwrite("D:/img/ǿ��_��ͨ��.jpg",dst1u[2]);  */
  
}
