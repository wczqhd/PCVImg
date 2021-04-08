package com.gifplus.pcvimg;

import java.io.FileOutputStream;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.OpenCVLoader;
import org.opencv.android.Utils;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

import com.gifplus.pcvimg.DragGridView.OnChanageListener;

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
import android.graphics.PointF;
import android.media.MediaRecorder;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.util.FloatMath;
import android.util.Log;
import android.view.Display;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.widget.Button;
import android.widget.HorizontalScrollView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class RecordActivity extends BaseActivity implements CvCameraViewListener2, View.OnTouchListener  {
	//////////////////////////////////////////////

/////////////////////////////////////////
    private Button buttonCamera, buttonEditAdd, buttonSwitchCam, buttonEditWhole, buttonEditBrowse, buttonDown,buttonCancel;
	private Animation animationTranslate, animationRotate, animationScale;
	private static int width, height;
	private static Boolean isClick = false;
	
	public static final int PICK_PHOTO = 125;
	public static final int INSERT_TEXT = 150;
	public static final int DANMU_TEXT = 130;
	public static final int NORMAL_TEXT = 135;
	public static final int PICK_FROM_PHOTO = 140;
	public static final int PICK_GIF_PHOTO = 160;
	
	private int nInsertX,nInsertY;
	private int nSaveType = 0;
    /////////////////////////////////////////
	
	
    private static final String  TAG = "PVCImg::Activity";

    private CameraBridgeViewBase mOpenCvCameraView;
  //  private JniUtility mUtil;
 
    private int                    mViewMode;
    private Mat                    mRgba;
    private Mat                    mRgb;
    
    private Mat                    mRgbaT;
    private Mat                    mRgbT;
    private Mat                     matHanda;
    private Mat                     matHand;
 //   private Mat                    mIntermediateMat;
    private Mat                    mGray;
    
    private Bitmap mTmpBitmap;
  //  private MenuItem             mItemHideNumbers;
   // private MenuItem             mItemStartNewGame;


    private int                  mGameWidth;
    private int                  mGameHeight;
    
    private float mX = 0;
	private float mY = 0;
	private float mGap = 0;
	
///////////////////////////////////////
	
	///////////////////////////////
    
    private final static float MIN_DEGREE = 0f;
	private final static float MAX_DEGREE = 360f;

	
	private float saveX; // 当前保存的x
	private float saveY; // 当前保存的y
	private float curTouchX; // 当前触屏的x
	private float curTouchY; // 当前触摸的y
	private float centerX; // 中心点x
	private float centerY; // 中心点y
	private float curDegree; // 当前角度
	private float changeDegree;

    
     private int retSave = -100;
    private String sFilePath;
    public  static String minfo;
    private double startAngle;//开始角度
 //   private int  mnProcess = 0;  
    ///////////////////////////////////////////
    private List<HashMap<String, Object>> dataSourceListWhole = new ArrayList<HashMap<String, Object>>();
    private List<HashMap<String, Object>> dataSourceListAdd = new ArrayList<HashMap<String, Object>>();

    private DragGridView mDragGridViewWhole;
    private DragGridView mDragGridViewAdd;
    private HorizontalScrollView  hView;
    int     viewVisableWhole = DragGridView.INVISIBLE;
    int     viewVisableAdd = DragGridView.INVISIBLE;
    int cameraIndex = CameraBridgeViewBase.CAMERA_ID_BACK;
    
    ////////////////////////////////////////////

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
	/*	TextView tv = new TextView(this); 
		int h = hello();
		if(h == 1)
        tv.setText("fuck the fuck"); */
		//tv.setText("FFFF");
		nInsertX = 150;
		nInsertY = 150;
		matHand = null;
		matHanda = null;
		MainActivity.m_size = 6;
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);//设置屏幕常亮
        
		    
	//	setContentView(R.layout.activity_main);
		
	//	Log.d(TAG, "Creating and seting view");
    //    mOpenCvCameraView = (CameraBridgeViewBase) new JavaCameraView(this, -1);
		mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.CameraView);
	//	mUtil = new JniUtility();
		//Log.d(TAG, "Creating and seting view22222222222222");
     //   setContentView(mOpenCvCameraView);  
       // Log.d(TAG, "Creating and seting view333333333333333");
		
     //   mOpenCvCameraView.setCvCameraViewListener(this);
        
        mOpenCvCameraView.enableView();///
        mOpenCvCameraView.setVisibility(SurfaceView.VISIBLE);
        mOpenCvCameraView.setOnTouchListener(RecordActivity.this);
        mOpenCvCameraView.setCvCameraViewListener(this);
        
     //   mOpenCvCameraView.setRotation(90);
        

      //  Log.d(TAG, "Creating and seting view55555");
        
         
        //Log.d(TAG, "Creating and seting view44444444444444");
        initialButton();
        mViewMode = 0;
        
        
        ////////////////////////////////
    //    mDragGridViewAdd = (DragGridView) findViewById(R.id.dragGridViewAdd);
      ///  hView = (HorizontalScrollView) findViewById(R.id.HView);
    //    initDataSrcWhole();
        
      //  initDataSrcAdd();
        
	//	mDragGridView.setOnClickListener(this);
        ///////////////////////////////////
      //  Start();    
       // SetMode(10);

        setGuideResId(R.drawable.yindao04);
	}
	 @Override
	    public void onPause()
	    {
		 //back键后响应
	        super.onPause();
	        if (mOpenCvCameraView != null)
	            mOpenCvCameraView.disableView();
	    }

	    @Override
	    public void onResume()
	    {
	        super.onResume();
	      //  Log.d(TAG, "Creating and seting view!!!!");
	        if(OpenCVLoader.initDebug())   
	        {
	        //	Log.d(TAG, "Creating and seting view!!!!");
	        	mOpenCvCameraView.setOnTouchListener(RecordActivity.this);
                mOpenCvCameraView.enableView();  
             //   Log.e(TAG, "onResume Start");///
                MainActivity.Start(MainActivity.TEST_FILE);//////
                MainActivity.SetSize(MainActivity.m_size);
       //        mOpenCvCameraView.setCameraIndex(cameraIndex);
               //mOpenCvCameraView.
	        }
	       // OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_3, this, mLoaderCallback);
	    }

	    public void onDestroy() {
	    	//home键后响应
	        super.onDestroy();//////
	      //  Log.e(TAG, "onDestroy!");
	        MainActivity.ReleaseImglist();
	        MainActivity.Stop();
	        if (mOpenCvCameraView != null)  
	            mOpenCvCameraView.disableView();   
	    }
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true; 
	}
	
	public void onCameraViewStarted(int width, int height) {
        mGameWidth = width;
        mGameHeight = height;
     //   mPuzzle15.prepareGameSize(width, height);
        mRgba = new Mat(height, width, CvType.CV_8UC4);
        
        

        mRgb = new Mat(height, width, CvType.CV_8UC3);
      //  mIntermediateMat = new Mat(height, width, CvType.CV_8UC4);
     //   mGray = new Mat(height, width, CvType.CV_8UC1);///
        ///////////////////////
        
      //  Log.e(TAG, "view started!");
      //  Log.e(TAG, " started  width = " + width + "height = "+ height);
       // Log.e(TAG, " started view width = " + mOpenCvCameraView.getWidth() + "   height = "+ mOpenCvCameraView.getHeight());
        MainActivity.SetMode(0);//
        MainActivity.SetColor(12);  ///
        MainActivity.SetSaveType(nSaveType);
      //  Drawable currentIcon = null;////////
        //currentIcon.
        /*Bitmap bt1 = BitmapFactory.decodeResource(getResources(),  
89.                R.drawable.im_beauty);  
90.        Bitmap bt2 = BitmapFactory.decodeResource(getResources(),  
91.                R.drawable.im_flower3);  
92.        Bitmap bt3 = null;  
93.  
94.        // 转换数据  
95.        Utils.bitmapToMat(bt1, mat1);  
*/
        MainActivity.SetImgBtnPath(MainActivity.TEMP_DIR + "btn.png");///、、、
       Bitmap bm = BitmapFactory.decodeResource(
				getResources(),
				R.drawable.hand);
       if(matHand != null)
    	   matHand.release();
       if(matHanda != null)
    	   matHanda.release();
    	   
       matHanda = new Mat(bm.getHeight(), bm.getWidth(), CvType.CV_8UC4);;
       matHand = new Mat(bm.getHeight(), bm.getWidth(), CvType.CV_8UC3);;
		Utils.bitmapToMat(bm, matHanda);
		
       int n = MainActivity.SetHandImgMat(matHand.getNativeObjAddr(),matHanda.getNativeObjAddr());
   //    Log.e(TAG, "SetHandImgMat return "+n);///
       
   //     Toast toast = Toast.makeText(this, minfo, Toast.LENGTH_SHORT);
     //   toast.show();
    }

    public void onCameraViewStopped() {
    	mRgba.release();
    	mRgb.release();
    	if(matHand != null)
     	   matHand.release();
        if(matHanda != null)
     	   matHanda.release();
        if(MainActivity.matKoutua != null)
        	MainActivity.matKoutua.release();
         if(MainActivity.matKoutu != null)
        	 MainActivity.matKoutu.release();
     //   mGray.release();
        
      //  Log.e(TAG, "view stopped!");
     //   String sss = " Going to stoping!!!!!!!!!!!";
    	//Log.d(TAG, sss);
      //  Stop(); 
       // String ss = " is stoping!!!!!!!!!!!";
    //	Log.d(TAG, ss);
    }

    
    private float getGap(float x0, float x1, float y0, float y1)
	{
		return (float) Math.pow(
				Math.pow((x0 - x1), 2) + Math.pow((y0 - y1), 2), 0.5);
	}
    /////////////////////////////////
    
    public void HideBar()
    {
    	if(viewVisableWhole == DragGridView.VISIBLE)
		{
			hView.setVisibility(DragGridView.INVISIBLE);
		//	mDragGridViewWhole.setVisibility(DragGridView.INVISIBLE);
			mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
		viewVisableWhole = DragGridView.INVISIBLE;
		}
    	if(viewVisableAdd == DragGridView.VISIBLE)
		{
			hView.setVisibility(DragGridView.INVISIBLE);
			mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
		viewVisableAdd = DragGridView.INVISIBLE;
		Log.e(TAG, "DragGridView.INVISIBLE");///
		}
    }
    //////////////////////////////////
    public boolean onTouch(View view, MotionEvent event) 
    {
    	 int vid = MediaRecorder.VideoEncoder.H264;
    	 int vid2 = MediaRecorder.VideoEncoder.MPEG_4_SP;
    //	Toast toast = Toast.makeText(MainActivity.this, "GetGif return " + "&&"+sFilePath+ "**" + retSave + " & " + nSaveType + "$$", Toast.LENGTH_SHORT);
      //  toast.show();///////
		int pointCount = event.getPointerCount();
	//	Log.e(TAG, "onTouch!!!!pointCount" + pointCount);
		if (pointCount == 1)
		{
		/*	final float x = event.getX();
			final float y = event.getY();
			switch (action)
			{
			case MotionEvent.ACTION_DOWN:
				mX = x;
				mY = y;
				
				

				break;
			case MotionEvent.ACTION_MOVE:
			{
				final float dx = (x - mX) / view.getWidth();
				final float dy = (y - mY) / view.getHeight();
				
				mX = x;
				mY = y;
				break;
			}
			}*/
			//接口函数返回值，判断是需要旋转，改变大小，还是移动
			
			handleTouch(event);//旋转或改变大小
			
			/////////////////////////////
			
			
			///////////////////////////////
		}
		if (pointCount >1)
		{
	//		Toast toast = Toast.makeText(MainActivity.this, "pointCount is " + pointCount, Toast.LENGTH_SHORT);
		//        toast.show();///////
			 int action = event.getAction()&MotionEvent.ACTION_MASK;
	    	
			 float x0 = event.getX(event.getPointerId(0));
			 float y0 = event.getY(event.getPointerId(0));

			 float x1 = event.getX(event.getPointerId(1));
			 float y1 = event.getY(event.getPointerId(1));

			float gap = getGap(x0, x1, y0, y1);
			switch (action)
			{
			case MotionEvent.ACTION_POINTER_DOWN:
		//	case MotionEvent.ACTION_POINTER_1_DOWN:
				mGap = gap;
				
				////////////////////
				
				mX = x0;  
				mY = y0;
				 
			//	 startAngle=angle(event);
				break;
		//	case MotionEvent.ACTION_POINTER_1_UP:
			//	mX = x1;
				//mY = y1;
			//	break;
			case MotionEvent.ACTION_POINTER_UP:
			
				  
				break;
			case MotionEvent.ACTION_MOVE: 
			{
			//	final float dgap = (gap - mGap) / mGap;
			//	Log.d(TAG, "onTouch!!!!ACTION_MOVE" + pointCount);
				if(Math.abs(gap - mGap) <= 50)
					break;
			//	Log.e(TAG, "onTouch!!!!GAP%%%%" + mGap + "   " + gap);
				if(mGap == 0)
					mGap = 1;
				float dgap = (gap) / mGap;
				// Log.d("Gap", String.valueOf(dgap));
			//	Log.d("Gap", String.valueOf((float) Math.pow(5, dgap)));
			//	Log.d("dgap", String.valueOf( mState.getZoom() ));
			/*	Log.d("mgap", String.valueOf( mGap));
				Log.d("ggggap", String.valueOf( gap));*/
			//	mState.setZoom(mState.getZoom() * (float) Math.pow(5, dgap));
			//	mZoomState.getZoom() + 0.25f;
			//	Log.e(TAG, "onTouch!!!!ACTION_MOVE  dgap" + dgap);
			/*	if(dgap > 1)
				{
				 //  mState.setZoom(mState.getZoom()  +  0.02f);
					ReSize(1,(float) 0.02);
				}
				else if(dgap < 1)
				{
					//if((mState.getZoom()  -  0.25f) > 0 )
					 //  mState.setZoom(mState.getZoom()  -  0.02f);
					ReSize(-1,(float) 0.02);
				}*/
					
				
			/*	mGap = gap;
				PointF pmid = mid(event);
				
				double ang = getActionDegrees(pmid.x, pmid.y, mX, mY,
						x0, y0);
				if(Math.abs(ang)>10)
				{
					if(ang < 0)
						ang +=360;
					
					Log.e(TAG, "onTouch!!!!trunAngel%%%%" + ang);
					Rotate((int)ang);
					mX = x0;  
					mY = y0;
				}
				Log.e(TAG, "onTouch!!!!wrong%%%%" + ang);*/
				/////////////////////////////
			/*	 int trunAngel = (int) (angle(event)-startAngle);//变化的角度
				 
				 if (Math.abs(trunAngel)>5)
                 {
                         //设置变化的角度
                      //  matrix.postRotate(-trunAngel, midPoint.x, midPoint.y);
					 Log.e(TAG, "onTouch!!!!trunAngel%%%%" + trunAngel);

					// Rotate(trunAngel);//
                 }
				*/
			//	mX = x0;  
				//mY = y0;
				///////////////////////////////
				break;
			}
			}
		}


        return true;
    }
    ///////////////////////////////////////////////
    
    private void handleTouch(MotionEvent event) {
		curTouchX = event.getX();
		curTouchY = event.getY();
		
		 int action = event.getAction()&MotionEvent.ACTION_MASK;
		 
		 int xx = (int) curTouchX;
			int yy = (int) curTouchY;
		//	Log.e(TAG, "onTouch!!!!OnTouch%%%%" + xx + "$$" + yy);
			int n = MainActivity.TouchScreen(action,xx,yy);
		//	Toast toast = Toast.makeText(this, action + "%%" + xx + "%%" + yy + "%%" + n, Toast.LENGTH_SHORT);
          //  toast.show();   
		switch (action) {
		case MotionEvent.ACTION_DOWN://0
		//	saveTouchPoint();
			 mX = curTouchX;
				mY = curTouchX;
			
			break;
		case MotionEvent.ACTION_MOVE://2
		//	Toast toast = Toast.makeText(this, action + "%%" + xx + "%%" + yy + "%%" + n, Toast.LENGTH_SHORT);
          //  toast.show();   
		//	Log.e(TAG, "onTouch!!!!ACTION_MOVE%%%%" + xx + "$$" + yy + "$$" + n);
		//	handleTouchMove();///
			break;
		case MotionEvent.ACTION_UP://1
			// 可以使用访问者模式这里让访问者获得当前角度
			break;
		}
	}

	//////////////////////////////////////////////
    
    //计算角度
    public static float distance(MotionEvent event) {
        float dx = event.getX(1) - event.getX(0);
        float dy = event.getY(1) - event.getY(0);
        return FloatMath.sqrt(dx*dx + dy*dy);
}
/**
 * 计算两点之间的中间点
* @param event
 * @return
 */
