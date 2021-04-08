//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
#define MAXSIZE (32768)  
using namespace cv;  
using namespace std;  
  
void casting(Mat& src)
{  
    Mat img;  
	//Mat src(source);
    src.copyTo(img);  
    int width=src.cols;  
    int heigh=src.rows;  
    try
    {
    Mat dst(img.size(),CV_8UC3);  
    for (int y=0;y<heigh;y++)  
    {  
        uchar* imgP=img.ptr<uchar>(y);  
        uchar* dstP=dst.ptr<uchar>(y);  
        for (int x=0;x<width;x++)  
        {  
            float b0=imgP[3*x];  
            float g0=imgP[3*x+1];  
            float r0=imgP[3*x+2];  
  
            float b = b0*255/(g0+r0+1);  
            float g = g0*255/(b0+r0+1);  
            float r = r0*255/(g0+b0+1);  
  
            r = (r>255 ? 255 : (r<0? 0 : r));  
            g = (g>255 ? 255 : (g<0? 0 : g));  
            b = (b>255 ? 255 : (b<0? 0 : b));  
  
            dstP[3*x] = (uchar)b;  
            dstP[3*x+1] = (uchar)g;  
            dstP[3*x+2] = (uchar)r;  
        }  
    }  
 //   imshow("ÈÛÖý",dst);  
   // imwrite("D:/img/ÈÛÖý.jpg",dst);  
//	IplImage  bb = dst;
	//cvCopy(&bb,source);
    dst.copyTo(src);
    dst.release();
        img.release();
    }
        catch(cv::Exception& e)
        {
        	if(pPlat == NULL)
        	       return;
        		string ss = "casting  cv";
        		ss+= e.err;
        		pPlat->SetInfo(ss);
        			   }

/*                    catch(Exception& e)
                    {
                    	if(pPlat == NULL)
                    	        return;
                    	string ss = "casting ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
        			    }*/
      
}  
  
void freezing(Mat& src)
{  
    Mat img;  
	//Mat src(source);
    src.copyTo(img);  
    int width=src.cols;  
    int heigh=src.rows;  
    try
    {
    Mat dst(img.size(),CV_8UC3);  
    for (int y=0;y<heigh;y++)  
    {  
        uchar* imgP=img.ptr<uchar>(y);  
        uchar* dstP=dst.ptr<uchar>(y);  
        for (int x=0;x<width;x++)  
        {  
            float b0=imgP[3*x];  
            float g0=imgP[3*x+1];  
            float r0=imgP[3*x+2];  
  
            float b = (b0-g0-r0)*3/2;  
            float g = (g0-b0-r0)*3/2;  
            float r = (r0-g0-b0)*3/2;  
  
            r = (r>255 ? 255 : (r<0? -r : r));  
            g = (g>255 ? 255 : (g<0? -g : g));  
            b = (b>255 ? 255 : (b<0? -b : b));  
//          r = (r>255 ? 255 : (r<0? 0 : r));  
//          g = (g>255 ? 255 : (g<0? 0 : g));  
//          b = (b>255 ? 255 : (b<0? 0 : b));  
            dstP[3*x] = (uchar)b;  
            dstP[3*x+1] = (uchar)g;  
            dstP[3*x+2] = (uchar)r;  
        }  
    }  
  //  imwrite("D:/img/±ù¶³.jpg",dst);  
//	IplImage  bb = dst;
	//cvCopy(&bb,source);
    dst.copyTo(src);
        dst.release();
            img.release();
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	       return;
     		string ss = "freezing  cv";
        	ss+= e.err;
        	pPlat->SetInfo(ss);
    }

/*    catch(Exception& e)
    {
    	if(pPlat == NULL)
    	       return;
        string ss = "freezing ";
        ss+= e.what();
        pPlat->SetInfo(ss);
    }*/
}  
  

