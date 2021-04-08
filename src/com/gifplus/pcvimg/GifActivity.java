package com.gifplus.pcvimg;


import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;



import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Environment;
import android.os.Message;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;

import android.graphics.Paint;
import android.graphics.Bitmap.Config;
import android.graphics.Rect;

import android.util.Log;
import android.view.Display;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.app.AlertDialog;
import android.content.DialogInterface;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;




import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

import com.gifplus.pcvimg.DragGridView.OnChanageListener;
import com.gifplus.pcvimg.GifEditView.GifEditViewListener;

import android.widget.Button;
import android.widget.HorizontalScrollView;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.SimpleAdapter;
import android.widget.Toast;

import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.SendMessageToWX;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.tencent.mm.sdk.openapi.WXAppExtendObject;
import com.tencent.mm.sdk.openapi.WXEmojiObject;
import com.tencent.mm.sdk.openapi.WXImageObject;
import com.tencent.mm.sdk.openapi.WXMediaMessage;
import com.tencent.mm.sdk.openapi.WXMusicObject;
import com.tencent.mm.sdk.openapi.WXTextObject;
import com.tencent.mm.sdk.openapi.WXVideoObject;
import com.tencent.mm.sdk.openapi.WXWebpageObject;
import com.tencent.mm.sdk.openapi.SendAuth;