public static PointF mid(MotionEvent event){
        float midX = (event.getX(1) + event.getX(0)) / 2;
        float midY = (event.getY(1) + event.getY(0)) / 2;
        return new PointF(midX, midY);
}
/**
 * 计算两个手指连线与坐标轴的角度（单位为。C）
* @param event
 * @return
 */

public static double angle(MotionEvent event)
{
        double x1=event.getX(0);
        double y1=event.getY(0);
        double x2=event.getX(1);
        double y2=event.getY(1);
        double sin=(y1-y2)/distance(event);
        if (x1>x2)
        {
                sin=-sin;
        }
        if (Math.abs(sin)>1)
        {
             //   Log.e("angle", "Math.abs(sin)>1");
                sin=1;
        }
    //    Log.e("angle", "Math.asin(sin)="+(Math.asin(sin)*180/Math.PI));
        return Math.asin(sin)*180/Math.PI;
}
    
    
    //////////////////////////////////////////////

	

	

	/**
	 * 获取两点到第三点的夹角。
	 * 
	 * @param x
	 * @param y
	 * @param x1
	 * @param y1
	 * @param x2
	 * @param y2
	 * @return
	 */
	/*xy为基准点，返回的角度顺时针为正，逆时针为负,度数在180范围内*/
	private double getActionDegrees(float x, float y, float x1, float y1,
			float x2, float y2) {

		double a = Math.sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		double b = Math.sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
		double c = Math.sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));
		// 余弦定理
		double cosA = (b * b + c * c - a * a) / (2 * b * c);
		// 返回余弦值为指定数字的角度，Math函数为我们提供的方法
		double arcA = Math.acos(cosA);
		double degree = arcA * 180 / Math.PI;

		// 接下来我们要讨论正负值的关系了，也就是求出是顺时针还是逆时针。
		// 第1、2象限
		if (y1 < y && y2 < y) {
			if (x1 < x && x2 > x) {// 由2象限向1象限滑动
				return degree;
			}
			// 由1象限向2象限滑动
			else if (x1 >= x && x2 <= x) {
				return -degree;
			}
		}
		// 第3、4象限
		if (y1 > y && y2 > y) {
			// 由3象限向4象限滑动
			if (x1 < x && x2 > x) {
				return -degree;
			}
			// 由4象限向3象限滑动
			else if (x1 > x && x2 < x) {
				return degree;
			}

		}
		// 第2、3象限
		if (x1 < x && x2 < x) {
			// 由2象限向3象限滑动
			if (y1 < y && y2 > y) {
				return -degree;
			}
			// 由3象限向2象限滑动
			else if (y1 > y && y2 < y) {
				return degree;
			}
		}
		// 第1、4象限
		if (x1 > x && x2 > x) {
			// 由4向1滑动
			if (y1 > y && y2 < y) {
				return -degree;
			}
			// 由1向4滑动
			else if (y1 < y && y2 > y) {
				return degree;
			}
		}

		// 在特定的象限内
		float tanB = (y1 - y) / (x1 - x);
		float tanC = (y2 - y) / (x2 - x);
		if ((x1 > x && y1 > y && x2 > x && y2 > y && tanB > tanC)// 第一象限
				|| (x1 > x && y1 < y && x2 > x && y2 < y && tanB > tanC)// 第四象限
				|| (x1 < x && y1 < y && x2 < x && y2 < y && tanB > tanC)// 第三象限
				|| (x1 < x && y1 > y && x2 < x && y2 > y && tanB > tanC))// 第二象限
			return -degree;
		return degree;
	}

	public float getCurDegree() {
		return curDegree;
	}

	public void setCurDegree(float curDegree) {
		if (curDegree >= MIN_DEGREE && curDegree <= MAX_DEGREE) {
			this.curDegree = curDegree;
		//	m.setRotate(curDegree, centerX, centerY);
			//setImageMatrix(m);
		}

	}
	//////////////////////////////////////
	
	public static String setProcess() {
		return String.valueOf(System.currentTimeMillis());
	}

	public void sayInfo(String msg) 
	{
		
			
	}
	private String getPhotoFileName() {  
        Date date = new Date(System.currentTimeMillis());  
        SimpleDateFormat dateFormat = new SimpleDateFormat(  
                "000yyyyMMddHHmmss");  
        return dateFormat.format(date);  
    } 

