
// TestPlatDlg.cpp : 实现文件
//
//#include "StdAfx.h"
#include <string.h>
#include "GifAppPlat.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

CGifAppPlat* pPlat;
#define ABS(x)    ((x) > 0 ? (x) : (-(x)))
//extern JNIEnv* jniEnv;

CGifAppPlat::CGifAppPlat()
{
	imagegl = NULL;
	pPlat = NULL;
	//	jniEnv = NULL;
	//////////////////////////////////////
	pw = NULL;
	gw = NULL;
	bGetGif = false;

	select_object = 0;
	track_object = 0;

	nImgWidth = 0;//传递过来的视频宽度
	nImgHeight = 0;
	nOldImgWidth = 0;//传递过来的视频宽度
	nOldImgHeight = 0;
	//md = NULL;
	nCurMode = enum_Empty;
	nCurOperMode = enum_OPER_EMPTY;
	nCurColor = 12; //COLORMAP_EMPTY=12,在colormap.cpp中定义

	nCurSelImg = -1;
	imgBtn = NULL;

	nTrackId = -1;
	bPause = false;


	nEvery = 3;
	nEveryBigShow = 2;

	nDelay = 200;
	MHI_DURATION = 0.2;
	MAX_TIME_DELTA = 0.5;
	MIN_TIME_DELTA = 0.05;
	N = 3;
	//
	CONTOUR_MAX_AERA = 64;

	bGetTestImg = false;
	select_object = 0;
	track_object = 0;

	//////////////////////////////////////
	m_sCompany = "@WowGIF";
	cvInitFont( &m_font, CV_FONT_VECTOR0,1, 1, 0, 1, 8);

	cvGetTextSize(m_sCompany.c_str(),&m_font,&textSize,&baseline);

	InitMode();
	//////////////////////////////////////
	ChangeMode(1);

	origin.x = -1;
	origin.y = -1;

	sImgBtnPath = "";
	sImgHandImg = "";

	drawing_box = false;
	drawing_mode = false;
	drawing_text = false;
	drawing_text_id = -1;

	isMovingImg = false;
	orginx = -1;
	box = cvRect(-1,-1,0,0);
	noldx =-1;
	noldy =-1;
	m_scr = cvScalar(0xff,0x00,0x00);
	m_thickness = 2;
	testnum = 0;
	writer =NULL;
	saveType = 0;//0  gif  1  avi
	fps = 25;
	m_size = 3;
	capture = NULL;
	imageHand = NULL;
	imagePause = NULL;
	pre_bigshow_id = -1;
	///////////////////////////////////
	prev_pt_koutu.x = -1;
	prev_pt_koutu.y = -1;
    img_kt_koutu = NULL;
	nKoutuStatus = CV_EVENT_LBUTTONUP;//抠图时鼠标的状态，避免不当操作影响抠图效果

	///////////////////////////////////
}
CGifAppPlat::~CGifAppPlat()
{
	ReleaseImg();
	ReleaseImglist();
	ClearBMP();
	/*if(md)
	{
	md->ReleaseImages();
	delete md;
	md = NULL;
	}*/
	if(pw)
		delete  pw;
	if(gw)
		delete gw;

	if(imagegl)
	{
		cvReleaseImage(&imagegl);
	}
	if(imageHand)
	{
		cvReleaseImage(&imageHand);
	}
	//	cvReleaseVideoWriter(&writer);

}

void  CGifAppPlat::InitMode()
{
	menglong_Size = MENGLONG_SIZE ;


	wind_num2 = WIND_NUM2 ;
	wind_num1 = WIND_NUM1 ;


	xuanwo_Para=XUANWO_PARA ;


	jingxiangmohu_num=JINGXIANGMOHU_NUM;


	xuanzhuan_num=XUANZHUAN_NUM;



	//   IplImage  bb = src;
	bolang_angle = BOLANG_ANGLE;
	bolang_deltaI=BOLANG_DELTAI;  //波浪周期;
	bolang_A=BOLANG_A;       //波浪振幅;
}
////////////////////
void  CGifAppPlat::ChangeMode(int delta)
{
	switch(nCurMode)
	{


	case enum_Menglong:
		//Menglong(src,menglong_Size);
		menglong_Size = MENGLONG_SIZE * delta;

		break;
	case enum_Wind:
		wind_num2 = WIND_NUM2 + delta;
		wind_num1 = WIND_NUM1 + delta;

		break;
		/*	case enum_Xuanwo:
		xuanwo_Para=XUANWO_PARA * delta;

		break;
		*/


		/*
		case enum_JingxiangMohu:
		jingxiangmohu_num=JINGXIANGMOHU_NUM * delta;

		break;*/
		/*	case enum_XuanZhuan:
		xuanzhuan_num=XUANZHUAN_NUM * delta;


		break;*/
		/*
		case enum_BoLang:
		//   IplImage  bb = src;
		bolang_angle = BOLANG_ANGLE * delta;
		bolang_deltaI=BOLANG_DELTAI * delta;  //波浪周期;
		bolang_A=BOLANG_A * delta;       //波浪振幅;
		break;

		*/

	default:
		break;
	}
}
///////////////////

void CGifAppPlat::ReleaseImglist()
{
	//if(imglist.empty())
	//	return;

	//	IplImage* timg = imglist.front();
	//imglist.pop_front();
	//	cvReleaseImage(&timg);
	for(vector<IplImage*>::iterator iter=imglist.begin(); iter!=imglist.end(); )
	{

		IplImage* tt = (IplImage*)*iter;
		cvReleaseImage(&tt);
		iter = imglist.erase(iter);


	}

	for(vector<IplImage*>::iterator iter=imglistBak.begin(); iter!=imglistBak.end(); )
	{

		IplImage* tt = (IplImage*)*iter;
		cvReleaseImage(&tt);
		iter = imglistBak.erase(iter);


	}
	if(imagePause)
	{
		cvReleaseImage(&imagePause);
		imagePause = NULL;
	}

}
////////////////////
void CGifAppPlat::ReleaseBigShowImg()
{
	for(vector<BigShowIMGList>::iterator itr =   imgBigShow.begin();itr != imgBigShow.end();)
	{
		BigShowIMGList itt = (BigShowIMGList)*itr;
		for(BigShowIMGList::iterator iter1=itt.begin(); iter1!=itt.end(); )
		{

			IplImage* tt = (IplImage*)*iter1;
			cvReleaseImage(&tt);
			iter1 = itt.erase(iter1);


		}
		itr = imgBigShow.erase(itr);

	}
}
void CGifAppPlat::ReleaseImg()
{
	//if(imglist.empty())
	//	return;
	/*while(1)
	{
	if(imglist.empty())
	break;
	IplImage* timg = imglist.front();
	imglist.pop_front();
	cvReleaseImage(&timg);
	}*/
	//ReleaseImglist();
	for(vector<IMGItem*>::iterator iter=imgInsertList.begin(); iter!=imgInsertList.end(); )
	{
		IMGItem* tt = (IMGItem*)*iter;

		cvReleaseImage(&tt->imgInsert);
		cvReleaseImage(&tt->imgCover);
		cvReleaseImage(&tt->imgMask);
		iter = imgInsertList.erase(iter);


	}




	if(imagegl)
	{
		cvReleaseImage(&imagegl);
		imagegl = NULL;
	}
	if(imageHand)
	{
		cvReleaseImage(&imageHand);
		imageHand = NULL;
	}
	if(writer)
	{
		cvReleaseVideoWriter(&writer);
		writer = NULL;
	}









	///////////////////////////////////////////////
	if(imgBtn)
	{
		cvReleaseImage(&imgBtn);
		imgBtn = NULL;
	}
}
void  CGifAppPlat::Start(char* path)
{
	/*if(!md)
	{
	md = new CMotionDetect();

	}*/


	nCurMode = enum_Empty;
	if(pPlat == NULL)
		pPlat = this;

	//	SetTestFilePath(path);
	//	file.open(m_testfilepath.c_str());//输入的是D:\guo.txt
}
void   CGifAppPlat::InputLoginfo(string ss)
{
	// if(file.is_open())
	{
		// char* cc;
		//	 file<<ss<<endl;
	}
}
void  CGifAppPlat::Stop()
{
	ReleaseImg();
	ClearBMP();
	//	if(md)
	//	md->ReleaseImages();
	nCurMode = enum_Empty;
	nCurSelImg = -1;
	///////////////////////
	imagegl = NULL;
	pPlat = NULL;
	//	jniEnv = NULL;
	//////////////////////////////////////
	pw = NULL;
	gw = NULL;
	bGetGif = false;

	select_object = 0;
	track_object = 0;

	//	nImgWidth = 0;//传递过来的视频宽度
	//nImgHeight = 0;
	//md = NULL;
	nCurMode = enum_Empty;
	nCurOperMode = enum_OPER_EMPTY;
	nCurColor = 12; //COLORMAP_EMPTY=12,在colormap.cpp中定义

	nCurSelImg = -1;
	imgBtn = NULL;

	nTrackId = -1;
	bPause = false;

	MHI_DURATION = 0.2;
	MAX_TIME_DELTA = 0.5;
	MIN_TIME_DELTA = 0.05;
	N = 3;
	//
	CONTOUR_MAX_AERA = 64;

	select_object = 0;
	track_object = 0;

	//////////////////////////////////////
	//	m_sCompany = "@WowGIF";
	//cvInitFont( &m_font, CV_FONT_VECTOR0,1, 1, 0, 1, 8);

	InitMode();
	//////////////////////////////////////
	ChangeMode(1);

	origin.x = -1;
	origin.y = -1;

	//sImgBtnPath = "";
	//sImgHandImg = "";

	drawing_box = false;
	drawing_mode = false;
	drawing_text = false;
	drawing_text_id = -1;

	isMovingImg = false;
	orginx = -1;
	box = cvRect(-1,-1,0,0);
	noldx =-1;
	noldy =-1;
	//	if(!file)
	//	file.close();
}
void  CGifAppPlat::Pause()
{

	bPause = !bPause;

}



void CGifAppPlat::PrePareTrack(int &x,int &y)
{
	if( !imagegl )
		return;
	if( imagegl->origin )
		y = imagegl->height - y;
	if( select_object )
	{
		selection.x = MIN(x,origin.x);
		selection.y = MIN(y,origin.y);
		selection.width = pPlat->selection.x + CV_IABS(x - origin.x);
		selection.height = selection.y + CV_IABS(y - origin.y);

		selection.x = MAX( selection.x, 0 );
		selection.y = MAX( selection.y, 0 );
		selection.width = MIN( selection.width, imagegl->width );
		selection.height = MIN( selection.height, imagegl->height );
		selection.width -= selection.x;
		selection.height -= selection.y;
	}
}
//for test on pc
void on_event_test(int eve, int x, int y, int flags, void* )
{
	if(pPlat == NULL)
		return;
	pPlat->DrawingByHand(eve,x,y);
}
/////////////////////
int CGifAppPlat::on_event( int event, int x, int y)
{
	//if(pPlat == NULL)
	//return;
	//	pPlat->SelectImg(x, y);
	//	pPlat->PrePareTrack( x, y);
	/*	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:

	pPlat->SelectImg(x, y);
	pPlat->PrePareTrack( x, y);
	pPlat->origin = cvPoint(x,y);
	pPlat->selection = cvRect(x,y,0,0);
	pPlat->select_object = 1;
	break;
	case CV_EVENT_MOUSEMOVE:
	break;
	case CV_EVENT_LBUTTONUP:
	pPlat->select_object = 0;
	if( pPlat->selection.width > 0 && pPlat->selection.height > 0 )
	pPlat->track_object = -1;

	break;
	}*/
	/*
	*  MotionEvent.ACTION_DOWN  0

	MotionEvent.ACTION_MOVE  2

	MotionEvent.ACTION_UP  1*/
	int eve = -1;
	//将JAVA事件转换为c事件
	if(event == 0)
		eve = CV_EVENT_LBUTTONDOWN;
	if(event == 1)
		eve = CV_EVENT_LBUTTONUP;
	if(event == 2)
		eve = CV_EVENT_MOUSEMOVE;
	return DrawingByHand(eve,x,y);
}


/////////////////////////////////////////////
//图片处理，保存成GIF
//BITMAPINFOHEADER biHeader;
//BITMAPINFO  bInfo;
//unsigned char * bmpData;

//CxImage * pImages[50];
//int   pagecount;


////////////////////////////////////////////
//这是主要的方法
IplImage * FitRotate (IplImage* Img_old, double angle,int method);