public class GifActivity extends BaseActivity implements GifEditViewListener,
		View.OnTouchListener {

	// private GifView gf1;

	// private GifView gf2;

//	private ImageView imageview;
	// private ImageButton imagebutton;
//	private GifAnimationDrawable big;

	Handler handler;
	File ff;

	private Button buttonCancel, buttonEditAdd, buttonSave, buttonEditWhole,
			buttonBrowse,buttonPause,buttonShare;
	private static int width, height;
	private static Boolean isClick = false;
	
	private int WEIXIN_MAXSIZE = 400*1024;
	
	public static final int PICK_PHOTO_E = 1250;
	public static final int INSERT_TEXT_E = 1500;
	public static final int DANMU_TEXT_E = 1300;
	public static final int NORMAL_TEXT_E = 1350;
	public static final int PICK_FROM_PHOTO_E = 1400;
	public static final int PICK_GIF_PHOTO_E = 1600;
	
	public static final int PICK_EMOTION = 1610;
	public static final int PICK_BABY = 1620;
	public static final int PICK_BIGSHOW = 1636;
	public static final int PICK_FLOWER = 1630;
	

	public static final int TYPE_FILE = 1;
	public static final int TYPE_LIST = 2;

	public static  int test_num = 0;
	private int nInsertX, nInsertY;
	// ///////////////////////////////////////

	private static final String PHOTO_DIR = Environment
			.getExternalStorageDirectory().getPath() + "/DCIM/Camera/";

	private static final String TEMP_DIR = Environment
			.getExternalStorageDirectory().getPath() + "/DCIM/GIFPLUS/";

	public static final String TEST_FILE1 = Environment
			.getExternalStorageDirectory().getPath()
			+ "/DCIM/GIFPLUS/"
			+ "tgifacty.txt";

	private static final String TEMP_BMP = "tmp.bmp";

	public static final String TEST_FILE = Environment
			.getExternalStorageDirectory().getPath()
			+ "/DCIM/GIFPLUS/"
			+ "t.txt";

	private static final String TAG = "PCVImg::Activity";

	// private CameraBridgeViewBase mOpenCvCameraView;
	// private JniUtility mUtil;

	// private Mat mIntermediateMat;

	private Bitmap mTmpBitmap;
	// private MenuItem mItemHideNumbers;
	// private MenuItem mItemStartNewGame;

	private int mGameWidth;
	private int mGameHeight;

	private float mX = 0;
	private float mY = 0;
	private float mGap = 0;

	// /////////////////////////////////////

	private static final int THUMB_SIZE = 150;
	// IWXAPI 是第三方app和微信通信的openapi接口
  //  private IWXAPI api1;
	// /////////////////////////////

	private final static float MIN_DEGREE = 0f;
	private final static float MAX_DEGREE = 360f;

	private String sFilePath;
	public static String minfo;
	// private int mnProcess = 0;
	// /////////////////////////////////////////
	private List<HashMap<String, Object>> dataSourceListWhole = new ArrayList<HashMap<String, Object>>();
	private List<HashMap<String, Object>> dataSourceListAdd = new ArrayList<HashMap<String, Object>>();
	private List<HashMap<String, Object>> dataSourceListAddRes = new ArrayList<HashMap<String, Object>>();

	
	private DragGridView mDragGridViewAdd;
	private HorizontalScrollView hView;
	int viewVisableWhole = DragGridView.INVISIBLE;
	int viewVisableAdd = DragGridView.INVISIBLE;
	int viewVisableAddRes = DragGridView.INVISIBLE;

	// //////////////////////////
	private GifEditView mGifEditView;

//	private GifDecoder mGifDecoder;

	// private Bitmap mTmpBitmap;
	private int mViewMode;
	private Mat mRgba;
	private Mat mRgb;
	public  boolean isPause = false;
	private Thread thd;

	// private static final String PHOTO_DIR =
	// Environment.getExternalStorageDirectory().getPath() + "/DCIM/WowGIF/";

	// private static final File cam_file = new File(PHOTO_DIR);
	private static final String testfile = "test.txt";
	private static final File tfile = new File(TEST_FILE1);
	private static int num = 0;

	private String filename = " ";// PHOTO_DIR+name + ext;
	private boolean bShare = false;
	BufferedWriter output;

	int originalImageOffsetX;
	int originalImageOffsetY;

	public static int nSelType;
	
	private static int nChangeSize;//1  no change;n(n!=1),1/n
	private boolean bChanged = false;
	
	public static int FROM_FILE = 1;
	public static int FROM_REC = 2;
	
	public static int nFrom = FROM_REC;
	public static int nDelay;
	public static int nEvery;
	public static boolean bForceQuit = false;
	private int nBigShowNum;
	
	// IWXAPI 是第三方app和微信通信的openapi接口
    private IWXAPI api;

    private static final int MMAlertSelect1  =  0;
	private static final int MMAlertSelect2  =  1;
	private static final int MMAlertSelect3  =  2;
	private  int scene = 0;

	public static ProgressBar  pbar;
	// private boolean f = true;
	// /////////////////////////////////////
	final static int FINISH_ONE_ROUND = 1;
	final static int READY_TO_SHARE = 2;

	public Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case FINISH_ONE_ROUND:
				 GifOneRoundFinished();
				 break;
				//SaveGif();
			case READY_TO_SHARE:
				SelectSenttoWeixin();
				break;
			default:
				break;
			}
		}
	};

	// /////////////////////////////////

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// setContentView(R.layout.activity_main);

		setContentView(R.layout.activity_display_gif);

		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);// 设置屏幕常亮

		nInsertX = 0;
		nInsertY = 0;
		nBigShowNum = 0;
		mViewMode = -1;//////////////
		
		
		api = WXAPIFactory.createWXAPI(this, MainActivity.APP_ID);
		if(api != null)
		   api.registerApp(MainActivity.APP_ID);  
	//	SendTxtToWeiXin("  ");

		/*
		 * LinearLayout ll = (LinearLayout)findViewById(R.layout.main);
		 * 
		 * GifView bb = new GifView(this);
		 * 
		 * bb.setAsBackground(ll);
		 * 
		 * bb.setGifImage(R.drawable.a);
		 */

		// imageview = (ImageView)findViewById(R.id.imageview);
		// imagebutton = (ImageButton)findViewById(R.id.imagebutton);

		// imagebutton.setOnClickListener(this);
		// imageview.setOnClickListener(this);

		// and add the GifAnimationDrawable
		try {
			// android.util.Log.v("GifAnimationDrawable", "===>One");
			// little = new
			// GifAnimationDrawable(this,getResources().openRawResource(R.raw.anim1));
			// little.setOneShot(true);
			// android.util.Log.v("GifAnimationDrawable", "===>Two");
			// imagebutton.setImageDrawable(little);
			// android.util.Log.v("GifAnimationDrawable", "===>Three");
		//	CreateTestFile();
			String path;// = TEMP_DIR + "wow.gif";
			bForceQuit = false;
			// mGifEditView = (GifEditView) findViewById(R.id.imageview);
			mGifEditView = (GifEditView) findViewById(R.id.imageview);
			  pbar = (ProgressBar)findViewById(R.id.progressBar1);
			  pbar.setProgress(0);
			//  pbar.setMax(max);
			MainActivity.SetImgBtnPath(MainActivity.TEMP_DIR + "btest");
			// Log.e("gifactivity",
			// "mGifEditView = (GifEditView) findViewById(R.id.imageview)");
			
			try {
				Intent intent1 = this.getIntent();
				// Toast toast1 = Toast.makeText(GifActivity.this, "intent",
				// Toast.LENGTH_SHORT);

				// toast1.show();

				if (intent1 != null) {
					nSelType = intent1.getIntExtra("SelectType", TYPE_FILE);
					
					nChangeSize = intent1.getIntExtra("ChangeSize", 1);
					
					boolean isassets = intent1.getBooleanExtra("IsAssets", false);
					
				//	Toast toast1 = Toast.makeText(GifActivity.this, "intent  " + nSelType,
					//		 Toast.LENGTH_SHORT);
				//	toast1.show();
					
				//	OnlyInfoTestFile("mGifEditView 2 " + nSelType);

					if (nSelType == TYPE_FILE) {

						path = intent1.getStringExtra("path");
						
					//	OnlyInfoTestFile("path = " + path);
						// Toast toast = Toast.makeText(GifActivity.this, path,
						// Toast.LENGTH_SHORT);

						// toast.show();

						if (path != null) {
							// ff = new File(path);
					//		 OnlyInfoTestFile("mGifEditView 1 " + path);
							sFilePath = path;
							InputStream is;
							if(isassets)
							{
								is = this.getAssets().open(
										path);
							}
							else
							{
								File ff = new File(path);
								
							//	OnlyInfoTestFile(path);
								 is = new BufferedInputStream(new FileInputStream(ff),
										32768);
							}
					//		OnlyInfoTestFile("Beginacty");
							mGifEditView.Begin(is);
						//	OnlyInfoTestFile("Beginacty222");
							mGifEditView.mStopThread = false;
							
						//	OnlyInfoTestFile("mGifEditView 2 " + mGifEditView.mStopThread + "  " + mGifEditView.thdOver);
							nFrom = FROM_FILE;
							// Toast toast = Toast.makeText(GifActivity.this,
							// "Begin "+path,Toast.LENGTH_SHORT);
							// toast.show();
						}
					} else {

						path = "";
						//OnlyInfoTestFile("mGifEditView 2 " + path);
					//	Toast toast2 = Toast.makeText(GifActivity.this, "intent" + path.isEmpty() + nSelType,
						//		 Toast.LENGTH_SHORT);
						//toast2.show();
						mGifEditView.mStopThread = false;
						mGifEditView.Begin(null);
						nFrom = FROM_REC;
						bChanged = true;
					}
					// else
					// finish();
				}
				// else
				// finish();
			} catch (Exception e) {
				
			//	Toast toast2 = Toast.makeText(GifActivity.this, "exception  111 ",
				//				 Toast.LENGTH_SHORT);
					//	toast2.show();
					//	OnlyInfoTestFile("exceptopn");
				finish();
				return;
			}

			// mUtil = new JniUtility();
			// Log.d(TAG, "Creating and seting view22222222222222");
			// setContentView(mOpenCvCameraView);
			// Log.d(TAG, "Creating and seting view333333333333333");

			// mOpenCvCameraView.setCvCameraViewListener(this);

			// mGifEditView.Begin(path);
			mGifEditView.enableView();// /
			mGifEditView.setVisibility(SurfaceView.VISIBLE);
			mGifEditView.setOnTouchListener(GifActivity.this);
			mGifEditView.setGifViewListener(this);
			// Log.e("gifactivity", "mGifEditView.setGifViewListener");

			// File f;// = new File(path);
			// Toast toast = Toast.makeText(this, path, Toast.LENGTH_SHORT);
			// toast.show();
			handler = new Handler();

			initialButton();

			// //////////////////////////////
			mDragGridViewAdd = (DragGridView) findViewById(R.id.dragGridViewAdd);
			hView = (HorizontalScrollView) findViewById(R.id.HView);
			initDataSrcWhole();

			initDataSrcAdd();
			MainActivity.Start(MainActivity.TEST_FILE);
		//	MainActivity.SetSize(1);

			// CreateTestFile();

			// if(ff.canRead())
			// Begin();

			isPause  = false;

			if (filename == " ") {
				String name = getPhotoFileName();
				String ext = ".gif";
				filename = PHOTO_DIR + name + ext;// 为空则重新赋值，在打开新文件后重新赋值，否则就一直覆盖
			}
			// Toast toast = Toast.makeText(GifActivity.this,
			// "GetGif "+filename,Toast.LENGTH_SHORT);
			// toast.show();
			MainActivity.GetGif(filename);

			// SaveGif();
			/*
			 * thd = new Thread(new Runnable(){ public void run() {
			 * 
			 * handler.post(mUpdateResults); } }); thd.start();
			 */

		} catch (Exception ioe) {
		//	Log.e("EEEEEactivity", ioe.getMessage());
			// OnlyInfoTestFile(ioe.getMessage() + "123");
			finish();
			return;

		}
		setGuideResId(R.drawable.yindao02);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onPause() {
		// back键后响应
		super.onPause();
		// if (mGifEditView != null)
		// mGifEditView.disableView();
	}

	@Override
	public void onResume() {
		super.onResume();
		// Log.d(TAG, "Creating and seting view!!!!");
		// if(OpenCVLoader.initDebug())
		{
			// Log.d(TAG, "Creating and seting view!!!!");
			// mGifEditView.setOnTouchListener(GifActivity.this);
		//	mGifEditView.enableView();
			//mGifEditView.setVisibility(SurfaceView.VISIBLE);
		//	Log.e(TAG, "Gifavtivity  onResume Start");// /
		//	OnlyInfoTestFile("onresume");
			MainActivity.Start(TEST_FILE);/////
	//		MainActivity.SetSize(1);
			// mOpenCvCameraView.setCameraIndex(cameraIndex);
			// mOpenCvCameraView.
		}
		// OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this,
		// mLoaderCallback);
	}

	public void Begin() {
	}

	public void onDestroy() {
		isPause  = false;
		// MainActivity.Stop();

		

		// super.onDestroy();//////
	//	Log.e(TAG, "onDestroy!");
		
	//	OnlyInfoTestFile("onDestroy12345");
	//	MainActivity.SetSize(MainActivity.m_size);
		MainActivity.ReleaseImglist();
	//	OnlyInfoTestFile("onDestroy12345q");
		MainActivity.ReleaseBigShowImglist();
		//OnlyInfoTestFile("onDestroy12345k");
		MainActivity.Stop();
	//	OnlyInfoTestFile("onDestroy12345s");
		MainActivity.SetSize(MainActivity.m_size);
		//OnlyInfoTestFile("onDestroy12345m");

		 
		 
		 super.onDestroy();
		// if (mGifEditView != null)
		// mGifEditView.disableView();

		// gf2.destroy();

	}

	// ////////////////////////////////////
	private float getGap(float x0, float x1, float y0, float y1) {
		return (float) Math.pow(
				Math.pow((x0 - x1), 2) + Math.pow((y0 - y1), 2), 0.5);
	}

	// ///////////////////////////////

	public void HideBar() {
		if (viewVisableWhole == DragGridView.VISIBLE) {
			hView.setVisibility(DragGridView.INVISIBLE);
			// mDragGridViewWhole.setVisibility(DragGridView.INVISIBLE);
			mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
			viewVisableWhole = DragGridView.INVISIBLE;
		}
		if (viewVisableAdd == DragGridView.VISIBLE) {
			hView.setVisibility(DragGridView.INVISIBLE);
			mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
			viewVisableAdd = DragGridView.INVISIBLE;
		//	Log.e(TAG, "DragGridView.INVISIBLE");// /
		}
	}

	// ////////////////////////////////
	public boolean onTouch(View view, MotionEvent event) {
		// Toast toast = Toast.makeText(MainActivity.this, "GetGif return " +
		// "&&"+sFilePath+ "**" + retSave + " & " + nSaveType + "$$",
		// Toast.LENGTH_SHORT);
		// toast.show();///////
		int pointCount = event.getPointerCount();
		// Log.e(TAG, "onTouch!!!!pointCount" + pointCount);
		if (pointCount == 1) {

			// 接口函数返回值，判断是需要旋转，改变大小，还是移动

			handleTouch(event);// 旋转或改变大小

			// ///////////////////////////

			// /////////////////////////////
		}
		if (pointCount > 1) {
			// Toast toast = Toast.makeText(MainActivity.this, "pointCount is "
			// + pointCount, Toast.LENGTH_SHORT);
			// toast.show();///////
			int action = event.getAction() & MotionEvent.ACTION_MASK;

			float x0 = event.getX(event.getPointerId(0));
			float y0 = event.getY(event.getPointerId(0));

			float x1 = event.getX(event.getPointerId(1));
			float y1 = event.getY(event.getPointerId(1));

			float gap = getGap(x0, x1, y0, y1);
			switch (action) {
			case MotionEvent.ACTION_POINTER_DOWN:
				// case MotionEvent.ACTION_POINTER_1_DOWN:
				mGap = gap;

				// //////////////////

				mX = x0;
				mY = y0;

				// startAngle=angle(event);
				break;
			// case MotionEvent.ACTION_POINTER_1_UP:
			// mX = x1;
			// mY = y1;
			// break;
			case MotionEvent.ACTION_POINTER_UP:

				break;
			case MotionEvent.ACTION_MOVE: {
				// final float dgap = (gap - mGap) / mGap;
				// Log.d(TAG, "onTouch!!!!ACTION_MOVE" + pointCount);
				if (Math.abs(gap - mGap) <= 50)
					break;
			//	Log.e(TAG, "onTouch!!!!GAP%%%%" + mGap + "   " + gap);
				if (mGap == 0)
					mGap = 1;
				// float dgap = (gap) / mGap;

				// /////////////////////////////
				break;
			}
			}
		}

		return true;
	}

	// /////////////////////////////////////////////

	private void handleTouch(MotionEvent event) {

		int action = event.getAction() & MotionEvent.ACTION_MASK;

		int xx = (int) event.getX();
		int yy = (int) event.getY();
		// Log.e(TAG, "onTouch!!!!OnTouch%%%%" + xx + "$$" + yy);
		FixPoint(xx, yy);
		// Log.e("PickFromPhoto", "Touch!!!!  up " + " " + upx +" "+upy);///

		xx = originalImageOffsetX;
		yy = originalImageOffsetY;
		int n = MainActivity.TouchScreen(action, xx, yy);
	//	 OnlyInfoTestFile("TouchScreen  " + xx + "  " + yy);
		
		switch (action) {
		case MotionEvent.ACTION_DOWN:// 0
			// saveTouchPoint();
			// Toast toast = Toast.makeText(this, action + "%%" + xx + "%%" + yy +
				//	 "%%" + n, Toast.LENGTH_SHORT);
					// toast.show();
		/*	String ss; 
			 int num = MainActivity.GetInfo(0,0); 
			 ss = "$" + num + "$" +
					 MainActivity.GetInfo(0,1)+"$" + MainActivity.GetInfo(0,2) + "$" + MainActivity.GetInfo(0,3) + "$" +
					 MainActivity.GetInfo(0,4) + "$" + MainActivity.GetInfo(5,5); 
			// OnlyInfoTestFile(ss);
			 Toast toast = Toast.makeText(this, ss,
					 Toast.LENGTH_SHORT);
					 toast.show();*/

			break;
		case MotionEvent.ACTION_MOVE:// 2
			// Toast toast = Toast.makeText(this, action + "%%" + xx + "%%" + yy
			// + "%%" + n, Toast.LENGTH_SHORT);
			// toast.show();
			// Log.e(TAG, "onTouch!!!!ACTION_MOVE%%%%" + xx + "$$" + yy + "$$" +
			// n);
			// handleTouchMove();///
			break;
		case MotionEvent.ACTION_UP:// 1
			// 可以使用访问者模式这里让访问者获得当前角度
			break;
		}
	}

	// //////////////////////////////////////

	private String getPhotoFileName() {
		Date date = new Date(System.currentTimeMillis());
		SimpleDateFormat dateFormat = new SimpleDateFormat("000yyyyMMddHHmmss");
		return dateFormat.format(date);
	}

	// /////////////////////////////////////////////

	private void initialButton() {
		// TODO Auto-generated method stub
		Display display = getWindowManager().getDefaultDisplay();
		height = display.getHeight();
		width = display.getWidth();
		// Log.v("width  & height is:", String.valueOf(width) + ", " +
		// String.valueOf(height));

		// params.height = 50;
		// params.width = 50;
		// 设置边距 (int left, int top, int right, int bottom)
		// params.setMargins(10, height - 98, 0, 0);

		buttonBrowse = (Button) findViewById(R.id.Browse);
		// buttonSleep.setLayoutParams(params);
		// buttonDown.setVisibility(Button.INVISIBLE);
		buttonCancel = (Button) findViewById(R.id.Cancel);
		buttonEditAdd = (Button) findViewById(R.id.EditAdd);
		buttonSave = (Button) findViewById(R.id.Save);
		buttonEditWhole = (Button) findViewById(R.id.EditWhole);
		buttonPause = (Button) findViewById(R.id.PausePlay);
		buttonShare = (Button) findViewById(R.id.Share);
		// buttonEditBrowse = (Button) findViewById(R.id.EditBrowse);
		// buttonDown = (Button) findViewById(R.id.Down);

		// buttonDelete.setLayoutParams(params);
		buttonShare.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				bShare = true;
				SaveGif();
			//	SelectSenttoWeixin();
				
			}
		});
		buttonPause.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				isPause = !isPause;
				if(isPause)
				{
					buttonPause.setText(R.string.pause);
				}
				else
				{
					buttonPause.setText(R.string.play);
				}
				mGifEditView.SetPause(isPause);
			}
		});
		buttonBrowse.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(GifActivity.this,
						SelFileActivity.class);

				intent.putExtra("SelectType", 1);

				// intent.putExtra("floderName", mFloderList.get(arg2));

				startActivityForResult(intent, PICK_GIF_PHOTO_E);
			/*	String ss; 
				 int num = MainActivity.GetInfo(0,0); 
				 ss = "$" + num + "$" +
						 MainActivity.GetInfo(0,1)+"$" + MainActivity.GetInfo(0,2) + "$" + MainActivity.GetInfo(0,3) + "$" +
						 MainActivity.GetInfo(0,4) + "$" + MainActivity.GetInfo(5,5); 
				// OnlyInfoTestFile(ss);
				 Toast toast = Toast.makeText(GifActivity.this, ss,
						 Toast.LENGTH_SHORT);
						 toast.show();*/
			}
		});

		buttonCancel.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub

				// buttonCamera.setBackgroundResource(R.drawable.btn);
				// buttonSave.setText(R.string.capture);
				// sFilePath = path;
				WaitForThreadDone();
					
				MainActivity.Stop();
				
				MainActivity.Resumelist();
				mGifEditView.Begin(null);
				MainActivity.Start(MainActivity.TEST_FILE);
			//	MainActivity.SetSize(1);
				// buttonCamera.SetT

			}
		});

		buttonEditAdd.setOnClickListener(new OnClickListener() {
			// thd.start();

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub

				// ////////////////////
				// thd.start();
				/*
				 * Intent intent = new Intent();
				 * intent.setClass(MainActivity.this,
				 * LeftRightSlideActivity.class); startActivity(intent);
				 * //设置切换动画，从右边进入，左边退出
				 * overridePendingTransition(R.anim.in_from_right,
				 * R.anim.out_to_left);
				 */

				/*
				 * Rotate(30);
				 * 
				 * String ss; int num = GetInfo(5,5); ss = "$" + num + "$" +
				 * num; Toast toast = Toast.makeText(MainActivity.this, ss +
				 * "&&", Toast.LENGTH_SHORT); toast.show();//
				 */
				if (viewVisableAdd == DragGridView.VISIBLE) {
					hView.setVisibility(DragGridView.INVISIBLE);
					mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
					viewVisableAdd = DragGridView.INVISIBLE;
				//	Log.e(TAG, "DragGridView.INVISIBLE");// /
				} else {
					hView.setVisibility(DragGridView.VISIBLE);
					if (viewVisableWhole == DragGridView.VISIBLE) {
						mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
						viewVisableWhole = DragGridView.INVISIBLE;

					}
					InitDragViewAdd();
					mDragGridViewAdd.setVisibility(DragGridView.VISIBLE);
					viewVisableAdd = DragGridView.VISIBLE;

					// Log.e(TAG, "mDragGridViewAdd.getBottom  " +
					// mDragGridViewAdd.getBottom());///

					// Log.e(TAG, "mDragGridViewAdd.getTop  " +
					// mDragGridViewAdd.getTop());///

				}
				// /////////////////////
			}
		});
		buttonSave.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				// ////////////////////
				// Stop();
				/*
				 * Intent intent = new Intent();
				 * intent.setClass(MainActivity.this,
				 * LeftRightSlideActivity.class); startActivity(intent);
				 * //设置切换动画，从右边进入，左边退出
				 * overridePendingTransition(R.anim.in_from_right,
				 * R.anim.out_to_left);
				 */

				// ///////////////////以上为测试代码
                SaveGif();
             //   SendToWeiXin(filename);
		    //	MainActivity.Stop();
		    //	MainActivity.ReleaseImglist();
			//	finish();

				// /////////

			}
		});
		buttonEditWhole.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				// SetOperMode(24);
				if (viewVisableWhole == DragGridView.VISIBLE) {
					hView.setVisibility(DragGridView.INVISIBLE);
					// mDragGridViewWhole.setVisibility(DragGridView.INVISIBLE);
					mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
					viewVisableWhole = DragGridView.INVISIBLE;
				} else {
					hView.setVisibility(DragGridView.VISIBLE);
					if (viewVisableAdd == DragGridView.VISIBLE) {
						mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
						viewVisableAdd = DragGridView.INVISIBLE;

					}
					InitDragViewWhole();
					// mDragGridViewWhole.setVisibility(DragGridView.VISIBLE);
					mDragGridViewAdd.setVisibility(DragGridView.VISIBLE);
					viewVisableWhole = DragGridView.VISIBLE;

					// Log.e(TAG, "mDragGridViewWhole.getBottom  " +
					// mDragGridViewWhole.getBottom());///

					// Log.e(TAG, "mDragGridViewWhole.getTop  " +
					// mDragGridViewWhole.getTop());///

				}
			}
		});

		/*
		 * buttonPic.setOnClickListener(new OnClickListener() {
		 * 
		 * @Override public void onClick(View v) { // TODO Auto-generated method
		 * stub // InSertTxt("",nInsertX,nInsertY,6);//// Intent intent = new
		 * Intent(MainActivity.this, SelFileActivity.class);
		 * 
		 * intent.putExtra("SelectType", 0);
		 * 
		 * // intent.putExtra("floderName", mFloderList.get(arg2));
		 * 
		 * startActivityForResult(intent, PICK_PHOTO);
		 * 
		 * 
		 * 
		 * } });
		 */

	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {

		/*
		 * public static final int PICK_PHOTO = 125; public static final int
		 * INSERT_TEXT = 150; public static final int DANMU_TEXT = 130; public
		 * static final int NORMAL_TEXT = 135; public static final int
		 * PICK_FROM_PHOTO = 140;
		 */
		if (null == data) {
			super.onActivityResult(requestCode, resultCode, data);
			
	//		OnlyInfoTestFile("requestCode  "+requestCode + "  " + resultCode);
			
		//	Toast toast = Toast.makeText(this, "null data", Toast.LENGTH_SHORT);
			//toast.show();
			return;
		}
	//	OnlyInfoTestFile("requestCode  "+requestCode + "  " + resultCode);
		// Toast toast1 = Toast.makeText(this, "start", Toast.LENGTH_SHORT);
		// toast1.show();
		if (requestCode == PICK_PHOTO_E && resultCode == RESULT_OK) {

			Bundle bundle = data.getExtras();
			// 获取相机返回的数据，并转换为图片格式
			String filepath = (String) bundle.get("filepath");
			if (null == filepath) {
				super.onActivityResult(requestCode, resultCode, data);
			//	Toast toast = Toast.makeText(this, "null filepath",
				//		Toast.LENGTH_SHORT);
			//	toast.show();
				mGifEditView.SetPause(false);
				isPause = false;
				return;
			}

			// int n = InSertImg(filepath,nInsertX,nInsertY,0);

//			
			 
			 
			// InSertTxt(filepath,nInsertX,nInsertY,6);//////
			// Toast toast2 = Toast.makeText(this, filepath,
			// Toast.LENGTH_SHORT);
			// toast2.show();

			int n = MainActivity.InSertImg(filepath, nInsertX, nInsertY, 0);

		//	mGifEditView.SetPause(false);
			//isPlaying = false;

			bChanged = true;
			 
		//	OnlyInfoTestFile("InsertImg");
			
			

		} else if (requestCode == NORMAL_TEXT_E && resultCode == RESULT_OK) {
			Bundle bundle = data.getExtras();
			// 获取相机返回的数据，并转换为图片格式
			String ttt = (String) bundle.get("text");
			if (null == ttt) {
				super.onActivityResult(requestCode, resultCode, data);
				mGifEditView.SetPause(false);
				isPause = false;
				return;
			}

			int clr = bundle.getInt("color");
			int type = 4;
			boolean bdanmu = bundle.getBoolean("Danmu");//6 normal;4 danmu
			
			if(bdanmu)
				type = 4;
			else 
				type = 6;
		//	String name = getPhotoFileName();
			// String ext = nSaveType==0?".gif":".avi";
			String filename = TEMP_DIR + "txt" + ".png";
			ArrayList<String> sdata = new ArrayList<String>();
			/*
			 * for(int h=0;h<ttt.length();h+= 6) { String tmp; if(h+6 <
			 * ttt.length() - 1) tmp= ttt.substring(h, h+6); else tmp =
			 * ttt.substring(h, ttt.length() - 1); sdata.add(tmp); }
			 */
			sdata.add(ttt);
			// Toast toast = Toast.makeText(this, "NORMAL_TEXT " + filename,
			// Toast.LENGTH_SHORT);
			// toast.show();
			if (writeImage(filename, sdata, clr, 50)) {

				int ret = MainActivity.InSertTxt(filename, nInsertX, nInsertY,
						type);
				bChanged = true;
				// Toast toast = Toast.makeText(this, "NORMAL_TEXT  return " +
				// ret, Toast.LENGTH_SHORT);
				// toast.show();///
			}
		//	mGifEditView.SetPause(false);
		} else if (requestCode == DANMU_TEXT_E && resultCode == RESULT_OK) {
			//暂时不用这部分代码
			Bundle bundle = data.getExtras();
			// 获取相机返回的数据，并转换为图片格式
			String ttt = (String) bundle.get("text");
			if (null == ttt) {
				super.onActivityResult(requestCode, resultCode, data);
				mGifEditView.SetPause(false);
				isPause = false;
				return;
			}
			int clr = bundle.getInt("color");
		//	String name = getPhotoFileName();
			// String ext = nSaveType==0?".gif":".avi";
			String filename = TEMP_DIR + "txt" + ".png";
			ArrayList<String> sdata = new ArrayList<String>();
			/*
			 * for(int h=0;h<ttt.length();h+= 6) { String tmp; if(h+6 <
			 * ttt.length() - 1) tmp= ttt.substring(h, h+6); else tmp =
			 * ttt.substring(h, ttt.length() - 1); sdata.add(tmp); }
			 */
			sdata.add(ttt);
			// Toast toast = Toast.makeText(this, "DANMU_TEXT " + filename,
			// Toast.LENGTH_SHORT);
			// toast.show();
			if (writeImage(filename, sdata, clr, 50)) {
				MainActivity.InSertTxt(filename, nInsertX, nInsertY, 4);
				bChanged = true;
			}
		//	mGifEditView.SetPause(false);

		} else if (requestCode == PICK_FROM_PHOTO_E && resultCode == RESULT_OK) {
			Bundle bundle = data.getExtras();
			// Toast toast = Toast.makeText(this, "PICK_FROM_PHOTO",
			// Toast.LENGTH_SHORT);
			// toast.show();
			// 获取相机返回的数据，并转换为图片格式
			String filepath = (String) bundle.get("filepath");
			if (null == filepath) {
				super.onActivityResult(requestCode, resultCode, data);
			//	Toast toast2 = Toast.makeText(this, "null filepath",
				//		Toast.LENGTH_SHORT);
			//	toast2.show();

				mGifEditView.SetPause(false);
				isPause = false;
				return;
			}
			// Toast toast3 = Toast.makeText(this, filepath,
			// Toast.LENGTH_SHORT);
			// toast3.show();
			Intent intent1 = new Intent(GifActivity.this,
					PickFromPhotoActy.class);

			intent1.putExtra("path", filepath);

			// intent.putExtra("floderName", mFloderList.get(arg2));

			startActivityForResult(intent1, PICK_FROM_PHOTO_E);

			// mGifEditView.SetPause(false);

			// InSertKoutu(int x,int y,int wid,int hei,long matAddrGr);

			// InSertTxt(ttt,nInsertX,nInsertY,4);
		} else if (requestCode == PICK_GIF_PHOTO_E && resultCode == RESULT_OK) {
			Bundle bundle = data.getExtras();
			// Toast toast = Toast.makeText(this, "PICK_FROM_PHOTO",
			// Toast.LENGTH_SHORT);
			// toast.show();
			// 获取相机返回的数据，并转换为图片格式
			String filepath = (String) bundle.get("filepath");
			if (null == filepath) {
				super.onActivityResult(requestCode, resultCode, data);
				//Toast toast2 = Toast.makeText(this, "null filepath",
					//	Toast.LENGTH_SHORT);
			//	toast2.show();
				return;
			}
			try {

				// ff = new File(filepath);
				nSelType = TYPE_FILE;
				MainActivity.Stop();

				MainActivity.Start(TEST_FILE);// ////*/

				 File ff = new File(filepath);
					
				//	OnlyInfoTestFile(m_path);
					InputStream is = new BufferedInputStream(new FileInputStream(ff),
							32768);
				mGifEditView.Begin(is);
				nFrom = FROM_FILE;
				bChanged = false;

			} catch (Exception e) {
				// finish();
				super.onActivityResult(requestCode, resultCode, data);
				return;
			}
			// if(ff.canRead())
			// {

			// if(filename == " ")

			String name = getPhotoFileName();
			String ext = ".gif";
			filename = PHOTO_DIR + name + ext;// 为空则重新赋值，在打开新文件后重新赋值，否则就一直覆盖

			MainActivity.GetGif(filename);
			// }

			//Log.e(TAG, "mainactivity----JniStop  ");// //////
			// Begin();
		}else if ((requestCode == PICK_EMOTION || 
				requestCode == PICK_BABY || 
				requestCode == PICK_BIGSHOW || 
				requestCode == PICK_FLOWER ) && 
				resultCode == RESULT_OK)
		{
			/*PICK_EMOTION = 1610;
	public static final int PICK_BABY = 1620;
	public static final int PICK_BIGSHOW = 1630;
	public static final int PICK_FLOWER = 1630*/
			Bundle bundle = data.getExtras();
			// 获取相机返回的数据，并转换为图片格式
			String path = (String) bundle.get("Respath");
			
		//	OnlyInfoTestFile("Respath  "+path);
	//		Toast toast = Toast.makeText(this, "Respath  "+path, Toast.LENGTH_SHORT);
		//	toast.show();
			if (null == path) {
				super.onActivityResult(requestCode, resultCode, data);
				mGifEditView.SetPause(false);
				isPause = false;
				return;
			}

		
			
			if (requestCode == PICK_FLOWER) {

				InputHandMat( path);
			//	int ret = MainActivity.InSertTxt(filename, nInsertX, nInsertY,
				//		type);
				// Toast toast = Toast.makeText(this, "NORMAL_TEXT  return " +
				// ret, Toast.LENGTH_SHORT);
				// toast.show();///
			}
			else 
			{
				InputBigShowMattoPCV( path);
			}
		//	mGifEditView.SetPause(false);
			bChanged = true;
		} 

		super.onActivityResult(requestCode, resultCode, data);

	}

	// //////////////////////////////////////
	// /////////////////////////////
	public void InsertDatatoDatasrc(Object val, String txt,
			List<HashMap<String, Object>> dataSource) {
		HashMap<String, Object> itemHashMap = new HashMap<String, Object>();
		itemHashMap.put("item_image", val);
		itemHashMap.put("item_text", txt);
		dataSource.add(itemHashMap);
	}

	public void initDataSrcWhole() {
		
		InsertDatatoDatasrc(R.drawable.jian, "自动", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian2, "素描", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian3, "朦胧", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian1, "怀旧-黑白", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian4, "怀旧-泛黄", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian5, "风", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian6, "毛玻璃", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian7, "雕刻", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian8, "哈哈镜-放大", dataSourceListWhole);
		InsertDatatoDatasrc(R.drawable.jian9, "哈哈镜-挤压", dataSourceListWhole);
		

	}

	public void InitDragViewWhole() {

		final SimpleAdapter mSimpleAdapter = new SimpleAdapter(this,
				dataSourceListWhole, R.layout.grid_item, new String[] {
						"item_image", "item_text" }, new int[] {
						R.id.item_image, R.id.item_text });

		/*
		 * mDragGridViewWhole.setAdapter(mSimpleAdapter); //
		 * mDragGridViewWhole.rem
		 * 
		 * mDragGridViewWhole.setVisibility(DragGridView.INVISIBLE);
		 * viewVisableWhole = DragGridView.INVISIBLE;
		 * 
		 * 
		 * mDragGridViewWhole.setOnChangeListener(new OnChanageListener() {
		 */
		// mDragGridViewAdd.removeAllViews();
	//	Log.e(TAG, "mDragGridViewAdd.setAdapter");// /
		mDragGridViewAdd.setAdapter(mSimpleAdapter);
		// mDragGridViewWhole.rem
	//	Log.e(TAG, "setVisibility");// /
		// mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
		// viewVisableWhole = DragGridView.INVISIBLE;

		mDragGridViewAdd.setOnChangeListener(new OnChanageListener() {

			@Override
			public void onChange(int pos) {

			//	Log.e(TAG, "onChange whole  " + pos);// /
				if (pos == 0) {
					MainActivity.SetMode(0);//
					MainActivity.SetColor(12); // /
					bChanged = true;
				}

				if (pos > 0 && pos < 4) {
					MainActivity.SetMode(pos + 1);
					MainActivity.SetColor(12);
					bChanged = true;
				}
				if (pos == 4) {
					MainActivity.SetMode(1);
					MainActivity.SetColor(5);
					bChanged = true;
				}
				if (pos > 4) {
					MainActivity.SetColor(12);
					MainActivity.SetMode(pos);
					bChanged = true;
				}
			}
		});
	}
