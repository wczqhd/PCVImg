#include <string.h>
#include <jni.h>
#include <opencv\cv.h>
//#include <opencv2\highgui\highgui.h>
//#include <math.h>
#include <opencv2\opencv.hpp>
#include "GifAppPlat.h"
using namespace cv;


static CGifAppPlat  gPlat;
JNIEnv* jniEnv;
#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////
char* jstring2String(JNIEnv* env, jstring jstr)
{
   if(jstr == NULL)
    {
      //  LOGI("NullPointerException!");
        return NULL;
    }
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
  //  LOGI("char*=%s",rtn);
    return rtn;
}

//////////////////////////////////////////////
/*E:\workspace\PCVImg*/



JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_hello
  (JNIEnv *env, jobject  jj)
{
	//jstring js = "哈哈完成自动化编译 !";
//	jstring js2 = env->NewStringUTF("哈哈完成自动化编译 !");
	return 0;

}
////////////////////

JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_IsGettingGif
  (JNIEnv *env, jobject  jj)
{
	//jstring js = "哈哈完成自动化编译 !";
//	jstring js2 = env->NewStringUTF("哈哈完成自动化编译 !");
	int ret = 0;
	if(gPlat.IsGettingGif() == true)
		ret = 1;
	else
		ret = 0;
	return ret;

}
/*
 * int   GetImgbyID(int id,Mat& src);
 * ReleaseImglist()*/
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetImgbyID(JNIEnv *env, jobject  jj,jint id,jlong matAddrGr)
{
	//Mat& mGr  = *(Mat*)matAddrGr;

	//		jniEnv = env;


	    Mat& mRgb = *(Mat*)matAddrGr;
	  //  Mat& mRgb2 = *(Mat*)matAddrGr;



	     return gPlat.GetImgbyID(id,mRgb);


}
JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_ReleaseBigShowImglist(JNIEnv *env, jobject  jj)
{
	gPlat.ReleaseBigShowImg();
}
JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_ReleaseImglist(JNIEnv *env, jobject  jj)
{
	gPlat.ReleaseImglist();
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetImgListSize
  (JNIEnv *env, jobject  jj)
{
	//jstring js = "哈哈完成自动化编译 !";
//	jstring js2 = env->NewStringUTF("哈哈完成自动化编译 !");
	return gPlat.GetImgListSize();

}
///////////////////
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_PerfCapVideo(JNIEnv *env, jobject  jj,jlong matAddrGr)
{
	 Mat& mRgb = *(Mat*)matAddrGr;


		     return gPlat.PerfCapVideo(mRgb);

	}
JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_SetSaveType(JNIEnv *env, jobject  jj,jint type)
{
	return gPlat.SetSaveType(type);
}
JNIEXPORT jint   JNICALL Java_com_gifplus_pcvimg_MainActivity_InitVideoFile(JNIEnv *env, jobject,jstring path)
{
	return gPlat.InitVideoFile(jstring2String(env,path));
	}
JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_SetEvery(JNIEnv *env, jobject  jj,jint ff)
{
	 gPlat.SetEvery(ff);
	}
JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_SetBigShowId(JNIEnv *env, jobject  jj,jint ff)
{
	 gPlat.SetBigShowId(ff);
	}
JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_SetEveryBigShow(JNIEnv *env, jobject  jj,jint ff)
{
	 gPlat.SetEveryBigShow(ff);
	}
JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_SetDelay(JNIEnv *env, jobject  jj,jint ff)
{
	 gPlat.SetDelay(ff);
	}

JNIEXPORT void  JNICALL Java_com_gifplus_pcvimg_MainActivity_Resumelist(JNIEnv *env, jobject  jj)
{
	 gPlat.ResumeList();
	}

JNIEXPORT   void  JNICALL Java_com_gifplus_pcvimg_MainActivity_SetSize(JNIEnv *env, jobject  jj,jint size)
{
	gPlat.SetSize(size);
	}
JNIEXPORT   jint  JNICALL Java_com_gifplus_pcvimg_MainActivity_SetHandImgMat(JNIEnv *env, jobject  jj,jlong matAddrGr, jlong matAddrRgba)
{
	   Mat& rgba = *(Mat*)matAddrRgba;
		    Mat& mRgb = *(Mat*)matAddrGr;
		  //  Mat& mRgb2 = *(Mat*)matAddrGr;

		    int from_to[] = { 0,0, 1,1, 2,2 };
		     mixChannels( &rgba, 1, &mRgb, 1, from_to, 3 );
	return gPlat.SetHandImgMat(mRgb);
	}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_PerfLiveFunc(JNIEnv *env, jobject  jj,jint nfun,jfloat nScale,jlong matAddrGr, jlong matAddrRgba, jlong matAddrRgbaF)
{
	//Mat& mGr  = *(Mat*)matAddrGr;

	//		jniEnv = env;

	    Mat& rgba = *(Mat*)matAddrRgba;
	    Mat& mRgb = *(Mat*)matAddrGr;
	    Mat& mRgbF = *(Mat*)matAddrRgbaF;
	  //  Mat& mRgb2 = *(Mat*)matAddrGr;

	    int from_to[] = { 0,0, 1,1, 2,2 };
	     mixChannels( &rgba, 1, &mRgb, 1, from_to, 3 );

	 //    captureFromImage(jstring2String(env,xml), jstring2String(env,filename), jstring2String(env,outfile));

	  //   cvtColor(mRgb,mRgb2,CV_BGR2RGB);
	  //  HuaiJiu(mRgb2);
	    // IplImage src = mRgb;
	    // BoLang(&src,0.0,10,10);
	  //   ColorMap(mRgb,4);
	    ////////////////////////

	     return gPlat.PerfMotionDetect(mRgb,mRgbF,nfun,nScale,0);


}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_PerfEditFunc(JNIEnv *env, jobject  jj,jint nfun,jint nPause,jlong matAddrGr)
{

	//   Mat& rgba = *(Mat*)matAddrRgba;
		    Mat& mRgb = *(Mat*)matAddrGr;
		  //  Mat& mRgb2 = *(Mat*)matAddrGr;


	//此方法暂时不用
	     return gPlat.PerfMotionDetect(mRgb,mRgb,nfun,1,nPause);


}
//void Init_Koutou(Mat src);
//void on_mouse_koutu(int event, int x, int y, int flags, void* );
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_onmousekoutu
  (JNIEnv *env, jobject jj,jint event, jint x, jint y)
{
	 gPlat.on_mouse_koutu( event,  x,  y);
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_InitKoutou
  (JNIEnv *env, jobject jj,jlong matAddrGr, jlong matAddrRgba)
{
	//Mat& mGr  = *(Mat*)matAddrGr;

	//		jniEnv = env;

	    Mat& rgba = *(Mat*)matAddrRgba;
	    Mat& mRgb = *(Mat*)matAddrGr;
	  //  Mat& mRgb2 = *(Mat*)matAddrGr;

	    int from_to[] = { 0,0, 1,1, 2,2 };
	     mixChannels( &rgba, 1, &mRgb, 1, from_to, 3 );

 //    return 0;
     return gPlat.Init_Koutou(mRgb);
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_InSertKoutu
  (JNIEnv *env, jobject jj,jint x,jint y,jint wid,jint hei,jlong matAddrGr, jlong matAddrRgba)
{
	//Mat& mGr  = *(Mat*)matAddrGr;

	//		jniEnv = env;

	    Mat& rgba = *(Mat*)matAddrRgba;
	    Mat& mRgb = *(Mat*)matAddrGr;
	  //  Mat& mRgb2 = *(Mat*)matAddrGr;

	    int from_to[] = { 0,0, 1,1, 2,2 };
	     mixChannels( &rgba, 1, &mRgb, 1, from_to, 3 );
	 Rect rt;

	 rt.x = x;
	 rt.y = y;
     rt.width = wid;
     rt.height = hei;
 //    return 0;
   return  gPlat.InsertKouTu(mRgb,rt);
}
//InputBigShowByPath(char* path,int id)
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_InputBigShowByPath
  (JNIEnv *env, jobject,jstring js,jint id)
{
	return gPlat.InputBigShowByPath(jstring2String(env,js),id);
}

JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_InputBigShowMat
(JNIEnv *env, jobject jj,jlong matAddrRgba,jlong matAddrGr, jint id)
//  (JNIEnv *env, jobject jj,jlong matAddrGr, jlong matAddrRgba,jint id)
{
	//Mat& mGr  = *(Mat*)matAddrGr;

	//		jniEnv = env;

	    Mat& rgba = *(Mat*)matAddrRgba;
	    Mat& mRgb = *(Mat*)matAddrGr;
	  //  Mat& mRgb2 = *(Mat*)matAddrGr;

	    int from_to[] = { 0,0, 1,1, 2,2 };
	     mixChannels( &rgba, 1, &mRgb, 1, from_to, 3 );

 //    return 0;
   return  gPlat.InputBigShowMat(mRgb,id);
}
////////////////////////////////////////
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_ConfirmKouTu
  (JNIEnv *env, jobject jj,jint x,jint y,jint wid,jint hei,jlong matAddrGr, jlong matAddrRgba)
{
	//Mat& mGr  = *(Mat*)matAddrGr;

	//		jniEnv = env;

	    Mat& rgba = *(Mat*)matAddrRgba;
	    Mat& mRgb = *(Mat*)matAddrGr;
	  //  Mat& mRgb2 = *(Mat*)matAddrGr;

	    int from_to[] = { 0,0, 1,1, 2,2 };
	     mixChannels( &rgba, 1, &mRgb, 1, from_to, 3 );
	 Rect rt;

	 rt.x = x;
	 rt.y = y;
     rt.width = wid;
     rt.height = hei;
 //    return 0;
   return  gPlat.ConfirmKouTu(mRgb,rt);
}

///////////////////////////////////////////
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_InSertImg
  (JNIEnv *env, jobject,jstring js,jint x,jint y,jint mode)
{
	return gPlat.InsertImg(jstring2String(env,js),x,y,mode);
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_InSertTxt
  (JNIEnv *env, jobject,jstring js,jint x,jint y,jint mode)
{
	return gPlat.InsertTxt(jstring2String(env,js),x,y,mode);
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetGif
  (JNIEnv *env, jobject j,jstring js)
{
	return gPlat.BuildingGifEntrance(jstring2String(env,js));

}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_Start
  (JNIEnv *env, jobject jo,jstring js)
{
       if(jniEnv == NULL)
			jniEnv = env;//不确定jniEnv初始默认值是否为NULL，所以就只能在这时设置。

	gPlat.Start(jstring2String(env,js));
	}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_Stop
  (JNIEnv *, jobject)
{
	gPlat.Stop();
}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_Pause
  (JNIEnv *, jobject)
{
	gPlat.Pause();
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_TouchScreen
  (JNIEnv *env, jobject jo,jint event, jint x, jint y)
{
	return gPlat.on_event( event,  x,  y);
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetBMPCnt
(JNIEnv *, jobject)
{
	return gPlat.GetBMPCnt();
	}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_Rotate
(JNIEnv *env, jobject jo,jint num)
{
	/*const char* str;
   str = env->GetStringUTFChars(prompt, false);
   if(str == NULL) {
       return NULL; /* OutOfMemoryError already thrown */
  /* }
   std::cout << str << std::endl;
   env->ReleaseStringUTFChars(prompt, str);
   char* tmpstr = "return string succeeded";
   jstring rtstr = env->NewStringUTF(tmpstr);
   return rtstr;
	 */
	gPlat.Rotate(num);

}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_ReSize
(JNIEnv *env, jobject jo,jint type,jfloat range)
{

	gPlat.ReSize(type,range);

}
JNIEXPORT jstring JNICALL Java_com_gifplus_pcvimg_MainActivity_GetBMP
(JNIEnv *env, jobject jo,jint num)
{
	/*const char* str;
   str = env->GetStringUTFChars(prompt, false);
   if(str == NULL) {
       return NULL; /* OutOfMemoryError already thrown */
  /* }
   std::cout << str << std::endl;
   env->ReleaseStringUTFChars(prompt, str);
   char* tmpstr = "return string succeeded";
   jstring rtstr = env->NewStringUTF(tmpstr);
   return rtstr;
	 */
	char* tmpstr =  gPlat.GetBMP(num);
	jstring rtstr = env->NewStringUTF(tmpstr);
	   return rtstr;
}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetInfo
(JNIEnv *env, jobject jo,jint num,jint type)
{
	/*const char* str;
   str = env->GetStringUTFChars(prompt, false);
   if(str == NULL) {
       return NULL; /* OutOfMemoryError already thrown */
  /* }
   std::cout << str << std::endl;
   env->ReleaseStringUTFChars(prompt, str);
   char* tmpstr = "return string succeeded";
   jstring rtstr = env->NewStringUTF(tmpstr);
   return rtstr;
	 */
    return gPlat.GetInfo(num,type);

//	jstring rtstr = " ";
	//jstring rtstr = env->NewStringUTF(tmpstr);

}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetDly
(JNIEnv *env, jobject jo,jint num)
{
	/*const char* str;
   str = env->GetStringUTFChars(prompt, false);
   if(str == NULL) {
       return NULL; /* OutOfMemoryError already thrown */
  /* }
   std::cout << str << std::endl;
   env->ReleaseStringUTFChars(prompt, str);
   char* tmpstr = "return string succeeded";
   jstring rtstr = env->NewStringUTF(tmpstr);
   return rtstr;
	 */
	return gPlat.GetDly(num);

}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetImgListWidth
(JNIEnv *env, jobject jo)
{

	return gPlat.GetImgListWidth();

}
JNIEXPORT jint JNICALL Java_com_gifplus_pcvimg_MainActivity_GetImgListHeight
(JNIEnv *env, jobject jo)
{

	return gPlat.GetImgListHeight();

}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_SetMode
(JNIEnv *env, jobject jo,jint  mode)
{
	gPlat.SetMode(mode);
	}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_SetOperMode
(JNIEnv *env, jobject jo,jint  mode)
{
	gPlat.SetOperMode(mode);
	}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_SetColor
(JNIEnv *env, jobject jo,jint  col)
{
	gPlat.SetColor(col);
	}
JNIEXPORT void JNICALL Java_com_gifplus_pcvimg_MainActivity_SetImgBtnPath
(JNIEnv *env, jobject jo,jstring ss)
{
	gPlat.SetImgBtnPath(jstring2String(env,ss));
}
#ifdef __cplusplus
}
#endif
