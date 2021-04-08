/*
 * [��������] Android ͨѶ¼
 * [����] xmobileapp�Ŷ�
 * [�ο�����] Google Android Samples 
 * [��ԴЭ��] Apache License, Version 2.0 (http://www.apache.org/licenses/LICENSE-2.0)
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.gifplus.pcvimg;


import com.gifplus.pcvimg.SwitchButton.OnSBtnChangeListener;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Color;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class TextEditor extends Activity {

	private static final String TAG = "TextEditor";

	

	

	private EditText text1;
	private String sText;
	
	private Button saveButton;
	private Button cancelButton;
	private Button colorButton;
	private ColorPickerDialog dialog; 
	private int nColor;

	private boolean nDanmu;
	

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		
		

		setContentView(R.layout.text_editor);
		text1 = (EditText) findViewById(R.id.editText1);
		
		
		/////////////////////////////
		
		sText = getIntent().getStringExtra("text");
		if(sText != null && sText.length() != 0)
			text1.setText(sText);
		
		/////////////////////////////
	
		nDanmu = true;//Ĭ�ϲ��õ�Ļ��ʽ��
		nColor = Color.BLUE;
		saveButton = (Button) findViewById(R.id.button1);
		cancelButton = (Button) findViewById(R.id.button2);
		colorButton = (Button) findViewById(R.id.button3);
		
		colorButton.setOnClickListener(new OnClickListener() {

			public void onClick(View v) 
			{
				 dialog = new ColorPickerDialog(TextEditor.this,  getResources().getString(R.string.app_name),    



	                        new ColorPickerDialog.OnColorChangedListener() {   



	                        



	                    public void colorChanged(int color) {   



	                        text1.setTextColor(color);  
	                        nColor = color;



	                    }   



	                });   



	                dialog.show();

			}
		});

		saveButton.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				String t1 = text1.getText().toString();
				
				if (t1.length() == 0)  {
				//	setResult(RESULT_CANCELED);
				//	deleteContact();
				//	finish();
					 Toast toast = Toast.makeText(TextEditor.this, R.string.notext, Toast.LENGTH_SHORT);
				        toast.show();
				} else {
				//	updateText();
					
					 Intent intent = new Intent(TextEditor.this,MainActivity.class); 

	                  Bundle bundle =new Bundle(); 
	                  bundle.putString("text", t1);
	                  bundle.putInt("color", nColor);
	                  bundle.putBoolean("Danmu", nDanmu);

	               //   bundle.putSerializable("selectedList", selectedImageDatas); 

	                  intent.putExtras(bundle); 
					setResult(RESULT_OK,intent);
					finish();
				}
			}

		});
		cancelButton.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				//if (mState == STATE_INSERT) {
					setResult(RESULT_CANCELED);
					//deleteContact();
					finish();
			

			}

		});

		////////////////////////////////
		SwitchButton sb = (SwitchButton) findViewById(R.id.SwitchBtn1);  
		        sb.setOnChangeListener(new OnSBtnChangeListener() {  
		              
		            @Override  
		            public void onSBtnChange(SwitchButton sb, boolean state) {  
		                // TODO Auto-generated method stub  
		            	nDanmu = state;
		            //    Log.d("switchButton", state ? "��":"��");  
		              //  Toast.makeText(MainActivity.this, state ? "��":"��", Toast.LENGTH_SHORT).show();  
		            }  
		        });  

		// ��ò�����ԭʼ��ϵ����Ϣ
	
	}

	@Override
	protected void onResume() {
		super.onResume();

		
			// ��ȡ����ʾ��ϵ����Ϣ
			
			
				setTitle(getText(R.string.text_edit));
			

		

	}

	@Override
	protected void onPause() {
		super.onPause();

		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);

		
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		
		return super.onOptionsItemSelected(item);
	}

	
	

	
	

	

}
