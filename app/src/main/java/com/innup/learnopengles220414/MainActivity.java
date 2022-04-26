package com.innup.learnopengles220414;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class  MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    private GLSurfaceView glSurfaceView;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    public static class AliceGLSurfaceViewRenderer implements GLSurfaceView.Renderer{

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            gl.glClearColor(0.1f, 0.4f, 0.6f, 1f);
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {
            gl.glViewport(0, 0, width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {
            gl.glClear(gl.GL_COLOR_BUFFER_BIT);
        }
    }

    public static class AliceGLSurfaceView extends GLSurfaceView{

        private GLSurfaceView.Renderer renderer;

        public AliceGLSurfaceView(Context context) {
            super(context);
            //设置gles版本，理论上和这里设置的关系不大，主要还是看你链接的库的版本
            setEGLContextClientVersion(2);
            renderer = new AliceGLSurfaceViewRenderer();
            setRenderer(renderer);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        glSurfaceView = new AliceGLSurfaceView(getApplicationContext());
        setContentView(glSurfaceView);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String testScence();

}