///////////////////////////////////////////////
	
	private void initialButton() 
	{
		// TODO Auto-generated method stub
		Display display = getWindowManager().getDefaultDisplay(); 
		height = display.getHeight();  
		width = display.getWidth();
	//	Log.v("width  & height is:", String.valueOf(width) + ", " + String.valueOf(height));
		
	//	params.height = 50;
		//params.width = 50;
		//设置边距  (int left, int top, int right, int bottom)
	//	params.setMargins(10, height - 98, 0, 0);
		
	//	buttonDown = (Button) findViewById(R.id.Down);	
	//	buttonSleep.setLayoutParams(params);
	//	buttonDown.setVisibility(Button.INVISIBLE);
		buttonCamera = (Button) findViewById(R.id.Camera);
	//	buttonEditAdd = (Button) findViewById(R.id.EditAdd);
		buttonSwitchCam = (Button) findViewById(R.id.SwitchCam);
	//	buttonEditWhole = (Button) findViewById(R.id.EditWhole);
	//	buttonEditBrowse = (Button) findViewById(R.id.EditBrowse);
		buttonCancel = (Button) findViewById(R.id.Cancel);
		//buttonDown = (Button) findViewById(R.id.Down);

	//	buttonDelete.setLayoutParams(params);
	//	buttonEditBrowse.setVisibility(Button.INVISIBLE);
		buttonCancel.setOnClickListener(new OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{
				finish();
			}
		});
			
	/*	buttonDown.setOnClickListener(new OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{
				if(nSaveType == 0)
				{
					nSaveType = 1;
				buttonDown.setText("Video");
				}
				else
				{
					nSaveType = 0;
					buttonDown.setText("Video");
					}
				MainActivity.SetSaveType(nSaveType);
			}
		});
			*/
		
		buttonCamera.setOnClickListener(new OnClickListener() 
		{
				
			@Override
			public void onClick(View v) 
			{
				// TODO Auto-generated method stub		
				String name= getPhotoFileName();
				String ext = nSaveType==0?".gif":".avi";
				   final String filename=MainActivity.PHOTO_DIR+name  + ext;
				if(isClick == false)
				{
					isClick = true;
					MainActivity.ReleaseImglist();
					MainActivity.GetGif(filename);
					
				//	buttonCamera.setBackgroundResource(R.drawable.btn);
					buttonCamera.setText(R.string.caping);
				}
				else
				{		
					//////////////////////////////////////////////
					
		/*			new AsyncTask<Void, Void, Void>() {
		                private ProgressDialog calibrationProgress;

		                @Override
		                protected void onPreExecute() {
		                	Resources res = getResources();
		                    calibrationProgress = new ProgressDialog(RecordActivity.this);
		                    calibrationProgress.setTitle(res.getString(R.string.saving));
		                    calibrationProgress.setMessage(res.getString(R.string.please_wait));
		                    calibrationProgress.setCancelable(false);
		                    calibrationProgress.setIndeterminate(true);
		                    calibrationProgress.show();
		                }

		                @Override
		                protected Void doInBackground(Void... arg0) {
		                  //  mCalibrator.calibrate();
		                	Log.e(TAG, "Saved file "+filename );
		                	sFilePath = filename;
		                	retSave =  MainActivity.GetGif(filename);
		                //	buttonCamera.setText(R.string.capture);//
		                	if(retSave == 1)
		                	{
		                		 String filepath = filename;
				              	   if(null == filepath)
				              	   {
				              		  return null;
				              	   }
		                	 if (mOpenCvCameraView != null)  
		 			            mOpenCvCameraView.disableView(); 
		 			        
		                	 MainActivity.Stop();
		 			 //       Log.e(TAG, "mainactivity----JniStop  ");////////
		 				Intent intent = new Intent(RecordActivity.this, GifActivity.class); 
		 				intent.putExtra("path", filepath); 

		                 intent.putExtra("SelectType", 1); 

		            //     intent.putExtra("floderName", mFloderList.get(arg2)); 

		           //      startActivityForResult(intent, PICK_PHOTO); 
		                 startActivity(intent);
		                	}
		                      return null;
		                }

		                @Override
		                protected void onPostExecute(Void result) {
		                    calibrationProgress.dismiss();
		                     
		                   
		                }
		            }.execute();*/
					
				 	 if (mOpenCvCameraView != null)  
	 			            mOpenCvCameraView.disableView(); 
	 			        
	                	 MainActivity.Stop();
	 			 //       Log.e(TAG, "mainactivity----JniStop  ");////////
	 				Intent intent = new Intent(RecordActivity.this, GifActivity.class); 
	 			//	intent.putExtra("path", filepath); 

	                 intent.putExtra("SelectType", GifActivity.TYPE_LIST); 
	                 intent.putExtra("ChangeSize", MainActivity.m_size); 

	            //     intent.putExtra("floderName", mFloderList.get(arg2)); 

	           //      startActivityForResult(intent, PICK_PHOTO); 
	                 startActivity(intent);
					//////////////////////////////////////////////
				//	GetGif(filename);
					isClick = false;
				//	buttonCamera.setBackgroundResource(R.drawable.btn);
					buttonCamera.setText(R.string.capture);
				//	buttonCamera.SetT
				}
					
			}
		});
		
	
		buttonSwitchCam.setOnClickListener(new OnClickListener() 
		{
				
			@Override
			public void onClick(View v) 
			{
				// TODO Auto-generated method stub		
//////////////////////
			//	Stop();
		/*	Intent intent = new Intent();
			intent.setClass(MainActivity.this, LeftRightSlideActivity.class);
			startActivity(intent);
			//设置切换动画，从右边进入，左边退出
			overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);*/				

			/////////////////////以上为测试代码
				
				///////////
				if(cameraIndex == CameraBridgeViewBase.CAMERA_ID_FRONT)
				{
					cameraIndex = CameraBridgeViewBase.CAMERA_ID_BACK;
				}
				else if(cameraIndex == CameraBridgeViewBase.CAMERA_ID_BACK)
					cameraIndex = CameraBridgeViewBase.CAMERA_ID_FRONT;
				MainActivity.Stop();
		        if (mOpenCvCameraView != null)  
		            mOpenCvCameraView.disableView(); 
				mOpenCvCameraView.setCameraIndex(cameraIndex);
				 mOpenCvCameraView.enableView();  
	             //   Log.e(TAG, "onResume Start");///
	                MainActivity.Start(MainActivity.TEST_FILE);///
				
			}
		});
	
		
	/*	buttonEditBrowse.setOnClickListener(new OnClickListener() 
		{
				
			@Override
			public void onClick(View v) 
			{
				// TODO Auto-generated method stub	
				 
				 
			
				Intent intent = new Intent(RecordActivity.this, SelFileActivity.class); 

			//	Intent intent = new Intent(MainActivity.this, GifActivity.class); 
 				
                intent.putExtra("SelectType", 1); 

                intent.putExtra("path", "1.gif"); 

                startActivityForResult(intent, PICK_GIF_PHOTO); 

				
				
			}
		});*/
		/*buttonPic.setOnClickListener(new OnClickListener() 
		{
				
			@Override
			public void onClick(View v) 
			{
				// TODO Auto-generated method stub		
			//	InSertTxt("",nInsertX,nInsertY,6);////
				Intent intent = new Intent(MainActivity.this, SelFileActivity.class); 

				                                 intent.putExtra("SelectType", 0); 

				                            //     intent.putExtra("floderName", mFloderList.get(arg2)); 

				                                 startActivityForResult(intent, PICK_PHOTO); 


				
			}
		});*/
		
	}
	
	@Override 

	         protected void onActivityResult(int requestCode, int resultCode, Intent data) { 

		/*public static final int PICK_PHOTO = 125;
	public static final int INSERT_TEXT = 150;
	public static final int DANMU_TEXT = 130;
	public static final int NORMAL_TEXT = 135;
	public static final int PICK_FROM_PHOTO = 140;*/
		             if(null == data)
   	                 {
   		                 super.onActivityResult(requestCode, resultCode, data);
   		              Toast toast = Toast.makeText(this, "null data", Toast.LENGTH_SHORT);
   		              toast.show();
   		                    return;
   	                 }
	                 if (requestCode == PICK_PHOTO && resultCode == RESULT_OK) {
	                	 
	                	 
	                	 Bundle bundle = data.getExtras();
	              	   //获取相机返回的数据，并转换为图片格式
	              	   String filepath = (String)bundle.get("filepath");
	              	   if(null == filepath)
	              	   {
	              		 super.onActivityResult(requestCode, resultCode, data);
	              		Toast toast = Toast.makeText(this, "null filepath", Toast.LENGTH_SHORT);
	   		              toast.show();   
	              		 return;
	              	   }
	              	   
	              	 
	              	//     int n = InSertImg(filepath,nInsertX,nInsertY,0);
	              	     
	              	   
	              	  /* String ss;
	              	   int num = GetInfo(0,0);
	              	   ss = "$" + num + "$" + GetInfo(0,1)+"$" + GetInfo(0,2) + "$" + GetInfo(0,3) + "$" + GetInfo(0,4);
	   				Toast toast = Toast.makeText(this, ss + "&&", Toast.LENGTH_SHORT);
	   		              toast.show(); */
	             // 	 InSertTxt(filepath,nInsertX,nInsertY,6);//////
	              	 MainActivity.InSertImg(filepath,nInsertX,nInsertY,0);
	              	     
	              	//   Toast toast = Toast.makeText(this, n + "%%" + filepath, Toast.LENGTH_SHORT);
	  		          //    toast.show();
	              		   
	                 } 
	                 else if(requestCode == NORMAL_TEXT && resultCode == RESULT_OK)
	                 {
	                	 Bundle bundle = data.getExtras();
		              	   //获取相机返回的数据，并转换为图片格式
		              	   String ttt = (String)bundle.get("text");
		              	   if(null == ttt)
		              	   {
		              		 super.onActivityResult(requestCode, resultCode, data);
		              		 return;
		              	   }
		              	   
		              	 int clr = bundle.getInt("color");
		              	 String name= getPhotoFileName();
		 				//String ext = nSaveType==0?".gif":".avi";
		 				String filename=MainActivity.PHOTO_DIR+name  + ".png";
		 				ArrayList<String> sdata = new ArrayList<String>();
		 			/*	for(int h=0;h<ttt.length();h+= 6)
		 				{
		 					String tmp;
		 					if(h+6 < ttt.length() - 1)
		 					tmp= ttt.substring(h, h+6);
		 					else
		 						tmp = ttt.substring(h, ttt.length() - 1);
		 					sdata.add(tmp);
		 				}*/
		 				sdata.add(ttt);
		 				//Toast toast = Toast.makeText(this, "NORMAL_TEXT " + filename, Toast.LENGTH_SHORT);
	   		              //toast.show();
		              	 if(writeImage(filename,sdata,clr,50))
		              	 {
		              		
		              	     int ret = MainActivity.InSertTxt(filename,nInsertX,nInsertY,6);
		              	//   Toast toast = Toast.makeText(this, "NORMAL_TEXT  return " + ret, Toast.LENGTH_SHORT);
		   		          //    toast.show();///
		              	 }
	                 }
	                 else if(requestCode == DANMU_TEXT && resultCode == RESULT_OK)
	                 {
	                	 Bundle bundle = data.getExtras();
		              	   //获取相机返回的数据，并转换为图片格式
		              	   String ttt = (String)bundle.get("text");
		              	   if(null == ttt)
		              	   {
		              		 super.onActivityResult(requestCode, resultCode, data);
		              		 return;
		              	   }
		              	 int clr = bundle.getInt("color");
		              	 String name= getPhotoFileName();
		 				//String ext = nSaveType==0?".gif":".avi";
		 				String filename=MainActivity.PHOTO_DIR+name  + ".png";
		 				ArrayList<String> sdata = new ArrayList<String>();
		 			/*	for(int h=0;h<ttt.length();h+= 6)
		 				{
		 					String tmp;
		 					if(h+6 < ttt.length() - 1)
		 					tmp= ttt.substring(h, h+6);
		 					else
		 						tmp = ttt.substring(h, ttt.length() - 1);
		 					sdata.add(tmp);
		 				}*/
		 				sdata.add(ttt);
		 			//	Toast toast = Toast.makeText(this, "DANMU_TEXT " + filename, Toast.LENGTH_SHORT);
	   		          //    toast.show();
		              	 if(writeImage(filename,sdata,clr,50))
		              	 {
		              		MainActivity.InSertTxt(filename,nInsertX,nInsertY,4);
		              	 }
		              	   
	                 }
	                 else if(requestCode == PICK_FROM_PHOTO && resultCode == RESULT_OK)
	                 {
	                	 Bundle bundle = data.getExtras();
	                	// Toast toast = Toast.makeText(this, "PICK_FROM_PHOTO", Toast.LENGTH_SHORT);
	   		              //toast.show();
		              	   //获取相机返回的数据，并转换为图片格式
		              	   String filepath = (String)bundle.get("filepath");
		              	   if(null == filepath)
		              	   {
		              		 super.onActivityResult(requestCode, resultCode, data);
		              		Toast toast2 = Toast.makeText(this, "null filepath", Toast.LENGTH_SHORT);
		   		              toast2.show();   
		              		 return;
		              	   }
		              //	 Toast toast3 = Toast.makeText(this, filepath, Toast.LENGTH_SHORT);
	   		            //  toast3.show(); 
		              	 Intent intent1 = new Intent(RecordActivity.this, PickFromPhotoActy.class); 

		                 intent1.putExtra("path", filepath); 

		            //     intent.putExtra("floderName", mFloderList.get(arg2)); 

		                 startActivityForResult(intent1, PICK_FROM_PHOTO);
		              	   
		              	   
		              //	 InSertKoutu(int x,int y,int wid,int hei,long matAddrGr);
		             	
		              	 //    InSertTxt(ttt,nInsertX,nInsertY,4);
	                 }
	                 else if(requestCode == PICK_GIF_PHOTO && resultCode == RESULT_OK)
	                 {
	                	 Bundle bundle = data.getExtras();
		                	// Toast toast = Toast.makeText(this, "PICK_FROM_PHOTO", Toast.LENGTH_SHORT);
		   		              //toast.show();
			              	   //获取相机返回的数据，并转换为图片格式
			              	   String filepath = (String)bundle.get("filepath");
			              	   if(null == filepath)
			              	   {
			              		 super.onActivityResult(requestCode, resultCode, data);
			              	//	Toast toast2 = Toast.makeText(this, "null filepath", Toast.LENGTH_SHORT);
			   		          //    toast2.show();   
			              		 return;
			              	   }
	                	 if (mOpenCvCameraView != null)  
	 			            mOpenCvCameraView.disableView(); 
	 			        
	                	 MainActivity.ReleaseImglist();
	                	 MainActivity.Stop();
	 			   //     Log.e(TAG, "mainactivity----JniStop  ");////////
	 				Intent intent = new Intent(RecordActivity.this, GifActivity.class); 
	 				intent.putExtra("path", filepath); 

	                 intent.putExtra("SelectType", GifActivity.TYPE_FILE); 

	            //     intent.putExtra("floderName", mFloderList.get(arg2)); 

	           //      startActivityForResult(intent, PICK_PHOTO); 
	                 startActivity(intent);
	                 }

	               
	                 super.onActivityResult(requestCode, resultCode, data); 

	         } 


	
	
	

    /////////////////////////////////////////////////////////////
	
    
    
    //////////////////////////////////////////////////

 
    
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
    // TODO Auto-generated method stub
    	
    //	setRequestedOrientation(orientMode);//设定需要的朝向，横或者竖
    	Toast.makeText(this,"现在是竖屏", Toast.LENGTH_SHORT).show();   
     
     if(newConfig.orientation==this.getResources().getConfiguration().ORIENTATION_PORTRAIT)
     {
  //   count=2;
     Toast.makeText(this,"现在是竖屏", Toast.LENGTH_SHORT).show();

     
     }
     else if(newConfig.orientation==this.getResources().getConfiguration().ORIENTATION_LANDSCAPE)
     {
   //  count=3;
     Toast.makeText(this,"现在是横屏", Toast.LENGTH_SHORT).show();

     }

  //   CreateFood(sql,foodtypeid);
    super.onConfigurationChanged(newConfig);
    }
    
    public boolean CheckSelType(String path)
	{
		boolean ret = false;
		
		//	Toast.makeText(SelFileActivity.this, path,Toast.LENGTH_SHORT).show();

			if(path.endsWith(".gif"))
				ret = true;
			if(path.endsWith(".GIF"))
				ret = true;
		
		
		
		return ret;
		
	}