void  CGifAppPlat::draw_box( IplImage* img, CvRect rect ,CvScalar scr/*1  red;2 green ;3 blue*/) 
{
	/*CvScalar scr;
	if(nClr == 1)
	scr = cvScalar(0xff,0x00,0x00);
	else if(nClr == 3)
	scr = cvScalar(0x00,0x00,0xff);
	else 
	scr = cvScalar(0x00,0xff,0x00);*/
	cvRectangle (
		img,
		cvPoint(box.x,box.y),
		cvPoint(box.x+box.width,box.y+box.height),
		scr /* blue */
		);
}
void OnPlatTouch(int vevent,int x, int y)
{
}
bool  CGifAppPlat::TextMovingByItSelf(int id)
{
	if(id < 0 || id > imgInsertList.size() )
		return false;
	if(imgInsertList[id] == NULL)
		return false;
	if(nImgWidth == 0 || nImgHeight == 0)
		return  false;
	//int flag = (id%2)?1:-1;
	if(imgInsertList[id]->angList == -1)
	{
		imgInsertList[id]->angList = id%DIRECTION_NUM;
		//	angList[id] = 1;
	}
	if(imgInsertList[id]->xList == -100  && imgInsertList[id]->yList == -100)
	{
		switch(imgInsertList[id]->angList)
		{
			//0 -> 1 <- 2 /  3 \  4  5(23为下往上，45对应为上往下)
		case 0:
			imgInsertList[id]->xList = 0;
			imgInsertList[id]->yList = (id%10)%nImgHeight;
			break;
		case 1:
			imgInsertList[id]->xList = nImgWidth;
			imgInsertList[id]->yList = (id%10)%nImgHeight;
			break;
		case 2:
			imgInsertList[id]->xList = 0;
			imgInsertList[id]->yList = nImgHeight - imgInsertList[id]->imgInsert->height;
			break;
		case 3:
			imgInsertList[id]->xList = nImgWidth;
			imgInsertList[id]->yList = nImgHeight - imgInsertList[id]->imgInsert->height;
			break;
		case 4:
			imgInsertList[id]->xList = nImgWidth;
			imgInsertList[id]->yList = 0;
			break;
		case 5:
			imgInsertList[id]->xList = 0;
			imgInsertList[id]->yList = 0;
			break;
		default:
			break;

		}
	}
	switch(imgInsertList[id]->angList)
	{
		//0 -> 1 <- 2 /  3 \  4  5(23为下往上，45对应为上往下)
		//IMAGE_SPEED  //弹幕移动速度
	case 0:
		imgInsertList[id]->xList += IMAGE_SPEED;
		imgInsertList[id]->yList = (id%10)%nImgHeight;

		if(imgInsertList[id]->xList+imgInsertList[id]->widList > nImgWidth)
		{//->
			imgInsertList[id]->widList = nImgWidth - imgInsertList[id]->xList;
		}
		break;
	case 1:
		//考虑到人们的阅读习惯，右向左也应该逐次展现，左向右倒不必
		imgInsertList[id]->xList -= IMAGE_SPEED;
		imgInsertList[id]->yList = (id%10)%nImgHeight;
		if(imgInsertList[id]->xList>0)
		{
			/*  if(widList[id] < imgInsert[id]->width)
			{//<-
			widList[id] = (widList[id] + IMAGE_SPEED) > imgInsert[id]->width?(widList[id] + IMAGE_SPEED):imgInsert[id]->width;
			}*/
			if(nImgWidth - imgInsertList[id]->xList < imgInsertList[id]->imgInsert->width)
			{
				imgInsertList[id]->widList = nImgWidth - imgInsertList[id]->xList;
			}
			else
				imgInsertList[id]->widList = imgInsertList[id]->imgInsert->width;
		}
		else
		{//右向左消失
			imgInsertList[id]->widList -=IMAGE_SPEED;
		}
		break;
	case 2:
		imgInsertList[id]->xList += IMAGE_SPEED;
		imgInsertList[id]->yList -= IMAGE_SPEED;
		break;
	case 3:
		imgInsertList[id]->xList -= IMAGE_SPEED;
		imgInsertList[id]->yList -= IMAGE_SPEED;

		if(imgInsertList[id]->xList>0)
		{
			/*  if(widList[id] < imgInsert[id]->width)
			{//<-
			widList[id] = (widList[id] + IMAGE_SPEED) > imgInsert[id]->width?(widList[id] + IMAGE_SPEED):imgInsert[id]->width;
			}*/
			if(nImgWidth - imgInsertList[id]->xList < imgInsertList[id]->imgInsert->width)
			{
				imgInsertList[id]->widList = nImgWidth - imgInsertList[id]->xList;
			}
			else
				imgInsertList[id]->widList = imgInsertList[id]->imgInsert->width;
		}
		else
		{//右向左消失
			imgInsertList[id]->widList -=IMAGE_SPEED;
		}
		break;
	case 4:
		imgInsertList[id]->xList -= IMAGE_SPEED;
		imgInsertList[id]->yList += IMAGE_SPEED;

		if(imgInsertList[id]->xList>0)
		{
			/*  if(widList[id] < imgInsert[id]->width)
			{//<-
			widList[id] = (widList[id] + IMAGE_SPEED) > imgInsert[id]->width?(widList[id] + IMAGE_SPEED):imgInsert[id]->width;
			}*/
			if(nImgWidth - imgInsertList[id]->xList < imgInsertList[id]->imgInsert->width)
			{
				imgInsertList[id]->widList = nImgWidth - imgInsertList[id]->xList;
			}
			else
				imgInsertList[id]->widList = imgInsertList[id]->imgInsert->width;
		}
		else
		{//右向左消失
			imgInsertList[id]->widList -=IMAGE_SPEED;
		}
		break;
	case 5:
		imgInsertList[id]->xList += IMAGE_SPEED;
		imgInsertList[id]->yList += IMAGE_SPEED;

		if(imgInsertList[id]->xList+imgInsertList[id]->widList > nImgWidth)
		{//->
			imgInsertList[id]->widList = nImgWidth - imgInsertList[id]->xList;
		}
		break;
	default:
		break;

	}
	if(imgInsertList[id]->xList < 0)
		imgInsertList[id]->xList = 0;
	if(imgInsertList[id]->xList > nImgWidth)
		imgInsertList[id]->xList = nImgWidth;

	if(imgInsertList[id]->yList < 0)
		imgInsertList[id]->yList = 0;
	if(imgInsertList[id]->yList > nImgHeight)
		imgInsertList[id]->yList = nImgHeight;
	if(imgInsertList[id]->yList+imgInsertList[id]->heiList > nImgHeight)
		imgInsertList[id]->heiList = nImgHeight - imgInsertList[id]->yList;




	if(imgInsertList[id]->heiList < imgInsertList[id]->imgInsert->height || imgInsertList[id]->widList <= 0)
	{
		DeleteImg(id);
		return false;
	}
	//////////////////////////////////
	Rect rt;
	if(imgInsertList[id]->widList < imgInsertList[id]->imgInsert->width)
	{
		IplImage* dst = NULL;
		//此处是为了处理字幕留在屏幕上一部分的情况
		switch(imgInsertList[id]->angList)
		{
			//0 -> 1 <- 2 /  3 \  4  5(23为下往上，45对应为上往下)
			//IMAGE_SPEED  //弹幕移动速度
		case 0:
			rt.x = 0;
			rt.y = 0;
			rt.width = imgInsertList[id]->widList;
			rt.height = imgInsertList[id]->heiList;
			break;
		case 1:
			if(imgInsertList[id]->xList>0)
				rt.x = 0;
			else
				rt.x = imgInsertList[id]->imgInsert->width - imgInsertList[id]->widList;
			rt.y = 0;
			rt.width = imgInsertList[id]->widList;
			rt.height = imgInsertList[id]->heiList;
			break;
		case 2:
			rt.x = 0;
			rt.y = 0;
			rt.width = imgInsertList[id]->widList;
			rt.height = imgInsertList[id]->heiList;
			break;
		case 3:
			if(imgInsertList[id]->xList>0)
				rt.x = 0;
			else
				rt.x = imgInsertList[id]->imgInsert->width - imgInsertList[id]->widList;
			rt.y = 0;
			rt.width = imgInsertList[id]->widList;
			rt.height = imgInsertList[id]->heiList;
			break;
		case 4:
			if(imgInsertList[id]->xList>0)
				rt.x = 0;
			else
				rt.x = imgInsertList[id]->imgInsert->width - imgInsertList[id]->widList;
			rt.y = 0;
			rt.width = imgInsertList[id]->widList;
			rt.height = imgInsertList[id]->heiList;
			break;
		case 5:
			rt.x = 0;
			rt.y = 0;
			rt.width = imgInsertList[id]->widList;
			rt.height = imgInsertList[id]->heiList;
			break;
		default:
			break;

		}
		/*	dst = GetSubImage(imgInsert[id],rt);
		if(dst)
		{
		cvReleaseImage(&imgInsert[id]);
		imgInsert[id] = dst;
		}*/

	}
	imgInsertList[id]->flagList = 1;
	/////////////////////////////////
	if(UpdateSPImg(id,imgInsertList[id]->typeList,rt) == 0)
		return true;
	return false;
}
///////////////////////
int EdgeandEllipse( IplImage* img,IplImage* back,IplImage* res1,IplImage* roi1,int x,int y );
extern  string stest;
int CGifAppPlat::ConfirmKouTu(Mat& src,CvRect ro)
{
//	IplImage ima = src;
	IplImage ima = dst_koutu;
	//	rt_kt.width -= rt_kt.x;

		stest = sImgBtnPath;
	//	rt_kt.height-=rt_kt.y;
	IplImage*	img_kt = NULL;
	try
	{
		img_kt = GetSubImage(&ima,rt_kt_koutu);
	}
	catch(std::exception& e)
	{
		string ss = "PerfMotionDetect 289 std";
		ss+= e.what();
		SetInfo(ss);
		return -1;
	}
	if(img_kt == NULL)
		return -17;
	int ret = 0;
	//Pause();
	//int ret = InsertSPImg(img_kt,100,100,2);//坐标的位置需要根据实际情况调整
	//Pause();
	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	//	IplImage* dst = FixImg( insert, x, y, mode);
	//	if(dst == NULL)
	//	return -1;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvGetSize(&ima), 8, 3);
		roi1 = cvCreateImage(cvGetSize(&ima), 8, 1);
		cvZero(res1);
		cvZero(roi1);
//		cvShowImage("confirm",img_kt);

		EdgeandEllipse(img_kt ,NULL,res1,roi1,rt_kt_koutu.x,rt_kt_koutu.y);


	}
	catch(Exception& e)
	{
		string ss = "InsertSPImg  ";
		ss+= e.what();
		SetInfo(ss);

		return 110;
	}

	cvAddWeighted(res1,0.5,&ima,0.5,0,res1);
	Mat dis = res1;


	dis.copyTo(src);
	dis.release();
	if(res1 != NULL)
		cvReleaseImage(&res1);
	if(roi1 != NULL)
		cvReleaseImage(&roi1);
	//	ret = InsertLPImg(dst,res1,roi1,x,y,mode);//res ROI 会不会被自动释放？

	return ret;
	//	return 0;
}

int CGifAppPlat::InsertKouTu(Mat& src,CvRect ro)
{
//	IplImage ima = src;
	IplImage ima = dst_koutu;
	stest = sImgBtnPath + "zz";
	//	rt_kt.width -= rt_kt.x;
/*	string ss = sImgBtnPath;
	ss += "1.png";
	cvSaveImage(ss.c_str(),&ima);*/

	//	rt_kt.height-=rt_kt.y;
	IplImage*	img_kt = NULL;
	try
	{
		img_kt = GetSubImage(&ima,rt_kt_koutu);
	//	ss = sImgBtnPath;
	//ss += "2.png";
	//cvSaveImage(ss.c_str(),img_kt);
	}
	catch(std::exception& e)
	{
		string ss = "PerfMotionDetect 289 std";
		ss+= e.what();
		SetInfo(ss);
		return -1;
	}

	Pause();
	int ret = InsertSPImg(img_kt,100,100,2);//坐标的位置需要根据实际情况调整
	Pause();
	return ret;
}

