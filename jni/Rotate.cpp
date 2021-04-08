//OpenCV 4下的图像任意角度的旋转
//待旋转的图像IplImage* Img_old
//返回的旋转后图像 IplImage* Img_tmp.
//旋转的角度,单位度.
//三种不同的方法.其中方法二没有完全测试,方法一可以满足大部分需要
//Vastsky - Nercita   2005 6 12  
//vastsky_sun#126.com
//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>

#include <opencv2\opencv.hpp>
IplImage * FitRotate (IplImage* Img_old, double angle,int method) 
{
	IplImage* Img_tmp = NULL; 
	//if(angle)

	double anglerad  = (CV_PI* (angle/180)) ;
	int newheight =int (fabs(( sin(anglerad)*Img_old->width )) + fabs(( cos(anglerad)*Img_old->height )) );
	int newwidth  =int (fabs(( sin(anglerad)*Img_old->height)) + fabs(( cos(anglerad)*Img_old->width)) );

	Img_tmp = cvCreateImage(cvSize(newwidth,newheight), IPL_DEPTH_8U, 3);
	cvZero( Img_tmp );//目的图像 使用扩展的大小
	IplImage* dst = NULL;
//	IplImage* dst = cvCreateImage(cvGetSize(Img_old), IPL_DEPTH_8U, 3);//目的图像 与原图像等大  

	//cvCopy(Img_old,dst);
	float m[6];            
	CvMat M = cvMat( 2, 3, CV_32F, m );
	if(1==method)
	{
		//方法一  提取象素四边形，使用子象素精度

		int w = Img_old->width;
		int h = Img_old->height;
		m[0] = (float)(cos(angle*CV_PI/180.));
		m[1] = (float)(sin(angle*CV_PI/180.));
		m[2] = w*0.5f;
		m[3] = -m[1];
		m[4] = m[0];
		m[5] = h*0.5f;

	//	cvGetQuadrangleSubPix( Img_old, dst, &M);

		cvGetQuadrangleSubPix( Img_old, Img_tmp, &M);//+CV_WARP_FILL_OUTLIERS

		//方法一  提取象素四边形，使用子象素精度
	}
	if(2==method)
	{
		//方法二 使用 二维旋转的仿射变换矩阵 存在问题 要求输入和输出图像一样大 旋转中心不对

		CvPoint2D32f center;
		center.x=float (Img_old->width/2.0+0.5);//float (Img_tmp->width/2.0+0.5);
		center.y=float (Img_old->height/2.0+0.5);//float (Img_tmp->height/2.0+0.5);   
		cv2DRotationMatrix( center, angle,1, &M);

	//	cvWarpAffine( Img_old, dst, &M,CV_INTER_LINEAR,cvScalarAll(0) );//小图
		//小目标图像

		//对图像进行扩展
		//   只能一定角度以内 不同象限的不同对待
		int dx=int((newwidth -Img_old->width )/2+0.5);
		int dy=int((newheight-Img_old->height)/2+0.5); 
		uchar* old_ptr,*temp_ptr;

		for( int y=0 ; y<Img_old->height; y++) //为了不越界
		{ 
			for (int x=0 ; x< Img_old->width; x++)
			{
				old_ptr = &((uchar*)(Img_old->imageData + Img_old->widthStep*y))[(x)*3];
				temp_ptr = &((uchar*)(Img_tmp->imageData + Img_tmp->widthStep*(y+dy)))[(x+dx)*3];
				temp_ptr[0]=old_ptr[0]; //green 
				temp_ptr[1]=old_ptr[1]; //blue
				temp_ptr[2]=old_ptr[2]; //Red
			}
		}


		center.x=float (Img_tmp->width/2.0+0.5);
		center.y=float (Img_tmp->height/2.0+0.5);   
		cv2DRotationMatrix( center, angle,1, &M);

		IplImage* temp = cvCloneImage( Img_tmp );//生成输出图像 
		cvWarpAffine( Img_tmp, temp    , &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );//大图
		Img_tmp=cvCloneImage( temp );

		//问题
		//cvWarpAffine( Img_tmp, Img_tmp, &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );//大图

		//方法二 使用 二维旋转的仿射变换矩阵
	}
	if(3==method)
	{
		//方法三 透视变换
		CvPoint2D32f src_point[4];
		CvPoint2D32f dst_point[4];
		src_point[0].x=0.0;                    src_point[0].y=0.0;
		src_point[1].x=0.0;                    src_point[1].y=(float) Img_old->height; 
		src_point[2].x=(float) Img_old->width; src_point[2].y=(float) Img_old->height;
		src_point[3].x=(float) Img_old->width; src_point[3].y=0.0;

		dst_point[0].x=0;               
		dst_point[0].y=(float) fabs(( sin(anglerad)*Img_old->width ));
		dst_point[1].x=(float) fabs(( sin(anglerad)*Img_old->height));              
		dst_point[1].y=(float) fabs(( sin(anglerad)*Img_old->width ))+(float) fabs(( cos(anglerad)*Img_old->height));

		dst_point[2].x=(float) fabs(( sin(anglerad)*Img_old->height))+(float) fabs(( cos(anglerad)*Img_old->width));
		dst_point[2].y=(float) fabs(( cos(anglerad)*Img_old->height));
		dst_point[3].x=(float) fabs(( cos(anglerad)*Img_old->width));
		dst_point[3].y=0;


		float newm[9];            
		CvMat newM = cvMat( 3, 3, CV_32F, newm );
		cvWarpPerspectiveQMatrix(src_point,dst_point,&newM);

		//cvWarpPerspective(Img_old,dst,&newM,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
		cvWarpPerspective(Img_old,Img_tmp,&newM,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );

	

		//方法三 透视变换
	}
	////////////////////////////////////
	if(4==method)
	{
		//方法四 透视变换顺时针
		CvPoint2D32f src_point[4];
		CvPoint2D32f dst_point[4];
		src_point[0].x=0.0;                    src_point[0].y=0.0;
		src_point[1].x=0.0;                    src_point[1].y=(float) Img_old->height; 
		src_point[2].x=(float) Img_old->width; src_point[2].y=(float) Img_old->height;
		src_point[3].x=(float) Img_old->width; src_point[3].y=0.0;

		dst_point[0].x=(float) fabs(( sin(anglerad)*Img_old->height));             
		dst_point[0].y=0;
		dst_point[1].x=0;              
		dst_point[1].y=(float) fabs(( cos(anglerad)*Img_old->height));

		dst_point[2].x=(float) fabs(( cos(anglerad)*Img_old->width));
		dst_point[2].y=(float) fabs(( sin(anglerad)*Img_old->width ))+(float) fabs(( cos(anglerad)*Img_old->height));

		dst_point[3].x=(float) fabs(( sin(anglerad)*Img_old->height))+(float) fabs(( cos(anglerad)*Img_old->width));
		dst_point[3].y=(float) fabs(( sin(anglerad)*Img_old->width));;


		float newm[9];            
		CvMat newM = cvMat( 3, 3, CV_32F, newm );
		cvWarpPerspectiveQMatrix(src_point,dst_point,&newM);

		//cvWarpPerspective(Img_old,dst,&newM,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
		cvWarpPerspective(Img_old,Img_tmp,&newM,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );

	

		//方法三 透视变换
	}


	//  cvNamedWindow( "dst_litter", 1 );
	//  cvShowImage( "dst_litter", dst );

	// cvNamedWindow( "dst_big", 1 );
	// cvShowImage( "dst_big", Img_tmp );

	return Img_tmp;
}
