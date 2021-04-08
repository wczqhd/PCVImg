///椭圆曲线拟合
//程序首先发现图像轮廓，然后用椭圆逼近它

////
//
//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>

#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>


using namespace cv;
using namespace std;
int slider_pos = 70;
IplImage *image02 = 0, *image03 = 0, *image04 = 0;

 CvSeq* g_cont;
 string stest;
void process_image(IplImage* src,IplImage* back,IplImage* res1,IplImage* roi1,int x,int y);
int EdgeandEllipse( IplImage* img,IplImage* back,IplImage* res1,IplImage* roi1,int x,int y )
{
   // const char* filename = "d:\\img\\lena.jpg";
//	const char* filename = "d:\\img\\guoan.jpg";
    
	
    // 读入图像，强制为灰度图像
	//int h = CV_LOAD_IMAGE_COLOR ;
  //  if( (image03 = cvLoadImage(filename, 0)) == 0 )
    //    return -1;
    // Create the destination images

	//image03 = img;
	image03 = cvCreateImage(  cvSize(img->width, img->height), IPL_DEPTH_8U, 1 );
		cvZero( image03 );
	cvCvtColor( img, image03, CV_BGR2GRAY );
	image02 = cvCreateImage(  cvSize(img->width, img->height), IPL_DEPTH_8U, 1 );
	image04 = cvCreateImage(  cvSize(img->width, img->height), IPL_DEPTH_8U, 1 );
	cvCopy(image03,image02);
	cvCopy(image03,image04);
 //   image02 = cvCloneImage( image03 );//据说clone会有内存泄露
   // image04 = cvCloneImage( image03 );
	//image05 = cvCloneImage( image03 );
    // Create windows.
  //  cvNamedWindow("Source", 1);
 //   cvNamedWindow("Result", 1);
    // Show the image.
   // cvShowImage("Source", image03);
    // Create toolbars. HighGUI use.
   // cvCreateTrackbar( "Threshold", "Result", &slider_pos, 255, process_image );
    process_image(img,back,res1, roi1,x,y);

    // Show image. HighGUI use.
  //  cvShowImage( "Result", image04 );


	///////////////////////////////////////
    // Wait for a key stroke; the same function arranges events processing                
 //   cvWaitKey(9000);
    cvReleaseImage(&image02);
    cvReleaseImage(&image03);
	 cvReleaseImage(&image04);
	// cvReleaseImage(&image05);
  //  cvDestroyWindow("Source");
    //cvDestroyWindow("Result");
    return 0;
}

//////////////////////////////////
int CircleROI(IplImage* src,IplImage* ba,IplImage* res1,IplImage* roi1,int x,int y) 
{
	//IplImage  * res, * roi,*res1,*roi1;
	IplImage  * res, * roi;
//	src = cvLoadImage(argv, 1);
	res = cvCreateImage(cvGetSize(src), 8, 3);
	roi = cvCreateImage(cvGetSize(src), 8, 1);
		cvZero(roi);
		string ss = stest;
///	ss += "2.png";
//	cvSaveImage(ss.c_str(),res);
//	roi = cvCloneImage( image04 );
	//back = cvLoadImage("d:\\img\\hello3.jpg");
//	res1 = cvCreateImage(cvGetSize(back), 8, 3);
	//roi1 = cvCreateImage(cvGetSize(back), 8, 1);

	cvZero(res);//为防止出现透明底，预先设定位黑色
	//cvZero(res1);
	//cvZero(roi1);
//	cvZero(black);
//	cvCircle( roi, cvPoint(130, 100), 50, CV_RGB(255, 255, 255), -1, 8, 0 );
	 cvDrawContours(roi,g_cont,CV_RGB(255,255,255),
CV_RGB(255,255,255),0,CV_FILLED,8,cvPoint(0,0));

	// ss = stest;
//	ss += "3.png";
	//cvSaveImage(ss.c_str(),roi);

	 cvDrawContours(roi1,g_cont,CV_RGB(255,255,255),
CV_RGB(255,255,255),0,CV_FILLED,8,cvPoint(x,y));
//	cvSaveImage("d:\\img\\cvCircle.png", roi);//黑底白圆
	cvAnd(src, src, res, roi); 
	////////////////////////
	CvRect rect1=cvRect(x, y,res->width ,res->height);

	cvSetImageROI(res1,rect1);

	cvAddWeighted(res1,0,res,1,0,res1);//形成与背景图同尺寸图

//	ss = stest;
	//ss += "r.png";
//	cvSaveImage(ss.c_str(),res1);

	cvResetImageROI(res1);

	cvNot(roi1, roi1);

	//cvAdd(back, res1, res1, roi1);//这一步挪到外边执行，这个函数里面只是执行
	//cvAdd(back, res1, back, roi1);

//	cvSaveImage("d:\\img\\res1.png", res1);//到这里实现不规则图形贴图
	////////////////////////
	//cvSaveImage("d:\\img\\cvAnd.png", res);//摘出需要的部位
//	cvNot(res, res);

//	cvOr(res, black, res,roi); 
//	cvSaveImage("d:\\img\\cvNot.png", res);//取反
/*	IplImage* roi_C3 = cvCreateImage(cvGetSize(src), 8, 3);
	cvMerge(roi, roi, roi, NULL, roi_C3);//单通道变为三通道
	//cvSaveImage("d:\\img\\cvMerge.png", roi_C3);//黑底白园
	cvAnd(res, roi_C3, res, NULL);

//	cvNot(res, res);
//	cvSaveImage("d:\\img\\cvAnd1.png", res);//取反后被摘出的区域，黑底
	cvNot(roi, roi);
	//cvSaveImage("d:\\img\\cvNot1.png", roi);//再次取反，白底黑圆
	cvAdd(src, res, res, roi);
	//cvSaveImage("d:\\img\\cvAdd.png", res);//合并*/
	//cvNamedWindow("lili", 1);
	//cvNamedWindow("res1", 1);
	//cvShowImage("lili", src);
	//cvShowImage("res1", back);
//	cvWaitKey(0);
	//cvDestroyAllWindows();
	//cvReleaseImage(&src);
	cvReleaseImage(&res);
	cvReleaseImage(&roi);
	return 0;
}


