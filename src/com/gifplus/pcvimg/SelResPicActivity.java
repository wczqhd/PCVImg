package com.gifplus.pcvimg;

import java.util.ArrayList;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import com.gifplus.pcvimg.SelFileActivity.FloderAdapter;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.ThumbnailUtils;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.Media;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.AdapterView.OnItemClickListener;

public class SelResPicActivity extends Activity {
	
	//private ArrayList<int> mPhotoList = new ArrayList<int>(); 
	private String[] mPhotoList; 
	private int nCount = 0;
	private int fromType = 0;
	private String mDir;
	private int FROM_RECORD = 2;
	public BufferedWriter output;
	public static final String TEST_FILE1 = Environment
			.getExternalStorageDirectory().getPath()
			+ "/DCIM/GIFPLUS/"
			+ "tressel.txt";
	private static final File tfile = new File(TEST_FILE1);
	
	//private int FROM_RECORD = 2;
//	private FloderAdapter adapter;
	
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_ressel);
	//	CreateTestFile();
		
		fromType = getIntent().getIntExtra("ActivityType", 0);
		
		GridView gridview=(GridView)findViewById(R.id.gviewressel);//找到main.xml中定义gridview 的id
        
	//	int ndir = getIntent().getIntExtra("dir",1);
		///////////////////////////////////
	
	
		mDir  = getIntent().getStringExtra("dir");

		try
		{
			mPhotoList = this.getAssets().list(mDir);
			
			mDir +="/";
		//	OnlyInfoTestFile("mPhotoList size " + mPhotoList.length);
		}
		catch(IOException e)
		{
			finish();
			return;
		}


		
		//////////////////////////////////////
		gridview.setAdapter(new ResAdapter(this));//调用ImageAdapter.java
        gridview.setVerticalScrollBarEnabled(true);
    //    gridview.setBackgroundResource(R.drawable.background);
     //   gridview.set
        gridview.setOnItemClickListener(new OnItemClickListener(){//监听事件
         public void onItemClick(AdapterView<?> parent, View view, int position, long id) 
         {
        //  Toast.makeText(SelFileActivity.this, "file "+mPhotoList.get(position),Toast.LENGTH_SHORT).show();//显示信息;
        	 Intent intent;
        //	 if(fromType == FROM_EDIT)
        	     intent = new Intent(SelResPicActivity.this,GifActivity.class); 
        //	 else
        	//	 intent = new Intent(SelResPicActivity.this,RecordActivity.class);

        	 //    OnlyInfoTestFile("Respath 111   "+mDir+mPhotoList[position]);
 
        	                  Bundle bundle =new Bundle(); 
        	               //   bundle.putString("filepath", mPhotoList.get(position));
        	                  bundle.putString("Respath", mDir+mPhotoList[position]);

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
	
	////////////////////////////////////
	
	public void onDestroy() {
		
		 CloseTestFile();
		 
		 super.onDestroy();
		// if (mGifEditView != null)
		// mGifEditView.disableView();

		// gf2.destroy();

	}
////////////////////////////////////////////////////

	class ResAdapter extends BaseAdapter{ 
		 
		 private Context mContext;
			//private PictureShowUtils mAllImages = new PictureShowUtils();
			 
			 public ResAdapter(Context c)
			 {
				// OnlyInfoTestFile("context");
			  mContext=c;
			 }
			 @Override
				   public int getCount() { 

				                        // TODO Auto-generated method stub 

				                         return mPhotoList.length; 

				                } 

				   

				                 @Override 

				                public Object getItem(int position) { 

				                         // TODO Auto-generated method stub 

				                	// OnlyInfoTestFile( "123 " + mDir+mPhotoList[position]);
				                         return mPhotoList[position]; 

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
				             //  	OnlyInfoTestFile(mDir+mPhotoList[position]);
		                         imageview.setImageBitmap(getImageThumbnail(mDir+mPhotoList[position],70,70));
				                	
				               	  return imageview;
				                	 

				                       //  photoNumber.setText(imageCount+" 张"); 

				                          

				                       

				                 

				                   

				         } 

				           

				      //    @SuppressLint("NewApi") 

				         private Bitmap getImageThumbnail(String imagePath, int width, int height)  {   

				                          Bitmap bitmap = null;   

				                          BitmapFactory.Options options = new BitmapFactory.Options();   

				                          options.inJustDecodeBounds = true;   

				                          // 获取这个图片的宽和高，注意此处的bitmap为null   

				                          try
				                          {
				                          bitmap = BitmapFactory.decodeStream(mContext.getAssets().open(imagePath));   
				                          }
				                          catch(IOException e)
				                          {
				                        	  return null;
				                          }
				                      /*    options.inJustDecodeBounds = false; // 设为 false   

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

				                          try
				                          {
				                          bitmap = BitmapFactory.decodeStream(mContext.getAssets().open(imagePath));   
				                          }
				                          catch(IOException e)
				                          {
				                        	  return null;
				                          }
				                          // 利用ThumbnailUtils来创建缩略图，这里要指定要缩放哪个Bitmap对象   
*/
				                         bitmap = ThumbnailUtils.extractThumbnail(bitmap, width, height,   

				                                  ThumbnailUtils.OPTIONS_RECYCLE_INPUT);   

				                          return bitmap;   

				                      } 

			
			
			}
			
			
			
			//////////////////////////////////////////////////
	
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
			/*try {
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
			//////////////
			
		}

		// //////////////////////////////////////

	////////////////////////////////////////

}