//////////////////////
int   CGifAppPlat::DrawingByHandNoImgWhole(int eve ,int x,int y)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return false;
	int delta = 1;
	switch( eve ) {
	case CV_EVENT_MOUSEMOVE: {
		if( drawing_mode ) {


			//	draw_box(image, box,1);
			//	RefreshImage();
			//delta = ABS(x - orginx);
			delta = (x - orginx);
			ChangeMode(delta);
			//ChangeMode(1);
		}
							 }
							 break;
	case CV_EVENT_LBUTTONDOWN: {
		orginx = x;
		drawing_mode = true;

		//origin = cvPoint(x,y);
							   }
							   break;
	case CV_EVENT_LBUTTONUP: {
		drawing_mode = false;

		//	bPause = false;
		/*if(box.width<0) {
		box.x+=box.width;
		box.width *=-1;
		}
		if(box.height<0) {
		box.y+=box.height;
		box.height*=-1;
		}*/
		//	draw_box(image, box,2);
							 }
							 break;
	default:
		break;
	}
	return 0;
}
////////////////////
int  CGifAppPlat::DrawingByHandNoImg(int eve ,int x,int y)
{
	int ret = -1;
	if(nCurOperMode == enum_SELECT_RECT)
	{
		switch( eve ) {
		case CV_EVENT_MOUSEMOVE: {
			if( drawing_box ) {

				box.width = x-box.x;
				box.height = y-box.y;
				ret = 1;
				//	draw_box(image, box,1);
				//	RefreshImage();
			}
								 }
								 break;
		case CV_EVENT_LBUTTONDOWN: {
			drawing_box = true;
			box = cvRect(x, y, 0, 0);
			//	bPause = true;

			origin = cvPoint(x,y);
			ret = 13;
								   }
								   break;
		case CV_EVENT_LBUTTONUP: {
			drawing_box = false;

			//		bPause = false;
			/*if(box.width<0) {
			box.x+=box.width;
			box.width *=-1;
			}
			if(box.height<0) {
			box.y+=box.height;
			box.height*=-1;
			}*/
			//	draw_box(image, box,2);
			ret = 2;
								 }
								 break;
		default:
			break;
		}
	}
	else if(nCurOperMode == enum_DRAW_BIGSHOW)
	{
		switch( eve ) {
		case CV_EVENT_MOUSEMOVE: {

			ret = 3;
								 }
								 break;
		case CV_EVENT_LBUTTONDOWN: {

			InsertBigShowMat(x,y,nCurBigshowId,-1);


			ret = 4;
								   }
								   break;
		case CV_EVENT_LBUTTONUP: {
			ret = 5;
								 }
								 break;
		default:
			break;
		}
	}
	else if(nCurOperMode == enum_DRAW_WITHHAND)
	{
		switch( eve ) {
		case CV_EVENT_MOUSEMOVE: {
			int xx = ABS(x - prev_hand_pt.x);
			int yy = ABS(y - prev_hand_pt.y);
			xx = xx*xx;
			yy = yy*yy;
			xx = xx+yy;

			if(pow(xx,0.5) >HAND_IMG_SEP)
			{
				//考虑到手机负载，暂时不加这个
				//InsertHandImg( x, y,6);
			}
			ret = 3;
								 }
								 break;
		case CV_EVENT_LBUTTONDOWN: {
			prev_hand_pt.x = x;
			prev_hand_pt.y = y;
			InsertHandImg( x, y,6);
			ret = 4;
								   }
								   break;
		case CV_EVENT_LBUTTONUP: {
			ret = 5;
								 }
								 break;
		default:
			break;
		}
	}
	else if(nCurOperMode == enum_DRAW_WRITE)
	{
		switch( eve ) {
		case CV_EVENT_MOUSEMOVE: {
			if(drawing_text)
			{
				if(drawing_text_id == -1)
				{
					ret = 6;
					break;
				}
				bPause = true;
				CvPoint pt;
				pt.x = x;
				pt.y = y;
				cvLine(imgInsertList[drawing_text_id]->imgInsert, prev_hand_pt, pt, m_scr,m_thickness,8,0); //模板上划线

				//	cvLine(imgCover[drawing_text_id], prev_hand_pt, pt, m_scr,m_thickness,8,0); //模板上划线
				prev_hand_pt = pt;
				Rect rt;
				imgInsertList[drawing_text_id]->flagList = 1;
				//	UpdateSPImg(drawing_text_id,typeList[drawing_text_id],rt);
				bPause = false;
			}
			ret = 7;
								 }
								 break;
		case CV_EVENT_LBUTTONDOWN: {
			if(drawing_text)
			{
				ret = 8;
				prev_hand_pt.x = x;
				prev_hand_pt.y = y;
				if(drawing_text_id == -1)
				{
					if(InsertDrawTxt() == 0)
					{
						drawing_text_id = imgInsertList.size() - 1;
					}
				}
			}
			//	InsertHandImg( x, y,6);

								   }
								   break;
		case CV_EVENT_LBUTTONUP: {
			imgInsertList[drawing_text_id]->flagList = 0;

								 }
								 break;
		default:
			break;
		}
	}
	return ret;
}
///////////////////
int  CGifAppPlat::DrawingByHand(int eve ,int x,int y)
{
	if((nCurOperMode == enum_SELECT_RECT)||
		(nCurOperMode == enum_DRAW_WITHHAND)||
		(nCurOperMode == enum_DRAW_WRITE)||
		(nCurOperMode == enum_DRAW_BIGSHOW))
	{
		return DrawingByHandNoImg(eve , x, y);

	}
	int ret = FindtheImg(x,y);

	if(ret == -1)
	{//未选中图片

		return DrawingByHandNoImgWhole(eve , x, y);
	}
	else
	{//选中图片

		switch( eve ) {
		case CV_EVENT_MOUSEMOVE: {
			if(isMovingImg && nCurSelImg != -1)
			{
				bPause = true;
				imgInsertList[nCurSelImg]->flagList = 1;
				imgInsertList[nCurSelImg]->xList += (x - noldx);
				imgInsertList[nCurSelImg]->yList += (y - noldy);
				noldx = x;
				noldy = y;
				if(imgInsertList[nCurSelImg]->xList<0)
					imgInsertList[nCurSelImg]->xList = 0;
				if(imgInsertList[nCurSelImg]->yList<0)
					imgInsertList[nCurSelImg]->yList = 0;
				Rect rt;
				//	UpdateSPImg(nCurSelImg,typeList[nCurSelImg],rt);
				bPause = false;
				ret = 10;
			}
			else
				ret = 20;
								 }
								 break;
		case CV_EVENT_LBUTTONDOWN: {
			isMovingImg = true;
			noldx = x;
			noldy = y;
			//	bPause = true;
			SelectImg(x,y);
			ret = 30;
			//	SelectImg(x,y);
								   }
								   break;
		case CV_EVENT_LBUTTONUP: {
			if(isMovingImg)
			{
				isMovingImg = false;
				ret = 40;
				//	origin = cvPoint(x,y);
				//nTrackId = ret;
			}
			//bPause = false;

								 }
								 break;
		default:
			break;
		}
	}
	return ret;
}
int  CGifAppPlat::GetImgListWidth()
{
	if(imglist.size() <= 0)
		return 0;
	return imglist[0]->width;
}
int  CGifAppPlat::GetImgListHeight()
{
	if(imglist.size() <= 0)
		return 0;
	return imglist[0]->height;
}
void CGifAppPlat::ResumeList()
{
	if(imglist.size() <= 0 || imglistBak.size()<= 0 || imglist.size() != imglistBak.size())
		return;
	// vector<IplImage*>::iterator iter1=imglistBak.begin();
	for(int i=0; i<imglist.size(); i++)
	{


		IplImage* tt = imglist[i];
		IplImage* tt1 = imglistBak[i];
		try
		{
			cvCopy(tt1,tt);
		}
		catch(Exception ee)
		{
			return;
		}
		//cvReleaseImage(&tt);
		//iter = imglist.erase(iter);


	}
}
int CGifAppPlat::PerfEdit(Mat& src,int num,int nPause/*1 pause;0 no pause*/)
{
	if(num < 0|| num >= imglist.size() || imglist.size() <= 0)
		return -199;
	IplImage* itemp = NULL;
	if(nPause == 1)
	{
		if(imagePause == NULL )
		{
			imagePause = cvCreateImage( cvGetSize(imglist[num]), 8, 3 );
		}

		cvCopy(imglist[num],imagePause);
		itemp = imagePause;
	}
	else
	{
		if(imagePause)
		{
			cvReleaseImage(&imagePause);
			imagePause = NULL;
		}
		itemp= imglist[num];
	}
	int ret =  PerfImageShow(itemp);
	if(ret == 0)
	{
		Mat dis = itemp;//imglist[num];
		dis.copyTo(src);
		dis.release();
	}
	else
		return ret;
	return 0;
}
int CGifAppPlat::PerfImageShow(IplImage* image)
{
	IplImage* itmp = NULL;
	IplImage* dst = NULL;
	try
	{

		if( image )
		{
			//nImgWidth = image->width;
			//nImgHeight = image->height;

			//	ang = 0;
			//md->Process(image,0,ang,origin);
			if(nCurMode != enum_Empty)
			{

				Mat sss(image);

				SwitchMode(sss,nCurMode);

				IplImage  bbc = sss;


				cvCopy(&bbc,image);

				sss.release();
			}
			//m_font.
			//	cvPutText(image,m_sCompany.c_str(),cvPoint(nImgWidth - textSize.width,nImgHeight - textSize.height),&m_font,CV_RGB(255,255,255));
			//	string ss;
			//ss.
			//	char buf[10];
			//	itoa(nImgWidth,buf,10);
			//	ss = "here";
			//	ss+= " &&" ;
			//	itoa(nImgHeight,buf,10);
			//	ss+=buf;
			//	InputLoginfo(ss);
			//			InsertTxt("",100,100);

			//	return 5;
		}
	}
	catch(cv::Exception& e)
	{
		string ss = "PerfMotionDetect 289 cv";
		ss+= e.err;
		SetInfo(ss);
		return 101;
	}
	catch(std::exception& e)
	{
		string ss = "PerfMotionDetect 289 std";
		ss+= e.what();
		SetInfo(ss);
		return 102;
	}
	/*            catch(Exception& e)
	{
	string ss = "PerfMotionDetect 289 ";
	ss+= e.what();
	SetInfo(ss);
	}*/
	//   update_mhi( image, motion, 60 );

	//	 track_window = track_comp.rect;

	/* if( backproject_mode )
	cvCvtColor( backproject, image, CV_GRAY2BGR ); // 使用backproject灰度图像
	if( image->origin )
	track_box.angle = -track_box.angle;*/
	/*
	CvPoint2D32f center;   Center of the box.                          
	CvSize2D32f  size;    /* Box width and length.                       
	float angle;          /* Angle between the horizontal axis           
	/* and the first side (i.e. length) in degrees 

	*/


	//    cvEllipseBox( image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );


	/*   if( select_object && selection.width > 0 && selection.height > 0 )
	{
	cvSetImageROI( image, selection );
	cvXorS( image, cvScalarAll(255), image, 0 );
	cvResetImageROI( image );
	}*/
	//	img256 = cvCreateImage( cvSize(image->width,image->height), 8, 3 );
	//cvZero( img256 );
	//	img256->origin = image->origin;
	//	cvCopy(image,img256,NULL);
	//cvCvtColor( image, img256, CV_BGR2GRAY );

	//	img256

	int ntest = 0,n1=0,n2=0,n3 = 0,n4 = 0;
	bool bSelTrack = false;
	try
	{
		if(!bPause)
		{

			for(int i = 0;i<imgInsertList.size();i++)
			{
				//	IplImage* itmp;

				testnum = 3456;
				if(i == nTrackId)
				{
					//IplImage* itmp = NULL;
					//  itmp = FitRotate (imgInsert[i], ang,3);
					itmp = imgInsertList[i]->imgInsert;
					//angList[i] = ang;

					FixRect(origin.x,origin.y,imgInsertList[i]->widList ,imgInsertList[i]->heiList);
					CvRect rect1=cvRect(origin.x,origin.y,imgInsertList[i]->widList ,imgInsertList[i]->heiList);


					cvSetImageROI(image,rect1);

					cvSetImageROI(itmp,cvRect(0,0,imgInsertList[i]->widList ,imgInsertList[i]->heiList));

					cvAddWeighted(image,0,itmp,1,0,image);

					cvResetImageROI(image);
					if(i == nCurSelImg)
						bSelTrack = true;
					if(imgBtn && (i == nCurSelImg))
					{
						xBtn = origin.x;
						yBtn = origin.y;

					}
				}
				else if(i != nCurSelImg)
				{

					//FitRotate (itmp,imgInsert[i], angList[i],3);
					//   itmp = FitRotate (imgInsert[i], angList[i],3);
					if(imgInsertList[i]->typeList == 0)
					{
						testnum = 345;
						itmp = imgInsertList[i]->imgInsert;

						//  cvNamedWindow("itmp", 1);  

						// cvShowImage("itmp",itmp);  

						//    CvRect rect1=cvRect(xList[i],yList[i],widList[i] ,heiList[i]);
						ntest = 1;
						FixRect(imgInsertList[i]->xList,imgInsertList[i]->yList,imgInsertList[i]->widList ,imgInsertList[i]->heiList);
						CvRect rect1=cvRect(imgInsertList[i]->xList,imgInsertList[i]->yList,imgInsertList[i]->widList ,imgInsertList[i]->heiList);

						ntest =2;
						n1 = imgInsertList[i]->xList;n2 = imgInsertList[i]->yList;n3 = imgInsertList[i]->widList;n4 = imgInsertList[i]->heiList;

						cvSetImageROI(image,rect1);
						ntest = 3;
						cvSetImageROI(itmp,cvRect(0,0,imgInsertList[i]->widList ,imgInsertList[i]->heiList));
						ntest = 4;

						cvAddWeighted(image,0,itmp,1,0,image);
						ntest = 5;
						cvResetImageROI(image);
						cvResetImageROI(itmp);
					}
					else if(imgInsertList[i]->typeList == 8)//bigshow
					{

						Rect rt;
						UpdateSPImg(i,imgInsertList[i]->typeList,rt);
						dst = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
						cvCopy(imgInsertList[i]->imgCover,dst);

						//	cvSaveImage("d:\\img\\cvCircle.png", dst);
						cvAdd(image, imgInsertList[i]->imgCover, dst, imgInsertList[i]->imgMask);//此处需要实验   dst(I)=src1(I)+src2(I) if mask(I)!=0
						cvCopy(dst,image);

						/*	cvSaveImage("d:\\img\\test\\cvCircle.png", image);
						cvSaveImage("d:\\img\\test\\cvCircle1.png", imgInsertList[i]->imgCover);
						cvSaveImage("d:\\img\\test\\cvCircle2.png", imgInsertList[i]->imgMask);*/
						cvReleaseImage(&dst);

						InsertBigShowMat(imgInsertList[i]->xList,imgInsertList[i]->yList,imgInsertList[i]->bigshowidList,i);
					}
					else if(imgInsertList[i]->typeList != 4)// !=0  !=4
					{
						testnum = 34568;
						Rect rt;
						UpdateSPImg(i,imgInsertList[i]->typeList,rt);
						dst = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
						cvCopy(imgInsertList[i]->imgCover,dst);

						//	cvSaveImage("d:\\img\\cvCircle.png", dst);
						cvAdd(image, imgInsertList[i]->imgCover, dst, imgInsertList[i]->imgMask);//此处需要实验   dst(I)=src1(I)+src2(I) if mask(I)!=0
						cvCopy(dst,image);
						cvReleaseImage(&dst);

					}
					else if(imgInsertList[i]->typeList == 4)
					{
						if(TextMovingByItSelf(i))
						{
							dst = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
							cvCopy(imgInsertList[i]->imgCover,dst);
							cvAdd(image, imgInsertList[i]->imgCover, dst, imgInsertList[i]->imgMask);//此处需要实验   dst(I)=src1(I)+src2(I) if mask(I)!=0
							cvCopy(dst,image);
							cvReleaseImage(&dst);
						}
					}


					//	cvResetImageROI(itmp);
				}
			}//for

			if(!bSelTrack && (nCurSelImg != -1))
			{
				int i = nCurSelImg;
				testnum = 34569;
				if(imgInsertList[i]->typeList == 0)	
				{
					//	itmp = FitRotate (imgInsert[i], angList[i],3);
					testnum = 345670;
					itmp = imgInsertList[i]->imgInsert;

					//  cvNamedWindow("itmp", 1);  

					// cvShowImage("itmp",itmp);  

					//    CvRect rect1=cvRect(xList[i],yList[i],widList[i] ,heiList[i]);
					ntest = 1;
					FixRect(imgInsertList[i]->xList,imgInsertList[i]->yList,imgInsertList[i]->widList ,imgInsertList[i]->heiList);
					CvRect rect1=cvRect(imgInsertList[i]->xList,imgInsertList[i]->yList,imgInsertList[i]->widList ,imgInsertList[i]->heiList);

					ntest =2;
					//	n1 = xList[i];n2 = yList[i];n3 = widList[i];n4 = heiList[i];

					cvSetImageROI(image,rect1);
					ntest = 3;
					cvSetImageROI(itmp,cvRect(0,0,imgInsertList[i]->widList ,imgInsertList[i]->heiList));
					ntest = 4;

					cvAddWeighted(image,0,itmp,1,0,image);
					ntest = 5;
					cvResetImageROI(image);

				}
				else if(imgInsertList[i]->typeList == 8)//bigshow
				{

					Rect rt;
					UpdateSPImg(i,imgInsertList[i]->typeList,rt);
					dst = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
					cvCopy(imgInsertList[i]->imgCover,dst);

					//	cvSaveImage("d:\\img\\cvCircle.png", dst);
					cvAdd(image, imgInsertList[i]->imgCover, dst, imgInsertList[i]->imgMask);//此处需要实验   dst(I)=src1(I)+src2(I) if mask(I)!=0
					cvCopy(dst,image);
					cvReleaseImage(&dst);

					InsertBigShowMat(imgInsertList[i]->xList,imgInsertList[i]->yList,imgInsertList[i]->bigshowidList,i);
				}
				else
				{
					Rect rt;
					testnum = 3456710;

					UpdateSPImg(nCurSelImg,imgInsertList[nCurSelImg]->typeList,rt);

					////////////////////
					/*		cvSaveImage("d:\\img\\cvAnd.png", imgInsert[nCurSelImg]);
					cvSaveImage("d:\\img\\cvAnd1.png", imgCover[nCurSelImg]);
					cvSaveImage("d:\\img\\cvAnd2.png", imgMask[nCurSelImg]);*/
					///////////////////

					dst = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
					cvCopy(imgInsertList[i]->imgCover,dst);
					cvAdd(image, imgInsertList[i]->imgCover, dst, imgInsertList[i]->imgMask);//此处需要实验   dst(I)=src1(I)+src2(I) if mask(I)!=0
					cvCopy(dst,image);
					cvReleaseImage(&dst);

					///////////////////////
					/*cvRectangle (
					image,
					cvPoint(xList[i],yList[i]),
					cvPoint(xList[i]+widList[i],yList[i]+heiList[i]),
					m_scr 
					);*/

					///////////////////////

				}


				if(imgBtn && (i == nCurSelImg))
				{
					xBtn = imgInsertList[i]->xList;
					yBtn = imgInsertList[i]->yList;
				}
			}//if


			/*	if (imgInsert.size() > 0) {
			if (!bGetTestImg) {
			bGetTestImg = true;
			String ss = sImgBtnPath + ".png";
			//	cvSaveImage(ss.c_str(), image);
			testnum = 34567;
			}
			}*/

		}//bpause
		// cvCopy(iii,img256,NULL);
		//	cvShowImage( "Motion", img256 );
		//cvShowImage( "Motion", motion );
	}
	catch(cv::Exception& e)
	{
		string ss = "PerfMotionDetect 395 cv";
		ss+= e.err;
		ntest++;
		n1=n2=n3=n4;
		SetInfo(ss);
		return 103;
	}
	catch(std::exception& e)
	{
		string ss = "PerfMotionDetect 395 std";
		ss+= e.what();
		SetInfo(ss);
		return 104;
	}
	try
	{
		if(nCurOperMode == enum_SELECT_RECT)
		{
			draw_box(image, box,m_scr);
		}
	}
	catch(cv::Exception& e)
	{
		string ss = "PerfMotionDetect 395 cv";
		ss+= e.err;
		ntest++;
		n1=n2=n3=n4;
		SetInfo(ss);
		return 105;
	}
	catch(std::exception& e)
	{
		string ss = "PerfMotionDetect 395 std";
		ss+= e.what();
		SetInfo(ss);
		return 107;
	}
	return 0;
}
int CGifAppPlat::PerfLive(IplImage* image,int num)
{
	IplImage* convert = NULL;
	IplImage* convert2 = NULL;
	IplImage* convert3 = NULL;
	IplImage* boximg = NULL;
	IplImage* dst;


	IplImage* itmp = NULL;
	//	CvCapture* capture = 0; //视频获取结构
	double ang = 0;
	int jump = 0;

	int ret = PerfImageShow(image);

	if(ret != 0)
		return ret;
	try
	{

		if(bGetGif && !bPause && num == STATUS_LIVE)
		{
			//num等于STATUS_LIVE，则为拍摄或者首次播放文件状态，写入imglist
			//  convert=cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 3);
			if(nCurOperMode == enum_SELECT_RECT)
			{
				boximg = GetSubImage(image, box);
				//   cvReleaseImage(&image);
			}
			else
			{
				boximg = cvCreateImage( cvGetSize( image ), IPL_DEPTH_8U, 3);
				cvCopy(image,boximg);

			}


			//	cvReleaseImage(&convert2);//pc端这么做可以，手机再实验


			/*if(m_sCompany.length() > 0)
			{
			if((convert2->width - textSize.width) > 0 && (convert2->height - textSize.height)>0)
			cvPutText(convert2,m_sCompany.c_str(),cvPoint(convert2->width - textSize.width,convert2->height - textSize.height),&m_font,CV_RGB(255,255,255));
			}*/
			imglist.push_back(boximg);
			IplImage* boximg2 = cvCreateImage(cvGetSize(boximg),boximg->depth,boximg->nChannels);
			cvCopy(boximg,boximg2);
			imglistBak.push_back(boximg2);

		}
	}
	catch(cv::Exception& e)
	{
		string ss = "PerfMotionDetect 450 cv";
		ss+= e.err;
		SetInfo(ss);
		return -113;
	}
	try
	{

		if(imgBtn)
		{
			//xBtn = xList[ret]-imgBtn->width + widList[ret];
			//yBtn = yList[ret]-imgBtn->height + heiList[ret];
			widBtn = imgBtn->width;
			heiBtn = imgBtn->height;
			FixRect(xBtn,yBtn,widBtn ,heiBtn);

			//
			CvRect rectBtn=cvRect(xBtn,yBtn,widBtn ,heiBtn);//
			/*	if(ang != 0)
			{
			cvSaveImage("d:\\dst.png", m_pdst);
			cvSaveImage("d:\\src.png", itmp);
			cvSaveImage("d:\\mask.png", m_pMask);
			}*/
			/*	jump += 20;
			jump = jump%300;*///需要移动、缩放的时候就打开注释

			cvSetImageROI(image,rectBtn);

			cvAddWeighted(image,0,imgBtn,1,0,image);

			cvResetImageROI(image);
		}
	}
	catch(cv::Exception& e)
	{
		string ss = "PerfMotionDetect 450 cv";
		ss+= e.err;
		SetInfo(ss);
		return -114;
	}
	catch(std::exception& e)
	{
		string ss = "PerfMotionDetect 450 std";
		ss+= e.what();
		SetInfo(ss);
		return -115;
	}
	/*		    catch(Exception& e)
	{
	string ss = "PerfMotionDetect 450 ";
	ss+= e.what();
	SetInfo(ss);
	}*/
	//	GetBMP(*image);




	return 0;
}
//PerfMotionDetect(mRgb,mRgbF,nfun,nScale,0);
int CGifAppPlat::PerfMotionDetect(Mat& src,Mat& srcF,int num,float nScale/*考虑到编辑各种尺寸的文件，有可能需要缩放（为了方便显示），这个参数代表了缩放比例*/,int nPause/*1 pause;0 no pause*/)
{
	//	IplImage* motion = 0;

	//	CvCapture* capture = 0; //视频获取结构


	//	if(!bPause)
	//	return;
	//	if(num == -1)


	/*{
	nImgWidth = src.cols;
	nImgHeight = src.rows;
	}*/


	//InsertImg("d:\\0.jpg",100,100);

	//	IplImage* iii=cvLoadImage(imgpath);
	//	CMotionDetect* md = new CMotionDetect();

	//	IplImage* mmmm = cvLoadImage("d:\\dst.png");
	//capture = cvCaptureFromCAM( 0 );
	//	if( capture )

	//	cvNamedWindow( "Motion", 1 );
//	cvSetMouseCallback( "video", on_event_test, NULL ); // on_mouse 自定义事件
	//	for(;;)

	//   IplImage* image;
	//	if( !cvGrabFrame( capture )) //从摄像头或者视频文件中抓取帧
	//	break;
	//image = cvRetrieveFrame( capture ); //取回由函数cvGrabFrame抓取的图像,返回由函数cvGrabFrame 抓取的图像的指针


	IplImage  bb = src;

	if(num == STATUS_LIVE)
	{
		try
		{
		if(nScale == 1)
		{
			if(!imagegl)
			   imagegl = cvCreateImage( cvSize(bb.width,bb.height), 8, 3 );
		//   if(!bPause)
		    cvCopy(&bb,imagegl);
			nImgWidth = src.cols;
	        nImgHeight = src.rows;
			nOldImgWidth = src.cols;
	        nOldImgHeight = src.rows;
		}
		else if(nScale > 0)
		{
			IplImage* dst = cvCreateImage( cvSize(bb.width,bb.height),  8, 3); //构造目标图象
			cvCopy(&bb,dst);
			if(!imagegl)
			   imagegl = cvCreateImage( cvSize(nScale*bb.width,nScale*bb.height), 8, 3 );

			cvResize(dst, imagegl, CV_INTER_LINEAR);
			nImgWidth = nScale*bb.width;
	        nImgHeight = nScale*bb.height;
			nOldImgWidth = bb.width;
	        nOldImgHeight = bb.height;
			cvReleaseImage(&dst);
		}
		}
		catch(Exception e)
		{
			return -33;

		}
		
	}
	else
	{
		//	if(num < 0|| num >= imglist.size() || imglist.size() <= 0)
		//		return -199;
		//	imagegl = imglist[num];
		nImgWidth = src.cols;
	    nImgHeight = src.rows;
		return PerfEdit(src,num,nPause);
	}


	//	if(!bPause)
	//	if(num == -1)

	//	else
	//	return 0;
	int ret = 0;
	//	if(num == -1)
	{
		ret = PerfLive(imagegl,num);
	}
	//else
	//return PerfEdit(num);

	//	if( cvWaitKey(10) >= 0 )
	//	break;

	//	cvReleaseCapture( &capture );
	//cvDestroyWindow( "Motion" );
	try
	{
	if(ret == 0)
	{
		Mat dis = imagegl;
		dis.copyTo(srcF);
		dis.release();
	}
	else
		return ret;
	}
	catch(Exception e)
	{
		return -131;
	}
	return 0;
}