//////////////////////////////////////
// Define trackbar callback functon. This function find contours,
// draw it and approximate it by ellipses.
void process_image(IplImage* src,IplImage* back,IplImage* res1,IplImage* roi1,int x,int y)
{
    CvMemStorage* stor;
    CvSeq* cont;
    CvBox2D32f* box;
    CvPoint* PointArray;
    CvPoint2D32f* PointArray2D32f;

	float presize = 0;
    
    // 创建动态结构序列
    stor = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
	
    
    // 二值话图像.
  //  cvThreshold( image03, image02, slider_pos, 255, CV_THRESH_BINARY );
	cvThreshold( image03, image02, 0, 255, CV_THRESH_BINARY );
//	string ss = stest;
	//ss += "1.png";
//	cvSaveImage(ss.c_str(),image02);

	//cvSaveImage("d:\\temp\\image03.png", image03);
	//cvSaveImage("d:\\temp\\image02.png", image02);
    
    // 寻找所有轮廓.
    cvFindContours( image02, stor, &cont, sizeof(CvContour), 
                    CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0,0));

//	g_cont->
	g_cont = cont;
    
    // Clear images. IPL use.
    cvZero(image02);
    cvZero(image04);

	int n = 0;
    
    // 本循环绘制所有轮廓并用椭圆拟合.
    for(;cont;cont = cont->h_next)
    {   n++;
	int i;
         // Indicator of cycle.
	    float minx = 5000,maxx = 0; 
        float count = cont->total; // This is number point in contour
        CvPoint center;
        CvSize size;
        
        // Number point must be more than or equal to 6 (for cvFitEllipse_32f).        
        if( count < 6 )
            continue;
        
        // Alloc memory for contour point set.    
        PointArray = (CvPoint*)malloc( count*sizeof(CvPoint) );
        PointArray2D32f= (CvPoint2D32f*)malloc( count*sizeof(CvPoint2D32f) );
        
        // Alloc memory for ellipse data.
        box = (CvBox2D32f*)malloc(sizeof(CvBox2D32f));
        
        // Get contour point set.
        cvCvtSeqToArray(cont, PointArray, CV_WHOLE_SEQ);
        
        // Convert CvPoint set to CvBox2D32f set.
        for(i=0; i<count; i++)
        {
            PointArray2D32f[i].x = (float)PointArray[i].x;
            PointArray2D32f[i].y = (float)PointArray[i].y;
			minx = minx> PointArray2D32f[i].x?PointArray2D32f[i].x:minx;
			maxx = maxx< PointArray2D32f[i].x?PointArray2D32f[i].x:maxx;
        }
        
        //拟合当前轮廓.
     //   cvFitEllipse(PointArray2D32f, count, box);
        
        // 绘制当前轮廓.
   //     cvDrawContours(image04,cont,CV_RGB(255,255,255),
	//		CV_RGB(255,255,255),0,CV_FILLED,8,cvPoint(0,0));

	//	cvShowImage("0404",image04);
	//	 Mat stt = image04;
		// imshow("dst", stt);
//CV_RGB(255,255,255),0,1,8,cvPoint(0,0));

	////	cvDrawContours(roi1,cont,CV_RGB(255,255,255),
//CV_RGB(255,255,255),0,CV_FILLED,8,cvPoint(x,y));
//CV_RGB(255,255,255),0,1,8,cvPoint(x,y));
	
	//	if(presize<box->size.width && box->size.width != image03->width)
	//	if(box->center.x < 0 || box->center.y < 0 || box->size.height > image03->height || box->size.width > image03->width)
		//	continue;
		int bb = image03->width;
		if(presize<(maxx - minx))
		{
			presize = maxx - minx;
			g_cont = cont;
		}
        
        // Convert ellipse data from float to integer representation.
        center.x = cvRound(box->center.x);
        center.y = cvRound(box->center.y);
        size.width = cvRound(box->size.width*0.5);
        size.height = cvRound(box->size.height*0.5);
        box->angle = -box->angle;
        
        // Draw ellipse.
    /*    cvEllipse(image04, center, size,
                  box->angle, 0, 360,
                  CV_RGB(0,0,255), 1, CV_AA, 0);*/
        
        // Free memory.          
        free(PointArray);
        free(PointArray2D32f);
        free(box);
    }
	//int h = n;
    CircleROI(src,back,res1,roi1,x, y);
	cvReleaseMemStorage( &stor );
//	cvClearSeq(g_cont);
//	CircleROI("d:\\src.png");
    // Show image. HighGUI use.
  //  cvShowImage( "Result", image04 );
}