@Override
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		// TODO Auto-generated method stub
	//	inputFrame.
	/*String path = "/mnt/dcim/123.jpg";
	if(CheckSelType(path))
		Log.e("EEEEEEEEEEEEE", "endsWith(.gif");
	else
		Log.e("EEEEEEEEEEEEE", "not  endsWith(.gif");*/
		mRgba = inputFrame.rgba();    
     //   mGray = inputFrame.gray();////
        
      //  SetMode(10);
      //  FindFeatures(mGray.getNativeObjAddr(), mRgba.getNativeObjAddr());
		//mRgb.getNativeObjAddr()是单独打开的存储空间，与灰度图无关
		
	/*	mTmpBitmap = BitmapFactory.decodeResource(
				MainActivity.this.getResources(), R.drawable.bg);
		int h = mTmpBitmap.getHeight();
    	int w = mTmpBitmap.getWidth();
		 mRgbaT = new Mat(h, w, CvType.CV_8UC4);
	        /////////
	        

	        mRgbT = new Mat(h, w, CvType.CV_8UC3);
	        Log.e("EEEEEEEEEEEEE", "mRgba dep == " + mRgba.depth());
	        Log.e("EEEEEEEEEEEEE", "mRgb dep == " + mRgb.depth());
	//	Log.e(TAG, "bitmapToMat" );
        Utils.bitmapToMat(mTmpBitmap, mRgbaT);//(modified, mTmpBitmap);*/
	//	if(canvas == null)
	//	  canvas = mOpenCvCameraView.getHolder().lockCanvas();
		//Log.e("EEEEEEEEEEEEE", "mRgba wid == " + mRgba.width());
		//Log.e("EEEEEEEEEEEEE", "mRgba hei == " + mRgba.height());
    	int n = MainActivity.PerfLiveFunc(-1,1,mRgb.getNativeObjAddr(), mRgba.getNativeObjAddr(),mRgba.getNativeObjAddr());
    	//mOpenCvCameraView.getHolder().unlockCanvasAndPost(canvas);
    //    int n = PerfFunc(mViewMode,mRgbT.getNativeObjAddr(), mRgbaT.getNativeObjAddr());