/*
	public void initDataSrcAdd() {

		// viewVisable = DragGridView.VISIBLE;
		InsertDatatoDatasrc(R.drawable.z, "无手绘", dataSourceListAdd);
		InsertDatatoDatasrc(R.drawable.z, "发大招儿", dataSourceListAdd);
		
		InsertDatatoDatasrc(R.drawable.z, "添加扣图", dataSourceListAdd);
		InsertDatatoDatasrc(R.drawable.z, "装饰", dataSourceListAdd);
		InsertDatatoDatasrc(R.drawable.z, "添加文字", dataSourceListAdd);
	//	InsertDatatoDatasrc(R.drawable.z, "添加弹幕", dataSourceListAdd);改为输文字的时候选择，默认为弹幕
		InsertDatatoDatasrc(R.drawable.z, "手写", dataSourceListAdd);
		
		InsertDatatoDatasrc(R.drawable.z, "添加照片", dataSourceListAdd);
//		InsertDatatoDatasrc(R.drawable.z, "录制局部区域", dataSourceListAdd);改为按钮

	}*/
	////////////////////////////////////////////
	public void initDataSrcAdd() 
	{
		InsertDatatoDatasrc(R.drawable.x11, "无手绘", dataSourceListAdd);
		InsertDatatoDatasrc(R.drawable.x12, "表情", dataSourceListAdd);
		InsertDatatoDatasrc(R.drawable.x13, "可爱宝贝", dataSourceListAdd);
		InsertDatatoDatasrc(R.drawable.x14, "放大招儿", dataSourceListAdd);
		InsertDatatoDatasrc(R.drawable.x15, "装饰", dataSourceListAdd);//
		InsertDatatoDatasrc(R.drawable.x16, "抠图", dataSourceListAdd);//也许该改个名字
		
		InsertDatatoDatasrc(R.drawable.x171, "文字", dataSourceListAdd);
	//	InsertDatatoDatasrc(R.drawable.z, "添加弹幕", dataSourceListAdd);改为输文字的时候选择，默认为弹幕
		InsertDatatoDatasrc(R.drawable.x181, "手写", dataSourceListAdd);
		
		InsertDatatoDatasrc(R.drawable.x19, "照片", dataSourceListAdd); 
		

	}
	
	/////////////////////////////////////////
	public void InitDragViewAdd() {
		// mDragGridViewAdd.removeAllViews();
		final SimpleAdapter mSimpleAdapterAdd = new SimpleAdapter(this,
				dataSourceListAdd, R.layout.grid_item, new String[] {
						"item_image", "item_text" }, new int[] {
						R.id.item_image, R.id.item_text });

		mDragGridViewAdd.setAdapter(mSimpleAdapterAdd);// /

		// mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
		// viewVisableAdd = DragGridView.INVISIBLE;
		mDragGridViewAdd.setOnChangeListener(new OnChanageListener() {

			@Override
			public void onChange(int pos) {

				// Log.e(TAG, "onChange add  " + pos);///
				// HideBar();
				/*PICK_EMOTION = 1610;
	public static final int PICK_BABY = 1620;
	public static final int PICK_BIGSHOW = 1630;
	public static final int PICK_FLOWER = 1630;*/
				switch (pos) {
				case 0:
					// 原图，无操作
					/*
					 * enum_OPER_EMPTY= 21, enum_SELECT_RECT= 22,
					 * enum_DRAW_WITHHAND = 23, enum_DRAW_WRITE = 24
					 */
					//HideBar();
					MainActivity.SetOperMode(21);
					break;
				case 1:
					//表情
					mGifEditView.SetPause(true);
					isPause = true;
					Intent intent0 = new Intent(GifActivity.this,
							SelResPicActivity.class);
						//	PickFromPhotoActy.class);

					// Intent intent1 = new Intent(MainActivity.this,
					// PickFromPhotoActy.class);
					
				//	OnlyInfoTestFile("face111emtions" );
					

					intent0.putExtra("dir", "face/emtions");

					// intent.putExtra("floderName", mFloderList.get(arg2));

				startActivityForResult(intent0, PICK_EMOTION);
					break;
				case 2:
					//宝贝
					mGifEditView.SetPause(true);
					isPause = true;
					Intent intent2 = new Intent(GifActivity.this,
							SelResPicActivity.class);

					// Intent intent1 = new Intent(MainActivity.this,
					// PickFromPhotoActy.class);

				//	OnlyInfoTestFile("face111babys" );
					intent2.putExtra("dir", "face/babys");

					// intent.putExtra("floderName", mFloderList.get(arg2));

				startActivityForResult(intent2, PICK_BABY);
					break;
				case 3:
					//大招儿
					mGifEditView.SetPause(true);
					isPause = true;
					Intent intent3 = new Intent(GifActivity.this,
							SelResPicActivity.class);

					// Intent intent1 = new Intent(MainActivity.this,
					// PickFromPhotoActy.class);

				//	OnlyInfoTestFile("face111bigshow" );
					intent3.putExtra("dir", "face/bigshow");

					// intent.putExtra("floderName", mFloderList.get(arg2));

				startActivityForResult(intent3, PICK_BIGSHOW);
					break;
				case 4:
					// 装饰
					mGifEditView.SetPause(true);
					isPause = true;
					Intent intent4 = new Intent(GifActivity.this,
							SelResPicActivity.class);

					// Intent intent1 = new Intent(MainActivity.this,
					// PickFromPhotoActy.class);

				//	OnlyInfoTestFile("face111flower" );
					intent4.putExtra("dir", "face/flower");

					// intent.putExtra("floderName", mFloderList.get(arg2));

				startActivityForResult(intent4, PICK_FLOWER);
				//	HideBar();
				//	MainActivity.SetOperMode(23);
					break;
				case 5:
					//抠图
					mGifEditView.SetPause(true);
					isPause = true;
					Intent intent1 = new Intent(GifActivity.this,
							SelFileActivity.class);

				//	 Intent intent1 = new Intent(GifActivity.this,
					// PickFromPhotoActy.class);

					intent1.putExtra("SelectType", 0);

					// intent.putExtra("floderName", mFloderList.get(arg2));

					startActivityForResult(intent1, PICK_FROM_PHOTO_E);
					// InSertKoutu(int x,int y,int wid,int hei,long matAddrGr);
					// 选择图片区域后插入
					break;
				case 6:
					//文字
					HideBar();
					mGifEditView.SetPause(true);
					isPause = true;
					// public static native void InSertTxt
					// (String js,int x,int y,int mode);
					Intent intent5 = new Intent(GifActivity.this,
							TextEditor.class);

					intent5.putExtra("SelectType", 0);

					// intent.putExtra("floderName", mFloderList.get(arg2));

					startActivityForResult(intent5, NORMAL_TEXT_E);
					break;
				case 7:
					//手写
					HideBar();
					MainActivity.SetOperMode(24);
					bChanged = true;
					break;
				case 8:
					// 插入图片
				//	HideBar();
					mGifEditView.SetPause(true);
					isPause = true;
					Intent intent = new Intent(GifActivity.this,
							SelFileActivity.class);

					intent.putExtra("SelectType", 0);

					// intent.putExtra("floderName", mFloderList.get(arg2));

					startActivityForResult(intent, PICK_PHOTO_E);

					// public static native int InSertImg
					// (String js,int x,int y,int mode);

					break;
				
				
				    
			/*	case 3:
					// public static native void InSertTxt
					// (String js,int x,int y,int mode);
					HideBar();
					mGifEditView.SetPause(true);
					Intent intent3 = new Intent(GifActivity.this,
							TextEditor.class);

					intent3.putExtra("SelectType", 0);

					// intent.putExtra("floderName", mFloderList.get(arg2));

					startActivityForResult(intent3, DANMU_TEXT_E);
					// 弹幕
					break;*/
				
				
					default:
						break;
				}
			}
		});
	}

	// ///////////////////////////////////
	/**
	 * @param path
	 *            文件保存路径
	 * @param data
	 *            保存数据
	 * */
	public boolean writeImage(String path, ArrayList<String> data, int clr,
			int tsize) {
		// public void writeImage(String path,String data,int clr){
		boolean ret = false;
		try {
			int height = data.size() * 100; // 图片高
			Bitmap bitmap = Bitmap.createBitmap(500, height,
					Bitmap.Config.ARGB_8888);
			Canvas canvas = new Canvas(bitmap);
			canvas.drawColor(Color.BLACK); // 背景颜色

			Paint p = new Paint();

			p.setColor(clr); // 画笔颜色
			p.setTextSize(tsize); // 画笔粗细
			for (int i = 0; i < data.size(); i++) {
				canvas.drawText(data.get(i), 0, (i + 1) * 100, p);
				// canvas.drawText(data, 20, 20, p);
			}

			// Log.e("path", path);
			// 将Bitmap保存为png图片
			 File f = new File(TEMP_DIR, "txt.png");
			   if (f.exists()) {
			    f.delete();
			   }
			 
			FileOutputStream out = new FileOutputStream(f);
			ret = bitmap.compress(Bitmap.CompressFormat.PNG, 90, out);
			// Log.e("done", "done");

		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return false;
		}
		return ret;
	}

	// //////////////////////////////////////

	// //////////////////////////

	private void CreateTestFile() {
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

	private void WriteTestInfo(String info) {
	/*	try {
			output.write(info + "\n");
		} catch (IOException e) {
			Log.i("IOException===", e.getMessage());
			// ((TextView)findViewById(R.id.textview1)).setText("IOException==="
			// + e.getMessage());
		}*/

	}

	private void CloseTestFile() {
	/*	try {
			output.close();
		} catch (IOException e) {
			Log.i("IOException===", e.getMessage());
			// ((TextView)findViewById(R.id.textview1)).setText("IOException==="
			// + e.getMessage());
		}*/
	}

	private void OnlyInfoTestFile(String info) {
		/*
		 * CreateTestFile(); WriteTestInfo(info); CloseTestFile();
		 */
	//	WriteTestInfo(info);
	//	MainActivity.OnlyInfoTestFile(info);////
	}
	/////////

	// //////////////////////////////////////

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub

		// setRequestedOrientation(orientMode);//设定需要的朝向，横或者竖
		// Toast.makeText(this,"现在是竖屏", Toast.LENGTH_SHORT).show();
	//	Log.e("config", "onConfigurationChanged");
		super.onConfigurationChanged(newConfig);
		if (newConfig.orientation == this.getResources().getConfiguration().ORIENTATION_PORTRAIT) {
			// count=2;
			// Toast.makeText(this,"现在是竖屏", Toast.LENGTH_SHORT).show();
			//Log.e("config", "ORIENTATION_PORTRAIT");

		} else if (newConfig.orientation == this.getResources()
				.getConfiguration().ORIENTATION_LANDSCAPE) {
			// count=3;
			// Toast.makeText(this,"现在是横屏", Toast.LENGTH_SHORT).show();
		//	Log.e("config", "ORIENTATION_LANDSCAPE");

		}

		// CreateFood(sql,foodtypeid);
		// newConfig.orientation=this.getResources().getConfiguration().ORIENTATION_PORTRAIT;

		//
	}

	@Override
	public void onGifEditViewStarted(int width, int height, String path) {
		// TODO Auto-generated method stub

	//	Toast toast2 = Toast.makeText(GifActivity.this, "onGifEditViewStarted " + height,
		//		 Toast.LENGTH_SHORT);
	//	toast2.show();
	}

	@Override
	public void onGifEditViewStopped() {
		// TODO Auto-generated method stub

	//	CloseTestFile();
	}

	@Override
	public void onGifOneRoundFinished() {

		// ///////////////////////////
		Message msg = new Message();
		msg.what = FINISH_ONE_ROUND;
		Bundle bundle = new Bundle();
		// bundle.putString("text1","大明的消息传递参数的例子！"); //往Bundle中存放数据
		// bundle.putString("text2","Time：2011-09-05"); //往Bundle中put数据
		msg.setData(bundle);// mes利用Bundle传递数据
		GifActivity.this.mHandler.sendMessage(msg);// 用activity中的handler发送消息

	}

	public void GifOneRoundFinished() {
		// /////////////////////////////
		// OnlyInfoTestFile("SaveGif111");
		//
	//	Log.e(TAG, "Saved file " + filename);
		sFilePath = filename;
	//	OnlyInfoTestFile("doInBackground  " + filename);
		/*
		 * IsGettingGif(); public static native int GetImgListSize();
		 */
		// OnlyInfoTestFile("MainActivity  getgif  " +
		// MainActivity.IsGettingGif() + "  size  " +
		// MainActivity.GetImgListSize());
		
	//	n = MainActivity.GetGif(filename);//不在这里保存
		
	//	 OnlyInfoTestFile("onroundsave");

		
			
				do {
					try {
						if (!mGifEditView.thdOver) {
							Thread.sleep(10);
						//	 OnlyInfoTestFile("AAAThreadsleep");
						}
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();

					//	OnlyInfoTestFile("exception  " + e.getMessage());
						;
					}

				} while (!mGifEditView.thdOver);
			//	OnlyInfoTestFile("Stop  again");

			nSelType = TYPE_LIST;
			if(bForceQuit)
				return;
			else
				mGifEditView.mStopThread = false;
				MainActivity.Stop();
			//	if(!mGifEditView.mFromFile)
				//    WaitFor3Seconds();
		//	MainActivity.ReleaseImglist();
				// 
				MainActivity.Start(TEST_FILE);// ////
			//	OnlyInfoTestFile("GetGif  again");
				MainActivity.GetGif(filename);
				// Toast toast = Toast.makeText(GifActivity.this,
				// "GetGif thread  "+filename,Toast.LENGTH_SHORT);
				// toast.show();
		//		OnlyInfoTestFile("Begin  again");
				mGifEditView.Begin(null);
				// Toast toast2 = Toast.makeText(GifActivity.this,
				// "begin thread "+filename,Toast.LENGTH_SHORT);
				// toast2.show();
				// OnlyInfoTestFile("Begin  end");
				// mGifEditView.thdOver =

			
		
	}

	public void SaveGif() {
		/*
		 * String name= getPhotoFileName(); String ext = ".gif"; final String
		 * filename=PHOTO_DIR+name + ext;
		 */

		/*
		 * if(isClick == false) { isClick = true; MainActivity.GetGif(filename);
		 * 
		 * // buttonCamera.setBackgroundResource(R.drawable.btn); //
		 * buttonSave.setText(R.string.caping); } else
		 */
		{
			// MainActivity.GetGif(filename);
			// ////////////////////////////////////////////

			new AsyncTask<Void, Void, Void>() {
				private ProgressDialog calibrationProgress;

				@Override
				protected void onPreExecute() {

					// OnlyInfoTestFile("onPreExecute");
					Resources res = getResources();
					// OnlyInfoTestFile("onPreExecute 11");
					calibrationProgress = new ProgressDialog(GifActivity.this);

					// OnlyInfoTestFile("onPreExecute  22");
					calibrationProgress
							.setTitle(res.getString(R.string.saving));

					// OnlyInfoTestFile("onPreExecute  33");
					calibrationProgress.setMessage(res
							.getString(R.string.please_wait));

					// OnlyInfoTestFile("onPreExecute  44");
					calibrationProgress.setCancelable(false);

					// OnlyInfoTestFile("onPreExecute  55");
					calibrationProgress.setIndeterminate(true);
					// OnlyInfoTestFile("onPreExecute  show");
					calibrationProgress.show();

					// OnlyInfoTestFile("onPreExecute  end");
				}

				@Override
				protected Void doInBackground(Void... arg0) {
					// mCalibrator.calibrate();

					// OnlyInfoTestFile("doInBackground  end  " + n);
					// OnlyInfoTestFile("doInBackground  end  " + n);
					// buttonCamera.setText(R.string.capture);//
			//		GifOneRoundFinished();
				//	int tmp1,tmp2;
					if(nChangeSize == 1)
						MainActivity.SetSize(1);
					if(nFrom == FROM_FILE)
					{
					//	OnlyInfoTestFile("doInBackground  end  1");
						MainActivity.SetDelay(nDelay);
						MainActivity.SetEvery(1);
					}
				//	OnlyInfoTestFile("doInBackground  end  2" );
					WaitForThreadDone();
				//	MainActivity.SetEvery(1);
					int nn = MainActivity.GetGif(filename);
				//	OnlyInfoTestFile("GetGif"+nn );
				//	OnlyInfoTestFile("isPause" + isPause);
					bChanged = false;
					if(nFrom == FROM_FILE)
					{
						MainActivity.SetDelay(MainActivity.nDelay);
						MainActivity.SetEvery(MainActivity.nEvery);
						
					}
					MainActivity.SetSize(MainActivity.m_size);
					
					/////////////////
					MainActivity.Stop();
					if(bShare)
					{
						
					  //  SelectSenttoWeixin();
						Message msg = new Message();
						msg.what = READY_TO_SHARE;
						Bundle bundle = new Bundle();
						// bundle.putString("text1","大明的消息传递参数的例子！"); //往Bundle中存放数据
						// bundle.putString("text2","Time：2011-09-05"); //往Bundle中put数据
						msg.setData(bundle);// mes利用Bundle传递数据
						GifActivity.this.mHandler.sendMessage(msg);
						OnlyInfoTestFile("msgsend");
					//    SendImgToWeiXin(filename);
					    bShare = false;
					}
					//	if(!mGifEditView.mFromFile)
						//    WaitFor3Seconds();
				//	MainActivity.ReleaseImglist();
						// 
						MainActivity.Start(TEST_FILE);// ////
					//	OnlyInfoTestFile("GetGif  again");
						MainActivity.GetGif(filename);
						// Toast toast = Toast.makeText(GifActivity.this,
						// "GetGif thread  "+filename,Toast.LENGTH_SHORT);
						// toast.show();
				//		OnlyInfoTestFile("Begin  again");
						mGifEditView.mStopThread = false;
						mGifEditView.Begin(null);
					
					/////////////////////
					
					return null;
				}

				@Override
				protected void onPostExecute(Void result) {
					calibrationProgress.dismiss();

				}
			}.execute();

			// ////////////////////////////////////////////
			// GetGif(filename);
			isClick = false;
		}
	}

	@Override
	public Bitmap onGifFrame(Bitmap inputFrame,float nScale) {
		// TODO Auto-generated method stub
		Bitmap copydstBmp = inputFrame.copy(Config.ARGB_8888, true);
	//	OnlyInfoTestFile("onGifFramemRgba");
		mRgba = new Mat(inputFrame.getHeight(), inputFrame.getWidth(),
				CvType.CV_8UC4);
		mRgb = new Mat(inputFrame.getHeight(), inputFrame.getWidth(),
				CvType.CV_8UC3);
		Mat mRgbF = new Mat((int)(inputFrame.getHeight()*nScale), (int)(inputFrame.getWidth()*nScale),
				CvType.CV_8UC3);
	//	OnlyInfoTestFile("bitmapToMat");
		Utils.bitmapToMat(copydstBmp, mRgba);// (modified, mTmpBitmap);
		
		// MainActivity.SetMode(2);
		//test
	//	OnlyInfoTestFile("PerfLiveFunc");
	//	MainActivity.SetImgBtnPath(MainActivity.TEMP_DIR + "test");///、、、
		if(nScale == 0)
		    nScale = 1;
		int nn = MainActivity.PerfLiveFunc(-1, nScale,mRgb.getNativeObjAddr(),
				mRgba.getNativeObjAddr(),mRgbF.getNativeObjAddr());//头一个参数为1，表明是编辑状态
/////
	//	 OnlyInfoTestFile("PerfLiveFunc222");////

	//	Log.e("EEEEEEEEEEEEE", "mRgbF.cols() " + mRgbF.cols() + "  " + mRgbF.rows() + " " + nScale);

		Bitmap cBmp = Bitmap.createBitmap(mRgbF.cols(), mRgbF.rows(), Config.ARGB_8888);
		Utils.matToBitmap(mRgbF, cBmp);

		mRgba.release();
		mRgb.release();
		mRgbF.release();
		copydstBmp.recycle();

		return cBmp;
	}

	// ///////////////////////////////////

	public void FixPoint(float downx2, float downy2) {
		// ImageView imageView = (ImageView)findViewById(R.id.photoview);
		// Drawable drawable = mGifEditView.getDrawable();
		// android.graphics.Rect imageBounds = drawable.getBounds();

		// Log.e("PickFromPhoto", "FixPoint Start  " );///
		// float[] values = new float[9];
		// Matrix max = imageView.getImageMatrix();
		// max.getValues(values);
		if (mGifEditView == null)
			return;
		Rect rt = mGifEditView.getRect();
		if(rt == null)
			return;
		/////////////////
		float left = rt.left;
		float top = rt.top;
		float right = rt.right;
		float bottom = rt.bottom;

		// 使用fitXY
		float heightRatio = mGifEditView.getImgHeight() / (bottom - top);
		float widthRatio = mGifEditView.getImgWidth() / (right - left);

		// 获取图像边界值
		float scaledImageOffsetX = (downx2 - left);
		float scaledImageOffsetY = (downy2 - top);

		// Log.e("PickFromPhoto", "scaledImageOffsetX   " + " " +
		// scaledImageOffsetX +" "+scaledImageOffsetY);///

		// 根据你图像的缩放比例设置
		originalImageOffsetX = (int) (scaledImageOffsetX * widthRatio);
		originalImageOffsetY = (int) (scaledImageOffsetY * heightRatio);

		// Log.e("PickFromPhoto", "downx2   " + " " + downx2 +" "+downy2);///

		// Log.e("PickFromPhoto", "originalImageOffsetX   " + " " +
		// originalImageOffsetX +" "+originalImageOffsetY);///

	}

	public void Stopped() {
		mRgba.release();
		mRgb.release();

		if (MainActivity.matKoutua != null)
			MainActivity.matKoutua.release();
		if (MainActivity.matKoutu != null)
			MainActivity.matKoutu.release();
		// mGray.release();

	//	Log.e(TAG, "view stopped!");
		// String sss = " Going to stoping!!!!!!!!!!!";
		// Log.d(TAG, sss);
		// Stop();
		// String ss = " is stoping!!!!!!!!!!!";
		// Log.d(TAG, ss);
	}
	
	
	/////////////////////////////////
    @Override
    public void onBackPressed() {
    	if(bChanged == true)
    	{
    	//	OnlyInfoTestFile("onBackPressed");
	new AlertDialog.Builder(this).setTitle(R.string.verifyquit)
		.setIcon(android.R.drawable.ic_dialog_info)
		.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
 
		    @Override
		    public void onClick(DialogInterface dialog, int which) {
			// 点击“确认”后的操作
		    /*	OnlyInfoTestFile("setPositiveButton");
		    	SaveGif();
		    	MainActivity.Stop();
		    	MainActivity.ReleaseImglist();
		    	
		    	*/
		    	
		//	GifActivity.this.finish();
		    	bForceQuit = true;
		    	
		        //	OnlyInfoTestFile("backpress 2222 " + mGifEditView.mStopThread + "  " + mGifEditView.thdOver);
		    		
		        	WaitForThreadDone();
		       // 	CloseTestFile();
		    	GifActivity.super.onBackPressed();
 
		    }
		})
		.setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
 
		    @Override
		    public void onClick(DialogInterface dialog, int which) {
			// 点击“返回”后的操作,这里不设置没有任何操作
		    	return;
		    }
		}
		).show();
    	}
    	else
    	{
    		WaitForThreadDone();
    	//	CloseTestFile();
    		super.onBackPressed();
    	}
    	
    //	
			

				
    	//	     
    	
	// super.onBackPressed();
    }

	@Override
	public void onTestInfo(String ss) {
		// TODO Auto-generated method stub
	//	OnlyInfoTestFile(ss);
	}

	@Override
	public Bitmap onGifFrameBynum(int n,boolean bp) {
		// TODO Auto-generated method stub
		int nsize = MainActivity.GetImgListSize();
		int nwidth =MainActivity.GetImgListWidth();
		int nheight = MainActivity.GetImgListHeight();
		
		if(nsize<=0 || nwidth <= 0 || nheight <= 0)
		{
		//	OnlyInfoTestFile("null");
		    return null;
		}
		mRgb = new Mat(MainActivity.GetImgListHeight(), MainActivity.GetImgListWidth(),
				CvType.CV_8UC3);
	//	Utils.bitmapToMat(copydstBmp, mRgba);// (modified, mTmpBitmap);
		// MainActivity.SetMode(2);
		//test
	//	MainActivity.SetImgBtnPath(MainActivity.TEMP_DIR + "test");///、、、
		
		int pp = bp?1:0;
		//OnlyInfoTestFile("PerfEditFunc1"+bp);
		int nn = MainActivity.PerfEditFunc(n, pp,mRgb.getNativeObjAddr());//头一个参数为1，表明是编辑状态

		 //OnlyInfoTestFile("PerfEditFunc2"+bp);
		// MainActivity.IsGettingGif() + "  size  " +
		// MainActivity.GetImgListSize());

		//Log.e("EEEEEEEEEEEEE", "return == " + nn);

		Bitmap cBmp = Bitmap.createBitmap(nwidth, nheight, Config.ARGB_8888);
		Utils.matToBitmap(mRgb, cBmp);

	//	mRgba.release();
		mRgb.release();
	//	copydstBmp.recycle();

		return cBmp;
	}

	public void WaitForThreadDone()
	{
		mGifEditView.mStopThread = true;
    //	bForceQuit = true;
				do {
					try {
						if (!mGifEditView.thdOver) {
							Thread.sleep(300);
							// OnlyInfoTestFile("Thread.sleep");
						}
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();

					//	OnlyInfoTestFile("exception  " + e.getMessage());
						;
					}

				} while (!mGifEditView.thdOver);
	}
	private void InputBigShowMattoPCV(String path)
	{
		
		int nn = -1000;
	//	Toast toast = Toast.makeText(this, path, Toast.LENGTH_SHORT);
		//toast.show();
		
		try {
			GifDecoder lGifDecoder = new GifDecoder();
			InputStream is=this.getAssets().open(path);
		      
		
		InputStream bis = is;
		if (!BufferedInputStream.class.isInstance(bis))
			bis = new BufferedInputStream(is, 32768);
		// decoded = false;
	//	OnlyInfoTestFile("BufferedInputStream ");
		
		
		lGifDecoder.read(bis);
		
		if(lGifDecoder == null)
			return;
		
        lGifDecoder.complete();
		
		int n = lGifDecoder.getFrameCount();
		
		
		
		for(int i=1;i<=n;i++)
		{
			Bitmap mTmp = lGifDecoder.getFrame(i);
		//	Bitmap cBmp = Bitmap.createBitmap(mTmp.getHeight(), mTmp.getWidth(), Config.ARGB_8888);
			
			saveBitmap(mTmp,TEMP_BMP );////
		/*	Mat lRgba = new Mat(mTmp.getHeight(), mTmp.getWidth(),
					CvType.CV_8UC4);
			Mat lRgb = new Mat(mTmp.getHeight(), mTmp.getWidth(),
					CvType.CV_8UC3);
			
			//Mat ll = new Mat();
			Utils.bitmapToMat(mTmp, lRgba);// (modified, mTmpBitmap);*/
		//	nn = MainActivity.InputBigShowMat(lRgba.getNativeObjAddr(),lRgb.getNativeObjAddr(),nBigShowNum);
		nn = 	MainActivity.InputBigShowByPath(TEMP_DIR+TEMP_BMP, nBigShowNum);
		//	OnlyInfoTestFile("MainActivity.InputBigShowMat ret " + nn);
			
		
		//	Toast toast1 = Toast.makeText(this, "MainActivity.InputBigShowMat ret " + nn, Toast.LENGTH_SHORT);
			//toast1.show();
		//	lRgb.release();
			MainActivity.SetBigShowId(nBigShowNum);
			MainActivity.SetOperMode(25);
		//	mTmp.recycle();
		}
		
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		//	Toast toast = Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT);
			//toast.show();
		//	return;
		}
		nBigShowNum++;
	//	Toast toast1 = Toast.makeText(this, "lGifDecoder.getFrameCount " + nn, Toast.LENGTH_SHORT);
		//toast1.show();
		return;
		
	}
	
	public void InputHandMat(String path)
	{
//		Toast toast = Toast.makeText(this, "hand  "+path, Toast.LENGTH_SHORT);
	//	toast.show();
		try
		{
		Bitmap bm = BitmapFactory.decodeStream(this.getAssets().open(path));
      
    	   
       Mat lmatHanda = new Mat(bm.getHeight(), bm.getWidth(), CvType.CV_8UC4);;
       Mat lmatHand = new Mat(bm.getHeight(), bm.getWidth(), CvType.CV_8UC3);;
		Utils.bitmapToMat(bm, lmatHanda);
		
       int n = MainActivity.SetHandImgMat(lmatHand.getNativeObjAddr(),lmatHanda.getNativeObjAddr());
       bm.recycle();
       lmatHanda.release();
       lmatHand.release();
       MainActivity.SetOperMode(23);
		}
		catch(Exception e)
		{
			
		}
   // 
	}
	////////////////////////////////
	
	
	/** 保存方法 */
	  public static void saveBitmap(Bitmap bm,String path ) {
	//   Log.e(TAG, "保存图片");
	   File f = new File(TEMP_DIR, path);
	   if (f.exists()) {
	    f.delete();
	   }
	//   else
		//   f.mkdirs();
	/*   try {
		f.createNewFile();
	} catch (IOException e1) {
		// TODO Auto-generated catch block
		return;
	}*/
	   try {
	    FileOutputStream out = new FileOutputStream(f);
	    bm.compress(Bitmap.CompressFormat.PNG, 90, out);
	    out.flush();
	    out.close();
	 //   Log.i(TAG, "已经保存");
	   } catch (FileNotFoundException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	   } catch (IOException e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	   }

	 }
