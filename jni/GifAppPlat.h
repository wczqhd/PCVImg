#ifndef _GIFAPPPLAT_H
 #define _GIFAPPPLAT_H

//#define  SKIP_INCLUDES  1

//
//#include <jni.h>
#include <opencv/cv.h>
 #include <opencv/highgui.h>
 #include <opencv/cvaux.h>
#include <opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
//#include "MotionDetect.h"

#include <iostream>
#include <string>
#include <fstream>
#include "gif/gifwidget.h"
#include "gif/palettewidget.h"


//#include "CxImage/ximagif.h"
//#include "CxImage/ximabmp.h"

using namespace cv;
using namespace std;

typedef vector<IplImage*> BigShowIMGList;
typedef struct  IMGItem{
	IplImage*  imgInsert;//for insert img
  IplImage*  imgCover;//for insert img
   IplImage*  imgMask;//for insert img mask仅针对非方图形使用
  int    xList;//x,y,width,height,angle
  int    yList;
  int    widList;
  int    heiList;
  int    angList;//如果该贴图是弹幕，则指定弹幕移动的方向，默认为-1；0 -> 1 <- 2 /  3 \  4  5(23为下往上，45对应为上往下)
  int    typeList;//指示图片类型，/*0 普通方图  1 矩形截图  2 不规则抠图 3 方图旋转  4 文字  5 指尖花*/
  int    dirList ;//图形旋转的方向，为逆时针，360度范围内的角度
  int    flagList ;//图形旋转的方向，为逆时针，360度范围内的角度,默认为0，如果为1则表示改变了图
  int    bigshowidList;//大招儿对应的imgBigShow id*/
   int    bigshowListInId;//大招儿对应的imgBigShow 中的vector中的ID*/
   int    bigshowEvery;//大招儿每隔多少帧更新一次*/
}IMGItem;
enum
     {
	enum_Empty = 0,

	//int BoLang(IplImage *src,double angle,int deltaI,int A);
	enum_ColorMap = 1,

	enum_SuMiao = 2,
	enum_Menglong = 3,
	enum_HuaiJiu_BW = 4,
	
	enum_Wind = 5,
	enum_Maoboli = 6,
//	enum_Xuanwo = 5,
	//enum_casting =6,
//	enum_freezing = 7,
	enum_Diaoke = 7,
	enum_HAHAJing_FangDa = 8,
	enum_HAHAJing_JiYa = 9

//	enum_XuanZhuan = 10,
	
//	enum_ManHua = 9,

	//enum_HuaiJiu = 10,

	//enum_FuDiao = 14,
	
};
enum//opermode
{
	enum_OPER_EMPTY= 21,
	enum_SELECT_RECT= 22,
	enum_DRAW_WITHHAND = 23,
	enum_DRAW_WRITE = 24,
	enum_DRAW_BIGSHOW = 25

     };
#define DIRECTION_NUM  6 //弹幕方向
#define IMAGE_NUM      6 //贴图种类
#define IMAGE_SPEED      40 //弹幕移动速度

/////////////////////////////////
#define MENGLONG_SIZE  0.5
#define 	WIND_NUM2  10
#define 	WIND_NUM1  20
#define 	XUANWO_PARA 20
#define 	JINGXIANGMOHU_NUM  40
#define 	XUANZHUAN_NUM 20

