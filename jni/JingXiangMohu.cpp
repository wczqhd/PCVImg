//��Ч���ڹ���4���л�����ʱ����, ��Ʒ�ɳ�12���˶�ģ��Ҳ�����ָо�.
//
//PS���Ϊ ����ģ��->���ţ� ����һ�� ����ģ��->��ת��
//
//���ŵ�ԭ��:
//
//ȷ��һ�����ĵ�(��0.5, 0.5), ����ǰ������һ����. �Ե�ǰ����Ϊ����, �����ϵĸ������ؽ��в���, ���ȡһ��ƽ��ֵ.
//ͬ������ת��ԭ�����ƣ�������ͬ�뾶��һ�������ڵľ�ֵ��
//
//����ʱ���뾶�仯����תʱ���Ƕȱ仯��
//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
//int num=40;//num����ֵ����;  
  
void JingxiangMohu(Mat& src,int num)
{  
  //  Mat src = imread("D:/test3.jpg",1);  
	//Mat src(source);
    Mat src1u[3];  
    try
    {
    split(src,src1u);  
  
    int width=src.cols;  
    int heigh=src.rows;  
    Mat img;  
    src.copyTo(img);  
    Point center(width/2,heigh/2);  
  
  
    for (int y=0; y<heigh; y++)  
    {  
          
        uchar *imgP  = img.ptr<uchar>(y);  
  
        for (int x=0; x<width; x++)  
        {  
            int R = norm(Point(x,y)-center);  
            double angle = atan2((double)(y-center.y),(double)(x-center.x));  
  
            int tmp0=0,tmp1=0,tmp2=0;  
              
            for (int i=0;i<num;i++)      //num����ֵ���� ��iΪ�仯����;  
            {  
                int tmpR = (R-i)>0?(R-i):0;  
                  
                int newX = tmpR*cos(angle) + center.x;  
                int newY = tmpR*sin(angle) + center.y;  
                  
                if(newX<0)newX=0;  
                if(newX>width-1)newX=width-1;  
                if(newY<0)newY=0;  
                if(newY>heigh-1)newY=heigh-1;  
  
                tmp0 += src1u[0].at<uchar>(newY,newX);  
                tmp1 += src1u[1].at<uchar>(newY,newX);  
                tmp2 += src1u[2].at<uchar>(newY,newX);  
  
            }  
            imgP[3*x]=(uchar)(tmp0/num);  
            imgP[3*x+1]=(uchar)(tmp1/num);  
            imgP[3*x+2]=(uchar)(tmp2/num);  
        }  
          
    }  
 //   imshow("����ģ��",img);  
   // waitKey();  
//	IplImage  bb = img;
	//    cvCopy(&bb,source);
    img.copyTo(src);
    img.release();
    for(int g=0;g<3;g++)
    	src1u[g].release();
    }
        catch(cv::Exception& e)
        {
        	    if(pPlat == NULL)
        	        		  return;
        				string ss = "JingxiangMohu  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
        }

/*        catch(Exception& e)
        {
        	if(pPlat == NULL)
        	    return;
                    	string ss = "JingxiangMohu ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
       }*/
  //  imwrite("D:/����ģ�������ţ�.jpg",img);  
}  

//int num=20; //��ֵ����;  
  
void XuanZhuan(Mat& src,int num)
{  
//	num=20;
//	Mat src(source);
  //  Mat src = imread("D:/test3.jpg",1);  
    Mat src1u[3];  
    try
    {
    split(src,src1u);  
  
    int width=src.cols;  
    int heigh=src.rows;  
    Mat img;  
    src.copyTo(img);  
    Point center(width/2,heigh/2);  
  
  
    for (int y=0; y<heigh; y++)  
    {  
  
        uchar *imgP  = img.ptr<uchar>(y);  
  
        for (int x=0; x<width; x++)  
        {  
            int R = norm(Point(x,y)-center);  
            double angle = atan2((double)(y-center.y),(double)(x-center.x));  
  
            int tmp0=0,tmp1=0,tmp2=0;  
  
            for (int i=0;i<num;i++)  //��ֵ����;  
            {  
  
                angle+=0.01;        //0.01���Ʊ仯Ƶ�ʣ�����  
  
                int newX = R*cos(angle) + center.x;  
                int newY = R*sin(angle) + center.y;  
  
                if(newX<0)newX=0;  
                if(newX>width-1)newX=width-1;  
                if(newY<0)newY=0;  
                if(newY>heigh-1)newY=heigh-1;  
  
                tmp0 += src1u[0].at<uchar>(newY,newX);  
                tmp1 += src1u[1].at<uchar>(newY,newX);  
                tmp2 += src1u[2].at<uchar>(newY,newX);  
  
            }  
            imgP[3*x]=(uchar)(tmp0/num);  
            imgP[3*x+1]=(uchar)(tmp1/num);  
            imgP[3*x+2]=(uchar)(tmp2/num);  
        }  
  
    }  
  //  imshow("����ģ��",img);  
    //waitKey();
	//IplImage  bb = img;
	  //  cvCopy(&bb,source);
    img.copyTo(src);
    img.release();
    for(int g=0;g<3;g++)
       	src1u[g].release();
    }
    catch(cv::Exception& e)
    {
    	if(pPlat == NULL)
    	    return;
        				string ss = "xuanzhuan  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
     }
/*    catch(Exception& e)
    {
    	if(pPlat == NULL)
    	    return;
                    	string ss = "xuanzhuan ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
     }*/
 //   imwrite("D:/����ģ��(��ת).jpg",img);  
} 