//
   // 	Utils.matToBitmap(mRgbT, mTmpBitmap);/////
    	
    //	Log.e("EEEEEEEEEEEEE", "return == " + n);////
      //  Log.e("EEEEEEEEEEEEE", "mRgb dep == " + mRgb.depth());
    //	Log.e(TAG, "mainactivity----perffunc == " +  n);/////
    /*	Point org = new Point();
    	org.x = 100;
    	org.y = 100;
    	int fontFace = 0;
    	Scalar color = new Scalar(255, 0, 0, 255);
    	Core.putText(mRgb, "我勒个去", org,3 , 1, color, 2, 8, false);*/
    	
    //	InSertTxt("123",100,100);   
////////
    //	return mRgba;//
    	//String ss = n + " is result!!!!!!!!!!!";
    	//Log.d(TAG, ss);
    //	return mGray;
    //	Log.e("EEEEEEEEEEEEE", "mRgb wid == " + mRgb.width());
		//Log.e("EEEEEEEEEEEEE", "mRgb hei == " + mRgb.height());
		return mRgb;//////
    //	return inputFrame.rgba();
	}
	
///////////////////////////////
public void   InsertDatatoDatasrc(Object val,String txt,List<HashMap<String, Object>> dataSource)
{
	HashMap<String, Object> itemHashMap = new HashMap<String, Object>();
	itemHashMap.put("item_image",val);
	itemHashMap.put("item_text", txt);
	dataSource.add(itemHashMap);
	}
