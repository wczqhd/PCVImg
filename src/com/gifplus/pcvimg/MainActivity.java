package com.gifplus.pcvimg;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Observable;
import java.util.Observer;
import android.util.FloatMath;


import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.core.Core;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.JavaCameraView;
import org.opencv.core.CvType;

import com.gifplus.pcvimg.DragGridView.OnChanageListener;
import com.gifplus.pcvimg.SelResPicActivity.ResAdapter;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

import android.content.res.Configuration;
import android.content.res.Resources;
import android.content.pm.ActivityInfo;
import android.media.MediaRecorder;
import android.media.ThumbnailUtils;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.R.anim;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.Display;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.View.OnClickListener;

import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.GridView;
import android.widget.HorizontalScrollView;
import android.widget.ImageView;
import android.widget.SimpleAdapter;
import android.widget.TextView;
import android.view.SurfaceView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;



public class MainActivity extends BaseActivity {
	// ////////////////////////////////////////////
	public static native int hello();

	public static native int IsGettingGif();

	public static native int GetImgbyID(int id, long matAddrGr);

	public static native void ReleaseImglist();
	public static native void ReleaseBigShowImglist();
	

	public static native int GetImgListSize();

	public static native int SetHandImgMat(long matAddrGr, long aa);

	public static native void ReSize(int type, float range);

	public static native int PerfCapVideo(long matAddrGr);

	public static native void SetSaveType(int type);

	public static native int InitVideoFile(String path);

	public static native void SetEvery(int ff);

	public static native void SetEveryBigShow(int ff);

	public static native void SetBigShowId(int ff);

	public static native void SetDelay(int ff);

	public static native void SetSize(int size);

	// public static native int PerfFunc(int nfun,long matAddrGr, long
	// matAddrRgba);

	public static native int PerfLiveFunc(int nfun, float nScale,long matAddrGr,
			long matAddrRgba,long matAddrRgbaF);

	public static native int PerfEditFunc(int nfun, int nPause,long matAddrGr);
	public static native void onmousekoutu
	  (int event, int x, int y);
	public static native int InitKoutou
	  (long matAddrGr, long matAddrRgba);

	public static native int GetImgListWidth();

	public static native int GetImgListHeight();

	public static native int InSertKoutu(int x, int y, int wid, int hei,
			long matAddrGr, long aa);

	public static native int InputBigShowByPath(String js,int id);
	public static native int InputBigShowMat(long matAddrGra,long matAddrGr, int id);

	public static native int ConfirmKouTu(int x, int y, int wid, int hei,
			long matAddrGr, long aa);

	public static native int InSertImg(String js, int x, int y, int mode);

	public static native int InSertTxt(String js, int x, int y, int mode);

	public static native int GetGif(String js);

	public static native void Start(String js);

	// public native void PerfFunc(JNIEnv *env, jobject jj,jint nfun,jlong
	// matAddrGr, jlong matAddrRgba);
	public static native void Stop();

	//public static native void Pause();

	public static native int TouchScreen(int event, int x, int y);

	public static native void Rotate(int num);

	public static native void SetOperMode(int mode);

	public static native int GetBMPCnt();

	public static native String GetBMP(int num);

	public static native int GetInfo(int num, int type);

	public static native int GetDly(int num);

	public static native void SetMode(int mode);

	public static native void SetImgBtnPath(String ss);

	public static native void SetColor(int color);

	public static native void Resumelist();

	// ///////////////////////////////////////////////////////

	static {

		System.loadLibrary("opencv_java");// 、、//
		// System.loadLibrary("opencv_java");
		System.loadLibrary("pcvimg");

	}
	// /////////////////////////////

	public static final String PHOTO_DIR = Environment
			.getExternalStorageDirectory().getPath() + "/DCIM/Camera/";

	public static final String TEMP_DIR = Environment
			.getExternalStorageDirectory().getPath() + "/DCIM/GIFPLUS/";

	public static final String TEST_FILE = Environment
			.getExternalStorageDirectory().getPath()
			+ "/DCIM/GIFPLUS/"
			+ "tmain.txt";
	
