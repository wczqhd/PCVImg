package com.gifplus.pcvimg;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class SlideSecondActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.sec_main);
		
		Button button = (Button)findViewById(R.id.buttonsec);
        button.setOnClickListener(new View.OnClickListener() {	
			@Override
			public void onClick(View v) {
				Intent intent = new Intent();
				intent.setClass(SlideSecondActivity.this, MainActivity.class);
				startActivity(intent);
				//�����л����������ұ߽��룬����˳�
				overridePendingTransition(R.anim.in_from_right, R.anim.out_to_left);				
			}
		});
	}

}
