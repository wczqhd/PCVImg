//OpenCV 4�µ�ͼ������Ƕȵ���ת
//����ת��ͼ��IplImage* Img_old
//���ص���ת��ͼ�� IplImage* Img_tmp.
//��ת�ĽǶ�,��λ��.
//���ֲ�ͬ�ķ���.���з�����û����ȫ����,����һ��������󲿷���Ҫ
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
	cvZero( Img_tmp );//Ŀ��ͼ�� ʹ����չ�Ĵ�С
	IplImage* dst = NULL;
//	IplImage* dst = cvCreateImage(cvGetSize(Img_old), IPL_DEPTH_8U, 3);//Ŀ��ͼ�� ��ԭͼ��ȴ�  

	//cvCopy(Img_old,dst);
	float m[6];            
	CvMat M = cvMat( 2, 3, CV_32F, m );
	if(1==method)
	{
		//����һ  ��ȡ�����ı��Σ�ʹ�������ؾ���

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

		//����һ  ��ȡ�����ı��Σ�ʹ�������ؾ���
	}
	if(2==method)
	{
		//������ ʹ�� ��ά��ת�ķ���任���� �������� Ҫ����������ͼ��һ���� ��ת���Ĳ���

		CvPoint2D32f center;
		center.x=float (Img_old->width/2.0+0.5);//float (Img_tmp->width/2.0+0.5);
		center.y=float (Img_old->height/2.0+0.5);//float (Img_tmp->height/2.0+0.5);   
		cv2DRotationMatrix( center, angle,1, &M);

	//	cvWarpAffine( Img_old, dst, &M,CV_INTER_LINEAR,cvScalarAll(0) );//Сͼ
		//СĿ��ͼ��

		//��ͼ�������չ
		//   ֻ��һ���Ƕ����� ��ͬ���޵Ĳ�ͬ�Դ�
		int dx=int((newwidth -Img_old->width )/2+0.5);
		int dy=int((newheight-Img_old->height)/2+0.5); 
		uchar* old_ptr,*temp_ptr;

		for( int y=0 ; y<Img_old->height; y++) //Ϊ�˲�Խ��
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

		IplImage* temp = cvCloneImage( Img_tmp );//�������ͼ�� 
		cvWarpAffine( Img_tmp, temp    , &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );//��ͼ
		Img_tmp=cvCloneImage( temp );

		//����
		//cvWarpAffine( Img_tmp, Img_tmp, &M,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS,cvScalarAll(0) );//��ͼ

		//������ ʹ�� ��ά��ת�ķ���任����
	}
	if(3==method)
	{
		//������ ͸�ӱ任
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

	

		//������ ͸�ӱ任
	}
	////////////////////////////////////
	if(4==method)
	{
		//������ ͸�ӱ任˳ʱ��
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

	

		//������ ͸�ӱ任
	}


	//  cvNamedWindow( "dst_litter", 1 );
	//  cvShowImage( "dst_litter", dst );

	// cvNamedWindow( "dst_big", 1 );
	// cvShowImage( "dst_big", Img_tmp );

	return Img_tmp;
}