////////////////////////////////////
	  
	  public void WaitFor3Seconds()
	  {
		  new AsyncTask<Void, Void, Void>() {
				private ProgressDialog calibrationProgress;

				@Override
				protected void onPreExecute() {

					// OnlyInfoTestFile("onPreExecute");
					Resources res = getResources();
					// OnlyInfoTestFile("onPreExecute 11");
					calibrationProgress = new ProgressDialog(GifActivity.this);

					// OnlyInfoTestFile("onPreExecute  22");
					calibrationProgress
							.setTitle(res.getString(R.string.saving));

					// OnlyInfoTestFile("onPreExecute  33");
					calibrationProgress.setMessage(res
							.getString(R.string.please_wait));

					// OnlyInfoTestFile("onPreExecute  44");
					calibrationProgress.setCancelable(false);

					// OnlyInfoTestFile("onPreExecute  55");
					calibrationProgress.setIndeterminate(true);
					// OnlyInfoTestFile("onPreExecute  show");
					calibrationProgress.show();

					// OnlyInfoTestFile("onPreExecute  end");
				}

				@Override
				protected Void doInBackground(Void... arg0) {
					try {
						Thread.sleep(3*1000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					
					return null;
				}

				@Override
				protected void onPostExecute(Void result) {
					calibrationProgress.dismiss();

				}
			}.execute();
	  }
	public void SendTxtToWeiXin(String path)
	{
		String text = "ouguan!!!";
		
		// 初始化一个WXTextObject对象
	//	Log.e("gifactivity","SendTxtToWeiXin 11" );
		WXTextObject textObj = new WXTextObject();
		textObj.text = text;

		// 用WXTextObject对象初始化一个WXMediaMessage对象
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
	//	Log.e("gifactivity","SendTxtToWeiXin 22" );
		// 发送文本类型的消息时，title字段不起作用
		// msg.title = "Will be ignored";
		msg.description = text;

		// 构造一个Req
		SendMessageToWX.Req req = new SendMessageToWX.Req();
	//	Log.e("gifactivity","SendTxtToWeiXin 33" );
		req.transaction = buildTransaction("text"); // transaction字段用于唯一标识一个请求
		req.message = msg;
	//	Log.e("gifactivity","SendTxtToWeiXin 44" );
		req.scene =  scene;//SendMessageToWX.Req.WXSceneSession;//SendMessageToWX.Req.WXSceneTimeline ;
		
		// 调用api接口发送数据到微信
		//Log.e("gifactivity","SendTxtToWeiXin 55" );
		boolean bb = api.sendReq(req);
		
	//	Log.e("gifactivity","api.sendReq return " + bb);
	}
	public void SendBmpToWeiXin()
	{
		final WXAppExtendObject appdata = new WXAppExtendObject();
		final String path = TEMP_DIR + "e.jpg";
		appdata.fileData = Util.readFromFile(path, 0, -1);
		appdata.extInfo = "this is ext info";

		final WXMediaMessage msg = new WXMediaMessage();
		msg.setThumbImage(Util.extractThumbNail(path, 150, 150, true));
		msg.title = "Hi";
		msg.description = "test info from GIF+";
		msg.mediaObject = appdata;
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("appdata");
		req.message = msg;
		req.scene = scene;
		boolean bb=  api.sendReq(req);
	//	OnlyInfoTestFile("sendReq" + bb);
	}
	public void SendGifToWeiXin(String path)
	{
		File file = new File(path);
	//	Log.e("gifactivity!!&&&!",path);
		if (!file.exists()) {
		//	String tip = SendToWXActivity.this.getString(R.string.send_img_file_not_exist);
			//Toast.makeText(SendToWXActivity.this, tip + " path = " + path, Toast.LENGTH_LONG).show();
			return;
		}
	//	OnlyInfoTestFile("leng" +file.length());
		if(file.length() > WEIXIN_MAXSIZE)
		{
			Toast toast = Toast.makeText(this, R.string.file_too_big, Toast.LENGTH_SHORT);
				toast.show();
		//	return;
		}
	//	Log.e("gifactivity!!&&&!","fgfgfg");
	//	OnlyInfoTestFile("SendImgToWeiXin1" );
	///	
		Bitmap bmp = BitmapFactory.decodeFile(path);
		
		saveBitmap(bmp,"tt.bmp" );
		bmp.recycle();
		String THUMB_PATH = TEMP_DIR + "tt.bmp";
	/*	WXEmojiObject emoji = new WXEmojiObject();
		emoji.emojiPath = path;
		OnlyInfoTestFile("SendImgToWeiXin1" );
		WXMediaMessage msg = new WXMediaMessage(emoji);
		msg.title = "Emoji Title";
		msg.description = "Emoji Description";
		msg.thumbData = Util.readFromFile(THUMB_PATH, 0, (int) new File(THUMB_PATH).length());
*/
		WXEmojiObject emoji = new WXEmojiObject();
	//	OnlyInfoTestFile("SendImgToWeiXin2" );
		emoji.emojiData = Util.readFromFile(path, 0, (int) new File(path).length());
		WXMediaMessage msg = new WXMediaMessage(emoji);
	//	OnlyInfoTestFile("SendImg"+(emoji.emojiData==null) );
		msg.title = "Emoji Title";
		msg.description = "Emoji Description";
		msg.setThumbImage(Util.extractThumbNail(path, 150, 150, true));
	//	msg.thumbData = Util.readFromFile(THUMB_PATH, 0, (int) new File(THUMB_PATH).length());
	//	OnlyInfoTestFile("sendReqthum" + (msg.thumbData==null));
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("emoji");
		req.message = msg;
		req.scene = scene;
		boolean bb=  api.sendReq(req);
	//	OnlyInfoTestFile("sendReq" + bb);
		
		
	}
	private void SelectSenttoWeixin()
	{
		
		// SelectSenttoWeixin();
	//	OnlyInfoTestFile("SelectSenttoWeixin");
		    
		MMAlert.showAlert(GifActivity.this, getString(R.string.send_appdata), 
				GifActivity.this.getResources().getStringArray(R.array.send_appdata_item),
				null, new MMAlert.OnAlertSelectId(){

				@Override
				public void onClick(int whichButton) {
					switch(whichButton){
					case MMAlertSelect1:
						scene = SendMessageToWX.Req.WXSceneTimeline;
					//	String ss = PHOTO_DIR + "00020150611195836.gif";
						//Log.e("gifactivity!!!",ss);
					//	SendImgToWeiXin(ss);
					//	SendTxtToWeiXin("  ");
						SendGifToWeiXin(filename);
							break;
					case MMAlertSelect2: {
						scene =   SendMessageToWX.Req.WXSceneSession;
					//	String sss = PHOTO_DIR + "00020150612022529.gif";
						//Log.e("gifactivity!!!",sss);
					//	SendBmpToWeiXin();
						SendGifToWeiXin(filename);
					//	SendGifToWeiXin(sss);
				//		SendTxtToWeiXin("  ");
						break;
					}
					
					default:
						break;
					}
				}
				/////
			});
	}
	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
	
	/////////////////////////////////
}