///////////////////////////////////

int   CGifAppPlat::GetImgbyID(int id,Mat& src)
{
	if(id < 0 || id > imglist.size() ||(imglist.size()==0))
		return -134;
	IplImage* timg = imglist[id];
	Mat dis = timg;
	dis.copyTo(src);
	dis.release();
	return 0;
}



////////////////////////////////////////
//读取GIF里的每一帧，并生成临时文件
void CGifAppPlat::GetPages(char* filename)//
{
	FILE* hFile;	//file handle to write the image

	//char filename[256] = "d:\\img\\4.gif";





	//	GetImages();
	//	if (pImages==NULL || pagecount<=0 || pImages[0]==NULL) return;



	//CxImage* newima = new CxImage(pImages[0]->GetWidth(),pImages[0]->GetHeight(),pImages[0]->GetBpp());
	/*	int type = CXIMAGE_FORMAT_GIF;
	try
	{
	CxImage* image = new CxImage(filename,CXIMAGE_FORMAT_GIF);
	//	newima->SetFrameDelay(5);
	//newima.Ghost(pImages[0]);
	//	 if(newima->Encode(hFile,pImages,pagecount,CXIMAGE_FORMAT_GIF))
	int n = image->GetNumFrames();
	if (n>0)
	{


	//vector<string> pLcv;
	//vector<int> pDelay;
	// create the documents for the other images
	for(int i=0;i<image->GetNumFrames();i++)
	{

	CxImage *newImage = new CxImage();
	newImage->SetFrame(i);
	newImage->Load(filename,type);
	//	int w = newImage->GetWidth();
	//int h = newImage->GetHeight();
	char name[256];
	sprintf(name,"%s%d\0",filename,i);
	newImage->Save(name,CXIMAGE_SUPPORT_BMP);
	pLcv.push_back(name);
	//	int nn = image->GetFrameDelay();
	pDelay.push_back(10*max(1,image->GetFrameDelay()));
	delete[] name;
	}

	//	IplImage* lll = cvCreateImage( cvSize(newImage->GetWidth(),newImage->GetHeight()), 8, 3 );
	//	IplImage* lll2 = cvCreateImage( cvSize(newImage->GetWidth(),newImage->GetHeight()), 8, 3 );
	//	memcpy(lll->imageData, newImage->GetBits(), newImage->GetWidth()*3*newImage->GetHeight());
	//10*max(1,image->GetFrameDelay())
	//	 CopyData((char*)lll->imageData, (char*)newImage->GetBits(), newImage->GetWidth()*3*newImage->GetHeight(), true, newImage->GetHeight()); 
	//	cvNamedWindow( "wave", 1 );




	}

	}
	catch(Exception& e)
	{
	string ss = "GetPages 556 ";
	ss+= e.what();
	SetInfo(ss);
	}

	/*} 
	else 
	{

	image->SetRetreiveAllFrames(true);
	image->SetFrame(image->GetNumFrames()-1);
	image->Load(filename, type);



	}*/

	return;
}


/////////////////////////////////////////
/////////////////
//no thread



int CGifAppPlat::CreatingGIF()
{
	IplImage*  timg;
	IplImage* boximg;
	IplImage* convert;
	IplImage* convert2;
	IplImage* convert3;
	ColorMapObject* map;
	int ret = 3;
	int w,h;
	if(imglist.empty())
		return 0;

	//	while(1)
	if(gw)
		gw->setDuration(nDelay);
	int tmp;
	if(saveType == 1)
	{
		tmp = 1;
	}
	else
	{
		tmp = nEvery;
	}
	for(int i = 0 ; i < imglist.size(); i+= tmp)
	{
		//	if(imglist.empty())
		//	break;
		//	timg = imglist.front();
		//imglist.pop_front();
	
		try
		{
		if((nImgWidth != nOldImgWidth)||(nImgHeight != nOldImgHeight))
		{
			timg = cvCreateImage(cvSize(nOldImgWidth,nOldImgHeight),imglist[i]->depth,imglist[i]->nChannels);
			cvResize(imglist[i], timg, CV_INTER_LINEAR);
		}
		else
           timg = imglist[i];
		}
		catch(Exception e)
		{
			return -10;
		}
		//	cvReleaseImage(&tt);
		//iter = imglist.erase(iter);



		/*	convert=cvCreateImage( cvGetSize( timg ), IPL_DEPTH_8U, 3);
		convert2=cvCreateImage( cvGetSize( timg ), IPL_DEPTH_8U, 3);
		cvConvertImage(timg,convert,CV_CVTIMG_FLIP); 
		*/
		if(saveType == 1)
		{
			if(!writer)
				writer=cvCreateVideoWriter("d:\\img\\1120.avi",CV_FOURCC('X', 'V', 'I', 'D'),fps,cvSize(timg->width,timg->height),1);
			if(!writer)
			{

				return -123;
			}
			try
			{
				ret = cvWriteFrame(writer, timg);
				//cvWaitKey(33);
			}
			catch(Exception ex)
			{
				return -1;
			}
		}
		else if(saveType == 0)
		{
			try
			{
				bool dither = false;

				/////////////////////

				if((m_size == 1) || (nCurOperMode == enum_SELECT_RECT))//因为需要判断nCurOperMode，所以本函数务必要在STOP函数之前调用。
				{
					boximg = cvCreateImage( cvGetSize( timg ), IPL_DEPTH_8U, 3);
					cvCopy(timg,boximg);
				}
				else  //if(m_size == 2)
				{
					int tWidth = timg->width / m_size;
					int tHeight = timg->height / m_size;
					boximg = cvCreateImage( cvSize(tWidth,tHeight), timg->depth, timg->nChannels); //构造目标图象

					cvResize(timg, boximg, CV_INTER_LINEAR);

				}
			/*	else if(m_size == 3)
				{
					int tWidth = timg->width / 3;
					int tHeight = timg->height / 3;
					boximg = cvCreateImage( cvSize(tWidth,tHeight), timg->depth, timg->nChannels); //构造目标图象

					cvResize(timg, boximg, CV_INTER_LINEAR);
				}
				else if(m_size == 4)
				{
					int tWidth = timg->width / 4;
					int tHeight = timg->height / 4;
					boximg = cvCreateImage( cvSize(tWidth,tHeight), timg->depth, timg->nChannels); //构造目标图象

					cvResize(timg, boximg, CV_INTER_LINEAR);
				}
*/

				convert2=cvCreateImage( cvGetSize( boximg ), IPL_DEPTH_8U, 3);
				//	convert3=cvCreateImage( cvSize(boximg->width/4,boximg->height/4), IPL_DEPTH_8U, 3);//在pc测试，不需要变小
				//	convert3=cvCreateImage( cvSize(boximg->width,boximg->height), IPL_DEPTH_8U, 3);//在pc测试，不需要变小


				//以下为pc端做法，手机再实验，两者的区别需要思考
				cvConvertImage(boximg,convert2,CV_CVTIMG_FLIP);
				//	convert = FitRotate (convert2, 0,3);
				//	convert=cvCreateImage( cvGetSize( convert2 ), IPL_DEPTH_8U, 3);

				cvReleaseImage(&boximg);

				if(pw != NULL && pw->map() == NULL)
				{
					///////////////////////////
					pw->fromImage(convert2, 256, 1);
					map = pw->mapCopy();
					if(map == NULL)
					{
						//AfxMessageBox("wrong!!!!");
						return 1;
					}
					gw->addFrame(convert2,map,dither);
				}
				else
					gw->addFrame(convert2,NULL,dither);


				cvReleaseImage(&convert2);
			}
			catch(Exception& e)
			{
				string ss = "CreatingGIF 618 ";
				ss+= e.what();
				return -2;
				//   SetInfo(ss);
			}
		}
		if(timg != imglist[i])
		{
			cvReleaseImage(&timg);
		}
	}
	return ret;
}


////////////////////

//thread

/////////////////////
/*
int Rotateandresize( char* argv );
int laplace(  );
int ImgSeperate(  );
int EdgeandEllipse(  );
int full( );

int light(char* argv );
void FastMatch();
int KeyPointMatch();
int BoLang(IplImage *src,double angle,int deltaI,int A);
void ColorMap(IplImage *source,int nColor);
void LianHuanHua(IplImage *source);
void SuMiao(IplImage *source);
void Menglong(IplImage *source,float mSize);
void Wind(IplImage *source,int num2,int num1);
void Xuanwo(IplImage *source,double Para);
void casting(IplImage *source);
void freezing(IplImage *source) ;
void DiaokeFuDiao(IplImage *source,int nType/*0  浮雕   1 雕刻*///);
//void HAHAJing(IplImage *source,int nType/*0  放大   1 挤压*/);
//void HiLightRGB(IplImage *source);
//void HuaiJiu(IplImage *source);
/*void JingxiangMohu(IplImage *source,int num);
void XuanZhuan(IplImage *source,int num);
void Maoboli(IplImage *source);
void ManHua(IplImage *source);*/


//////////////////////////////////////////

//////////////////////////////////
//处理播放视频的功能
int CGifAppPlat::PerfCapVideo(Mat& src)
{
	IplImage* frame; 
	if(!capture)
		return -1;



	frame = cvQueryFrame( capture );

	if( !frame )
		return  -2;
	Mat dis = frame;
	dis.copyTo(src);
	dis.release();
	cvReleaseImage(&frame);

//	return PerfMotionDetect(src,-1,1,0);
	return 8;
}
int   CGifAppPlat::InitVideoFile(char* path)
{
	try
	{
		capture = cvCreateFileCapture( path );
	}
	catch(Exception& e)
	{


		return -1;
	}
	if(capture == NULL)
		return  -2;
	return 0;
}
//////////////////////////////


