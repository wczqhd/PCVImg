//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>
#include <math.h> 
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
  
using namespace cv;  
using namespace std;  
  
//double angle;  
//int deltaI=10;  //波浪周期;  
//int A=10;       //波浪振幅;  
//Mat src,img;  
  

  
  
int BoLang(Mat& ss,double angle,int deltaI,int A)
{  
//    src = imread("D:/test4.jpg");  
      
 //   src.copyTo(img);  
  
  //  namedWindow("Waves map", 1);  
  
    // create a toolbar  
 //  createTrackbar("振幅", "Waves map", &A, 100, onTrackbar);  
      
   // createTrackbar("频率", "Waves map", &deltaI, 100, onTrackbar);  
    // Show the image  
  //  onTrackbar(0, 0);  
  
 //   waitKey();
	IplImage *img,*src;
	IplImage bb = ss;

	src = cvCreateImage( cvSize(ss.cols,ss.rows), 8, 3 );
	cvCopy(&bb,src);

	img = cvCreateImage( cvSize(ss.cols,ss.rows), 8, 3 );
	int width=ss.cols;
	int heigh=ss.rows;
 //   angle = 0.0;  
      try
      {
    for (int y=0; y<heigh; y++)  
    {  
        int changeX = A*sin(angle);  
		uchar *srcP = ((uchar*)(src->imageData + src->widthStep*y));  
        uchar *imgP = ((uchar*)(img->imageData + img->widthStep*y));  
        for (int x=0; x<width; x++)  
        {  
            if(changeX+x<width && changeX+x>0)        //正弦分布（-1,1）  
            {  
                imgP[3*x]=srcP[3*(x+changeX)];  
                imgP[3*x+1]=srcP[3*(x+changeX)+1];  
                imgP[3*x+2]=srcP[3*(x+changeX)+2];  
				
            }  
            //每行开始和结束的空白区;  
            else if(x<=changeX)         
            {  
                imgP[3*x]=srcP[0];  
                imgP[3*x+1]=srcP[1];  
                imgP[3*x+2]=srcP[2];  
				
            }  
            else if (x>=width-changeX)  
            {  
                imgP[3*x]=srcP[3*(width-1)];  
                imgP[3*x+1]=srcP[3*(width-1)+1];  
                imgP[3*x+2]=srcP[3*(width-1)+2];  
				
            }  
        }  
     //   angle += ((double)deltaI)/100;
    }  
	//cvCopy(img,src);
    Mat dst(img);
    dst.copyTo(ss);
	dst.release();
	cvReleaseImage(&img);
	cvReleaseImage(&src);
      }
          catch(cv::Exception& e)
          			{
        	  if(pPlat == NULL)
        		  return 0;
          				string ss = "bolang  cv";
          				ss+= e.err;
          				pPlat->SetInfo(ss);
          			   }

/*                      catch(Exception& e)
                      {
                    	  if(pPlat == NULL)
                    	        return 0;
                      	string ss = "bolang ";
                      	ss+= e.what();
                      	pPlat->SetInfo(ss);
          			    }*/
//	cvNamedWindow( "wave", 1 );
//	cvShowImage("wave",src);
//	cvWaitKey(0);
	return 0;
  //  imwrite("D:/wave.jpg",img);  
  
}  