	public static int nFilenum = 0;

	public static String MainPagePath = "files";
	
	public static String APP_ID = "wxfd1fd652ef1e1e11"; 
	private static final int TIMELINE_SUPPORTED_VERSION = 0x21020001;
	
	// IWXAPI 是第三方app和微信通信的openapi接口
	private IWXAPI api;

//	public static String MainPagePath = "face/emtions";
	private static final File tfile = new File(TEST_FILE);
	private static int num = 0;
	public boolean thdOver = false;
	private boolean m_pause = false;
	public static BufferedWriter output;
	public static int m_size = 6;

	public static Mat matKoutu = null;
	public static Mat matKoutua = null;

	public static final int PICK_GIF_PHOTO = 166;

	public static int nDelay = 120;
	public static int nEvery = 2;

	private String[] mPhotoList;

	private int  nNum;
	// ///////////////////////////////////////
	private Button buttonRecord, buttonEdit, buttonCreate;// buttonEditWhole,
															// buttonEditBrowse,
															// buttonDown;

	// //////////////////////////////////////////

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		initialButton();

		MainActivity.SetDelay(nDelay);
		MainActivity.SetEvery(nEvery);
		
		File f = new File(TEMP_DIR, "tmp.bmp");
		  
			   f.mkdirs();
			   File f1 = new File(TEMP_DIR, "txt.png");
				  
			   f1.mkdirs();

		// /////////////////////////////////
	//	GridView gridview = (GridView) findViewById(R.id.maingrid);// 找到main.xml中定义gridview
																	// 的id

		// int ndir = getIntent().getIntExtra("dir",1);
		// ///////////////////////////////////

		nNum = 0;
	//	CreateTestFile();
	//	String dir = "files";
		api = WXAPIFactory.createWXAPI(this, APP_ID, false);

		
		api.registerApp(MainActivity.APP_ID);

	/*	try {
			mPhotoList = this.getAssets().list(MainPagePath);
			MainPagePath += "/";
		} catch (IOException e) {
			finish();
			return;
		}///*/
	//	OnlyInfoTestFile("mPhotoList  "+  mPhotoList.length);
		

		// ////////////////////////////////////
	/*	gridview.setAdapter(new ResAdapter(this));// 调用ImageAdapter.java
		gridview.setVerticalScrollBarEnabled(true);
		// gridview.setBackgroundResource(R.drawable.background);
		// gridview.set
		gridview.setOnItemClickListener(new OnItemClickListener() {// 监听事件
			public void onItemClick(AdapterView<?> parent, View view,
					int position, long id) {
				// Toast.makeText(SelFileActivity.this,
				// "file "+mPhotoList.get(position),Toast.LENGTH_SHORT).show();//显示信息;
				Intent intent = new Intent(MainActivity.this, GifActivity.class);
				intent.putExtra("path", MainPagePath+ mPhotoList[position]);

				nNum = 0;
				intent.putExtra("SelectType", GifActivity.TYPE_FILE);
				
				intent.putExtra("IsAssets", true);

				// intent.putExtra("floderName", mFloderList.get(arg2));

				// startActivityForResult(intent, PICK_PHOTO);
				ClearData();
				startActivity(intent);

			//	finish();

			}
		});*/