//真正生成GIF的函数
int CGifAppPlat::BuildingGifEntrance(char* filename)//参数是路径，带斜杠
{
	// TODO: 在此添加控件通知处理程序代码
	//	 CreateBMP();
	//以下为生成BMP文件的代码
	//char path[256] = "d:\\";
	char filepath[256];
	sprintf(filepath,"%s\0",filename);
	//	
	//	cvSaveImage(filepath,img256);
	///////////////////////


	//	CreateGIF();
	if(!bGetGif )
	{//第一次调用，直接进入录制状态
		bGetGif = true;
		//GetDlgItem(IDC_BUTTON7)->SetWindowText("Getting");
		//	ResumeThrd();//不用线程
		return  0;
	}



	bGetGif = false;//第二次调用，实际进行录制
	//	bool bb = false;
	int ret = 0;
	//SuspendThrd();//不用线程
	if(saveType == 1)
	{
		if(!writer)
		{


			int codec[] = {CV_FOURCC('X', 'V', 'I', 'D'),CV_FOURCC('P', 'I', 'M', '1'),
				CV_FOURCC('M', 'J', 'P', 'G'),
				CV_FOURCC('M', 'P', '4', '2'),
				CV_FOURCC('D', 'I', 'V', '3'),
				CV_FOURCC('D', 'I', 'V', 'X'),
				CV_FOURCC('U', '2', '6', '3'),
				CV_FOURCC('I', '2', '6', '3'),
				CV_FOURCC('F', 'L', 'V', '1'),
				CV_FOURCC('H', '2', '6', '4')};
			/*	for(int i=0;i<9;i++)
			{
			writer=cvCreateVideoWriter(filepath,codec[i],fps,cvSize(tWidth,tHeight),1);
			if(writer)
			break;
			}*/

			//   writer=cvCreateVideoWriter(filepath,CV_FOURCC('X', 'V', 'I', 'D'),fps,cvSize(320,240),1);
		}
		ret = CreatingGIF();

		cvReleaseVideoWriter(&writer);
		writer = NULL;
	}
	else
	{
		try
		{
			if(!pw)
				pw = new PaletteWidget();
			if(!gw)
				gw = new GifWidget();
		}
		catch(Exception ee)
		{
			return -9;
		}
		if(gw != NULL)
		{
			ret = 3;
			try
			{
				ret = CreatingGIF();
				if(ret == 3)
					ret = gw->save(filepath);
				//	if(!bb)
				//	ret = 5;
				//	SetProgress(1);
			}
			catch(Exception& e)
			{
				string ss = "BuildingGifEntrance 700 ";
				ss+= e.what();
				// SetInfo(ss);

				return ret;
			}
		}
		try
		{
			if(pw)
			{
				delete  pw;
				pw = NULL;
			}
			if(gw)
			{
				delete gw;
				gw = NULL;
			}
		}
		catch(Exception eee)
		{
			return -201;
		}
	}
	//	if(!bb)
	//	return 5;
	return ret;
	//if(!bb)
	//   AfxMessageBox("wwwwwwwww");




}
int  CGifAppPlat::InsertBigShowMat(int x,int y,int id,int nInsertListId)
{
	if(imgBigShow.size() <= 0)
		return -1;
	//	if(pre_bigshow_id != -1)
	//    DeleteImg(pre_bigshow_id);
	/*	if(pre_bigshow_id == imgBigShow.size())
	return -4;
	BigShowIMGList blist = imgBigShow[id];

	try
	{


	IplImage* tt = blist[0];
	cvReleaseImage(&tt);
	vector<IplImage*>::iterator iter1=imgBigShow[id].begin();
	imgBigShow[id].erase(iter1); 




	}
	catch(Exception& e)
	{
	string ss = "insertbigshow ";
	ss+= e.what();
	SetInfo(ss);
	return -46;
	}
	if(imgBigShow[id].size() <= 0)
	{
	vector<BigShowIMGList>::iterator iter2=imgBigShow.begin() + id;
	imgBigShow.erase(iter2); 
	return -34;
	}*/
	try
	{
		if(id == 1)
			id = 1;
		if(nInsertListId != -1)
		{
			if(imgBigShow[id].size() == 0)
				return -57;
			if(imgInsertList[nInsertListId]->bigshowEvery > 0)
			{
				imgInsertList[nInsertListId]->bigshowEvery--;
				return 53;
			}
			else
				imgInsertList[nInsertListId]->bigshowEvery = nEveryBigShow;
			imgInsertList[nInsertListId]->bigshowListInId += 1;
			imgInsertList[nInsertListId]->bigshowListInId %= (imgBigShow[id].size());
			cvReleaseImage(&imgInsertList[nInsertListId]->imgInsert);
			imgInsertList[nInsertListId]->imgInsert = cvCreateImage(cvGetSize(imgBigShow[id][imgInsertList[nInsertListId]->bigshowListInId]), 8, 3);
			cvCopy(imgBigShow[id][imgInsertList[nInsertListId]->bigshowListInId],imgInsertList[nInsertListId]->imgInsert);
			Rect rt;
			imgInsertList[nInsertListId]->flagList = 1;
			IplImage* tt = imgBigShow[id][imgInsertList[nInsertListId]->bigshowListInId];
			imgInsertList[nInsertListId]->widList = tt->width;
			imgInsertList[nInsertListId]->heiList = tt->height;
			UpdateSPImg(nInsertListId,8,rt);

		}
		else
		{
			IplImage* dst = cvCreateImage(cvGetSize(imgBigShow[id][0]), 8, 3);
			cvCopy(imgBigShow[id][0],dst);

			//////////////////////////////
			//bibshow要做到点击位置是图像中间，而不是左上角
			x -= dst->width/2;
			y -= dst->height/2;
			if(x<0) x=0;
			if(y<0) y=0;
			////////////////////////////////
			if(InsertSPImg(dst,x, y,8) == 0)
				pre_bigshow_id = imgInsertList.size() - 1;
			imgInsertList[pre_bigshow_id]->bigshowListInId = 0;
			imgInsertList[pre_bigshow_id]->bigshowEvery = nEvery;
			imgInsertList[pre_bigshow_id]->bigshowidList = id;
		}

	}
	catch(Exception& e)
	{
		string ss = "insertbigshow ";
		ss+= e.what();
		SetInfo(ss);
		return -46;
	}


	return 0;

}
int  CGifAppPlat::InputBigShowByPath(char* path,int id)
{
	Mat src;
	IplImage* insert = cvLoadImage(path);
	IplImage* dst1 = cvCreateImage( cvGetSize(insert), insert->depth, insert->nChannels); //构造目标图象

	if(dst1 == NULL)
		return 112;
	cvConvertImage(insert,dst1,CV_CVTIMG_SWAP_RB);
	src = dst1;

	int ret =  InputBigShowMat( src, id);
	cvReleaseImage(&insert);
	cvReleaseImage(&dst1);
	return ret;
}
int  CGifAppPlat::InputBigShowMat(Mat& src,int id)
{
	IplImage  bb = src;

	try{

		IplImage* imagB = cvCreateImage( cvSize(bb.width,bb.height), 8, 3 );

		//	if(!bPause)
		if(imagB == NULL)
			return -12;
		cvCopy(&bb,imagB);
		if(id>= imgBigShow.size())
		{
			BigShowIMGList blist;
			blist.push_back(imagB);
			imgBigShow.push_back(blist);
		}
		else if(id >=0)
		{
			imgBigShow[id].push_back(imagB);
		}
	}
	catch(Exception e)
	{
		return -35;
	}

	return bb.nChannels;
}
int  CGifAppPlat::SetHandImgMat(Mat& src)
{
	IplImage  bb = src;
	if(imageHand)
	{
		cvReleaseImage(&imageHand);
		imageHand = NULL;
	}
	if(!imageHand)
	{
		imageHand = cvCreateImage( cvSize(bb.width,bb.height), 8, 3 );
	}
	//	if(!bPause)
	cvCopy(&bb,imageHand);

	return bb.nChannels;

	//cvSaveImage("d:\\img\\cvCircle.png", imageHand);
}
void  CGifAppPlat::InsertHandImg(int x,int y,int mode)
{
	IplImage* insert = NULL;
	try
	{
		//	insert = cvLoadImage(sImgHandImg.c_str());
		insert = cvCreateImage( cvGetSize(imageHand), 8, 3 );
		cvCopy(imageHand,insert);
	}
	catch(Exception& e)
	{
		return ;
	}
	if(insert == NULL)
		return;
	if(mode == 4)
	{
		InsertSPTxt(insert,-100, -100,mode);//移动图形
		return;
	}
	x -= insert->width/2;
	y -= insert->height/2;
	if(x<0) x=0;
	if(y<0) y=0;
	InsertSPImg(insert,x, y,mode);
	return;
}
//////////////////////////////////
int  CGifAppPlat::InsertDrawTxt()
{
	IplImage* insert  = NULL;
	if(nImgWidth == 0 || nImgHeight == 0)
		return  -1;
	//if(insert == NULL)
	//return ;

	//    ret = InsertLPImg(insert,x,y);
	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		insert = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		cvZero(insert);
		cvZero(res1);
		cvZero(roi1);
		//	EdgeandEllipse(insert ,NULL,res1,roi1,x,y);

		//	cvSaveImage("d:\\img\\cvCircle.png", res1);

		//cvCvtColor( res1, roi1, CV_BGR2GRAY );
		//cvNot( roi1, roi1 );
		//cvSaveImage("d:\\img\\cvCircle1.png", roi1);
	}
	catch(Exception& e)
	{
		string ss = "InsertDrawTxt  ";
		ss+= e.what();
		SetInfo(ss);

		return -1;
	}

	try
	{
		//这种图片不会越界，不用判断尺寸
		if(insert == NULL || res1 == NULL || roi1 == NULL)
			return  -1;
		IMGItem* item = new IMGItem;
		if(item == NULL)
			return -23;
		item->imgInsert = insert;
		item->imgMask =roi1;
		item->imgCover = res1;
		item->xList = 0;
		item->yList = 0;
		item->widList = insert->width;
		item->heiList = insert->height;
		item->typeList = 7;
		item->angList = -1;
		item->dirList =0;
		item->flagList = 0;

		imgInsertList.push_back(item);

		//	if(bresize)
		//    cvReleaseImage(&insert);
		//cvReleaseImage(&mask);
	}
	catch(Exception& e)
	{
		string ss = "InsertImg 700 ";
		ss+= e.what();
		SetInfo(ss);

		return 103;
	}


	return 0;
}


/////////////////////////////////

IplImage*  CGifAppPlat::CreateTestImg()
{
	IplImage*  ret = cvCreateImage(cvSize(200, 200), IPL_DEPTH_8U, 3);
	cvZero(ret);
	Rect rt;
	rt.x = 30;
	rt.y = 30;
	rt.width = 100;
	rt.height = 100;
	cvRectangle (
		ret,
		cvPoint(rt.x,rt.y),
		cvPoint(rt.x+rt.width,rt.y+rt.height),
		m_scr /* blue */
		);
	//	cvSaveImage("e:\\a.png",ret);
	return ret;
}
int  CGifAppPlat::InsertTxt(char* imgpath,int x,int y,int mode)
{
	//文字在JAVA端生成图片后传入路径
	//	wchar_t* wstr = L"我了换个区.";

	//char* ascii = new char[wcslen(wstr) + 1];
	//	IplImage* itext=cvLoadImage(imgpath);

	/*	wcstombs( ascii, wstr, wcslen(wstr) );

	CvFont font;

	cvInitFont( &font, CV_FONT_VECTOR0,1, 1, 0, 1, 8);

	if(image)
	cvPutText(image, ascii , cvPoint(100, 100), &font, CV_RGB(255,0,0));*/

	//InsertImg(imgpath,x, y,4);
	IplImage* insert = NULL;
	try
	{
		insert = cvLoadImage(imgpath);
		//insert = CreateTestImg();//为了测试
	}
	catch(Exception& e)
	{
		return -1;
	}
	if(insert == NULL)
		return -2;
	IplImage* dst1 = cvCreateImage( cvGetSize(insert), insert->depth, insert->nChannels); //构造目标图象

	if(dst1 == NULL)
		return 112;
	cvConvertImage(insert,dst1,CV_CVTIMG_SWAP_RB);
	cvReleaseImage(&insert);
	if(dst1 == NULL)
		return 1234;
	if(mode == 4)
	{
		int ret = InsertSPTxt(dst1,-100, -100,mode);//移动弹幕
		return  ret;
	}
	return InsertSPTxt(dst1,x, y,mode);

}
void CGifAppPlat::FixRect(int &x,int &y,int &wid,int &het)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return  ;
	if(x < 0) x=0;
	if(y<0) y=0;
	if(x+wid >= nImgWidth)
	{
		x = nImgWidth - wid - 5;
		if(x<0)
		{
			x = 0;
			wid = nImgWidth -  5;
		}
	}
	if(y+het >= nImgHeight)
	{
		y = nImgHeight - het - 5;
		if(y<0)
		{
			y = 0;
			het = nImgHeight -  5;
		}
	}
	//if(x < 0) x=0;
	//if(y<0) y=0;
}
int  CGifAppPlat::InsertImg(char* imgpath,int x,int y,int mode)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return  1;

	////////////

	IplImage* insert = NULL;
	try
	{
		insert = cvLoadImage(imgpath);
	}
	catch(Exception& e)
	{
		return 9;
	}
	IplImage* dst1 = cvCreateImage( cvGetSize(insert), insert->depth, insert->nChannels); //构造目标图象

	if(dst1 == NULL)
		return 112;
	cvConvertImage(insert,dst1,CV_CVTIMG_SWAP_RB);
	cvReleaseImage(&insert);
	return InsertLPImg(dst1,NULL,NULL,x, y,mode);
}
////////////////////////////////////
void  CGifAppPlat::GetRectFromMat(Mat& src,CvRect roi)
{
	IplImage bb = src;
	IplImage* dst = NULL;
	dst = GetSubImage(&bb, roi);
	if(dst== NULL)
		return;
	src.release();
	src = Mat(cvGetSize(dst),CV_8UC3); 
	Mat dt(dst);
	dt.copyTo(src);
	dt.release();
	cvReleaseImage(&dst);
	//nImgWidth = src.cols;
	//	nImgHeight = src.rows;
}
//////////////////////////////////
IplImage* CGifAppPlat::GetSubImage(IplImage *image, CvRect roi)
{
	IplImage *result = NULL;
	// 设置 ROI
	try
	{
		cvSetImageROI(image,roi);
		// 创建子图像
		result = cvCreateImage( cvSize(roi.width, roi.height), image->depth, image->nChannels );
		if(result == NULL)
			return NULL;
		if(roi.x <0 || roi.y <0 || (roi.x + roi.width) > image->width || (roi.y + roi.height) > image->height)
			return NULL;
		cvCopy(image,result);
		cvResetImageROI(image);
	}
	catch(std::exception& e)
	{
		string ss = "PerfMotionDetect 289 std";
		ss+= e.what();
		SetInfo(ss);
		return NULL;
	}
	return result;
}
//IplImage* GrabOut(  Mat image, Rect rt );
int  CGifAppPlat::InsertMat(Mat& src,Rect rt,int x,int y,int mode/*0 普通方图  1 矩形截图  2 不规则抠图 3 方图旋转  4 文字*/)
{
	int ret = 0;
	if(nImgWidth == 0 || nImgHeight == 0)
		return  1;
	IplImage  img =  src;
	//IplImage* insert = NULL;
	try
	{
		if(mode == 1)
		{
			//	IplImage* insert = GrabOut( src,  rt );//
			//ret = InsertLPImg(insert,NULL,x, y,1);
			//ret = InsertSPImg( insert,x, y, mode);

		}
		else if(mode == 0)
		{
			IplImage* insert = GetSubImage(&img,rt);//可以全图，rt则为全图的大小；也可以是子图，选择其中一部分
			ret = InsertLPImg(insert,NULL,NULL,x, y,0);
		}
		else 
		{
			IplImage* insert = GetSubImage(&img,rt);
			ret = InsertSPImg( insert,x, y, mode);

		}
	}
	catch(Exception& e)
	{
		string ss = "InsertMat  ";
		ss+= e.what();
		SetInfo(ss);

		return 104;
	}

	return ret;
}
////////////////////////////////
int   CGifAppPlat::UpdateSPTxt(int id,int typehhh,CvRect roi)
{
	int ret = 0;
	if(nImgWidth == 0 || nImgHeight == 0)
		return  1;
	if(id < 0 || id > imgInsertList.size())
		return 1;
	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	IplImage* dst1 = NULL;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		if(res1 == NULL || roi1 == NULL)
			return 11;
		cvZero(res1);
		cvZero(roi1);


		//EdgeandEllipse(imgInsert[id] ,NULL,res1,roi1,xList[id],yList[id]);
		IplImage* dst = NULL;
		if(imgInsertList[id]->widList < imgInsertList[id]->imgInsert->width  || imgInsertList[id]->heiList < imgInsertList[id]->imgInsert->height)
		{

			//此处是为了处理字幕留在屏幕上一部分的情况
			dst = GetSubImage(imgInsertList[id]->imgInsert,roi);

		}
		else
			dst = imgInsertList[id]->imgInsert;


		if(dst ==NULL)
			return 11;

		CvRect rect1=cvRect(imgInsertList[id]->xList,imgInsertList[id]->yList,dst->width ,dst->height);

		cvSetImageROI(res1,rect1);

		//	cvSaveImage("d:\\img\\test\\cvCircle1.png", dst);

		cvAddWeighted(res1,0,dst,1,0,res1);//形成与背景图同尺寸图

		cvResetImageROI(res1);
		//cvSaveImage("d:\\img\\test\\cvCircle2.png", res1);

		//cvSaveImage("d:\\img\\cvCircle.png", dst);

		cvCvtColor( res1, roi1, CV_BGR2GRAY );
		//	cvSaveImage("d:\\img\\test\\cvCircle3.png", roi1);
		cvNot( roi1, roi1 );

		//cvSaveImage("d:\\img\\test\\cvCircle4.png", roi1);
		/////////////////////


		///////////////////
		if(dst != imgInsertList[id]->imgInsert)
		{
			cvReleaseImage(&dst);
			//imgInsert[id] = dst;
		}
		//	cvSaveImage("d:\\img\\cvCircle1.png", roi1);
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPTxt  ";
		ss+= e.what();
		SetInfo(ss);

		return 105;
	}
	try
	{
		IplImage* tt = imgInsertList[id]->imgCover;
		if(tt != NULL)
			cvReleaseImage(&tt);
		tt = imgInsertList[id]->imgMask;
		if(tt != NULL)
			cvReleaseImage(&tt);
		imgInsertList[id]->imgCover = res1;
		imgInsertList[id]->imgMask = roi1;

	}
	catch(Exception& e)
	{
		string ss = "UpdateSPTxt  ";
		ss+= e.what();
		SetInfo(ss);

		return 18;
	}
	return 0;
}

