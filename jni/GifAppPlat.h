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
   IplImage*  imgMask;//for insert img mask����ԷǷ�ͼ��ʹ��
  int    xList;//x,y,width,height,angle
  int    yList;
  int    widList;
  int    heiList;
  int    angList;//�������ͼ�ǵ�Ļ����ָ����Ļ�ƶ��ķ���Ĭ��Ϊ-1��0 -> 1 <- 2 /  3 \  4  5(23Ϊ�����ϣ�45��ӦΪ������)
  int    typeList;//ָʾͼƬ���ͣ�/*0 ��ͨ��ͼ  1 ���ν�ͼ  2 �������ͼ 3 ��ͼ��ת  4 ����  5 ָ�⻨*/
  int    dirList ;//ͼ����ת�ķ���Ϊ��ʱ�룬360�ȷ�Χ�ڵĽǶ�
  int    flagList ;//ͼ����ת�ķ���Ϊ��ʱ�룬360�ȷ�Χ�ڵĽǶ�,Ĭ��Ϊ0�����Ϊ1���ʾ�ı���ͼ
  int    bigshowidList;//���ж���Ӧ��imgBigShow id*/
   int    bigshowListInId;//���ж���Ӧ��imgBigShow �е�vector�е�ID*/
   int    bigshowEvery;//���ж�ÿ������֡����һ��*/
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
#define DIRECTION_NUM  6 //��Ļ����
#define IMAGE_NUM      6 //��ͼ����
#define IMAGE_SPEED      40 //��Ļ�ƶ��ٶ�

/////////////////////////////////
#define MENGLONG_SIZE  0.5
#define 	WIND_NUM2  10
#define 	WIND_NUM1  20
#define 	XUANWO_PARA 20
#define 	JINGXIANGMOHU_NUM  40
#define 	XUANZHUAN_NUM 20

#define 	BOLANG_ANGLE  0
#define 	BOLANG_DELTAI  10  //��������;
#define 	BOLANG_A  10      //�������;
#define     HAND_IMG_SEP   500  //��ָ�ƶ������Ļ������
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
  int PerfMotionDetect(Mat& src,Mat& srcF,int num,float nScale,int nPause/*1 pause;0 no pause*/);//num Ϊ-1����ʾΪ¼��ʵʱ����������Ϊ�༭���ж���״̬��
  int PerfLive(IplImage* image,int num);//num����STATUS_EDIT����Ϊ�༭״̬
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

  void RefreshImage();//ˢ����Ƶ����
  int InsertKouTu(Mat& src,CvRect roi);
  int  InsertImg(char* imgpath,int x,int y,int mode);
  int  InsertLPImg(IplImage* insert,IplImage* cover,IplImage* mask,int x,int y,int mode);//��ͼ����������Դ�����߽�ͼ����ͼ
  int   InsertSPImg(IplImage* insert,int x,int y,int mode);//����Ƿ�ͼ�Ĳ���
 IplImage*  FixImg(IplImage* insert,int x,int y,int mode);
  int   InsertSPTxt(IplImage* insert,int x,int y,int mode);//�����ַ��Ĳ���
  int   UpdateSPTxt(int id,int type,CvRect roi);
  int   UpdateSPImg(int id,int type,CvRect roi);//�޸ķǷ�ͼ����Ϣ.����֮ǰҪ������꣬��С��ԭͼ���޸ģ��ú���ֻ�����޸�cover��mask��type
  //int  InsertStrangeImg(IplImage* insert,int x,int y);
  int  InsertMat(Mat& src,Rect rt,int x,int y,int mode/*0 ��ͨ��ͼ  1 ���ν�ͼ  2 �������ͼ 3 ��ͼ��ת  4 ���ֵ�Ļ 5 ��Чͼ  6 �ǵ�Ļ���ֺ�װ�λ����ģ���,ˮ�ε���������ƶ����Ƶ�ͼ�� 7 ��д  8 ���ж���bigshow��*/);
  IplImage*  GetSubImage(IplImage *image, CvRect roi);
  void OnPlatTouch(int vevent,int x, int y);//�������¼�
  int  InsertTxt(char* imgpath,int x,int y,int mode);
  int  InsertDrawTxt();
  void  InsertHandImg(int x,int y,int mode);
  void  Start(char* path);
  void  Stop();
  void  Pause();
  void  GetPages(char* filename);//��ȡGIF�ļ���֡S

  int  CreatingGIF();
  int on_event( int event, int x, int y);
  int  BuildingGifEntrance(char* path);//������·������б��
  void  ReleaseImg();
  void ReleaseImglist();
  void ReleaseBigShowImg();
  int   GetBMPCnt();
  char*  GetBMP(int num);
  int    GetDly(int num);
  void  ClearBMP();//���BMP·�����ļ�
  int   FindtheImg(int x,int y);//�жϴ�����ѹ���Ǹ���ͼ��

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
  void  SetSize(int size);//{m_size = size;}//1 ԭ�ߴ�  2  ����֮һ  3 ����֮һ  4  �ķ�֮һ
  int  SelectImg(int x,int y);
  void FixRect(int &x,int &y,int &wid,int &het);
  void  DeleteImg(int id);//ɾ��ָ����ͼƬ
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
  void ReSize(int type/*1 �Ŵ�  -1  ��С*/,float range = 0.02);
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
CvPoint prev_hand_pt;// = {-1, -1};//��ָ������ǰһ��λ��
int  orginx;//��������ʱ�ĳ�ʼλ��
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

  int nImgWidth;//���ݹ�������Ƶ���
  int nImgHeight;
  int nOldImgWidth;//���ݹ�������Ƶ���
  int nOldImgHeight;
  CvPoint origin;
  CvRect selection;

  CvBox2D track_box; // tracking ���ص����� box�����Ƕ�

