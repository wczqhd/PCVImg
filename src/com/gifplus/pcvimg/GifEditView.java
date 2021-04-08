package com.gifplus.pcvimg;

import java.io.BufferedInputStream;
import java.io.BufferedWriter;
import java.io.FileInputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;

import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.Bitmap.Config;
import android.os.Environment;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;

public class GifEditView extends SurfaceView implements SurfaceHolder.Callback {

	private GifDecoder mGifDecoder;
//	private File ff;
	private boolean mSurfaceExist;
	protected boolean mEnabled;
	private GifEditViewListener mListener;
	private static final int MAX_UNSPECIFIED = -1;
	private static final int STOPPED = 0;
	private static final int STARTED = 1;
	private int mState = STOPPED;
	private Bitmap mCacheBitmap;
	private Object mSyncObject = new Object();

	protected int mFrameWidth;
	protected int mFrameHeight;
	protected int mMaxHeight;
	protected int mMaxWidth;
	protected float mScale = 0;
	private Thread mThread;
	public boolean mStopThread;
	public boolean mFromFile;
//	private String m_path;

	private static final String PHOTO_DIR = Environment
			.getExternalStorageDirectory().getPath() + "/DCIM/Camera/";

	private static final String TEMP_DIR = Environment
			.getExternalStorageDirectory().getPath() + "/DCIM/GIFPLUS/";

	public static final String TEST_FILE = Environment
			.getExternalStorageDirectory().getPath()
			+ "/DCIM/GIFPLUS/"
			+ "gifedit.txt";

	private static final File tfile = new File(TEST_FILE);
	private static int num = 0;
	public boolean thdOver = true;
	private boolean m_pause = false;
	BufferedWriter output;
	Bitmap mTmp2;// =
					// BitmapFactory.decodeResource(GifActivity.this.getResources(),R.drawable.bg256
					// );
	public Rect m_rt;

	public GifEditView(Context context) {
		super(context);
		mMaxWidth = MAX_UNSPECIFIED;
		mMaxHeight = MAX_UNSPECIFIED;
		getHolder().addCallback(this);
		// TODO Auto-generated constructor stub
	}

	public GifEditView(Context context, AttributeSet attrs) {
		super(context, attrs);

		int count = attrs.getAttributeCount();

		// TypedArray styledAttrs = getContext().obtainStyledAttributes(attrs,
		// R.styleable.CameraBridgeViewBase);
		// if (styledAttrs.getBoolean(R.styleable.CameraBridgeViewBase_show_fps,
		// false))
		// enableFpsMeter();

		// mCameraIndex =
		// styledAttrs.getInt(R.styleable.CameraBridgeViewBase_camera_id, -1);

		getHolder().addCallback(this);
		mMaxWidth = MAX_UNSPECIFIED;
		mMaxHeight = MAX_UNSPECIFIED;
		mTmp2 = BitmapFactory.decodeResource(getResources(), R.drawable.bg256);

		// styledAttrs.recycle();
	}

	

