package com.gifplus.pcvimg;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import android.media.MediaRecorder; 
import android.media.MediaRecorder.OnInfoListener;

public class LeftRightSlideActivity extends Activity implements OnInfoListener {
	
	//public native void Pause
	  //();
	private MediaRecorder mMediaRecorder;
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	
        super.onCreate(savedInstanceState); 
        setContentView(R.layout.main);     
        
        Button button = (Button)findViewById(R.id.button1);
      //  Pause();
        
     //   mMediaRecorder = new MediaRecorder();
        
       // mMediaRecorder.setOnInfoListener(this);
       
        
        button.setOnClickListener(new View.OnClickListener() {	
			@Override
			public void onClick(View v) {
				Intent intent = new Intent();
				intent.setClass(LeftRightSlideActivity.this, SlideSecondActivity.class);
				startActivity(intent);
				//设置切换动画，从右边进入，左边退出
				overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);				
			}
		});
    }
	@Override
	public void onInfo(MediaRecorder mr, int what, int extra) {
		// TODO Auto-generated method stub
		
	}
}