//  CMotionDetect* md;
  CvCapture* capture;
  vector<IplImage*>  imglist;//for gif
   vector<IplImage*>  imglistBak;//for gif
////////////////////////////////////
/*  vector<IplImage*>  imgInsert;//for insert img
  vector<IplImage*>  imgCover;//for insert img
   vector<IplImage*>  imgMask;//for insert img mask����ԷǷ�ͼ��ʹ��
  vector<int>    xList;//x,y,width,height,angle
  vector<int>    yList;
  vector<int>    widList;
  vector<int>    heiList;
  vector<int>    angList;//�������ͼ�ǵ�Ļ����ָ����Ļ�ƶ��ķ���Ĭ��Ϊ-1��0 -> 1 <- 2 /  3 \  4  5(23Ϊ�����ϣ�45��ӦΪ������)
  vector<int>    typeList;//ָʾͼƬ���ͣ�/*0 ��ͨ��ͼ  1 ���ν�ͼ  2 �������ͼ 3 ��ͼ��ת  4 ����  5 ָ�⻨*/
 /* vector<int>    dirList;//ͼ����ת�ķ���Ϊ��ʱ�룬360�ȷ�Χ�ڵĽǶ�
  vector<int>    flagList;//ͼ����ת�ķ���Ϊ��ʱ�룬360�ȷ�Χ�ڵĽǶ�,Ĭ��Ϊ0�����Ϊ1���ʾ�ı���ͼ
  vector<int>    bigshowidList;//���ж���Ӧ��imgBigShow id*/
   vector<IMGItem*> imgInsertList; 
  vector<BigShowIMGList>  imgBigShow;//for insert img mask����ԷǷ�ͼ��ʹ��
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

  vector<string> pLcv;//��ʱBMP�ļ����·�������ļ���
  vector<int> pDelay;//֡����
  ///////////////////////////

  int nCurMode;
  int nCurOperMode;
  int nCurColor;
  int nCurSelImg;
  int testnum;
  /////////////////////////////
  string  sImgBtnPath;//ͼƬת����ť��·��
  string  sImgHandImg;//������ָ�ƶ����ƵĶ�������Ҫ��ͼƬ·��
  int   nTrackId;//�û����ض�����ѡ����������ٵ�ͼƬID,����Ӧ��ָ�����ĺ�����ı�

  bool  bPause;
  /////////////////////////////
  //����ģʽ��Ҫ�Ķ��Ʊ�������Ҫ���ֻ������Ľӿڴ���
  float menglong_Size;
  int wind_num2;//   num�������ܶ�
  int wind_num1;//  num1�����߳���
  double xuanwo_Para;
  int jingxiangmohu_num;//num����ֵ����;

  int xuanzhuan_num; //��ֵ����;

  double bolang_angle;
  int bolang_deltaI;  //��������;
  int bolang_A;       //�������;
//////////////////////////////////////////
  //��JAVA��ͨ��
/* private:
  jclass cGifState;
   jobject mGifState;
   jmethodID mSetProgress;
   jmethodID mSetInfo;

   int GetProviderInstance(jclass obj_class);*/

   /**
    * ��ʼ�� �ࡢ���󡢷���
    */
   int InitProvider();




   //////////////////////////////////
     //��JAVA�㷵����Ϣ
 public:
     void SetProgress(int num);
     void SetInfo(string ss);


  ///////////////////////////////
 };

extern CGifAppPlat* pPlat;
 #endif
