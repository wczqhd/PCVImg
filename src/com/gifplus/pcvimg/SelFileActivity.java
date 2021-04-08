package com.gifplus.pcvimg;

import java.util.ArrayList;

import android.media.ThumbnailUtils;
import android.os.Bundle;
import android.provider.MediaStore.Images.Media;
import android.app.Activity;
import android.view.Menu;
import android.widget.GridView;
import android.widget.ImageView;
import android.content.Intent;
import android.widget.Toast;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView;
import android.view.View;
import android.util.Log;
import android.widget.BaseAdapter;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.provider.MediaStore;


public class SelFileActivity extends Activity{
	
//	public native void Pause
	//  ();
//	System.loadLibrary("pcvimg"); 
	//static{
		{
   // 	System.loadLibrary("opencv_java");//、、//
    	//System.loadLibrary("opencv_java");
      //  System.loadLibrary("pcvimg");   
        
        
    }
	private String[] floderProjection = { 

			                       "_id",//图片ID 
			                       "_data",//图片uri 
                                    "bucket_display_name"//图片所在文件夹名字 
                                       }; 

	private ArrayList<String> mPhotoList = new ArrayList<String>(); 
	//private FloderAdapter adapter;
	private int selType = 0;



	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_gridview);
		
		GridView gridview=(GridView)findViewById(R.id.gridview);//找到main.xml中定义gridview 的id
        
		selType = getIntent().getIntExtra("SelectType", 0);
		
		///////////////////////////////////
		String orderBy = MediaStore.Images.Media.BUCKET_DISPLAY_NAME;
		Cursor cursor = getContentResolver().query(Media.EXTERNAL_CONTENT_URI, floderProjection, null, null, orderBy); 

		// System.loadLibrary("pcvimg");
	//	Log.e("PVCImg::Activity", "MainActivity.Pause();");                  
		//MainActivity.Pause();
		if (cursor!=null) { 

		cursor.moveToFirst(); 
		
	//	 Toast.makeText(MainActivity.this, "dir count  "+cursor.getCount(),Toast.LENGTH_SHORT).show();

		for (int i = 0; i < cursor.getCount(); i++) { 

	    

		   //  mFloderList.add(cursor.getString(2)); 
			//if(CheckSelType(cursor.getString(2)))
		       addFiletoList(cursor.getString(2));

		 

		 cursor.moveToNext(); 

		} 

		                         

		                     //    Log.d("PickPhotoActivity", "here is the floderlist size is===>" + mFloderList.size()); 
		cursor.close();
		                 } 


		//mPhotoList.
		
		//////////////////////////////////////
		gridview.setAdapter(new FloderAdapter(this));//调用ImageAdapter.java
        gridview.setVerticalScrollBarEnabled(true);
    //    gridview.setBackgroundResource(R.drawable.background);
     //   gridview.set
        gridview.setOnItemClickListener(new OnItemClickListener(){//监听事件
         public void onItemClick(AdapterView<?> parent, View view, int position, long id) 
         {
        //  Toast.makeText(SelFileActivity.this, "file "+mPhotoList.get(position),Toast.LENGTH_SHORT).show();//显示信息;
        	 Intent intent = new Intent(SelFileActivity.this,GifActivity.class); 

        	                  Bundle bundle =new Bundle(); 
        	                  bundle.putString("filepath", mPhotoList.get(position));

        	               //   bundle.putSerializable("selectedList", selectedImageDatas); 

        	                  intent.putExtras(bundle); 
        	              //    intent.set

        	                  setResult(RESULT_OK, intent); 

        	                          finish(); 


         }
        });

	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	public boolean CheckSelType(String path)
	{
		boolean ret = false;
		switch(selType)
		{
		case 0:
			ret = true;
			break;
		case 1:
		//	
			if(path.endsWith(".gif"))
				ret = true;
			if(path.endsWith(".GIF"))
				ret = true;
			break;
			default:
				break;
		
		}
	
		
		return ret;
		
	}
	public void addFiletoList(String ss)
	{
		
		Cursor cursor = getContentResolver().query(Media.EXTERNAL_CONTENT_URI, floderProjection, "bucket_display_name = '" +ss + "'", null, null); 

		                 if (cursor !=null) { 

		                           

		                         cursor.moveToFirst(); 

		                         for (int i = 0; i < cursor.getCount(); i++) { 
		                        	 
		                        	 if(CheckSelType(cursor.getString(1)))
		                        	 {

		                        	     if (!mPhotoList.contains(cursor.getString(1))) 
		                      	         {
		                        	    	// Toast.makeText(SelFileActivity.this, cursor.getString(1),Toast.LENGTH_SHORT).show();

		                                     mPhotoList.add(cursor.getString(1)); 
		                      	         }
		                        	 }

		                                 cursor.moveToNext(); 

		                         } 
		                         cursor.close(); 

		                 } 

		                 


	}
	
