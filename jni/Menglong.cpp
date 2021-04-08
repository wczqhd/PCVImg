//��PHOTOSHOP��𻯾���ʹ��ѡ����Χ��ͼ��Ե�ﵽ���ʵ�Ч���� 
//
//��ֵԽ�����ʷ�ΧԽ����ֵԽС�����ʷ�ΧԽխ���ɸ�����������ͼ�Ĵ�С�����ڡ�
//�㷨������
//1��ͨ����rgbֵ���Ӷ����Vֵʵ������Ч��
//2��ͨ������Vֵ�Ĵ�Сʵ�ַ�Χ���ơ�
//3��V  = 255 * ��ǰ��Point���е�����ƽ��s1 / (������е�ľ���ƽ�� *mSize)s2;
//4��s1 �и��� ratio ���� dx dyֵ��



//[cpp] view plaincopyprint?

//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
#define MAXSIZE (32768)  
using namespace cv;  
using namespace std;  
  
  
  
//float mSize = 0.5;  
  
void Menglong(Mat& src,float mSize)
{  
   // Mat src = imread("D:/img/arrow04.jpg",1);  
//	Mat src(source);
  //  imshow("src",src);  
    int width=src.cols;  
    int heigh=src.rows;  
    int centerX=width>>1;  
    int centerY=heigh>>1;  
      
    int maxV=centerX*centerX+centerY*centerY;  
    int minV=(int)(maxV*(1-mSize));  
    int diff= maxV -minV;  
    float ratio = width >heigh ? (float)heigh/(float)width : (float)width/(float)heigh;  
      
    try
    {
    Mat img;  
    src.copyTo(img);  
  
    Scalar avg=mean(src);  
    Mat dst(img.size(),CV_8UC3);  
  //  Mat mask1u[3];
    float tmp,r;  
    for (int y=0;y<heigh;y++)  
    {  
        uchar* imgP=img.ptr<uchar>(y);  
        uchar* dstP=dst.ptr<uchar>(y);  
        for (int x=0;x<width;x++)  
        {  
            int b=imgP[3*x];  
            int g=imgP[3*x+1];  
            int r=imgP[3*x+2];  
  
            float dx=centerX-x;  
            float dy=centerY-y;  
              
            if(width > heigh)  
                 dx= (dx*ratio);  
            else  
                dy = (dy*ratio);  
  
            int dstSq = dx*dx + dy*dy;  
  
            float v = ((float) dstSq / diff)*255;  
  
            r = (int)(r +v);  
            g = (int)(g +v);  
            b = (int)(b +v);  
            r = (r>255 ? 255 : (r<0? 0 : r));  
            g = (g>255 ? 255 : (g<0? 0 : g));  
            b = (b>255 ? 255 : (b<0? 0 : b));  
  
            dstP[3*x] = (uchar)b;  
            dstP[3*x+1] = (uchar)g;  
            dstP[3*x+2] = (uchar)r;  
        }  
    }  
 //   imshow("��",dst);  
  
 //   waitKey();
//	IplImage  bb = dst;
	//    cvCopy(&bb,source);
    dst.copyTo(src);
    dst.release();
    img.release();
    }
    catch(cv::Exception& e)
   {
    	if(pPlat == NULL)
    	  return;
        				string ss = "menglong  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
   }
/*   catch(Exception& e)
   {
	   if(pPlat == NULL)
	     return;
                    	string ss = "menglong ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
   }*/
 //   imwrite("D:/img/��.jpg",dst);  
  
}