/////////////////////////////////
int   CGifAppPlat::UpdateSPImg(int id,int typehhh,CvRect roi)
{
	//	if(typehhh != 2 || typehhh != 7)
	if(typehhh == 0)
		return 0;
	////rotate change
	if(imgInsertList.size()<= 0)
		return -29;
	//////////////////////
	if(imgInsertList[id]->flagList == 0)
		return 0;//位置没有变化，就不用继续执行
	else
		imgInsertList[id]->flagList = 0;

	if(imgInsertList[id]->dirList != 0)
	{//需要旋转，就必须执行

		RotateUpdateEdge(id);

		return 0;
	}

		if(typehhh == 4  || typehhh == 6)
		return UpdateSPTxt(id, typehhh,roi);//不用EDGE算法
	int ret = 0;
	if(nImgWidth == 0 || nImgHeight == 0)
		return  1;
	if(id < 0 || id > imgInsertList.size())
		return 1;


	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	IplImage* dst1 = NULL;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		cvZero(res1);
		cvZero(roi1);
		EdgeandEllipse(imgInsertList[id]->imgInsert ,NULL,res1,roi1,imgInsertList[id]->xList,imgInsertList[id]->yList);



		/*	cvSaveImage("d:\\img\\cvCircle000.png", imgInsert[id]);
		cvSaveImage("d:\\img\\cvCircle111.png", res1);
		cvSaveImage("d:\\img\\cvCircle222.png", roi1);
		cvSaveImage("d:\\img\\cvCircle333.png", imgCover[id]);
		cvSaveImage("d:\\img\\cvCircle444.png", imgMask[id]);*/
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPImg  ";
		ss+= e.what();
		SetInfo(ss);

		return 107;
	}
	try
	{
		IplImage* tt = imgInsertList[id]->imgCover;
		if(tt != NULL)
			cvReleaseImage(&tt);
		tt = imgInsertList[id]->imgMask;
		if(tt != NULL)
			cvReleaseImage(&tt);
		imgInsertList[id]->imgCover = res1;
		imgInsertList[id]->imgMask = roi1;
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPImg  ";
		ss+= e.what();
		SetInfo(ss);

		return 108;
	}
	return 0;
}
///////////////////////////////

int   CGifAppPlat::InsertSPTxt(IplImage* insert,int x,int y,int mode)//处理字符的插入
{
	int ret = 0;
	if(nImgWidth == 0 || nImgHeight == 0)
		return  1;
	if(insert == NULL)
		return 1;
	//IplImage* insert = GetSubImage(&img,rt);
	//    ret = InsertLPImg(insert,x,y);
	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	IplImage* dst = FixImg( insert, x, y, mode);
	if(dst == NULL)
		return -1;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		cvZero(res1);
		cvZero(roi1);
		//	EdgeandEllipse(insert ,NULL,res1,roi1,x,y);
		if(x != -100 &&  y != -100)
		{
			CvRect rect1=cvRect(x, y,dst->width ,dst->height);

			cvSetImageROI(res1,rect1);

			cvAddWeighted(res1,0,dst,1,0,res1);//形成与背景图同尺寸图

			cvResetImageROI(res1);
		}
		//	cvSaveImage("d:\\img\\cvCircle.png", res1);

		cvCvtColor( res1, roi1, CV_BGR2GRAY );
		cvNot( roi1, roi1 );
		//cvSaveImage("d:\\img\\cvCircle1.png", roi1);
	}
	catch(Exception& e)
	{
		string ss = "InsertSPTxt  ";
		ss+= e.what();
		SetInfo(ss);

		return 109;
	}
	if(x != -100 &&  y != -100)
	{
		//抠图的时候发现resize后抠图就不精确，显示出不需要的部分，索性就把抠图需要的以外的内容直接变黑
		Rect rt;
		rt.x = x;rt.y = y;
		rt.width = dst->width;
		rt.height = dst->height;
		cvReleaseImage(&dst);

		dst = GetSubImage(res1,rt);
	}
	//////////////////////
	///////////////////////////
	/*	IplImage* dst1 = cvCreateImage( cvGetSize(res1), res1->depth, res1->nChannels); //构造目标图象

	if(dst1 == NULL)
	return 1121;
	cvConvertImage(res1,dst1,CV_CVTIMG_SWAP_RB);
	cvReleaseImage(&res1);*/
	//////////////////////////

	/////////////////////
	ret = InsertLPImg(dst,res1,roi1,x,y,mode);//res ROI 会不会被自动释放？

	return ret;
}

///////////////////////////////////

int    CGifAppPlat::InsertSPImg(IplImage* insert,int x,int y,int mode)//处理非方图的插入
{
	int ret = 0;
	if(nImgWidth == 0 || nImgHeight == 0)
		return  1;
	if(insert == NULL)
		return 1;
	//IplImage* insert = GetSubImage(&img,rt);
	//    ret = InsertLPImg(insert,x,y);
	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	IplImage* dst = FixImg( insert, x, y, mode);
	if(dst == NULL)
		return -1;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		cvZero(res1);
		cvZero(roi1);

		EdgeandEllipse(dst ,NULL,res1,roi1,x,y);


	}
	catch(Exception& e)
	{
		string ss = "InsertSPImg  ";
		ss+= e.what();
		SetInfo(ss);

		return 110;
	}
	Rect rt;
	rt.x = x;rt.y = y;
	rt.width = dst->width;
	rt.height = dst->height;
	cvReleaseImage(&dst);

	dst = GetSubImage(res1,rt);
	///////////////////////////
	/*	IplImage* dst1 = cvCreateImage( cvGetSize(res1), res1->depth, res1->nChannels); //构造目标图象

	if(dst1 == NULL)
	return 1121;
	cvConvertImage(res1,dst1,CV_CVTIMG_SWAP_RB);
	cvReleaseImage(&res1);*/
	//////////////////////////
	ret = InsertLPImg(dst,res1,roi1,x,y,mode);//res ROI 会不会被自动释放？

	return ret;
}
/////////////////////////////
//修正贴图的尺寸，使之能适合屏幕大小
IplImage*   CGifAppPlat::FixImg(IplImage* insert,int x,int y,int mode)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return  NULL;


	float tmp=0,tw,th;
	if(!insert)
		return NULL;
	IplImage* dst = NULL;
	int tWidth = insert->width;//传递过来的视频宽度
	int tHeight = insert->height;
	tw = tWidth;
	th = tHeight;
	tmp = tw/th;
	//	if(mode == 0)//普通方图需要做RESIZE处理
	if((tWidth > nImgWidth/3) || (tHeight > nImgHeight/3))
	{
		//tmp = tHeight/tWidth;
		//	tmp = tWidth / tHeight;

		if(tWidth > nImgWidth/3)
		{
			tWidth = nImgWidth/3;
			tHeight = tWidth/tmp;
		}
		if(tHeight > nImgHeight/3)
		{
			tHeight = nImgHeight/3;
			tWidth = tHeight*tmp;

		}
	}
	bool bresize = false;
	try
	{
		//这里要添加处理，防止图片贴出边界
		if(mode != 4)//弹幕留到移动函数里处理
			FixRect(x, y,tWidth,tHeight);

		if( tWidth != insert->width ||
			tHeight != insert->height)
		{
			dst = cvCreateImage( cvSize(tWidth,tHeight), insert->depth, insert->nChannels); //构造目标图象

			//	IplImage* dst_mask = cvCreateImage( cvSize(tWidth,tHeight), insert->depth, 1); //构造目标图象


			cvResize(insert, dst, CV_INTER_LINEAR);
			bresize = true;
		}
		else
			dst = insert;

		if(bresize)
		{
			cvReleaseImage(&insert);
			//insert = dst;
		}
		//cvReleaseImage(&mask);
	}
	catch(Exception& e)
	{
		string ss = "InsertImg 700 ";
		ss+= e.what();
		SetInfo(ss);

		return NULL;
	}

	return dst;
}
////////////////////////////
int  CGifAppPlat::InsertLPImg(IplImage* insert,IplImage* cover,IplImage* mask,int x,int y,int mode)
{
	/* list<IplImage*>  imgInsert;//for insert img
	list<int>    xList;//x,y,width,height,angle
	list<int>    yList;
	list<int>    widList;
	list<int>    heiList;
	list<int>    angList;*/
	//	if(!image)
	//		return 0;
	//临时测试
	//	if(imgInsert.size() > 0)
	//	return 1;
	////////////
	if(nImgWidth == 0 || nImgHeight == 0)
		return  1;


	int tmp=0;
	if(!insert)
		return 8;


	try
	{
		//这里要添加处理，防止图片贴出边界
		IplImage* dst = FixImg( insert, x, y, mode);
		//	cvResize(mask, dst_mask, CV_INTER_LINEAR);

		if(dst == NULL)
			return 111;
		/*	IplImage* dst1 = cvCreateImage( cvGetSize(dst), dst->depth, dst->nChannels); //构造目标图象

		if(dst1 == NULL)
		return 112;
		cvConvertImage(dst,dst1,CV_CVTIMG_SWAP_RB);
		cvReleaseImage(&dst);*/
		IMGItem* item= new IMGItem;
		if(item == NULL)
			return -133;
		item->imgInsert = (dst);
		item->imgMask = (mask);
		item->imgCover = (cover);
		item->xList = (x);
		item->yList = (y);
		item->widList = (dst->width);
		item->heiList = (dst->height);
		item->typeList = (mode);
		item->angList = (-1);
		item->dirList = (0);
		item->flagList = (0);

		imgInsertList.push_back(item);

		//	if(bresize)
		//    cvReleaseImage(&insert);
		//cvReleaseImage(&mask);
	}
	catch(Exception& e)
	{
		string ss = "InsertImg 700 ";
		ss+= e.what();
		SetInfo(ss);

		return 113;
	}

	return 0;
}

///////////////////////////////////
int  CGifAppPlat::GetBMPCnt()
{
	return pLcv.size();
}
char*  CGifAppPlat::GetBMP(int i)
{
	char  tmp[256];
	strcpy(tmp, pLcv[i].c_str());
	return tmp;
	//	nDelay = pDelay[i];
}
int    CGifAppPlat::GetDly(int num)
{
	return pDelay[num];
}
void  CGifAppPlat::ClearBMP()
{
	try
	{
		for(int i=0;i<pLcv.size();i++)
		{
			string ss = pLcv[i];
			remove(ss.c_str());
		}
		//pLcv.
		//int nn = pLcv.size();
		for(vector<string>::iterator iter=pLcv.begin(); iter!=pLcv.end(); )
		{

			iter = pLcv.erase(iter);


		}
		for(vector<int>::iterator iter2=pDelay.begin(); iter2!=pDelay.end(); )
		{

			iter2 = pDelay.erase(iter2);


		}
	}

	catch(Exception& e)
	{
		string ss = "ClearBMP 807 ";
		ss+= e.what();
		SetInfo(ss);
	}
}
void  CGifAppPlat::DeleteImg(int id)
{
	if(id == -1)
		return;
	if(id >= imgInsertList.size())
		return;
	int i=0;
	try
	{
		//	for(vector<IplImage*>::iterator iter=imgInsert.begin(); iter!=imgInsert.end(); )
		//{

		//	if(i==id)
		//{

		vector<IMGItem*>::iterator iter=imgInsertList.begin() + id; 


		IMGItem* tt = (IMGItem*)*iter;

		cvReleaseImage(&tt->imgInsert);
		cvReleaseImage(&tt->imgCover);
		cvReleaseImage(&tt->imgMask);
		iter = imgInsertList.erase(iter);



	}
	catch(Exception& e)
	{
		string ss = "DeleteBMP 899 ";
		ss+= e.what();
		SetInfo(ss);
	}


}
////////////////////////////

