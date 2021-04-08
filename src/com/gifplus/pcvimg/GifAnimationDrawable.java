/**
 * Copyright (C) 2013 Orthogonal Labs, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.gifplus.pcvimg;

import java.io.File;
import java.io.FileWriter;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import android.util.Log;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.android.Utils;




import android.graphics.drawable.AnimationDrawable;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.Bitmap.Config;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.content.Context;
import android.widget.TextView;
import android.widget.Toast;
import android.os.Environment;
import android.os.Handler;

/**
 * <p>Creates an AnimationDrawable from a GIF image.</p>
 *
 * @author wangchengzhi
 */
public class GifAnimationDrawable extends AnimationDrawable
{
	private boolean decoded;
	
	private GifDecoder mGifDecoder;

	private Bitmap mTmpBitmap;
	private int                    mViewMode;
	private Mat                    mRgba;
    private Mat                    mRgb;

	private int height, width;
	
	private Context context;
	private Handler hnd;
	public  String sinfo;
	
	private static final String PHOTO_DIR = Environment.getExternalStorageDirectory().getPath() + "/DCIM/GIFPLUS/";  
	  
	//    private static final File cam_file = new File(PHOTO_DIR);
	    private static final String testfile = "test.txt";
	    private static final File tfile = new File(PHOTO_DIR + testfile);
	    private static int num = 0;
	BufferedWriter output;
	
	Runnable mUpdateResults = new Runnable() {
        public void run() {
        	mGifDecoder.complete();
        //	MainActivity.minfo = "mGifDecoder.complete";
        			int i; 
        			int n = mGifDecoder.getFrameCount();
        	//		MainActivity.minfo = "getFrameCount";
        		//	OnlyInfoTestFile(MainActivity.minfo);
        					int t;
        		// Toast toast1 = Toast.makeText(context, "num" + num, Toast.LENGTH_SHORT);

        			//		        toast1.show();
        				//	        num++;
        			try
        			{
        	        for(i=1;i<n;i++)
        			//for(i=1;i<10;i++)
        	        {
        	            mTmpBitmap = mGifDecoder.getFrame(i);
        	        //    MainActivity.minfo = "mGifDecoder.getFrame";
        	          //  OnlyInfoTestFile(MainActivity.minfo + i);
        	        //	mTmpBitmap = BitmapFactory.decodeResource(context.getResources(), R.drawable.bg256);

        	        //	t=5;
        	            t = mGifDecoder.getDelay(i);
        	            //MainActivity.minfo = "mGifDecoder.getDelay" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%" + t+"**";
        	            //OnlyInfoTestFile(MainActivity.minfo + i);
        	            Bitmap copydstBmp= mTmpBitmap.copy(Config.ARGB_8888, true);
        	           // MainActivity.minfo = "mTmpBitmap.copy11111" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%";
        	            //OnlyInfoTestFile(MainActivity.minfo + i);
        	            mRgba = new Mat(mTmpBitmap.getHeight(), mTmpBitmap.getWidth(), CvType.CV_8UC4);
        	           // MainActivity.minfo = "mRgba = new Mat" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%";
        	            //OnlyInfoTestFile(MainActivity.minfo + i);
        	            mRgb = new Mat(mTmpBitmap.getHeight(), mTmpBitmap.getWidth(), CvType.CV_8UC3);
        	          //  android.util.Log.v("GifAnimationDrawable", "===>Frame "+i+": "+t+"]");
        	            //MainActivity.minfo = "mRgb = new Mat" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%";
        	            //OnlyInfoTestFile(MainActivity.minfo + i);
        	            Utils.bitmapToMat(copydstBmp, mRgba);//(modified, mTmpBitmap);
        	            //MainActivity.minfo = "Utils.bitmapToMat" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%";
        	            //OnlyInfoTestFile(MainActivity.minfo + i);
        	            
        	            
        	       //     Toast toast1 = Toast.makeText(context, "PerfFunc" + i, Toast.LENGTH_SHORT);
        	         //   toast1.show();
        	        //    mTmpBitmap.
        	        //    Log.e("EEEEEEEEEEEEE", "mRgba dep == " + mRgba.depth());
        	          //  Log.e("EEEEEEEEEEEEE", "mRgb dep == " + mRgb.depth());
        	      //      MainActivity.SetMode(2);
        	        //    int nn =MainActivity.PerfFunc(mViewMode,mRgb.getNativeObjAddr(), mRgba.getNativeObjAddr());

        	       //     MainActivity.minfo = "MainActivity.PerfFunc" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%";
        	         //   OnlyInfoTestFile(MainActivity.minfo + i);
        	          //  Log.e("EEEEEEEEEEEEE", "return == " + nn);
        	            
        	          //  sinfo = "return ==" + nn;
        	            Bitmap cBmp = mTmpBitmap.copy(Config.ARGB_8888, true);
        	         //   android.util.Log.v("GifAnimationDrawable", "===>Frame "+i+": "+t+"]");
        	            Utils.matToBitmap(mRgb, cBmp);
        	            //MainActivity.minfo = "Utils.matToBitmap" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%";
        	            //OnlyInfoTestFile(MainActivity.minfo + i);
        	          //  mTmpBitmap = new Bitmap();
        	            
        	            
        	            
        	            
        	            addFrame(new BitmapDrawable(context.getResources(),cBmp), t);

        	            //MainActivity.minfo = "addFrame(new BitmapDrawable" +"**"+ mTmpBitmap.getHeight()+ "&&" + mTmpBitmap.getWidth() + "%%";
        	           // OnlyInfoTestFile(MainActivity.minfo + i);
        	            copydstBmp.recycle();
        	            mTmpBitmap.recycle();
        	            mRgba.release();
        	            mRgb.release();
        	            //MainActivity.minfo = "mTmpBitmap.recycle();";
        	            //OnlyInfoTestFile(MainActivity.minfo + i);
        	            
        	      //      addFrame(new BitmapDrawable(context.getResources(),mTmpBitmap), t);
        	          //  mRgba.release();
        	           // mRgb.release();
        	        }
        			}
        			catch(Exception ioe){
        			//	Toast toast1 = Toast.makeText(context, ioe.getMessage(), Toast.LENGTH_SHORT);
        	          //  toast1.show();
        			//	Log.e("EEEErunnable", ioe.getMessage());
        				//OnlyInfoTestFile(ioe.getMessage());
        				sinfo = ioe.getMessage();
        	            return;
        				
        			}
        	     //   Toast toast1 = Toast.makeText(context, "PerfFunc" , Toast.LENGTH_SHORT);
        	       //     toast1.show();
        	        decoded = true;
        	        mGifDecoder = null; 
        	        //MainActivity.minfo = "mGifDecoder = null" +"**";
        	       // OnlyInfoTestFile(MainActivity.minfo);
        }
    };
	