////////////////////////////////////////////////////
	
	class FloderAdapter extends BaseAdapter{ 
 
 private Context mContext;
	//private PictureShowUtils mAllImages = new PictureShowUtils();
	 
	 public FloderAdapter(Context c)
	 {
	  mContext=c;
	 }
	 @Override
		   public int getCount() { 

		                        // TODO Auto-generated method stub 

		                         return mPhotoList.size(); 

		                } 

		   

		                 @Override 

		                public Object getItem(int position) { 

		                         // TODO Auto-generated method stub 

		                         return mPhotoList.get(position); 

		                 } 

		 

		                 @Override 

		                 public long getItemId(int position) { 

		                         // TODO Auto-generated method stub 

		                         return position; 

		                } 

		   

		                 @Override 

		                 public View getView(int position, View convertView, ViewGroup parent) { 

		                       //  LayoutInflater inflater = MainActivity.this.getLayoutInflater(); 

		                       //  convertView=inflater.inflate(R.layout.pick_photo_list_item, null); 
		                	 ImageView imageview;
		               	  if(convertView==null)
		               	  {
		               	   imageview=new ImageView(mContext);
		               	   imageview.setLayoutParams(new GridView.LayoutParams(200, 200));
		               	   imageview.setScaleType(ImageView.ScaleType.CENTER_CROP);
		               	   imageview.setPadding(8,8,8,8);
		               	   
		               	 }
		               	  else
		               	  {
		               	   imageview=(ImageView) convertView;
		               	  }
		             //  	 String image_path = null; 

                       //  int imageCount = 0; 

                       /*  Cursor cursor = getContentResolver().query(Media.EXTERNAL_CONTENT_URI, floderProjection, "bucket_display_name = '" +mPhotoList.get(position) + "'", null, null); 

                         if (cursor !=null) { 

                                 imageCount =cursor.getCount(); 

                                 cursor.moveToFirst(); 

                                 image_path = cursor.getString(1);//图片路径 

                                 Log.d("PickPhoto", "the floder photo id is ===> " + image_path); 

                         } 

                         cursor.close(); */
                         imageview.setImageBitmap(getImageThumbnail(mPhotoList.get(position),70,70));
		                	
		               	  return imageview;
		                	 

		                       //  photoNumber.setText(imageCount+" 张"); 

		                          

		                       

		                 

		                   

		         } 

		           

		      //    @SuppressLint("NewApi") 

		         private Bitmap getImageThumbnail(String imagePath, int width, int height) {   

		                          Bitmap bitmap = null;   

		                          BitmapFactory.Options options = new BitmapFactory.Options();   

		                          options.inJustDecodeBounds = true;   

		                          // 获取这个图片的宽和高，注意此处的bitmap为null   

		                          bitmap = BitmapFactory.decodeFile(imagePath, options);   

		                          options.inJustDecodeBounds = false; // 设为 false   

		                        // 计算缩放比   

		                          int h = options.outHeight;   

		                          int w = options.outWidth;   

		                          int beWidth = w / width;   

		                          int beHeight = h / height;   

		                          int be = 1;   

		                          if (beWidth < beHeight) {   

		                              be = beWidth;   

		                          } else {   

		                             be = beHeight;   

		                          }   

		                          if (be <= 0) {   

		                              be = 1;   

		                          }   

		                          options.inSampleSize = be;   

		                          // 重新读入图片，读取缩放后的bitmap，注意这次要把options.inJustDecodeBounds 设为 false   

		                          bitmap = BitmapFactory.decodeFile(imagePath, options);   

		                          // 利用ThumbnailUtils来创建缩略图，这里要指定要缩放哪个Bitmap对象   

		                         bitmap = ThumbnailUtils.extractThumbnail(bitmap, width, height,   

		                                  ThumbnailUtils.OPTIONS_RECYCLE_INPUT);   

		                          return bitmap;   

		                      } 

	
	
	}
	
	
	
	//////////////////////////////////////////////////
}