int  CGifAppPlat::RotateUpdateMask(int id)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return -1;
	if(id == -1 || id >= imgInsertList.size())
		return -1;
	//angList[id] = ang;

	IplImage* itmp = NULL;
	IplImage* insert = NULL;

	IplImage* convert2 = NULL;

	insert = imgInsertList[id]->imgInsert;
	if(!insert)
		return -1;
	if(imgInsertList[id]->typeList == 4 || imgInsertList[id]->typeList == 5)
		return -1;


	if(imgInsertList[id]->dirList <= 90 )
	{
		itmp = FitRotate (insert, imgInsertList[id]->dirList,3);

	}
	if(imgInsertList[id]->dirList > 90 && imgInsertList[id]->dirList < 180)
	{
		itmp = FitRotate (insert, 90,3);
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvCopy(itmp,convert2);
		cvReleaseImage(&itmp);
		itmp = FitRotate (convert2, imgInsertList[id]->dirList-90,3);
		cvReleaseImage(&convert2);
	}

	if(imgInsertList[id]->dirList == 180)
	{
		itmp = FitRotate (insert, 180,3);
		cvFlip(itmp,NULL,-1);//-1为先Y轴后X轴，逆时针旋转180度
		/*
		//镜像
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvConvertImage(itmp,convert2,CV_CVTIMG_FLIP);
		itmp = convert2;*/
	}
	if(imgInsertList[id]->dirList > 180 && imgInsertList[id]->dirList <=270 )
	{
		itmp = FitRotate (insert, 90,4);
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvCopy(itmp,convert2);
		cvReleaseImage(&itmp);
		itmp = FitRotate (convert2, imgInsertList[id]->dirList-90,4);
		cvReleaseImage(&convert2);
	}
	if(imgInsertList[id]->dirList > 270 )
	{
		itmp = FitRotate (insert, imgInsertList[id]->dirList,4);
	}

	//cvSaveImage("d:\\img\\cvAnd.png", itmp);
	if(itmp == NULL)
		return -1;
	int wid = itmp->width;
	int hei = itmp->height;
	FixRect(imgInsertList[id]->xList, imgInsertList[id]->yList,wid,hei);
	IplImage* dst = NULL;
	try
	{
		if(wid != itmp->width || hei != itmp->height)
		{
			dst = cvCreateImage( cvSize(wid,hei), itmp->depth, itmp->nChannels); //构造目标图象

			cvResize(itmp, dst, CV_INTER_LINEAR);
			//这里要添加处理，防止图片贴出边界

			/*if(y<0 || x<0)
			{
			Sleep(10);
			}*/
			cvReleaseImage(&itmp);
		}
		else
			dst = itmp;

		//	imgInsert[id] = dst;
		//	xList[id] = x;
		//	yList[id] = y;
		imgInsertList[id]->widList = wid;
		imgInsertList[id]->heiList = hei;
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPTxt  ";
		ss+= e.what();
		SetInfo(ss);

		return 114;
	}

	bPause = true;
	//	cvReleaseImage(&insert);
	/////////////////////////
	//	int x=0,y=0;
	//int id = id;
	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	IplImage* dst1 = NULL;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		if(res1 == NULL || roi1 == NULL)
		{
			bPause = false;
			return 11;
		}

		cvZero(res1);
		cvZero(roi1);


		//EdgeandEllipse(imgInsert[id] ,NULL,res1,roi1,xList[id],yList[id]);
		//	IplImage* dst = NULL;
		//
		//dst = imgInsert[id];


		if(dst ==NULL)
		{
			bPause = false;
			return 11;
		}

		CvRect rect1=cvRect(imgInsertList[id]->xList,imgInsertList[id]->yList,dst->width ,dst->height);

		cvSetImageROI(res1,rect1);

		//cvSaveImage("d:\\img\\cvCircle.png", dst);

		cvAddWeighted(res1,0,dst,1,0,res1);//形成与背景图同尺寸图

		cvResetImageROI(res1);
		//	cvSaveImage("d:\\img\\cvCircle.png", res1);

		//cvSaveImage(sImgBtnPath + ".png", dst);

		cvCvtColor( res1, roi1, CV_BGR2GRAY );
		cvNot( roi1, roi1 );

		testnum = 2793;
		/*	string ss = sImgBtnPath + ".png";
		cvSaveImage(ss.c_str(), res1);
		ss = sImgBtnPath + "2.png";
		cvSaveImage(ss.c_str(), roi1);*/
		/////rotate change
		if(dst != imgInsertList[id]->imgInsert)
		{
			cvReleaseImage(&dst);
			//imgInsert[id] = dst;
		}
		/////////////////////////////
		//	cvSaveImage("d:\\img\\cvCircle1.png", roi1);
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPTxt  ";
		ss+= e.what();
		SetInfo(ss);

		bPause = false;
		return 115;
	}
	try
	{
		IplImage* tt = imgInsertList[id]->imgCover;
		if(tt != NULL)
			cvReleaseImage(&tt);
		tt = imgInsertList[id]->imgMask;
		if(tt != NULL)
			cvReleaseImage(&tt);
		/////rotate change
		/*	tt = imgInsert[id];
		if(tt != NULL)
		cvReleaseImage(&tt);
		imgInsert[id] = dst;*/
		////////////////////
		imgInsertList[id]->imgCover = res1;
		imgInsertList[id]->imgMask = roi1;
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPTxt  ";
		ss+= e.what();
		SetInfo(ss);

		bPause = false;
		return 116;
	}
	bPause = false;
	return 0;
}
int  CGifAppPlat::RotateUpdateEdge(int id)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return -1;
	//if(nCurSelImg == -1)
	//return -1;
	//angList[nCurSelImg] = ang;
	if(id >= imgInsertList.size() || id < 0)
		return -300;
	if(imgInsertList[id]->typeList != 2  && imgInsertList[id]->typeList != 3)
		return RotateUpdateMask(id);//不用EDGE算法

	IplImage* itmp = NULL;
	IplImage* insert = NULL;

	IplImage* convert2 = NULL;

	insert = imgInsertList[id]->imgInsert;
	if(!insert)
		return -1;
	if(imgInsertList[id]->typeList == 4 || imgInsertList[id]->typeList == 5)
		return -1;


	if(imgInsertList[id]->dirList <= 90 )
	{
		itmp = FitRotate (insert, imgInsertList[id]->dirList,3);

	}
	if(imgInsertList[id]->dirList > 90 && imgInsertList[id]->dirList < 180)
	{
		itmp = FitRotate (insert, 90,3);
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvCopy(itmp,convert2);
		cvReleaseImage(&itmp);
		itmp = FitRotate (convert2, imgInsertList[id]->dirList-90,3);
		cvReleaseImage(&convert2);
	}

	if(imgInsertList[id]->dirList == 180)
	{
		itmp = FitRotate (insert, 180,3);
		cvFlip(itmp,NULL,-1);//-1为先Y轴后X轴，逆时针旋转180度
		/*
		//镜像
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvConvertImage(itmp,convert2,CV_CVTIMG_FLIP);
		itmp = convert2;*/
	}
	if(imgInsertList[id]->dirList > 180 && imgInsertList[id]->dirList <=270 )
	{
		itmp = FitRotate (insert, 90,4);
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvCopy(itmp,convert2);
		cvReleaseImage(&itmp);
		itmp = FitRotate (convert2, imgInsertList[id]->dirList-90,4);
		cvReleaseImage(&convert2);
	}
	if(imgInsertList[id]->dirList > 270 )
	{
		itmp = FitRotate (insert, imgInsertList[id]->dirList,4);
	}

	//cvSaveImage("d:\\img\\cvAnd.png", itmp);
	if(itmp == NULL)
		return -1;

	bPause = true;
	//	cvReleaseImage(&insert);
	/////////////////////////
	//	int x=0,y=0;
	int wid = itmp->width;
	int hei = itmp->height;
	FixRect(imgInsertList[id]->xList, imgInsertList[id]->yList,wid,hei);
	IplImage* dst = NULL;
	if(wid != itmp->width || hei != itmp->height)
	{
		dst = cvCreateImage( cvSize(wid,hei), itmp->depth, itmp->nChannels); //构造目标图象

		cvResize(itmp, dst, CV_INTER_LINEAR);
		//这里要添加处理，防止图片贴出边界

		/*if(y<0 || x<0)
		{
		Sleep(10);
		}*/
		cvReleaseImage(&itmp);
	}
	else
		dst = itmp;

	//	imgInsert[id] = dst;
	//	xList[id] = x;
	//	yList[id] = y;
	imgInsertList[id]->widList = wid;
	imgInsertList[id]->heiList = hei;


	/////////////////////
	//imgInsert[nCurSelImg] = itmp;
	//xList[nCurSelImg] = x;
	//yList[nCurSelImg] = y;
	//	widList[nCurSelImg] = itmp->width;
	//heiList[nCurSelImg] = itmp->height;
	//angList[nCurSelImg] = ang;
	//  if(typeList[nCurSelImg] == 0)
	//   typeList[nCurSelImg] = 3;
	//Rect rt;
	//UpdateSPImg(nCurSelImg,3,rt);


	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	//	IplImage* dst1 = NULL;
	//	int id = nCurSelImg;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		cvZero(res1);
		cvZero(roi1);
		EdgeandEllipse(dst ,NULL,res1,roi1,imgInsertList[id]->xList,imgInsertList[id]->yList);




		/*	cvSaveImage("d:\\img\\cvCircle000.png", imgInsert[id]);
		cvSaveImage("d:\\img\\cvCircle111.png", res1);
		cvSaveImage("d:\\img\\cvCircle222.png", roi1);
		cvSaveImage("d:\\img\\cvCircle333.png", imgCover[id]);
		cvSaveImage("d:\\img\\cvCircle444.png", imgMask[id]);*/
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPImg  ";
		ss+= e.what();
		SetInfo(ss);

		bPause = false;
		return 117;
	}
	try
	{
		IplImage* tt = imgInsertList[id]->imgCover;
		if(tt != NULL)
			cvReleaseImage(&tt);
		tt = imgInsertList[id]->imgMask;
		if(tt != NULL)
			cvReleaseImage(&tt);
		/////rotate change
		/*	tt = imgInsert[id];
		if(tt != NULL)
		cvReleaseImage(&tt);
		imgInsert[id] = dst;*/
		////////////////////
		imgInsertList[id]->imgCover = res1;
		imgInsertList[id]->imgMask = roi1;

		//////////////////////////////


		//////////////////////////////

		cvReleaseImage(&dst);
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPImg  ";
		ss+= e.what();
		SetInfo(ss);
		bPause = false;
		return 118;
	}
	bPause = false;
	return 0;
}
int  CGifAppPlat::Rotate(int ang)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return -1;
	if(nCurSelImg == -1)
		return -1;
	//angList[nCurSelImg] = ang;

	IplImage* itmp = NULL;
	IplImage* insert = NULL;

	IplImage* convert2 = NULL;

	insert = imgInsertList[nCurSelImg]->imgInsert;
	if(!insert)
		return -1;
	if(imgInsertList[nCurSelImg]->typeList == 4 || imgInsertList[nCurSelImg]->typeList == 5)
		return -1;
	/////rotate change
	imgInsertList[nCurSelImg]->dirList += ang;
	imgInsertList[nCurSelImg]->dirList  = imgInsertList[nCurSelImg]->dirList%360;
	if(imgInsertList[nCurSelImg]->typeList == 0)
		imgInsertList[nCurSelImg]->typeList = 3;
	imgInsertList[nCurSelImg]->flagList = 1;
	/////rotate change
	if(ang != 0)
		return 0;
	//	return RotateUpdateEdge(nCurSelImg);
	////////////////////////////////////
	//flagList[nCurSelImg] = 1;
	// return RotateUpdateEdge(nCurSelImg);//ang为零，表示这是移动图片，而不是旋转图片，本函数在updatespimg里调用；不为零，则表示手动操作的旋转，不在updatespimg里执行。
	//以上是基于updatespimg只在perffunc里执行做出的处理


	if(imgInsertList[nCurSelImg]->dirList <= 90 )
	{
		itmp = FitRotate (insert, imgInsertList[nCurSelImg]->dirList,3);

	}
	if(imgInsertList[nCurSelImg]->dirList > 90 && imgInsertList[nCurSelImg]->dirList < 180)
	{
		itmp = FitRotate (insert, 90,3);
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvCopy(itmp,convert2);
		cvReleaseImage(&itmp);
		itmp = FitRotate (convert2, imgInsertList[nCurSelImg]->dirList-90,3);
		cvReleaseImage(&convert2);
	}

	if(imgInsertList[nCurSelImg]->dirList == 180)
	{
		itmp = FitRotate (insert, 180,3);
		cvFlip(itmp,NULL,-1);//-1为先Y轴后X轴，逆时针旋转180度
		/*
		//镜像
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvConvertImage(itmp,convert2,CV_CVTIMG_FLIP);
		itmp = convert2;*/
	}
	if(imgInsertList[nCurSelImg]->dirList > 180 && imgInsertList[nCurSelImg]->dirList <=270 )
	{
		itmp = FitRotate (insert, 90,4);
		convert2=cvCreateImage( cvGetSize( itmp ), IPL_DEPTH_8U, 3);
		cvCopy(itmp,convert2);
		cvReleaseImage(&itmp);
		itmp = FitRotate (convert2, imgInsertList[nCurSelImg]->dirList-90,4);
		cvReleaseImage(&convert2);
	}
	if(imgInsertList[nCurSelImg]->dirList > 270 )
	{
		itmp = FitRotate (insert, imgInsertList[nCurSelImg]->dirList,4);
	}

	//cvSaveImage("d:\\img\\cvAnd.png", itmp);
	if(itmp == NULL)
		return -1;

	bPause = true;
	//	cvReleaseImage(&insert);
	/////////////////////////
	//	int x=0,y=0;
	int wid = itmp->width;
	int hei = itmp->height;
	FixRect(imgInsertList[nCurSelImg]->xList, imgInsertList[nCurSelImg]->yList,wid,hei);
	IplImage* dst = NULL;
	if(wid != itmp->width || hei != itmp->height)
	{
		dst = cvCreateImage( cvSize(wid,hei), itmp->depth, itmp->nChannels); //构造目标图象

		cvResize(itmp, dst, CV_INTER_LINEAR);
		//这里要添加处理，防止图片贴出边界

		/*if(y<0 || x<0)
		{
		Sleep(10);
		}*/
		cvReleaseImage(&itmp);
	}
	else
		dst = itmp;

	//	imgInsert[nCurSelImg] = dst;
	//	xList[nCurSelImg] = x;
	//	yList[nCurSelImg] = y;
	imgInsertList[nCurSelImg]->widList = wid;
	imgInsertList[nCurSelImg]->heiList = hei;


	/////////////////////
	//imgInsert[nCurSelImg] = itmp;
	//xList[nCurSelImg] = x;
	//yList[nCurSelImg] = y;
	//	widList[nCurSelImg] = itmp->width;
	//heiList[nCurSelImg] = itmp->height;
	//angList[nCurSelImg] = ang;
	//  if(typeList[nCurSelImg] == 0)
	//   typeList[nCurSelImg] = 3;
	//Rect rt;
	//UpdateSPImg(nCurSelImg,3,rt);


	IplImage* res1 = NULL;
	IplImage* roi1 = NULL;
	int id = nCurSelImg;
	try
	{

		//	IplImage* img1 = cvLoadImage("d:\\img\\girl.jpg");
		res1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 3);
		roi1 = cvCreateImage(cvSize(nImgWidth,nImgHeight), 8, 1);
		cvZero(res1);
		cvZero(roi1);
		EdgeandEllipse(dst ,NULL,res1,roi1,imgInsertList[id]->xList,imgInsertList[id]->yList);


		/*	cvSaveImage("d:\\img\\cvCircle000.png", imgInsert[id]);
		cvSaveImage("d:\\img\\cvCircle111.png", res1);
		cvSaveImage("d:\\img\\cvCircle222.png", roi1);
		cvSaveImage("d:\\img\\cvCircle333.png", imgCover[id]);
		cvSaveImage("d:\\img\\cvCircle444.png", imgMask[id]);*/
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPImg  ";
		ss+= e.what();
		SetInfo(ss);
		bPause = false;
		return 120;
	}
	try
	{
		IplImage* tt = imgInsertList[id]->imgCover;
		if(tt != NULL)
			cvReleaseImage(&tt);
		tt = imgInsertList[id]->imgMask;
		if(tt != NULL)
			cvReleaseImage(&tt);
		imgInsertList[id]->imgCover = res1;
		imgInsertList[id]->imgMask = roi1;

		cvReleaseImage(&dst);
	}
	catch(Exception& e)
	{
		string ss = "UpdateSPImg  ";
		ss+= e.what();
		SetInfo(ss);
		bPause = false;
		return 121;
	}
	bPause = false;
	return 0;
}
void CGifAppPlat::ReSize(int type/*1 放大  -1  缩小*/,float range)
{
	//该函数响应的是多点触控，range表示放大或者缩小的幅度，为一个比值，被选中的图片按照这个比值缩放。
	//默认为0.02，根据操作选择合适的数值
	if(nImgWidth == 0 || nImgHeight == 0)
		return ;
	if(nCurSelImg == -1)
		return;
	float zoom = 1;

	//string ss;
	//ss = "resize start";
	//	InputLoginfo(ss);
	zoom += type*range;


	IplImage* insert = NULL;


	int tmp=0;
	insert = imgInsertList[nCurSelImg]->imgInsert;
	if(!insert)
		return;
	if(imgInsertList[nCurSelImg]->typeList == 4 || imgInsertList[nCurSelImg]->typeList == 5)
		return ;
	int tWidth = insert->width;//传递过来的视频宽度
	int tHeight = insert->height;
	int x = imgInsertList[nCurSelImg]->xList;
	int y = imgInsertList[nCurSelImg]->yList;
	//	ss = "resize ";
	//	InputLoginfo(ss);
	try
	{
		bPause = true;
		tWidth *= zoom;
		tHeight *= zoom;
		x -= (tWidth - insert->width)/2;
		y -= (tHeight - insert->height)/2;
		FixRect(x, y,tWidth,tHeight);
		//ss = "FixRect";
		//InputLoginfo(ss);
		IplImage* dst = cvCreateImage( cvSize(tWidth,tHeight), insert->depth, insert->nChannels); //构造目标图象

		cvResize(insert, dst, CV_INTER_LINEAR);

		//	ss = "cvResize";
		//InputLoginfo(ss);

		if(dst == NULL)
		{
			//	ss = "NULL";
			//InputLoginfo(ss);
			bPause = false;
			return ;
		}
		//这里要添加处理，防止图片贴出边界

		/*if(y<0 || x<0)
		{
		Sleep(10);
		}*/

		//cvSaveImage("d:\\img\\cvCircleaaa.png", insert);
		cvReleaseImage(&insert);

		imgInsertList[nCurSelImg]->imgInsert = dst;
		imgInsertList[nCurSelImg]->xList = x;
		imgInsertList[nCurSelImg]->yList = y;
		imgInsertList[nCurSelImg]->widList = tWidth;
		imgInsertList[nCurSelImg]->heiList = tHeight;
		//angList.push_back(0);

		//	ss = "cvResize  end";
		//InputLoginfo(ss);
		///////////////////
		Rect rt;
		if(imgInsertList[nCurSelImg]->typeList != 0)
		{
			imgInsertList[nCurSelImg]->flagList = 1;
			//	UpdateSPImg(nCurSelImg,typeList[nCurSelImg],rt);
		}
		//////////////////
		bPause = false;

	}
	catch(Exception& e)
	{
		string ss = "InsertImg 700 ";
		ss+= e.what();
		//InputLoginfo(ss);
		bPause = false;
		return;
	}
}
int   CGifAppPlat::SelectImg(int x,int y)
{
	if(nImgWidth == 0 || nImgHeight == 0)
		return  -1;
	int ret = FindtheImg(x,y);
	int xx,yy,wid,hh,ang,cnt;
	IplImage*  tmp;
	if(ret == -1)
	{
		if(imgBtn)
			cvReleaseImage(&imgBtn);
		imgBtn = NULL;
		nCurSelImg = -1;
		return -1;
	}
	/*try
	{
	cnt = imgInsert.size() - 1;
	//  if((ret != -1 )&& (ret != 0))
	if((ret != -1 ))
	{
	tmp =  imgInsert[ret];//for insert img
	xx =    xList[ret];//x,y,width,height,angle
	yy =    yList[ret];
	wid =    widList[ret];
	hh =    heiList[ret];
	ang =    angList[ret];
	imgInsert[ret] = imgInsert[cnt];//for insert img
	xList[ret] = xList[cnt];//x,y,width,height,angle
	yList[ret] = yList[cnt];
	widList[ret] = widList[cnt];
	heiList[ret] = heiList[cnt];
	angList[ret] = angList[cnt];
	imgInsert[cnt] = tmp;//for insert img
	xList[cnt] = xx;//x,y,width,height,angle
	yList[cnt] = yy;
	widList[cnt] = wid;
	heiList[cnt] = hh;
	angList[cnt] = ang;
	}
	}
	catch(cv::Exception& e)
	{
	string ss = "SelectImg cv111";
	ss+= e.err;
	SetInfo(ss);
	}*/
	nCurSelImg = ret;

	/*	ReSize(1);
	ReSize(1);
	ReSize(-1);
	ReSize(-1);*/
	//return ret;//为了测试
	//暂时不用imgbtn不自找麻烦
	/*	try
	{
	nCurSelImg = ret;
	IplImage* src = cvLoadImage(sImgBtnPath.c_str());
	imgBtn = cvCreateImage( cvSize(20,20), src->depth, src->nChannels); //构造目标图象

	cvResize(src, imgBtn, CV_INTER_LINEAR);


	//imgInsert.push_back(insert);
	xBtn = xList[ret]-imgBtn->width + widList[ret];
	yBtn = yList[ret]-imgBtn->height + heiList[ret];
	widBtn = imgBtn->width;
	heiBtn = imgBtn->height;
	//   if
	}
	catch(cv::Exception& e)
	{
	string ss = "SelectImg cv";
	ss+= e.err;
	SetInfo(ss);
	}*/

	return ret;

	//	RefreshImage();
}
//刷新视频画面
void CGifAppPlat::RefreshImage()
{
	//Mat src(image);

	//	PerfMotionDetect(src);
}
//判断触点是压在那个贴图上
int   CGifAppPlat::FindtheImg(int x,int y)
{
	int ret = -1;
	int xx,yy,wid,hh,ang;
	IplImage*  tmp;
	for(int i=0; i<imgInsertList.size(); i++ )
	{
		if(imgInsertList[i]->typeList == 4 || imgInsertList[i]->typeList == 7)
			continue;

		int xx = imgInsertList[i]->xList;

		int yy = imgInsertList[i]->yList;
		int wid = imgInsertList[i]->widList;
		int hh = imgInsertList[i]->heiList;

		if((xx <= x) &&
			(yy <= y) &&
			(xx + wid >= x) &&
			(yy + hh >= y))
			ret = i;

	}



	return ret;
}
////////////////////////////
void HuaiJiu(Mat& src);
void ColorMap(Mat& src,int nColor);
void LianHuanHua(Mat& src);
void SuMiao(Mat& src);
void Menglong(Mat& src,float mSize);
void Wind(Mat& src,int num2,int num1);
void Xuanwo(Mat& src,double Para);
void casting(Mat& src);
void freezing(Mat& src);
void DiaokeFuDiao(Mat& src,int nType/*0  浮雕   1 雕刻*/);
void HAHAJing(Mat& src,int nType/*0  放大   1 挤压*/);
void HiLightRGB(Mat& src);  //分通道强光
void JingxiangMohu(Mat& src,int num);
void XuanZhuan(Mat& src,int num);
void Maoboli(Mat& src);
void ManHua(Mat& src1);
int BoLang(Mat& src1,double angle,int deltaI,int A);
int  CGifAppPlat::SwitchMode(Mat& src,int mode)
{
	if(mode == enum_Empty)
	{
		InitMode();
		ChangeMode(1);
		return 0;
	}
	switch(mode)
	{
	case enum_HuaiJiu_BW:
		HuaiJiu(src);
		break;
		//int BoLang(IplImage *src,double angle,int deltaI,int A);
	case enum_ColorMap:
		if(nCurColor != 12)
			ColorMap(src,nCurColor);
		break;
		/*case enum_LianHuanHua:
		LianHuanHua(src);
		break;*/
	case enum_SuMiao:
		SuMiao(src);
		break;
	case enum_Menglong:
		Menglong(src,menglong_Size);
		break;
	case enum_Wind:
		Wind(src,wind_num2,wind_num1);
		break;
		/*	case enum_Xuanwo:
		Xuanwo(src,xuanwo_Para);
		break;
		case enum_casting:
		casting(src);
		break;
		case enum_freezing:
		freezing(src);
		break;*/
	case enum_Diaoke:
		DiaokeFuDiao(src,1);
		break;
	case enum_HAHAJing_FangDa:
		HAHAJing(src,0);
		break;
		/*	case enum_HiLightRGB:
		HiLightRGB(src);
		break;*/
		//void HuaiJiu(IplImage *source);
		/*	case enum_JingxiangMohu:
		JingxiangMohu(src,jingxiangmohu_num);
		break;*/
		/*	case enum_XuanZhuan:
		XuanZhuan(src,xuanzhuan_num);
		break;*/
	case enum_Maoboli:
		Maoboli(src);
		break;
		/*	case enum_ManHua:
		ManHua(src);
		break;*/
		/*case enum_BoLang:
		//   IplImage  bb = src;
		BoLang(src,bolang_angle,bolang_deltaI,bolang_A);
		break;*/

		/*case enum_FuDiao:
		DiaokeFuDiao(src,0);
		break;*/
	case enum_HAHAJing_JiYa:
		HAHAJing(src,1);
		break;
	case enum_SELECT_RECT:
		break;
	default:
		break;
	}
	return 1;
}
/////////////////////////////////////
void  CGifAppPlat::SetOperMode(int mode)
{
	if((nCurOperMode != enum_DRAW_WRITE) && mode == enum_DRAW_WRITE)
	{
		drawing_text = true;
	}
	if((nCurOperMode == enum_DRAW_WRITE) && mode != enum_DRAW_WRITE)
	{
		drawing_text = false;
		drawing_text_id = -1;
	}
	nCurOperMode = mode;
}
void  CGifAppPlat::SetMode(int mode)
{

	nCurMode = mode;
}
int  CGifAppPlat::GetListSize()
{
	return imgInsertList.size();
}
int   CGifAppPlat::GetInfo(int num,int type)
{
	if(type == 0)
		return imgInsertList[num]->xList;
	if(type == 1)
		return imgInsertList[num]->yList;
	if(type == 2)
		return imgInsertList[num]->widList;
	if(type == 3)
		return imgInsertList[num]->heiList;
	if(type == 4)
		return imgInsertList.size();
	if(type == 5)
		return testnum;
	return 0;
}
///////////////////////////////

