
//#include "stdafx.h"
#include <opencv\cv.h> 
#include <math.h>  
//#include <opencv2\highgui\highgui.h>

#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;  
using namespace std;  

void HAHAJing(Mat& src,int nType/*0  放大   1 挤压*/)
{  
	//  Mat src = imread("D:/img/face02.jpg",1);  
	//Mat src(source);
	int width = src.cols;  
	int heigh = src.rows;  
	Point center(width/2,heigh/2);

	Mat img1(src.size(),CV_8UC3);  
	Mat img2(src.size(),CV_8UC3);  
	src.copyTo(img1);  
	src.copyTo(img2);  

	//【1】放大  
	if(nType == 0)
	{
		try
		{
		int R1 = sqrtf(width*width+heigh*heigh)/2; //直接关系到放大的力度,与R1成正比;  

		for (int y=0; y<heigh; y++)  
		{  
			uchar *img1_p = img1.ptr<uchar>(y);  
			for (int x=0; x<width; x++)  
			{  
				int dis = norm(Point(x,y)-center);  
				if (dis<R1)  
				{  
					int newX = (x-center.x)*dis/R1+center.x;  
					int newY = (y-center.y)*dis/R1+center.y;  

					img1_p[3*x]=src.at<uchar>(newY,newX*3);  
					img1_p[3*x+1]=src.at<uchar>(newY,newX*3+1);  
					img1_p[3*x+2]=src.at<uchar>(newY,newX*3+2);  
				}  
			}  
		} 
		img1.copyTo(src);
		}
		catch(cv::Exception& e)
		{
			if(pPlat == NULL)
			       return;
		    string ss = "HAHAJing0  cv";
		    ss+= e.err;
		    pPlat->SetInfo(ss);
		}
/*	     catch(Exception& e)
		{
	    	 if(pPlat == NULL)
	    	        return;
		   	string ss = "HAHAJing0 ";
		    ss+= e.what();
		    pPlat->SetInfo(ss);
		}*/
	//	img1.release();
	//	IplImage  bb = img1;
	  //  cvCopy(&bb,source);
	}
	if(nType == 1)
	{
		//【2】挤压  
		try
		{
		for (int y=0; y<heigh; y++)  
		{  
			uchar *img2_p = img2.ptr<uchar>(y);  
			for (int x=0; x<width; x++)  
			{  
				double theta = atan2((double)(y-center.y),(double)(x-center.x));//使用atan出现问题~  


				int R2 = sqrtf(norm(Point(x,y)-center))*8; //直接关系到挤压的力度，与R2成反比;  

				int newX = center.x+(int)(R2*cos(theta));  

				int newY = center.y+(int)(R2*sin(theta));  

				if(newX<0) newX=0;  
				else if(newX>=width) newX=width-1;  
				if(newY<0) newY=0;  
				else if(newY>=heigh) newY=heigh-1;  


				img2_p[3*x]=src.at<uchar>(newY,newX*3);  
				img2_p[3*x+1]=src.at<uchar>(newY,newX*3+1);  
				img2_p[3*x+2]=src.at<uchar>(newY,newX*3+2);  
			}  
		}  
		img2.copyTo(src);
	}
	    catch(cv::Exception& e)
	   {
	    	if(pPlat == NULL)
	    	       return;
	   		string ss = "HAHAJing1  cv";
	   		ss+= e.err;
	   		pPlat->SetInfo(ss);
	    }
/*	    catch(Exception& e)
	    {
	    	if(pPlat == NULL)
	    	        return;
	         string ss = "HAHAJing1 ";
	         ss+= e.what();
	         pPlat->SetInfo(ss);
	    }*/
		//IplImage  bb = img2;
	   // cvCopy(&bb,source);
	}
	img1.release();
	img2.release();
	/*  imshow("src",src);  
	imshow("img1",img1);  
	imshow("img2",img2);  
	waitKey();*/  
	//  imwrite("D:/img/扩张.jpg",img1);  
	//imwrite("D:/img/挤压.jpg",img2);  
} 
