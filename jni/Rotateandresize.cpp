//ͼ�����ת�����ţ�Ч����ק���õ���������ȸ�����Ƶģ�
//#include "stdafx.h"
#include <opencv\cv.h> 
//#include <opencv2\highgui\highgui.h>

#include <opencv2\opencv.hpp>
#include "math.h"
int Rotateandresize( char* argv )
{
IplImage* src;
/* the first command line parameter must be image file name */
if( (src = cvLoadImage(argv, -1))!=0)
{
   IplImage* dst = cvCloneImage( src );
   int delta = 1;
   int angle = 0;
        int opt = 1;   // 1�� ��ת������
                       // 0: ������ת
        double factor;
        cvNamedWindow( "src", 1 );
   cvShowImage( "src", src );
   for(;;)
   {
    float m[6];
            // Matrix m looks like:
            //
            // [ m0 m1 m2 ] ===> [ A11 A12   b1 ]
            // [ m3 m4 m5 ]       [ A21 A22   b2 ]
            // 
    CvMat M = cvMat( 2, 3, CV_32F, m );
    int w = src->width;
    int h = src->height;
    if(opt) // ��ת������
                factor = (cos(angle*CV_PI/180.) + 1.05)*2;
            else // ������ת
                factor = 1;
    m[0] = (float)(factor*cos(-angle*2*CV_PI/180.));
    m[1] = (float)(factor*sin(-angle*2*CV_PI/180.));
    m[3] = -m[1];
    m[4] = m[0];
    // ����ת��������ͼ���м�
            m[2] = w*0.5f; 
    m[5] = h*0.5f; 
            // dst(x,y) = A * src(x,y) + b
    cvGetQuadrangleSubPix( src, dst, &M);//��ȡ�����ı��Σ�ʹ�������ؾ���
    cvNamedWindow( "dst", 1 );
    cvShowImage( "dst", dst );
    if( cvWaitKey(5) == 27 )//esc
     break;
    angle =(int) (angle + delta) % 360;
   } // for-loop
}
return 0;
}
