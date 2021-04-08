//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>

#include <opencv2\opencv.hpp>
 #include "GifAppPlat.h"
using namespace cv;  
using namespace std;  
  
void ManHua(Mat& src1)
{  
   // string name="D:/cartoon0.jpg";  
  //  Mat src1=imread(name,1);  
	//Mat src1(source);
    Mat img;  
    //˫���˲�����3������d����˵d>5ʱ����ʵʱ�����������������sigma������һ����ͬ��  
    //<10ʱ����ûЧ��, >150ʱ����Ч��  
    try
    {
        bilateralFilter(src1,img,5,150,150);  
        bilateralFilter(img,src1,5,150,150);  
        //img.copyTo(src1);  
      
 //   imshow("bilateral",src1);
   // waitKey(0);
          
    Mat src;  
    cvtColor(src1,src,CV_BGR2GRAY);  
    //���ߣ�Խ��Խ�֣����ǻ��д������  
    Mat imgL;  
    //����������ddepth��ʾĿ��ͼ�����ȣ�ddepth=-1ʱ����ԭͼ��һ��  
    Laplacian(src,imgL,-1,3,1);  
 //   imshow("Laplacian",imgL);
  //  waitKey(0);
    //ϸ��  
    Mat imgC;  
    Canny(src,imgC,30,90);  
   // imshow("Canny",imgC);
   // waitKey(0);
  
    Mat imgS,imgSx,imgSy,imgS0;  
    Sobel(src,imgSx,-1,0,1);  
    Sobel(src,imgSx,-1,1,0);  
    imgS=imgSx+imgSy;  
    Sobel(src,imgS0,-1,1,1);  
  //  imshow("Sobel0",imgS0);
  //  imshow("Sobel",imgS);
   // waitKey(0);
      
    Mat imgTotal;  
    imgTotal=imgC+imgS+imgL;  
    //imgTotal.convertTo(imgTotal,CV_32FC1);  
    normalize(imgTotal,imgTotal,255,0,CV_MINMAX);  
    GaussianBlur(imgTotal,imgTotal,Size(3,3),3);  
    threshold(imgTotal,imgTotal,100,255,THRESH_BINARY_INV);  
   // imshow("Total",imgTotal);
   // waitKey(0);
  
    Mat imgTotalC3;  
    cvtColor(imgTotal,imgTotalC3,CV_GRAY2BGR);  
    bitwise_and(src1,imgTotalC3,src1);  
   // imshow("Result",src1);
  //  waitKey(0);
  
    imgTotalC3.release();
    imgTotal.release();
    imgS.release();
    imgSx.release();
    imgSy.release();
    imgS0.release();
    imgC.release();
    imgL.release();
    img.release();
    src.release();
    }
     catch(cv::Exception& e)
     {
    	 if(pPlat == NULL)
    	    return;
        				string ss = "manhua  cv";
        				ss+= e.err;
        				pPlat->SetInfo(ss);
      }

/*     catch(Exception& e)
     {
    	 if(pPlat == NULL)
    	     return;
                    	string ss = "manhua ";
                    	ss+= e.what();
                    	pPlat->SetInfo(ss);
     }*/
   // name.insert(11,"_edge");  
  //  imwrite(name,src1);  
  
    /* 
    Mat img(imgTotal.rows,imgTotal.cols,CV_32FC1); 
65.    for(int i=0;i<imgTotal.rows;i++) 
66.    { 
67.        //uchar* p=(uchar*)(imgTotal.ptr()+i*imgTotal.step); 
68.        for(int j=0;j<imgTotal.cols;j++) 
69.        { 
70.            if(imgTotal.at<float>(i,j)==0) 
71.                img.at<float>(i,j)=1; 
72.        } 
73.    } 
74.    imshow("Reverse",img); 
75.    waitKey(0); 
76.    */  
    /* 
78.    Mat imgSc; 
79.    Scharr(src,imgSc,-1,1,0); 
80.    imshow("Scharr",imgSc); 
81.    waitKey(0); 
82.    */  
  
}  