public  void  initDataSrcWhole()
{
	// mDragGridViewWhole = (DragGridView) findViewById(R.id.dragGridViewWhole);
    // viewVisable = DragGridView.VISIBLE;
	//	for (int i = 0; i < 10; i++) {
		/*	HashMap<String, Object> itemHashMap = new HashMap<String, Object>();
			itemHashMap.put("item_image",R.drawable.z);
			itemHashMap.put("item_text", "哦哦哦");///
			dataSourceListWhole.add(itemHashMap);*/
	/*enum_Empty = 0,

	//int BoLang(IplImage *src,double angle,int deltaI,int A);
	enum_ColorMap = 1,

	enum_SuMiao = 2,
	enum_Menglong = 3,
	enum_Wind = 4,
	enum_Xuanwo = 5,
	enum_casting =6,
	enum_freezing = 7,
	enum_Diaoke = 8,
	enum_HAHAJing_FangDa = 9,


	enum_XuanZhuan = 10,
	enum_Maoboli = 11,
	enum_ManHua = 12,

	enum_HuaiJiu = 13,

	enum_FuDiao = 14,
	enum_HAHAJing_JiYa = 15*/
	 InsertDatatoDatasrc(R.drawable.z,"自动",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"素描",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"朦胧",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"怀旧-黑白",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"怀旧-泛黄",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"风",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"毛玻璃",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"雕刻",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"哈哈镜-放大",dataSourceListWhole);
	 InsertDatatoDatasrc(R.drawable.z,"哈哈镜-挤压",dataSourceListWhole);
		// InsertDatatoDatasrc(R.drawable.z,"连环画",dataSourceListWhole);

//	 InsertDatatoDatasrc(R.drawable.z,"漩涡",dataSourceListWhole);
//	 InsertDatatoDatasrc(R.drawable.z,"投影",dataSourceListWhole);//??
//	 InsertDatatoDatasrc(R.drawable.z,"冰冻",dataSourceListWhole);//del
	
	// InsertDatatoDatasrc(R.drawable.z,"高亮",dataSourceListWhole);
	// InsertDatatoDatasrc(R.drawable.z,"模糊",dataSourceListWhole);
	// InsertDatatoDatasrc(R.drawable.z,"旋转",dataSourceListWhole);//del
	
	// InsertDatatoDatasrc(R.drawable.z,"漫画",dataSourceListWhole);//??
	// InsertDatatoDatasrc(R.drawable.z,"波浪",dataSourceListWhole);
	// InsertDatatoDatasrc(R.drawable.z,"怀旧",dataSourceListWhole);
	// InsertDatatoDatasrc(R.drawable.z,"浮雕",dataSourceListWhole);//与雕刻重复
	 
	
	/* InsertDatatoDatasrc(R.drawable.z,"秋天",dataSourceListWhole);//蓝色
	 InsertDatatoDatasrc(R.drawable.z,"骨感",dataSourceListWhole);//发黄的照片
	 InsertDatatoDatasrc(R.drawable.z,"喷射",dataSourceListWhole);//红外
	 InsertDatatoDatasrc(R.drawable.z,"冬天",dataSourceListWhole);//跟上面大同小异
	 InsertDatatoDatasrc(R.drawable.z,"彩虹",dataSourceListWhole);//？？
	 InsertDatatoDatasrc(R.drawable.z,"海洋",dataSourceListWhole);//更黄的照片
	 InsertDatatoDatasrc(R.drawable.z,"夏天",dataSourceListWhole);//绿色的镜子？
	 InsertDatatoDatasrc(R.drawable.z,"春天",dataSourceListWhole);///粉色
	 InsertDatatoDatasrc(R.drawable.z,"酷",dataSourceListWhole);//黄色
	 InsertDatatoDatasrc(R.drawable.z,"HSV",dataSourceListWhole);//？？？？？
	 InsertDatatoDatasrc(R.drawable.z,"粉红",dataSourceListWhole);//怀旧
	 InsertDatatoDatasrc(R.drawable.z,"HOT",dataSourceListWhole);//蓝色？*/

		}
	 public void InitDragViewWhole()
	 {

		final SimpleAdapter mSimpleAdapter = new SimpleAdapter(this, dataSourceListWhole,
				R.layout.grid_item, new String[] { "item_image", "item_text" },
				new int[] { R.id.item_image, R.id.item_text });
		
	/*	mDragGridViewWhole.setAdapter(mSimpleAdapter);
	//	mDragGridViewWhole.rem
		
		mDragGridViewWhole.setVisibility(DragGridView.INVISIBLE);
		viewVisableWhole = DragGridView.INVISIBLE;
		
		
	mDragGridViewWhole.setOnChangeListener(new OnChanageListener() {*/
	//	mDragGridViewAdd.removeAllViews();
	//	Log.e(TAG, "mDragGridViewAdd.setAdapter");///
		mDragGridViewAdd.setAdapter(mSimpleAdapter);
		//	mDragGridViewWhole.rem
	//	Log.e(TAG, "setVisibility");///
	//	mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
		//	viewVisableWhole = DragGridView.INVISIBLE;
			
			
			mDragGridViewAdd.setOnChangeListener(new OnChanageListener() {
		
		
		@Override
		public void onChange(int pos) {
			
		//	Log.e(TAG, "onChange whole  " + pos);///
			if(pos == 0)
			{
				MainActivity.SetMode(0);//
				MainActivity.SetColor(12);  ///
			}
		
			if(pos > 0 && pos <4 )
			{
				MainActivity.SetMode(pos+1);
				MainActivity.SetColor(12);
			}
			if(pos == 4)
			{
				MainActivity.SetMode(1);
				MainActivity.SetColor(5);
			}
			if(pos >4)
			{
				MainActivity.SetColor(12);
				MainActivity.SetMode(pos);
			}
		}
	});}