	// ///////////////////////////////////////

	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		// Log.d(TAG, "call surfaceChanged event");
		synchronized (mSyncObject) {
			if (!mSurfaceExist) {
				mSurfaceExist = true;
				checkCurrentState();
				// OnlyInfoTestFile("surfaceChanged" + num + " " + mSurfaceExist
				// );

			} else {
				/**
				 * Surface changed. We need to stop camera and restart with new
				 * parameters
				 */
				/* Pretend that old surface has been destroyed */
				mSurfaceExist = false;
				checkCurrentState();
				/* Now use new surface. Say we have it now */
				// OnlyInfoTestFile("surfaceChanged  else" + num + " " +
				// mSurfaceExist );
				mSurfaceExist = true;
				checkCurrentState();
			}
		}
	}

	public void surfaceCreated(SurfaceHolder holder) {
		/* Do nothing. Wait until surfaceChanged delivered */
	//	CreateTestFile();
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		synchronized (mSyncObject) {
			mSurfaceExist = false;
	//		OnlyInfoTestFile("bForceQuit  mstopthread" +GifActivity.bForceQuit + " " +  mStopThread  + " "+thdOver);
			
		//	 OnlyInfoTestFile("surfaceDestroyed" + num + " " + mSurfaceExist
			// );

		//	Log.e("gifeditview", "surfaceDestroyed!!surfaceDestroyed");
			checkCurrentState();
			CloseTestFile();
		}
	}

	/**
	 * This method is provided for clients, so they can enable the camera
	 * connection. The actual onCameraViewStarted callback will be delivered
	 * only after both this method is called and surface is available
	 */
	public void enableView() {
		synchronized (mSyncObject) {
			mEnabled = true;
			checkCurrentState();
		}
	}

	/**
	 * This method is provided for clients, so they can disable camera
	 * connection and stop the delivery of frames even though the surface view
	 * itself is not destroyed and still stays on the scren
	 */
	public void disableView() {
		synchronized (mSyncObject) {
			mEnabled = false;
			checkCurrentState();
		}
	}

	/**
	 * This method enables label with fps value on the screen
	 */

	/**
	 * 
	 * @param listener
	 */

	public void setGifViewListener(GifEditViewListener listener) {
		mListener = listener;
	}

	/**
	 * This method sets the maximum size that camera frame is allowed to be.
	 * When selecting size - the biggest size which less or equal the size set
	 * will be selected. As an example - we set setMaxFrameSize(200,200) and we
	 * have 176x152 and 320x240 sizes. The preview frame will be selected with
	 * 176x152 size. This method is useful when need to restrict the size of
	 * preview frame for some reason (for example for video recording)
	 * 
	 * @param maxWidth
	 *            - the maximum width allowed for camera frame.
	 * @param maxHeight
	 *            - the maximum height allowed for camera frame
	 */
	public void setMaxFrameSize(int maxWidth, int maxHeight) {
		mMaxWidth = maxWidth;
		mMaxHeight = maxHeight;
	}

	/**
	 * Called when mSyncObject lock is held
	 */
	private void checkCurrentState() {
		int targetState;

		if (mEnabled && mSurfaceExist && getVisibility() == VISIBLE) {
			targetState = STARTED;
		} else {
			targetState = STOPPED;
		}

	//	Log.e("checkCurrentState", "processEnterState" + num + " " + mEnabled
		//		+ " " + mSurfaceExist + "  " + (getVisibility() == VISIBLE));

		if (targetState != mState) {
			/*
			 * The state change detected. Need to exit the current state and
			 * enter target state
			 */
			processExitState(mState);
			mState = targetState;
			// OnlyInfoTestFile("processEnterState" + num + " " + mState);
			// OnlyInfoTestFile("processEnterState" + num + " " + mEnabled + " "
			// + mSurfaceExist + "  " + (getVisibility()== VISIBLE));
			processEnterState(mState);
		}
	}

	private void processEnterState(int state) {
		// processEnterState
		switch (state) {
		case STARTED:
			onEnterStartedState();
			if (mListener != null) {
				mListener.onGifEditViewStarted(mFrameWidth, mFrameHeight,
						" ");
			}
			break;
		case STOPPED:
			onEnterStoppedState();
			if (mListener != null) {
				mListener.onGifEditViewStopped();
			}
			break;
		}
		;
	}

	private void processExitState(int state) {

		// OnlyInfoTestFile("processExitState" + num);
		switch (state) {
		case STARTED:
			onExitStartedState();
			break;
		case STOPPED:
			onExitStoppedState();
			break;
		}
		;
	}

	private void onEnterStoppedState() {
		/* nothing to do */
	}

	private void onExitStoppedState() {
		/* nothing to do */
	}

	// NOTE: The order of bitmap constructor and camera connection is important
	// for android 4.1.x
	// Bitmap must be constructed before surface
	private void onEnterStartedState() {
		/* Connect camera */
	//	Log.e("onEnterStartedState", "onEnterStartedState");
		/*
		 * if (!connectFile(getWidth(), getHeight())) { AlertDialog ad = new
		 * AlertDialog.Builder(getContext()).create(); ad.setCancelable(false);
		 * // This blocks the 'BACK' button ad.setMessage("读取文件失败.");
		 * ad.setButton(DialogInterface.BUTTON_NEUTRAL, "OK", new
		 * DialogInterface.OnClickListener() { public void
		 * onClick(DialogInterface dialog, int which) { dialog.dismiss();
		 * ((Activity) getContext()).finish(); } }); ad.show();
		 * 
		 * }
		 */
	}

	private void onExitStartedState() {
		/*
		 * disconnectFile(); // OnlyInfoTestFile("disconnectFile" + num); num++;
		 * if (mCacheBitmap != null) { mCacheBitmap.recycle(); }
		 */
	//	EndThread();
	}

	// ////////////////////////////////

	public Rect getRect() {

		return m_rt;
	}

	public int getImgHeight() {
		if (mCacheBitmap == null)
			return 0;
		return mCacheBitmap.getHeight();
	}

	public int getImgWidth() {
		if (mCacheBitmap == null)
			return 0;

		return mCacheBitmap.getWidth();
	}

	// //////////////////////////////////////////

	protected void OneRoundSave() {
		if (mListener != null) {
			// OnlyInfoTestFile("OneRoundSave ");
			mListener.onGifOneRoundFinished();
			// ///////////////////////////

			// //////////////////////
		}
	}

	// //////////////////////////////////////////
	/**
	 * This method shall be called by the subclasses when they have valid object
	 * and want it to be delivered to external client (via callback) and then
	 * displayed on the screen.
	 * 
	 * @param frame
	 *            - the current frame to be delivered
	 */
	protected void deliverFrame(Bitmap frame) {
		// Mat modified;

		if (mListener != null) {
		//	 OnlyInfoTestFile("deliverAndDrawFrame"+mScale);
			if (mCacheBitmap != null) 
			{ mCacheBitmap.recycle(); }
			mCacheBitmap = mListener.onGifFrame(frame,mScale);
		} else {
			mCacheBitmap = frame;
		}
	}
	protected void deliverAndDrawFrame(Bitmap frame) {
		// Mat modified;

		if (mListener != null) {
			// OnlyInfoTestFile("deliverAndDrawFrame ");
			if (mCacheBitmap != null) 
			{ mCacheBitmap.recycle(); }
			mCacheBitmap = mListener.onGifFrame(frame,mScale);
		} else {
			mCacheBitmap = frame;
		}
		DrawFrame();
	}
	protected void deliverAndDrawFrameBynum(int num) {
		// Mat modified;

		if (mListener != null) {
			// OnlyInfoTestFile("deliverAndDrawFrame ");
			if (mCacheBitmap != null) 
			{ mCacheBitmap.recycle(); }
			
			mCacheBitmap = mListener.onGifFrameBynum(num,m_pause);
		/*	if(m_pause)
			{
			    GifActivity.saveBitmap(mCacheBitmap,"text.bmp" );
			    OnlyInfoTestFile("deliverAndDrawFrameBynum  &&" + num);
			}*/
		} else {
			//mCacheBitmap = frame;
		}
		DrawFrame();
	}
	protected void DrawFrame() {

		boolean bmpValid = true;
		/*
		 * if (modified != null) { try { Utils.matToBitmap(modified,
		 * mCacheBitmap); } catch(Exception e) { bmpValid = false; } }
		 */

		if (bmpValid && mCacheBitmap != null) {
			Canvas canvas = getHolder().lockCanvas();
			if (canvas != null) {
				canvas.drawColor(0, android.graphics.PorterDuff.Mode.CLEAR);
				// OnlyInfoTestFile("mScale " + mScale + "  " +
				 //mCacheBitmap.getWidth() +"  " + mCacheBitmap.getHeight());
				
			/*	if (mScale != 0) {
					m_rt = new Rect((int) ((canvas.getWidth() - mScale
							* mCacheBitmap.getWidth()) / 2),
							(int) ((canvas.getHeight() - mScale
									* mCacheBitmap.getHeight()) / 2),
							(int) ((canvas.getWidth() - mScale
									* mCacheBitmap.getWidth()) / 2 + mScale
									* mCacheBitmap.getWidth()),
							(int) ((canvas.getHeight() - mScale
									* mCacheBitmap.getHeight()) / 2 + mScale
									* mCacheBitmap.getHeight()));
				
					canvas.drawBitmap(mCacheBitmap,
							new Rect(0, 0, mCacheBitmap.getWidth(),
									mCacheBitmap.getHeight()), m_rt, null);

				} else */{
					m_rt = new Rect(
							(canvas.getWidth() - mCacheBitmap.getWidth()) / 2,
							(canvas.getHeight() - mCacheBitmap.getHeight()) / 2,
							(canvas.getWidth() - mCacheBitmap.getWidth()) / 2
									+ mCacheBitmap.getWidth(), (canvas
									.getHeight() - mCacheBitmap.getHeight())
									/ 2 + mCacheBitmap.getHeight());
					canvas.drawBitmap(mCacheBitmap,
							new Rect(0, 0, mCacheBitmap.getWidth(),
									mCacheBitmap.getHeight()), m_rt, null);
				}

				/*
				 * if (mFpsMeter != null) { mFpsMeter.measure();
				 * mFpsMeter.draw(canvas, 20, 30); }
				 */
				getHolder().unlockCanvasAndPost(canvas);
			}
		}
	}

	/**
	 * This method is invoked shall perform concrete operation to initialize the
	 * camera. CONTRACT: as a result of this method variables mFrameWidth and
	 * mFrameHeight MUST be initialized with the size of the Camera frames that
	 * will be delivered to external processor.
	 * 
	 * @param width
	 *            - the width of this SurfaceView
	 * @param height
	 *            - the height of this SurfaceView
	 */
	protected boolean connectFile(int width, int height) {
		// OnlyInfoTestFile("connectFile" + num);
		num++;
	/*	mStopThread = false;
		mThread = new Thread(new GifWorker());
		mThread.start();*/

		return true;
	}
	//////////////////////////////
	protected boolean BeginThread() {
	//	 OnlyInfoTestFile("BeginThread" + num);
		num++;
	
		   mStopThread = false;
		   thdOver = false;
		mThread = new Thread(new GifWorker());
		mThread.start();

		return true;
	}
	
	////////////////////////////

	/**
	 * Disconnects and release the particular camera object being connected to
	 * this surface view. Called when syncObject lock is held
	 */
	protected void disconnectFile() {}
	
	protected void EndThread() {
		// Log.d(TAG, "Disconnecting from camera");
		try {
			mStopThread = true;
			// Log.d(TAG, "Notify thread");
			/*
			 * synchronized (this) { this.notify(); }
			 */
			// Log.d(TAG, "Wating for thread");
		//	OnlyInfoTestFile("mThread.join ");
			if (mThread != null)
				mThread.join();

		//	OnlyInfoTestFile("mCacheBitmap.recycle ");
			if (mCacheBitmap != null) {
				mCacheBitmap.recycle();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		//	OnlyInfoTestFile(e.toString());
		} finally {
			mThread = null;
		}
	}

	// NOTE: On Android 4.1.x the function must be called before SurfaceTextre
	// constructor!
	protected void AllocateCache() {
		mCacheBitmap = Bitmap.createBitmap(mFrameWidth, mFrameHeight,
				Bitmap.Config.ARGB_8888);
	}

	public interface ListItemAccessor {
		public int getWidth(Object obj);

		public int getHeight(Object obj);
	};

	// /////////////////////////////////

	private class GifWorker implements Runnable {

		public void run() {

		//	Log.e("GifWorker", "run");

		//	 OnlyInfoTestFile("worker start ");
			if(GifActivity.nSelType == GifActivity.TYPE_LIST)
			{
			//	OnlyInfoTestFile("Thread GifActivity.TYPE_LIST ");
				RunEditList();
			}
			else if(GifActivity.nSelType == GifActivity.TYPE_FILE)
			{
				RunEditFile();
			}
			

			// ////////////////////////////////
			// Log.d(TAG, "Finish processing thread");
		}
	}

	public interface GifEditViewListener {
		/**
		 * This method is invoked when camera preview has started. After this
		 * method is invoked the frames will start to be delivered to client via
		 * the onCameraFrame() callback.
		 * 
		 * @param width
		 *            - the width of the frames that will be delivered
		 * @param height
		 *            - the height of the frames that will be delivered
		 */
		public void onGifEditViewStarted(int width, int height, String path);
	
		/**
		 * This method is invoked when camera preview has been stopped for some
		 * reason. No frames will be delivered via onCameraFrame() callback
		 * after this method is called.
		 */
		public void onGifEditViewStopped();
	
		/**
		 * This method is invoked when delivery of the frame needs to be done.
		 * The returned values - is a modified frame which needs to be displayed
		 * on the screen. TODO: pass the parameters specifying the format of the
		 * frame (BPP, YUV or RGB and etc)
		 */
		public Bitmap onGifFrame(Bitmap inputFrame,float nScale);
		
		public Bitmap onGifFrameBynum(int n,boolean bp);
	
		public void onGifOneRoundFinished();
		
		public void onTestInfo(String ss);
	}

	/*
	 * public void SetFilePath(String path) { m_path = path; }
	 */
	public void Begin(InputStream is ) {
	//	OnlyInfoTestFile("Begin exe ");
		
		if(GifActivity.nSelType == GifActivity.TYPE_LIST)
		{
			try
			{
				
						
				//
				EndThread();
			//	OnlyInfoTestFile("Begin exe 222");
				BeginThread();
				
			}
			catch (Exception ioe) {
				//Log.e("EEEEEClickactivity", ioe.getMessage());
				// OnlyInfoTestFile("Begin exe excep  "+ioe.toString());
				return;
			}
			
		}
		else
		{
		try {
			//OnlyInfoTestFile("Beginstart10000 ");

			disconnectFile();
			
		//	OnlyInfoTestFile("Begistart33333 ");
			EndThread();
			
			
		//	m_path = path;
			
		//	OnlyInfoTestFile("Beginstart1111111 ");
			 
		/*	ff = new File(m_path);
			
			OnlyInfoTestFile(m_path);
			InputStream is = new BufferedInputStream(new FileInputStream(ff),
					32768);*/
			InputStream bis = is;
			
			if (!BufferedInputStream.class.isInstance(bis))
				bis = new BufferedInputStream(is, 32768);
			// decoded = false;
		//	OnlyInfoTestFile("BufferedInputStream ");
			if (mGifDecoder != null)
				mGifDecoder = null;
			mGifDecoder = new GifDecoder();
			mGifDecoder.read(bis);

		//	OnlyInfoTestFile("Begin File start ");
			if (!connectFile(getWidth(), getHeight())) {
				
				AlertDialog ad = new AlertDialog.Builder(getContext()).create();
				ad.setCancelable(false); // This blocks the 'BACK' button
				ad.setMessage("读取文件失败.");
				ad.setButton(DialogInterface.BUTTON_NEUTRAL, "OK",
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {
								dialog.dismiss();
								((Activity) getContext()).finish();
							}
						});
				ad.show();

			}
			EndThread();
			BeginThread();
			thdOver = false;

		//	OnlyInfoTestFile("Beginend ");
			
			// mTmpBitmap = mGifDecoder.getFrame(0);

			// mTmpBitmap = BitmapFactory.decodeResource(context.getResources(),
			// R.drawable.bg256);

			// android.util.Log.v("GifAnimationDrawable",
			// "===>Lead frame: ["+width+"x"+height+"; "+mGifDecoder.getDelay(0)+";"+mGifDecoder.getLoopCount()+"]");
			// height = mTmpBitmap.getHeight();
			// width = mTmpBitmap.getWidth();
		} catch (Exception ioe) {
			//Log.e("EEEEEClickactivity", ioe.getMessage());
		//	 OnlyInfoTestFile("Thread   Exp"+ioe.getMessage());
			return;

		}
		}
	}

	// /////////////////////////////////
	public void RunEditList()
	{
		
		int n = MainActivity.GetImgListSize();
		int width =MainActivity.GetImgListWidth();
		int height = MainActivity.GetImgListHeight();
		
		if(n<=0 || width <= 0 || height <= 0)
		{
			mStopThread = true;
			thdOver = true;
		//	OnlyInfoTestFile("thdOvertrue1");
			
		    return;
		}
	//	OnlyInfoTestFile("GetImgListSize" + n);
	//	Mat mRgb;
		int t = 0;
		mFrameWidth =width;
		mFrameHeight = height;

	/*	if ((getLayoutParams().width == LayoutParams.MATCH_PARENT)
				&& (getLayoutParams().height == LayoutParams.MATCH_PARENT))
			mScale = Math.min(((float) getHeight()) / mFrameHeight,
					((float) getWidth()) / mFrameWidth);
		else
			mScale = 0;*/
		
	//	OnlyInfoTestFile("mFrameWidth " + mFrameWidth + " " + " " + mFrameHeight + " " + mScale);
		
		AllocateCache();
		t = MainActivity.nDelay;
	//	for(int i=0;i<n;i++)
		int i=0;
		GifActivity.pbar.setMax(n);
		while(i<n)
		{
		//	if (m_pause)
			//	continue;
			if(mStopThread)
			{
				thdOver = true;
			//	OnlyInfoTestFile("thdOvertrue2");
				return;
			}
			 if(GifActivity.bForceQuit)
	           {
	        	   mStopThread = true;
					thdOver = true;
				//	OnlyInfoTestFile("thdOvertrue3");
					return;
	           }
		//	mRgb = new Mat(width, height,
				//	CvType.CV_8UC3);
			//OnlyInfoTestFile("loop  " + i );
			
			//MainActivity.GetImgbyID(i, mRgb.getNativeObjAddr());
			
		//	OnlyInfoTestFile("loop2  " + mRgb.width() +"  "+ mRgb.height() + " " + mRgb.channels() );
		//	Bitmap cBmp = Bitmap.createBitmap(width, height, Config.ARGB_8888);
		//	OnlyInfoTestFile("loop25  "  + mRgb.width() +"  "+ mRgb.height() + " " + mRgb.channels() );

			//Utils.matToBitmap(mRgb, cBmp);
			
		//	OnlyInfoTestFile("mStopThread  " + mStopThread );
			if (!mStopThread) {
				// if (!mFrameChain[mChainIdx].empty())
			//	OnlyInfoTestFile("loop4  " + i );
			//	OnlyInfoTestFile("MainActivity.GetImgListSize()  " + MainActivity.GetImgListSize() );
				deliverAndDrawFrameBynum(i);
			//	OnlyInfoTestFile("loop5  " + i );
			//	Log.e("gifeditview", "deliverAndDrawFrame" + mStopThread);
				// mChainIdx = 1 - mChainIdx;
			}
			else
				break;
			try {
				if ((long) t > 0)
					Thread.sleep((long) t);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				mStopThread = true;
				thdOver = true;
			//	OnlyInfoTestFile("thdOvertrue4");
				break;
			}
			if (!m_pause)
			{
				
					i++;
					GifActivity.pbar.setProgress(i);
			}

			//mRgba.release();
		//	mRgb.release();
		}
		////////////////////////////////////////
		
		// Bitmap mTmpBitmap = mTmp2;
		
			try {
			//	 OnlyInfoTestFile("while " + mStopThread + " " + i +
				// "  OneRoundSave" );

				OneRoundSave();
				mStopThread = true;
			//	break;
			} catch (Exception ee) {
				mStopThread = true;
				thdOver = true;
			//	OnlyInfoTestFile("thdOvertrue5");
				return;
			}
			mStopThread = true;
			thdOver = true;
		//	OnlyInfoTestFile("thdOvertrue6");
			return;
		
		
		////////////////////////////////////////////
	}

	public void RunEditFile()
	{
		int i = 1,t = 0;
		int n = 0;
		try
		{
		//	OnlyInfoTestFile("RunEditFile ");
		mGifDecoder.complete();
		mFromFile = true;
	
		n = mGifDecoder.getFrameCount();
		
	//	OnlyInfoTestFile("RunEditFile " + n);
		
		Bitmap mTmp = mGifDecoder.getFrame(i);
		mFrameWidth = mTmp.getWidth();
		mFrameHeight = mTmp.getHeight();

		if ((getLayoutParams().width == LayoutParams.MATCH_PARENT)
				&& (getLayoutParams().height == LayoutParams.MATCH_PARENT))
		{
			mScale = Math.min(((float) getHeight()) / mFrameHeight,
					((float) getWidth()) / mFrameWidth);
			
		//	Log.e("EEEEEEEEEEEEE", "getHeight() " + getHeight() + " " + mFrameHeight);

		}
			
		else
			mScale = 0;
		
		//Log.e("EEEEEEEEEEEEE", "mScale " + mScale);
		AllocateCache();
		}
		catch(Exception ee)
		{
			mStopThread = true;
			thdOver = true;
		//	OnlyInfoTestFile("thdOvertrue7");
			mFromFile = false;
			return;
		}
		do {
			// OnlyInfoTestFile("do start " + mStopThread);
			/*
			 * synchronized (GifEditView.this) { try {
			 * GifEditView.this.wait(); } catch (InterruptedException e) {
			 * // TODO Auto-generated catch block e.printStackTrace(); } }
			 */

			// /////////////////////////////

			// Toast toast = Toast.makeText(GifActivity.this, "start" + i,
			// Toast.LENGTH_SHORT);
			// toast.show();
	    //	if (m_pause)
			//	continue;
           if(GifActivity.bForceQuit)
           {
        	   mStopThread = true;
				thdOver = true;
			//	OnlyInfoTestFile("thdOvertrue8");
				mFromFile = false;
				return;
           }
			Bitmap mTmpBitmap = mGifDecoder.getFrame(i);
			t = mGifDecoder.getDelay(i);
			if(t>0)
				GifActivity.nDelay = t;

		//	OnlyInfoTestFile("delay time  " + t +  mStopThread);
			// Bitmap mTmpBitmap = mTmp2;
			i++;
			if (i > n) {
				try {
				//	 OnlyInfoTestFile("while " + mStopThread + " " + i +
					// "  OneRoundSave" );

					if(mStopThread == true)
					{
						thdOver = true;
					//	OnlyInfoTestFile("thdOvertrue9");
						return;
					}
					OneRoundSave();
					mFromFile = false;
					mStopThread = true;
					break;
				} catch (Exception ee) {
					mStopThread = true;
					thdOver = true;
				//	OnlyInfoTestFile("thdOvertrue10");
					mFromFile = false;
					return;
				}
				// i = 1;
			}
		//	Log.e("gifeditview", "run" + mStopThread);
			
		//	OnlyInfoTestFile("delay time  22"+ mStopThread);

			if (!mStopThread) {
				// if (!mFrameChain[mChainIdx].empty())
				deliverFrame(mTmpBitmap);
			//	Log.e("gifeditview", "deliverAndDrawFrame" + mStopThread);
				// mChainIdx = 1 - mChainIdx;
			}
			//OnlyInfoTestFile("delay time  55" + mStopThread);
		/*	try {
				if ((long) t > 0)
					Thread.sleep((long) t);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				mStopThread = true;
				thdOver = true;
				return;
			}*/
		//	 OnlyInfoTestFile("while " + mStopThread);
			// (long)t);
		} while (!mStopThread);

		// mGifDecoder = null;
		// OnlyInfoTestFile("mStopThread " + mStopThread + " thdOver  " +
		// thdOver );

		thdOver = true;
	//	OnlyInfoTestFile("thdOvertrue115");
	}
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
			output.write(info + "$$" + "\n");
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
		
		/* CreateTestFile();
		WriteTestInfo(info);
		 CloseTestFile();*/
	//	if (mListener != null) 
		//	 mListener.onTestInfo(info);
	//	MainActivity.OnlyInfoTestFile(info);
	}

	public void SetPause(boolean bb) {
		m_pause = bb;
	}

	// ///////////////////////////////

}