	//private DensityUtil du;
	/*
	public GifAnimationDrawable(Context cnxt,File f) throws IOException
	{
		
		this(cnxt,f, false);
		context = cnxt;
	}
	
	public GifAnimationDrawable(Context cnxt,InputStream is) throws IOException
	{
		this(cnxt,is, false);
		context = cnxt;
	}
	
	public GifAnimationDrawable(Context cnxt,File f, boolean inline) throws IOException
	{
	    this(cnxt,new BufferedInputStream(new FileInputStream(f), 32768), inline);
	    context = cnxt;
	}
	
	*/
	//public GifAnimationDrawable(Context cnxt,InputStream is, boolean inline) throws IOException
	public GifAnimationDrawable(Context cnxt,File ff,Handler handler,boolean inline) throws IOException
	
	{
		super();
		context = cnxt;
		hnd = handler;
		InputStream  is = new BufferedInputStream(new FileInputStream(ff), 32768);
		InputStream bis = is;
		if(!BufferedInputStream.class.isInstance(bis)) bis = new BufferedInputStream(is, 32768);
		decoded = false;
		mGifDecoder = new GifDecoder();
		mGifDecoder.read(bis);
		mTmpBitmap = mGifDecoder.getFrame(0);
		
	//	mTmpBitmap = BitmapFactory.decodeResource(context.getResources(), R.drawable.bg256);

			//	android.util.Log.v("GifAnimationDrawable", "===>Lead frame: ["+width+"x"+height+"; "+mGifDecoder.getDelay(0)+";"+mGifDecoder.getLoopCount()+"]");
		height = mTmpBitmap.getHeight();
    	width = mTmpBitmap.getWidth();
    	 Bitmap copydstBmp= mTmpBitmap.copy(Config.ARGB_8888, false);
    	 addFrame(new BitmapDrawable(context.getResources(),copydstBmp), mGifDecoder.getDelay(0));
    	// MainActivity.minfo = "addFrame1111";
    	// OnlyInfoTestFile(MainActivity.minfo);
    //	 addFrame(new BitmapDrawable(context.getResources(),copydstBmp), 5);
    	    
      //  addFrame(new BitmapDrawable(context.getResources(),mTmpBitmap), mGifDecoder.getDelay(0));
     //   du = new DensityUtil( context);
      //  setOneShot(mGifDecoder.getLoopCount() != 0);
    	 setOneShot(false);
        setVisible(true, true);
		if(inline){
		//	loader.run();
		}else{
			Thread t = new Thread() {
	             public void run() {
	            	 hnd.post(mUpdateResults); 
	            }
	         };
	         t.start();
		}
	}
	
	public boolean isDecoded(){ return decoded; }
	
	/*private Runnable loader = new Runnable(){
		public void run() 
		{
			hnd.post(mUpdateResults); 
	    }
	};*/
	////////////////////////////
	
	private void CreateTestFile()
	{
	/*	try{      
		 //   File file = new File("D:/abc.txt");      
		    if (tfile.exists())
		    {      
		    tfile.delete();          
		    } 
		    
		     
		    tfile.createNewFile();
		    
		    output= new BufferedWriter(new FileWriter(tfile));
		}
		catch (IOException e) 
	    {
	    	Log.i("IOException===", e.getMessage());
	    //	((TextView)findViewById(R.id.textview1)).setText("IOException===" + e.getMessage());
	    }*/
	}
	
	private void WriteTestInfo(String info)
	{
		/*try
		{
			output.write(info + "\n");
		}
		catch (IOException e) 
	    {
	    	Log.i("IOException===", e.getMessage());
	    //	((TextView)findViewById(R.id.textview1)).setText("IOException===" + e.getMessage());
	    }
		
		*/
		
	}
	private void CloseTestFile()
	{
		/*try
		{
		output.close();
		}
		catch (IOException e) 
	    {
	    	Log.i("IOException===", e.getMessage());
	    //	((TextView)findViewById(R.id.textview1)).setText("IOException===" + e.getMessage());
	    }*/
	}
	private void OnlyInfoTestFile(String info)
	{
	//	CreateTestFile();
		//WriteTestInfo(info);
		//CloseTestFile();
	}
	
	
	//////////////////////////////
	
	public int getMinimumHeight(){ return height; }
	public int getMinimumWidth(){ return width; }
	public int getIntrinsicHeight(){ return height; }
	public int getIntrinsicWidth(){ return width; }
}
