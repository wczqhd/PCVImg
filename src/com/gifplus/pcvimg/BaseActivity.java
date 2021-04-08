package com.gifplus.pcvimg;

import android.app.Activity;

import android.view.View;

import android.view.ViewGroup;

import android.view.ViewParent;

import android.widget.FrameLayout;

import android.widget.ImageView;

import android.widget.ImageView.ScaleType;

 

public class BaseActivity extends Activity {

    private int guideResourceId=0;//����ҳͼƬ��Դid

    @Override

    protected void onStart() {

        super.onStart();

        addGuideImage();//�������ҳ

    }

 

    /**

     * �������ͼƬ

     */

    public void addGuideImage() {

        View view = getWindow().getDecorView().findViewById(R.id.my_content_view);//����ͨ��setContentView�ϵĸ�����
///////////////////
        if(view==null)return;

        if(MyPreferences.activityIsGuided(this, this.getClass().getName())){

            //��������

            return;

        }

        ViewParent viewParent = view.getParent();

        if(viewParent instanceof FrameLayout){

            final FrameLayout frameLayout = (FrameLayout)viewParent;

            if(guideResourceId!=0){//����������ͼƬ

                final ImageView guideImage = new ImageView(this);

                FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT);

                guideImage.setLayoutParams(params);

                guideImage.setScaleType(ScaleType.FIT_XY);

                guideImage.setImageResource(guideResourceId);

                guideImage.setOnClickListener(new View.OnClickListener() {

                    @Override

                    public void onClick(View v) {

                        frameLayout.removeView(guideImage);

                        MyPreferences.setIsGuided(getApplicationContext(), BaseActivity.this.getClass().getName());//��Ϊ������

                    }

                });

                frameLayout.addView(guideImage);//�������ͼƬ

                 

            }

        }

    }

     

    /**������onCreate�е��ã���������ͼƬ����Դid

     *���ڲ���xml�ĸ�Ԫ��������android:id="@id/my_content_view"

     * @param resId

     */

    protected void setGuideResId(int resId){

        this.guideResourceId=resId;

    }

}