		setGuideResId(R.drawable.yindao01);
	}

	@Override
	public void onPause() {
		// back键后响应
		super.onPause();
	}

	@Override
	public void onResume() {
	//	OnlyInfoTestFile("MainonResume");
		api.registerApp(MainActivity.APP_ID);
		super.onResume();
	//	nNum = 0;
		
	}

	public void onDestroy() {
		// home键后响应
	//	CloseTestFile();
	//	OnlyInfoTestFile("MainonDestroy");
		super.onDestroy();// ////
	//	
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	// /////////////////////////////////////////////

	private void initialButton() {
		// TODO Auto-generated method stub

		// Log.v("width  & height is:", String.valueOf(width) + ", " +
		// String.valueOf(height));

		// params.height = 50;
		// params.width = 50;
		// 设置边距 (int left, int top, int right, int bottom)
		// params.setMargins(10, height - 98, 0, 0);

		buttonRecord = (Button) findViewById(R.id.buttonRec);
		buttonEdit = (Button) findViewById(R.id.buttonEdit);
	//	buttonCreate = (Button) findViewById(R.id.buttonCreate);
		//buttonSearch = (Button) findViewById(R.id.buttonSearch);

	/*	buttonCreate.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
			}
			});*/
		buttonRecord.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this,
						RecordActivity.class);

				nNum = 0;
				// Intent intent = new Intent(MainActivity.this,
				// GifActivity.class);

				// intent.putExtra("SelectType", 1);

				// intent.putExtra("path", "1.gif");

				ClearData();
				startActivityForResult(intent, 0);

			}
		});
		buttonEdit.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this,
						SelFileActivity.class);

				// Intent intent = new Intent(MainActivity.this,
				// GifActivity.class);

				intent.putExtra("SelectType", 1);

				nNum = 0;
				// intent.putExtra("path", "1.gif");

				ClearData();
				startActivityForResult(intent, PICK_GIF_PHOTO);

			}
		});

	}

	// buttonDown = (Button) findViewById(R.id.Down);

	// buttonDelete.setLayoutParams(params);

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		/*
		 * public static final int PICK_PHOTO = 125; public static final int
		 * INSERT_TEXT = 150; public static final int DANMU_TEXT = 130; public
		 * static final int NORMAL_TEXT = 135; public static final int
		 * PICK_FROM_PHOTO = 140;
		 */
	//	 OnlyInfoTestFile("Main onActivityResult");
	//	 OnlyInfoTestFile("MainonActivityResult" + requestCode + " "+resultCode);
		if (null == data) {
			super.onActivityResult(requestCode, resultCode, data);
		//	 Toast toast = Toast.makeText(this, "null data" + requestCode,
			// Toast.LENGTH_SHORT);
			// toast.show();
			return;   
		}// ////

		//Toast toast = Toast.makeText(this, " data" + requestCode,
			//	 Toast.LENGTH_SHORT);
				// toast.show();
		if (requestCode == PICK_GIF_PHOTO && resultCode == RESULT_OK) {
		//	OnlyInfoTestFile("onActivityResult  3");
			Bundle bundle = data.getExtras();
			// Toast toast = Toast.makeText(this, "PICK_FROM_PHOTO",
			// Toast.LENGTH_SHORT);
			// toast.show();
			// 获取相机返回的数据，并转换为图片格式
			String filepath = (String) bundle.get("filepath");
			//OnlyInfoTestFile("onActivityResult  4");
			if (null == filepath) {
				super.onActivityResult(requestCode, resultCode, data);
				// Toast toast2 = Toast.makeText(this, "null filepath",
				// Toast.LENGTH_SHORT);
				// toast2.show();
				return;
			}
		//	OnlyInfoTestFile("onActivityResult  5");
			Intent intent = new Intent(MainActivity.this, GifActivity.class);
			intent.putExtra("path", filepath);

			intent.putExtra("SelectType", GifActivity.TYPE_FILE);

			// intent.putExtra("floderName", mFloderList.get(arg2));

			// startActivityForResult(intent, PICK_PHOTO);
			startActivity(intent);
		}

		// OnlyInfoTestFile("onActivityResult6");
		super.onActivityResult(requestCode, resultCode, data);
	//	OnlyInfoTestFile("onActivityResult7");

	}

	// ///////////////////////////////////////////////////////////

	// ////////////////////////////////////////////////

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub

		// setRequestedOrientation(orientMode);//设定需要的朝向，横或者竖

		// CreateFood(sql,foodtypeid);
		super.onConfigurationChanged(newConfig);
	}

	// ///////////////////////////////////
	// //////////////////////////

	private static void CreateTestFileN(String tt) {
		File tg = new File(TEMP_DIR + nFilenum + tt);
		try {
			if (tg.exists()) {
				tg= new File(TEMP_DIR + nFilenum+tt + "1");
			}
			tg.createNewFile();
			nFilenum++;
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	private static void CreateTestFile() {
	/*	try {
			// File file = new File("D:/abc.txt");
			if (tfile.exists()) {
				tfile.delete();
			}

			tfile.createNewFile();

			output = new BufferedWriter(new FileWriter(tfile));
		} catch (IOException e) {
			Log.i("IOException===", e.getMessage());
			// ((TextView)findViewById(R.id.textview1)).setText("IOException==="
			// + e.getMessage());
		}*/
	}

	private static void WriteTestInfo(String info) {
	/*	try {
			output.write(info + "$$" + "\n");
		} catch (IOException e) {
		//	Log.i("IOException===", e.getMessage());
			// ((TextView)findViewById(R.id.textview1)).setText("IOException==="
			// + e.getMessage());
		}*/

	}

	private static  void CloseTestFile() {
/*		try {
			output.close();
		} catch (IOException e) {
			Log.i("IOException===", e.getMessage());
			// ((TextView)findViewById(R.id.textview1)).setText("IOException==="
			// + e.getMessage());
		}*/
	}

	public static void OnlyInfoTestFile(String info) {

	//	CreateTestFile();
	//	WriteTestInfo(info);
	//	CloseTestFile();

	//	CreateTestFileN(info);
	}

	// ///////////////////////////
	// //////////////////////////////////////////////////
/*
	class ResAdapter extends BaseAdapter {

		private Context mContext;

		// private PictureShowUtils mAllImages = new PictureShowUtils();

		public ResAdapter(Context c) {
			mContext = c;
		}

		@Override
		public int getCount() {

			// TODO Auto-generated method stub

			return mPhotoList.length;

		}

		@Override
		public Object getItem(int position) {

			// TODO Auto-generated method stub

			return mPhotoList[position];

		}

		@Override
		public long getItemId(int position) {

			// TODO Auto-generated method stub

			return position;

		}

		@Override
		public View getView(int position, View convertView, ViewGroup parent) {

			// LayoutInflater inflater = MainActivity.this.getLayoutInflater();

			// convertView=inflater.inflate(R.layout.pick_photo_list_item,
			// null);
			ImageView imageview;
			if (convertView == null) {
				imageview = new ImageView(mContext);
				imageview.setLayoutParams(new GridView.LayoutParams(200, 200));
				imageview.setScaleType(ImageView.ScaleType.CENTER_CROP);
				imageview.setPadding(8, 8, 8, 8);

			} else {
				imageview = (ImageView) convertView;
			}
			// String image_path = null;

			// int imageCount = 0;

			
		//	OnlyInfoTestFile("mPhotoList[" + position + "]   "  + mPhotoList[position]);
			imageview.setImageBitmap(getImageThumbnail(MainPagePath + mPhotoList[position],
					70, 70));

			return imageview;

			// photoNumber.setText(imageCount+" 张");

		}

		// @SuppressLint("NewApi")

		private Bitmap getImageThumbnail(String imagePath, int width, int height) {

			Bitmap bitmap = null;

			BitmapFactory.Options options = new BitmapFactory.Options();

			options.inJustDecodeBounds = true;

			// 获取这个图片的宽和高，注意此处的bitmap为null

			try {
				bitmap = BitmapFactory.decodeStream(mContext.getAssets().open(
						imagePath));
			} catch (IOException e) {
				return null;
			}
			
			bitmap = ThumbnailUtils.extractThumbnail(bitmap, width, height,

			ThumbnailUtils.OPTIONS_RECYCLE_INPUT);

			return bitmap;

		}

	}*/

	// ////////////////////////////////////
	@Override
    public void onBackPressed() {
		if(nNum <= 1)
		{
			nNum++;
			Toast toast = Toast.makeText(this, R.string.twicequit, Toast.LENGTH_SHORT);
			toast.show();
		}
		else{
			//finish();
		   super.onBackPressed();
		   api.unregisterApp();
		   System.exit(0);
		}
		
	}
	
	public static void ClearData()
	{
	
	}
	
}
