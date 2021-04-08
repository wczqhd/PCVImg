//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>

#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;  
  
void DiaokeFuDiao(Mat& src,int nType/*0  ¸¡µñ   1 µñ¿Ì*/)
{  
//   Mat src(source);
    Mat img0(src.size(),CV_8UC3);  
    Mat img1(src.size(),CV_8UC3);  
    try
    {
    for (int y=1; y<src.rows-1; y++)  
    {  
        uchar *p0 = src.ptr<uchar>(y);  
        uchar *p1 = src.ptr<uchar>(y+1);  
  
        uchar *q0 = img0.ptr<uchar>(y);  
        uchar *q1 = img1.ptr<uchar>(y);  
        for (int x=1; x<src.cols-1; x++)  
        {  
            for (int i=0; i<3; i++)  
            {  
				if(nType == 0)
				{
                int tmp0 = p1[3*(x+1)+i]-p0[3*(x-1)+i]+128;//¸¡µñ  
                if (tmp0<0)  
                    q0[3*x+i]=0;  
               else if(tmp0>255)  
                    q0[3*x+i]=255;  
                else  
                   q0[3*x+i]=tmp0;  
				}

                if(nType == 1)
				{
                int tmp1 = p0[3*(x-1)+i]-p1[3*(x+1)+i]+128;//µñ¿Ì  
                if (tmp1<0)  
                    q1[3*x+i]=0;  
                else if(tmp1>255)  
                    q1[3*x+i]=255;  
                else  
                    q1[3*x+i]=tmp1;  
				}
            }  
        }  
    } 

	if(nType == 0)
	{
		//IplImage  bb = img0;
	    //cvCopy(&bb,source);
		img0.copyTo(src);
	}
	if(nType == 1)
	{
		//IplImage  bb = img1;
	    //cvCopy(&bb,source);
		img1.copyTo(src);
	}
    img0.release();
    img1.release();
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	      return;
         string ss = "DiaokeFuDiao  cv";
         ss+= e.err;
         pPlat->SetInfo(ss);
     }

/*     catch(Exception& e)
     {
    	 if(pPlat == NULL)
    	     return;
         string ss = "DiaokeFuDiao ";
         ss+= e.what();
          pPlat->SetInfo(ss);
     }*/
	
 /*   imshow("src",src);  
    imshow("¸¡µñ",img0);  
    imshow("µñ¿Ì",img1);  
    waitKey(); */ 
}  
