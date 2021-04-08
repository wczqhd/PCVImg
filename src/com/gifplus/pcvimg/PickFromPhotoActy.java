package com.gifplus.pcvimg;

import java.io.FileNotFoundException;
import java.io.OutputStream;
import android.graphics.drawable.Drawable;
import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.util.Log;
import android.graphics.Paint;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore.Images.Media;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;
import org.opencv.core.Rect;
import android.widget.Toast;

public class PickFromPhotoActy extends BaseActivity implements OnTouchListener,
        OnClickListener {

    private ImageView image;
    private Paint paint;
    private Canvas canvas;
    private Bitmap bitmap;
    private Bitmap alterBitmap;
    private Button buttonok;
    private Button buttoncancel;
    private Button buttonbrowse;
    private int originalImageOffsetX;
	private int originalImageOffsetY;
	private String  spath;
	private boolean bConfirm;
	
	private int  nKoutuStatus = MotionEvent.ACTION_UP;///
    private Rect touchedRect = new Rect();
    private final static int RESULT = 0;
    public static final int BROWSE_PHOTO = 1400;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pickfromphoto);
        image = (ImageView) findViewById(R.id.photoview);
        buttonok = (Button) findViewById(R.id.buttonok);
        buttoncancel=(Button)findViewById(R.id.buttoncancel);
        buttonbrowse=(Button)findViewById(R.id.buttonbrowse);
        /*
         * bitmap = Bitmap.createBitmap(getWindowManager().getDefaultDisplay()
         * .getWidth(), getWindowManager().getDefaultDisplay().getHeight(),
         * Bitmap.Config.ARGB_8888); canvas = new Canvas(bitmap);// 画板 paint =
         * new Paint();// 画刷 paint.setColor(Color.BLUE);
         * image.setImageBitmap(bitmap);
         */

        image.setOnTouchListener(this);
        buttonok.setOnClickListener(this);
        buttoncancel.setOnClickListener(this);
        buttonbrowse.setOnClickListener(this);
        
        touchedRect.x = image.getWidth();
        touchedRect.y = image.getHeight();
        touchedRect.width = 0;
        touchedRect.height = 0;
        
        String path = "   ";

        Intent intent1 = this.getIntent();  
        
        if(intent1 != null)
        {

        path = intent1.getStringExtra("path");
       
        if(path != null)
        {
        	spath = path;
       DrawImage(spath);
       bConfirm = false;
        }
        }
        /////test
    //    DrawImage(null);

        setGuideResId(R.drawable.yindao03);
    }

    private int downx = 0;
    private int downy = 0;
    private int upx = 0;
    private int upy = 0;

    public boolean onTouch(View v, MotionEvent event) {
        int action = event.getAction();
        switch (action) {
        case MotionEvent.ACTION_DOWN:
        	
            downx = (int) event.getX();
            downy = (int) event.getY();
            FixPoint(downx,downy);
            downx = originalImageOffsetX;
            downy = originalImageOffsetY;
    //        Log.e("PickFromPhoto", "Touch!!!!  down "  + " " + downx +" "+downy);///

            touchedRect.x = (int)downx;
            touchedRect.y = (int)downy;
            MainActivity.onmousekoutu(action,downx,downy);
            break;
        case MotionEvent.ACTION_MOVE:
            // 路径画板
            upx = (int) event.getX();
            upy = (int) event.getY();
            FixPoint(upx,upy);
        //    Log.e("PickFromPhoto", "Touch!!!!  up "  + " " + upx +" "+upy);///

            upx = originalImageOffsetX;
            upy = originalImageOffsetY;
            
            if(touchedRect.x > upx)
            	touchedRect.x=(int) upx;
    	    if(touchedRect.y>upy)
    	    	touchedRect.y=(int) upy;
    	    if(touchedRect.width<upx)
    	    	touchedRect.width = (int) upx;

    		if(touchedRect.height<upy)
    			touchedRect.height=(int) upy;
    		
    		if(canvas == null)
    		{
    		//	Toast toast = Toast.makeText(this, "no image", Toast.LENGTH_SHORT);
              //  toast.show();
    			break;
    		}
    		MainActivity.onmousekoutu(action,upx,upy);
            canvas.drawLine(downx, downy, upx, upy, paint);
    	//	 canvas.drawLine(0, 0, 1160, 1324, paint);
            image.invalidate();
            downx = upx;
            downy = upy;
            break;
        case MotionEvent.ACTION_UP:
            // 直线画板

            upx = (int) event.getX();
            upy = (int) event.getY();
            FixPoint(upx,upy);
            //    Log.e("PickFromPhoto", "Touch!!!!  up "  + " " + upx +" "+upy);///

                upx = originalImageOffsetX;
                upy = originalImageOffsetY;
            if(canvas == null)
    			break;
            MainActivity.onmousekoutu(action,upx,upy);
            canvas.drawLine(downx, downy, upx, upy, paint);
            image.invalidate();// 刷新
            
            touchedRect.width -= touchedRect.x;

    		
        	touchedRect.height-=touchedRect.y;
        	int ret = ConfirmPick();
        	if(ret == 0)
        	{
        		if(MainActivity.matKoutu != null)
        		    Utils.matToBitmap(MainActivity.matKoutu, bitmap);
        		DrawBitmap(bitmap);
        		image.invalidate();// 刷新
        	}
            break;////

        default:
            break;
        }

        return true;
    }
    public void Cancel()
    {
    	 touchedRect.x = image.getWidth();
         touchedRect.y = image.getHeight();
         touchedRect.width = 0;
         touchedRect.height = 0;
       //  if(spath != null)
      //   {
     //   Uri imguri =  Uri.parse(spath);
   /*     if(imguri == null)
        {
     	   Toast toast = Toast.makeText(this, "null uri", Toast.LENGTH_SHORT);
            toast.show();
        }*/
//       	  Log.e("PickFromPhoto", "cancel  ");///

        DrawImage(spath);
        image.invalidate();
    }
    
    public int ConfirmPick()
    {
    	int ret = 0;
    	if(touchedRect.width <= 0)
    		return -1;
    	if(MainActivity.matKoutu != null)
    	   MainActivity.matKoutu.release();
    	if(MainActivity.matKoutua != null)
     	   MainActivity.matKoutua.release();
    //	MainActivity.matKoutua = new Mat(touchedRect.height, touchedRect.width, CvType.CV_8UC4);;
		
    	//MainActivity.matKoutu = new Mat(touchedRect.height, touchedRect.width, CvType.CV_8UC3);;
    
       MainActivity.matKoutua = new Mat(alterBitmap.getHeight(), alterBitmap.getWidth(), CvType.CV_8UC4);;
       MainActivity.matKoutu = new Mat(alterBitmap.getHeight(), alterBitmap.getWidth(), CvType.CV_8UC3);;
		
    	//MainActivity.matKoutu = new Mat(touchedRect.height, touchedRect.width, CvType.CV_8UC3);;
  
    	//Log.e("PickFromPhoto", "bitmapToMat");///
    	
        Utils.bitmapToMat(alterBitmap, MainActivity.matKoutua);
        
         ret = MainActivity.ConfirmKouTu(touchedRect.x, touchedRect.y, touchedRect.width, touchedRect.height, MainActivity.matKoutu.getNativeObjAddr(), MainActivity.matKoutua.getNativeObjAddr());

      //   Log.e("PickFromPhoto", "ConfirmKouTu!!! "  + " " + ret + " " + touchedRect.x +" "+touchedRect.y + " " + touchedRect.width + " " + touchedRect.height);///

        // Log.e("PickFromPhoto", "ConfirmKouTu## "  + " " + alterBitmap.getHeight()+ " " + alterBitmap.getWidth());///

         
         return ret;
    }

    public void onClick(View arg0) {
        if(arg0==buttonbrowse){
      //  Intent intent = new Intent(Intent.ACTION_PICK,
        //        android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        //startActivityForResult(intent, RESULT);
        	Intent intent1 = new Intent(PickFromPhotoActy.this, SelFileActivity.class); 

			//	Intent intent1 = new Intent(MainActivity.this, PickFromPhotoActy.class); 

                intent1.putExtra("SelectType", 0); 

           //     intent.putExtra("floderName", mFloderList.get(arg2)); 

                startActivityForResult(intent1, BROWSE_PHOTO);
        }else if(arg0==buttonok){
            //保存画好的图片
            if(alterBitmap!=null){
                try {
                	
                	
                //	touchedRect.width -= touchedRect.x;

            		
               // 	touchedRect.height-=touchedRect.y;
               // 	Log.e("PickFromPhoto ok", "touchedRect " + touchedRect.x+ "  "+touchedRect.y + "  "+touchedRect.width + "  "+touchedRect.height);///
                	int ret = ConfirmPick();
                    if(ret != 0)
                    {
                    //	Log.e("PickFromPhoto", "ConfirmPick return != 0 !!! "  + " " + ret + " "+ touchedRect.x +" "+touchedRect.y + " " + touchedRect.width + " " + touchedRect.height);///

                    	 return;
                    }
                //    Log.e("PickFromPhoto", "onok!!! "  + " " + touchedRect.x +" "+touchedRect.y + " " + touchedRect.width + " " + touchedRect.height);///

                    //Log.e("PickFromPhoto", "alterBitmap  " + alterBitmap.getHeight() + "  " + alterBitmap.getWidth());///
        		     ret = 	MainActivity.InSertKoutu(touchedRect.x, touchedRect.y, touchedRect.width, touchedRect.height, MainActivity.matKoutu.getNativeObjAddr(), MainActivity.matKoutua.getNativeObjAddr());
                /*    Uri imageUri=getContentResolver().insert(Media.EXTERNAL_CONTENT_URI, new ContentValues());
                    OutputStream outputStream=getContentResolver().openOutputStream(imageUri);
                    alterBitmap.compress(CompressFormat.PNG, 90, outputStream);
                    Toast.makeText(getApplicationContext(), "save!", Toast.LENGTH_SHORT).show();*/
                
        	//	Log.e("PickFromPhoto", "InSertKoutu return  " + ret  + " " + MainActivity.matKoutu.type());///
        	    
        		//	Log.e("PickFromPhoto", "finish");///
                
                    finish();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
        else if(arg0==buttoncancel)
        {
        	 Cancel();
              
        }
    }
    public void DrawBitmap(Bitmap bmp)
    {
    	try
    	{
    	alterBitmap = Bitmap.createBitmap(bmp.getWidth(),
                bmp.getHeight(), bmp.getConfig());
        canvas = new Canvas(alterBitmap);
        paint = new Paint();
        paint.setColor(Color.BLACK);
        paint.setStrokeWidth(15);
        Matrix matrix = new Matrix();
        canvas.drawBitmap(bmp, matrix, paint);
        image.setImageBitmap(alterBitmap);
        image.setOnTouchListener(this);
        //
//   } catch (FileNotFoundException e) {
    } catch (Exception e) {

        e.printStackTrace();
     //   Toast toast = Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT);
       // toast.show();
     //   Toast toast = Toast.makeText(this, e.toString(), Toast.LENGTH_SHORT);
       // toast.show();
        
    }
    
    	
    }
    public void DrawImage(String imageFileUri)
    {
    	//Uri imageFileUri = data.getData();
    	/////////////////为了测试注释掉
    	if(imageFileUri == null)
    	{
    	//	 String ss = imageFileUri.getPath() + " " + dw + "  " + dh;
      //       Toast toast = Toast.makeText(this, "null uri", Toast.LENGTH_SHORT);
        //     toast.show();
    		return;
    	}
    /*	else
    	{
    		 String ss = imageFileUri.getPath();
             Toast toast = Toast.makeText(this, ss + "%%%", Toast.LENGTH_SHORT);
             toast.show();
    	}*/
    	////////////////////
        Display display = getWindowManager().getDefaultDisplay();
        float dw = display.getWidth();
        float dh = display.getHeight();

        try {
            BitmapFactory.Options options = new BitmapFactory.Options();
            options.inJustDecodeBounds = true;
/////////////////为了测试注释掉
            spath = imageFileUri;//.getPath();///
          //  spath = ss;
      //      bitmap = BitmapFactory.decodeStream(getContentResolver()
        //            .openInputStream(imageFileUri), null, options);
        //    String ss = imageFileUri.getPath() + " " + dw + "  " + dh;
         //   bitmap = BitmapFactory.decodeFile(ss, options);
            bitmap = BitmapFactory.decodeFile(imageFileUri);
         //   bitmap = BitmapFactory.decodeResource(PickFromPhotoActy.this.getResources(), R.drawable.b1);
            
            DrawBitmap(bitmap);
            /////////////////////////////
            if(MainActivity.matKoutu != null)
         	   MainActivity.matKoutu.release();
         	if(MainActivity.matKoutua != null)
          	   MainActivity.matKoutua.release();
         //	MainActivity.matKoutua = new Mat(touchedRect.height, touchedRect.width, CvType.CV_8UC4);;
     		
         	//MainActivity.matKoutu = new Mat(touchedRect.height, touchedRect.width, CvType.CV_8UC3);;
         
            MainActivity.matKoutua = new Mat(bitmap.getHeight(), bitmap.getWidth(), CvType.CV_8UC4);;
            MainActivity.matKoutu = new Mat(bitmap.getHeight(), bitmap.getWidth(), CvType.CV_8UC3);;
     	
            Utils.bitmapToMat(bitmap, MainActivity.matKoutua);
            
        //    Log.e("PickFromPhoto", "canvas   "  + " " + canvas.getHeight() +" "+canvas.getWidth() );

            
             MainActivity.InitKoutou( MainActivity.matKoutu.getNativeObjAddr(), MainActivity.matKoutua.getNativeObjAddr());

            
            //////////////////////////////////////////////
          //  Toast toast = Toast.makeText(this, ss, Toast.LENGTH_SHORT);
            //toast.show();
            /////////////////
         //   Log.e("PickFromPhoto", "InitKoutou" +  "  " + bitmap.getHeight() + " " + bitmap.getWidth());///

        //    bitmap = BitmapFactory.decodeResource(
    		//		getResources(),
    			//	R.drawable.bg);
       /*     int heightRatio = (int) Math.ceil(options.outHeight / dh);
            int widthRatio = (int) Math.ceil(options.outWidth / dw);
            if (heightRatio > 1 && widthRatio > 1) {
                if (heightRatio > widthRatio) {
                    options.inSampleSize = heightRatio;
                } else {
                    options.inSampleSize = widthRatio;
                }
            }
            options.inJustDecodeBounds = false;*/
            //为了测试注释掉
        //    bitmap = BitmapFactory.decodeStream(getContentResolver()
          //          .openInputStream(imageFileUri), null, options);
         //   bitmap = BitmapFactory.decodeFile(ss, options);
            ///////////////////
        }
        catch (Exception e) {

            e.printStackTrace();
         //   Toast toast = Toast.makeText(this, e.getMessage(), Toast.LENGTH_SHORT);
           // toast.show();
         //   Toast toast = Toast.makeText(this, e.toString(), Toast.LENGTH_SHORT);
           // toast.show();
            
        }
            
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // TODO Auto-generated method stub
        super.onActivityResult(requestCode, resultCode, data);
        if(null == data)
           {
            
                  return;
           }
        if ( requestCode == BROWSE_PHOTO && resultCode == RESULT_OK) {
        	 Bundle bundle = data.getExtras();
        	   //获取相机返回的数据，并转换为图片格式
        	   String filepath = (String)bundle.get("filepath");
        	   if(null == filepath)
        	   {
        		 super.onActivityResult(requestCode, resultCode, data);
        	//	Toast toast = Toast.makeText(this, "null filepath", Toast.LENGTH_SHORT);
		      //        toast.show();   
        		 return;
        	   }
        	spath = filepath;
        //	 Toast toast = Toast.makeText(this, spath, Toast.LENGTH_SHORT);
          //   toast.show();
        	DrawImage(spath);
            
            	//
        }
    }
    
    public void FixPoint(float downx2,float downy2)
    {
    	ImageView imageView = (ImageView)findViewById(R.id.photoview);
    	Drawable drawable = imageView.getDrawable();
    	android.graphics.Rect imageBounds = drawable.getBounds();

    	//Log.e("PickFromPhoto", "FixPoint Start  " );///
    	float[] values = new float[9];
 	    Matrix max = imageView.getImageMatrix();
 	    max.getValues(values);
 	    float left = values[2];
 	    float top = values[5];
 	    float right = left + imageBounds.width()*values[0];
 	    float bottom = top + imageBounds.height()*values[0];
    	//初始化bitmap的宽高
    	//int intrinsicHeight = drawable.getIntrinsicHeight();
    	//int intrinsicWidth = drawable.getIntrinsicWidth();
    	
    	
    	//Log.e("PickFromPhoto", "imageView   "  + " " + left +" "+top + "  " + right + "  " + bottom);///

    	//Log.e("PickFromPhoto", "canvas   "  + " " + canvas.getHeight() +" "+canvas.getWidth() );

    	//可见image的宽高
    //	int scaledHeight = imageBounds.height();
    	//int scaledWidth = imageBounds.width();

    //	Log.e("PickFromPhoto", "scaledHeight   "  + " " + scaledHeight +" "+scaledWidth);///

    	//使用fitXY
    	float heightRatio = canvas.getHeight() / (bottom - top);
    	float widthRatio = canvas.getWidth() / (right-left);
    	
    	//Log.e("PickFromPhoto", "heightRatio   "  + " " + heightRatio +" "+widthRatio);///

    	//Log.e("PickFromPhoto", "imageBounds   "  + " " + imageBounds.left +" "+imageBounds.top + "" + imageBounds.height() + "" + imageBounds.width());///


    	//获取图像边界值
    	float scaledImageOffsetX =  (downx2 - left);
    	float scaledImageOffsetY =  (downy2 - top);
    	
    //	Log.e("PickFromPhoto", "scaledImageOffsetX   "  + " " + scaledImageOffsetX +" "+scaledImageOffsetY);///


    	//根据你图像的缩放比例设置
    	 originalImageOffsetX = (int) (scaledImageOffsetX * widthRatio);
    	 originalImageOffsetY = (int) (scaledImageOffsetY * heightRatio);
    	
    //	Log.e("PickFromPhoto", "downx2   "  + " " + downx2 +" "+downy2);///

    	//Log.e("PickFromPhoto", "originalImageOffsetX   "  + " " + originalImageOffsetX +" "+originalImageOffsetY);///

    }
    /////////////////////
    
    private void OnlyInfoTestFile(String info) {
		/*
		 * CreateTestFile(); WriteTestInfo(info); CloseTestFile();
		 */
	//	WriteTestInfo(info);
	//	MainActivity.OnlyInfoTestFile(info);
	}

}