void  CGifAppPlat::SetSize(int size)
{m_size = size;}

//向JAVA层返回信息
//  void SetProgress(int num);
// void SetInfo(string ss);



/*
jclass cGifState;
jobject mGifState;
jmethodID mSetProgress;
jmethodID mSetInfo;*/



/**
* 初始化 类、对象、方法
*/
int CGifAppPlat::InitProvider()
{

	// 	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "InitProvider Begin  1" );

	/*  	if(jniEnv == NULL) {
	return 0;
	}

	if(cGifState == NULL) {
	cGifState = jniEnv->FindClass("com/example/pcvimg");
	if(cGifState == NULL){
	return -1;
	}
	//	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "InitProvider Begin  2 ok" );
	}

	if (mGifState == NULL) {
	if (GetProviderInstance(cGifState) != 1) {
	jniEnv->DeleteLocalRef(cGifState);
	return -1;
	}
	//	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "InitProvider Begin  3 ok" );
	}

	if (mSetProgress == NULL) {
	mSetProgress = jniEnv->GetMethodID(cGifState, "SetProgress","()Ljava/lang/String;");
	if (mSetProgress == NULL) {
	jniEnv->DeleteLocalRef(cGifState);
	jniEnv->DeleteLocalRef( mGifState);
	return -2;
	}
	//	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "InitProvider Begin  4 ok" );
	}

	if (mSetInfo == NULL) {
	mSetInfo = jniEnv->GetMethodID(cGifState, "SetInfo","(Ljava/lang/String;)V");
	if (mSetInfo == NULL) {
	jniEnv->DeleteLocalRef( cGifState);
	jniEnv->DeleteLocalRef( mGifState);
	//		jniEnv->DeleteLocalRef( mSetProgress);
	return -3;
	}
	//__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "InitProvider Begin  5 ok" );
	}

	//	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "InitProvider Begin  6" );*/
	return 1;

}

/*  int CGifAppPlat::GetProviderInstance(jclass obj_class) {

if(obj_class == NULL) {
return 0;
}

jmethodID construction_id = jniEnv->GetMethodID( obj_class,
"<init>", "()V");

if (construction_id == 0) {
return -1;
}

mGifState = jniEnv->NewObject( obj_class,
construction_id);

if (mGifState == NULL) {
return -2;
}

return 1;
}

/**
* 获取时间 ---- 调用 Java 方法
*/
void CGifAppPlat::SetProgress(int num)
{
	/*  	if(cGifState == NULL || mSetProgress == NULL) {
	int result = InitProvider();
	if (result != 1) {
	return;
	}
	}

	//	jstring jstr = NULL;
	//	char* cstr = NULL;
	// 	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "GetTime Begin" );
	jint ji = 1;
	jniEnv->CallVoidMethod( mGifState, mSetProgress,ji);
	// 	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SayHello End" );

	//  	jniEnv->DeleteLocalRef( ji);*/
}

/**
* SayHello ---- 调用 Java 方法
*/
void CGifAppPlat::SetInfo(string ss)
{
	/*	if(cGifState == NULL || mGifState == NULL || mSetInfo == NULL) {
	int result = InitProvider() ;
	if(result != 1) {
	return;
	}
	}

	jstring jstrMSG = NULL;
	jstrMSG =jniEnv->NewStringUTF( ss.c_str());
	// 	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SayHello Begin" );
	jniEnv->CallVoidMethod( mGifState, mSetInfo,jstrMSG);
	// 	__android_log_print(ANDROID_LOG_INFO, "JNIMsg", "SayHello End" );

	jniEnv->DeleteLocalRef(jstrMSG);*/
}

///////////////////////////////////////
int CGifAppPlat::Init_Koutou(Mat src)
{
//	src.copyTo(img_koutu);

	//将模板设置成白色
	if(img_koutu.data)
		img_koutu.release();
	if(img_mask_koutu.data)
		img_mask_koutu.release();
	if(dst_koutu.data)
			dst_koutu.release();
	img_koutu.create(src.rows, src.cols, CV_8U);
	src.copyTo(img_koutu);
	img_mask_koutu.create(img_koutu.rows, img_koutu.cols, CV_8U); 
//	img_mask.setTo(Scalar(255));
	img_mask_koutu.setTo(Scalar(0));

	dst_koutu.create(img_koutu.rows, img_koutu.cols, CV_8U);
	//显示原图
//	imshow("image",img);
	
	rt_kt_koutu.x= img_koutu.rows;
	rt_kt_koutu.y= img_koutu.cols;
	rt_kt_koutu.width= 0;
	rt_kt_koutu.height= 0;
	nKoutuStatus = CV_EVENT_LBUTTONUP;//抠图时鼠标的状态，避免不当操作影响抠图效果

	return 0;
}
void  CGifAppPlat::on_mouse_koutu(int event, int x, int y )
{
	int eve = -1;
	//将JAVA事件转换为c事件
	if(event == 0)
		eve = CV_EVENT_LBUTTONDOWN;
	if(event == 1)
		eve = CV_EVENT_LBUTTONUP;
	if(event == 2)
		eve = CV_EVENT_MOUSEMOVE;
	return DrawingKoutu(eve,x,y);
}
void  CGifAppPlat::DrawingKoutu(int event, int x, int y )
{
	if(!img_koutu.data)
		return;
	if ( event == CV_EVENT_LBUTTONUP)  //判断事件为松开鼠标左键或者不是左拖拽
	{
		if(nKoutuStatus == CV_EVENT_MOUSEMOVE)
			nKoutuStatus = CV_EVENT_LBUTTONUP;
		else if(nKoutuStatus == CV_EVENT_LBUTTONUP)
			return;
//		prev_pt_koutu = cvPoint(-1, -1);
				floodFill(img_mask_koutu,Point(x,y),Scalar(255));//填充抠图模板
	//	imshow("img_mask", img_mask);
		//waitKey(0);
		img_koutu.copyTo(dst_koutu,img_mask_koutu);
	//	imshow("image-mask", dst);
	//	IplImage ima = dst;
		rt_kt_koutu.width -= rt_kt_koutu.x;

		
			rt_kt_koutu.height-=rt_kt_koutu.y;

			if(rt_kt_koutu.x > 3)
				rt_kt_koutu.x -= 3;
			if(rt_kt_koutu.y > 3)
				rt_kt_koutu.y -= 3;
			if(rt_kt_koutu.width < img_koutu.cols -3)
				rt_kt_koutu.width += 3;
            if(rt_kt_koutu.height <= img_koutu.rows -3)
				rt_kt_koutu.height +=3;
		
/*			rt_kt.width -= rt_kt.x;

		
			rt_kt.height-=rt_kt.y;*/
	}
	else if (event == CV_EVENT_LBUTTONDOWN)  //判断为按下左键
	{
		if(nKoutuStatus == CV_EVENT_LBUTTONUP)
		{
		       prev_pt_koutu = cvPoint(x,y);
			   nKoutuStatus = CV_EVENT_LBUTTONDOWN;//抠图时鼠标的状态，避免不当操作影响抠图效果

		}
		//rt_kt.x = x;
	//	rt_kt.y = y;
	}
	else if ( event == CV_EVENT_MOUSEMOVE)  //判断移动鼠标并且左拖拽
	{
		////
		if(nKoutuStatus == CV_EVENT_LBUTTONDOWN)
			nKoutuStatus = CV_EVENT_MOUSEMOVE;
		else if(nKoutuStatus == CV_EVENT_LBUTTONUP)
			return;
		if(y - prev_pt_koutu.y > 1000)
			y = prev_pt_koutu.y;//防止越界突然出现不合理变化
		if(y>img_koutu.rows )
			y = prev_pt_koutu.y;
		if(x - prev_pt_koutu.x > 1000)
			x = prev_pt_koutu.x;//防止越界突然出现不合理变化
		if(x>img_koutu.cols )
			x = prev_pt_koutu.x;
		CvPoint pt = cvPoint(x, y);
		if ( prev_pt_koutu.x < 0)
		{
			prev_pt_koutu = pt;
		}
		
		if(rt_kt_koutu.x > x)
			rt_kt_koutu.x=x;
	    if(rt_kt_koutu.y>y)
			rt_kt_koutu.y=y;
	    if(rt_kt_koutu.width<x)
			rt_kt_koutu.width = x;

		if(rt_kt_koutu.height<y)
			rt_kt_koutu.height=y;
		//img.cols;

		//CString cs;
		//cs.Format("height = %d,y = %d",rt_kt.height,y);
		//dlg->SetText(cs);
		//line(img_mask, prev_pt, pt, Scalar(0),2,8,0);
		line(img_mask_koutu, prev_pt_koutu, pt, Scalar(0),5,8,0); //模板上划线
		line(img_koutu, prev_pt_koutu, pt, Scalar::all(0),5,8,0);          //原图上划线
		prev_pt_koutu = pt;
	//	imshow("image", img);
	//	imshow("image-mask", img_mask);





	}
	if (event == CV_EVENT_RBUTTONUP)
	{

	//	floodFill(img_mask,Point(x,y),Scalar(0));//填充抠图模板

		
	
	
	//	img_kt = cvGetSubImage(&ima,rt_kt);
	//	cvNamedWindow("res1", 1);
	//cvShowImage("lili", src);
//	cvShowImage("res1", img_kt);
//	cvWaitKey(0);
	//	imshow("dst", dst);



	}
//	imshow("res1koutu", img_koutu);

}
