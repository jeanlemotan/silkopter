package com.silkopter.viewer.silkopter;

import android.content.Context;
import android.content.pm.ActivityInfo;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.view.SurfaceHolder;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

//import android.support.design.widget.FloatingActionButton;
//import android.support.design.widget.Snackbar;

public class OpenGLActivity extends AppCompatActivity {
    private GLSurfaceView m_glView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        requestWindowFeature((Window.FEATURE_NO_TITLE));
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.hide();
        }
        ;

        View decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY |
                View.SYSTEM_UI_FLAG_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_FULLSCREEN |
                View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION |
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE;
        decorView.setSystemUiVisibility(uiOptions);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        m_glView = new SilkGLSurfaceView(this);
        setContentView(m_glView);
    }


    private class SilkGLSurfaceView extends GLSurfaceView {

        private SilkOpenGLRenderer mRenderer;
        private Context mContext;

        public SilkGLSurfaceView(Context context) {
            super(context);
            mContext = context;
            setEGLContextClientVersion(2);
            mRenderer = new SilkOpenGLRenderer(mContext);
            setRenderer(mRenderer);
        }

        @Override
        public void onPause() {
        }

        @Override
        public void onResume() {
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            mRenderer.onSurfaceDestroyed();
        }
    }
}