public  void  initDataSrcAdd()
{
	
    // viewVisable = DragGridView.VISIBLE;
	 InsertDatatoDatasrc(R.drawable.z,"添加照片",dataSourceListAdd);
	 InsertDatatoDatasrc(R.drawable.z,"添加扣图",dataSourceListAdd);
	 InsertDatatoDatasrc(R.drawable.z,"添加文字",dataSourceListAdd);
	 InsertDatatoDatasrc(R.drawable.z,"添加弹幕",dataSourceListAdd);
	 InsertDatatoDatasrc(R.drawable.z,"无手绘",dataSourceListAdd);
	 InsertDatatoDatasrc(R.drawable.z,"手写",dataSourceListAdd);
	 InsertDatatoDatasrc(R.drawable.z,"修饰花",dataSourceListAdd);
	 InsertDatatoDatasrc(R.drawable.z,"录制局部区域",dataSourceListAdd);
		
}
public void InitDragViewAdd()
{
//	mDragGridViewAdd.removeAllViews();
		final SimpleAdapter mSimpleAdapterAdd = new SimpleAdapter(this, dataSourceListAdd,
				R.layout.grid_item, new String[] { "item_image", "item_text" },
				new int[] { R.id.item_image, R.id.item_text });
		
		mDragGridViewAdd.setAdapter(mSimpleAdapterAdd);///
		
	//	mDragGridViewAdd.setVisibility(DragGridView.INVISIBLE);
		//viewVisableAdd = DragGridView.INVISIBLE;
		mDragGridViewAdd.setOnChangeListener(new OnChanageListener() {
		
		@Override
		public void onChange(int pos) {
			
		//	Log.e(TAG, "onChange add  " + pos);///
		//	HideBar();
			switch(pos)
			{
			case 0:
				//插入图片
				HideBar();
				Intent intent = new Intent(RecordActivity.this, SelFileActivity.class); 

                intent.putExtra("SelectType", 0); 

           //     intent.putExtra("floderName", mFloderList.get(arg2)); 

                startActivityForResult(intent, PICK_PHOTO); 

			//	public static native int InSertImg
				//  (String js,int x,int y,int mode);
				
				break;
			case 1:
				HideBar();
				Intent intent1 = new Intent(RecordActivity.this, SelFileActivity.class); 

			//	Intent intent1 = new Intent(MainActivity.this, PickFromPhotoActy.class); 

                intent1.putExtra("SelectType", 0); 

           //     intent.putExtra("floderName", mFloderList.get(arg2)); 

                startActivityForResult(intent1, PICK_FROM_PHOTO);
			//	InSertKoutu(int x,int y,int wid,int hei,long matAddrGr);
				//选择图片区域后插入
				break;
			case 2:
				HideBar();
				//public static native void InSertTxt
				  //(String js,int x,int y,int mode);
				Intent intent2 = new Intent(RecordActivity.this, TextEditor.class); 

                intent2.putExtra("SelectType", 0); 

           //     intent.putExtra("floderName", mFloderList.get(arg2)); 

                startActivityForResult(intent2, NORMAL_TEXT);
				//普通文字
				break;
			case 3:
			//	public static native void InSertTxt
				//  (String js,int x,int y,int mode);
				HideBar();
				Intent intent3 = new Intent(RecordActivity.this, TextEditor.class); 

                intent3.putExtra("SelectType", 0); 

           //     intent.putExtra("floderName", mFloderList.get(arg2)); 

                startActivityForResult(intent3, DANMU_TEXT);
				//弹幕
				break;
			case 4:
				//矩形选择
				/*enum_OPER_EMPTY= 21,
	enum_SELECT_RECT= 22,
	enum_DRAW_WITHHAND = 23,
	enum_DRAW_WRITE = 24*/
				HideBar();
				MainActivity.SetOperMode(21);
				break;
			case 5:
				//矩形选择
				HideBar();
				MainActivity.SetOperMode(24);
				break;
			case 6:
				//手绘线条
				HideBar();
				MainActivity.SetOperMode(23);
				break;
			case 7:
				HideBar();
				MainActivity.SetOperMode(22);
				//手绘图形
				break;
			}
		}
	});}
/////////////////////////////////////
/**
 * @param path 文件保存路径
 * @param data 保存数据
 * */
public  boolean writeImage(String path,ArrayList<String> data,int clr,int tsize){
//public  void writeImage(String path,String data,int clr){
	boolean ret = false;
	try {
		int height = data.size()*100;     //图片高
		Bitmap bitmap = Bitmap.createBitmap(500, height, Bitmap.Config.ARGB_8888);
		Canvas canvas = new Canvas(bitmap);
		canvas.drawColor(Color.BLACK);   //背景颜色
		
		Paint p = new Paint();
		
		p.setColor(clr);   //画笔颜色
		p.setTextSize(tsize);         //画笔粗细
		for(int i=0;i<data.size();i++){
			canvas.drawText(data.get(i), 0, (i+1)*100, p);
		//canvas.drawText(data, 20, 20, p);
		}
		
	//	Log.e("path", path);
		//将Bitmap保存为png图片
		FileOutputStream out = new FileOutputStream(path);
		ret = bitmap.compress(Bitmap.CompressFormat.PNG, 90, out);
		//Log.e("done", "done");
		
	} catch (Exception e) {
		// TODO: handle exception
		e.printStackTrace();
		return false;
	}
	return ret;
}



//////////////////////////////////////
}