#define 	BOLANG_ANGLE  0
#define 	BOLANG_DELTAI  10  //波浪周期;
#define 	BOLANG_A  10      //波浪振幅;
#define     HAND_IMG_SEP   500  //手指移动动画的画画间隔
#define     STATUS_LIVE    -1
///////////////////////////////////
class CGifAppPlat
 {
 public:
	CGifAppPlat();
  ~CGifAppPlat();

public:
////////////////////
 // void on_event( int event, int x, int y, int flags, void* param);
  int PerfMotionDetect(Mat& src,Mat& srcF,int num,float nScale,int nPause/*1 pause;0 no pause*/);//num 为-1，表示为录制实时动画；否则即为编辑现有动画状态。
  int PerfLive(IplImage* image,int num);//num等于STATUS_EDIT，则为编辑状态
  int PerfImageShow(IplImage* image);
  int PerfEdit(Mat& src,int num,int nPause/*1 pause;0 no pause*/);
   int PerfCapVideo(Mat& src);
  int  DrawingByHand(int eve ,int x,int y);
  int  DrawingByHandNoImg(int eve ,int x,int y);
  int  DrawingByHandNoImgWhole(int eve ,int x,int y);
  void  ChangeMode(int delta);
  void  InitMode();
  bool  TextMovingByItSelf(int id);
  void  GetRectFromMat(Mat& src,CvRect roi);

  void RefreshImage();//刷新视频画面
  int InsertKouTu(Mat& src,CvRect roi);
  int  InsertImg(char* imgpath,int x,int y,int mode);
  int  InsertLPImg(IplImage* insert,IplImage* cover,IplImage* mask,int x,int y,int mode);//贴图可能来自资源，或者截图，抠图
  int   InsertSPImg(IplImage* insert,int x,int y,int mode);//处理非方图的插入
 IplImage*  FixImg(IplImage* insert,int x,int y,int mode);
  int   InsertSPTxt(IplImage* insert,int x,int y,int mode);//处理字符的插入
  int   UpdateSPTxt(int id,int type,CvRect roi);
  int   UpdateSPImg(int id,int type,CvRect roi);//修改非方图的信息.调用之前要完成坐标，大小，原图的修改，该函数只负责修改cover和mask，type
  //int  InsertStrangeImg(IplImage* insert,int x,int y);
  int  InsertMat(Mat& src,Rect rt,int x,int y,int mode/*0 普通方图  1 矩形截图  2 不规则抠图 3 方图旋转  4 文字弹幕 5 特效图  6 非弹幕文字和装饰花（心，云,水滴等随着鼠标移动绘制的图） 7 手写  8 大招儿（bigshow）*/);
  IplImage*  GetSubImage(IplImage *image, CvRect roi);
  void OnPlatTouch(int vevent,int x, int y);//处理触屏事件
  int  InsertTxt(char* imgpath,int x,int y,int mode);
  int  InsertDrawTxt();
  void  InsertHandImg(int x,int y,int mode);
  void  Start(char* path);
  void  Stop();
  void  Pause();
  void  GetPages(char* filename);//获取GIF文件的帧S

  int  CreatingGIF();
  int on_event( int event, int x, int y);
  int  BuildingGifEntrance(char* path);//参数是路径，带斜杠
  void  ReleaseImg();
  void ReleaseImglist();
  void ReleaseBigShowImg();
  int   GetBMPCnt();
  char*  GetBMP(int num);
  int    GetDly(int num);
  void  ClearBMP();//清空BMP路径和文件
  int   FindtheImg(int x,int y);//判断触点是压在那个贴图上

  int  SwitchMode(Mat& src,int mode);
  void  SetMode(int mode);//{nCurMode = mode;}
  void  SetOperMode(int mode);//{nCurMode = mode;}
  void  SetColor(CvScalar scr){m_scr = scr;}
  void  Setthickness(int thickness){m_thickness=thickness;}
  void  SetHandImgPath(char* path){sImgHandImg = path;}
  int  SetHandImgMat(Mat& src);
  int  InputBigShowMat(Mat& src,int id);
  int  InputBigShowByPath(char* path,int id);
  int  InsertBigShowMat(int x,int y,int id,int nInsertListId);
  void  SetSaveType(int type){saveType = type;}
  int  RotateUpdateEdge(int id);
   int  RotateUpdateMask(int id);
  void  SetColor(int col){nCurColor = col;}
  void  SetImgBtnPath(string ss){sImgBtnPath=ss;}
  bool  IsGettingGif(){return bGetGif;}
  int   GetImgListSize(){return imglist.size();}
  int   GetImgbyID(int id,Mat& src);

  void  SetGettingGif(bool bb){ bGetGif=bb;}
  void  SetEvery(int ff){nEvery = ff;}
  void  SetEveryBigShow(int n){nEveryBigShow = n;}
  void  SetDelay(int ndly){nDelay = ndly;}
  void  SetSize(int size);//{m_size = size;}//1 原尺寸  2  二分之一  3 三分之一  4  四分之一
  int  SelectImg(int x,int y);
  void FixRect(int &x,int &y,int &wid,int &het);
  void  DeleteImg(int id);//删除指定的图片
  int   InitVideoFile(char* path);
  int   ConfirmKouTu(Mat& src,CvRect roi);
  void  SetTestFilePath(char* path){m_testfilepath = path;}
  void  InputLoginfo(string ss);
  void  SetBigShowId(int id){nCurBigshowId = id;}
  IplImage*  CreateTestImg();
  //////////
  void PrePareTrack(int &x,int &y);
  void draw_box( IplImage* img, CvRect rect ,CvScalar scr);
  ////////
  void ReSize(int type/*1 放大  -1  缩小*/,float range = 0.02);
  int  Rotate(int ang);

  int   GetInfo(int num,int type);
  int  GetListSize();
  int  GetImgListWidth();//{return nImgWidth;}
  int  GetImgListHeight();//{return nImgHeight;}
  void ResumeList();
  ////////////////////////////////////
  ///////////////////
  CvRect box;
 // int nMovingImgId;
bool drawing_box;
bool drawing_mode;
bool drawing_text;
int  nEvery;
int  nEveryBigShow;
int nDelay;
bool bModifing;
CvScalar m_scr;
int m_thickness;
int  drawing_text_id;
int  pre_bigshow_id;
CvPoint prev_hand_pt;// = {-1, -1};//手指画画的前一个位置
int  orginx;//触屏操作时的初始位置
int  m_size;
string m_sCompany;
CvFont m_font;
string m_testfilepath;
CvSize textSize;
int baseline;
ofstream  file;
bool isMovingImg;
  ///////////////////
  double MHI_DURATION;
  double MAX_TIME_DELTA;
  double MIN_TIME_DELTA;
  int N;
  //
  int CONTOUR_MAX_AERA;
 public:
	  //for koutu
	  ////////////////////////
      CvPoint prev_pt_koutu;
      Mat img_koutu;

      Mat img_mask_koutu;
      IplImage*	 img_kt_koutu;
      Mat dst_koutu;

      Rect rt_kt_koutu;
	  int nKoutuStatus;
	  int Init_Koutou(Mat src);
void on_mouse_koutu(int event, int x, int y);
void DrawingKoutu(int event, int x, int y );
public:
  IplImage *imagegl;
  IplImage *imageHand;
  IplImage *imagePause;
  CvVideoWriter *writer;
  int  saveType;//0  gif  1  avi
  int fps;
  //////////////////////////////////////
  PaletteWidget* pw;
  GifWidget*	gw;
  bool  bGetGif;

  bool  bGetTestImg;
  int select_object;
  int track_object;

  int nImgWidth;//传递过来的视频宽度
  int nImgHeight;
  int nOldImgWidth;//传递过来的视频宽度
  int nOldImgHeight;
  CvPoint origin;
  CvRect selection;

  CvBox2D track_box; // tracking 返回的区域 box，带角度

//  CMotionDetect* md;
  CvCapture* capture;
  vector<IplImage*>  imglist;//for gif
   vector<IplImage*>  imglistBak;//for gif
////////////////////////////////////
/*  vector<IplImage*>  imgInsert;//for insert img
  vector<IplImage*>  imgCover;//for insert img
   vector<IplImage*>  imgMask;//for insert img mask仅针对非方图形使用
  vector<int>    xList;//x,y,width,height,angle
  vector<int>    yList;
  vector<int>    widList;
  vector<int>    heiList;
  vector<int>    angList;//如果该贴图是弹幕，则指定弹幕移动的方向，默认为-1；0 -> 1 <- 2 /  3 \  4  5(23为下往上，45对应为上往下)
  vector<int>    typeList;//指示图片类型，/*0 普通方图  1 矩形截图  2 不规则抠图 3 方图旋转  4 文字  5 指尖花*/
 /* vector<int>    dirList;//图形旋转的方向，为逆时针，360度范围内的角度
  vector<int>    flagList;//图形旋转的方向，为逆时针，360度范围内的角度,默认为0，如果为1则表示改变了图
  vector<int>    bigshowidList;//大招儿对应的imgBigShow id*/
   vector<IMGItem*> imgInsertList; 
  vector<BigShowIMGList>  imgBigShow;//for insert img mask仅针对非方图形使用
  int  nCurBigshowId;
  ///////////////////////////////////


  ////////////////////////////////////

  IplImage*  imgBtn;//for insert img
   int    xBtn;//x,y,width,height,angle
   int    yBtn;
   int    widBtn;
   int    heiBtn;
 //  int    angList;
   int noldx;
    int noldy;
  //////////////////////////////////////

  vector<string> pLcv;//临时BMP文件存放路径，含文件名
  vector<int> pDelay;//帧间间隔
  ///////////////////////////

  int nCurMode;
  int nCurOperMode;
  int nCurColor;
  int nCurSelImg;
  int testnum;
  /////////////////////////////
  string  sImgBtnPath;//图片转动按钮的路径
  string  sImgHandImg;//随着手指移动绘制的动画所需要的图片路径
  int   nTrackId;//用户用特定手势选择的用来跟踪的图片ID,在响应手指操作的函数里改变

  bool  bPause;
  /////////////////////////////
  //各种模式需要的定制变量，需要从手机操作的接口传入
  float menglong_Size;
  int wind_num2;//   num：风线密度
  int wind_num1;//  num1：风线长度
  double xuanwo_Para;
  int jingxiangmohu_num;//num：均值力度;

  int xuanzhuan_num; //均值力度;

  double bolang_angle;
  int bolang_deltaI;  //波浪周期;
  int bolang_A;       //波浪振幅;
//////////////////////////////////////////
  //与JAVA层通信
/* private:
  jclass cGifState;
   jobject mGifState;
   jmethodID mSetProgress;
   jmethodID mSetInfo;

   int GetProviderInstance(jclass obj_class);*/

   /**
    * 初始化 类、对象、方法
    */
   int InitProvider();




   //////////////////////////////////
     //向JAVA层返回信息
 public:
     void SetProgress(int num);
     void SetInfo(string ss);


  ///////////////////////////////
 };

extern CGifAppPlat* pPlat;
 #endif
