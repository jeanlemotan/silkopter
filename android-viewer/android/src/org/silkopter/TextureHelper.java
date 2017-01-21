package org.silkopter;

import java.io.IOException;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.util.Log;
import android.view.Surface;
import android.os.Environment;

public class TextureHelper {
        private static final String TAG = "TextureStreaming";
        private static Surface mSurface;
        private static MySurfaceTexture mTexture;
        private static MediaPlayer mp;
        public static Context context = null;

        public static SurfaceTexture loadVideoTexture(/* final Context c */ int textureId) {
                mTexture = new MySurfaceTexture(textureId);
                mSurface = new Surface(mTexture);
                mp = new MediaPlayer();
                try {
                        String path = Environment.getExternalStorageDirectory() + "/Download/sample.mp4";
                        Log.i("", "Reading movie from " + path);

                        // lcarlon: chenge this to play a different video.
                        mp.setDataSource(path);
                        mp.prepare();
                        mp.start();
                } catch (IllegalArgumentException e) {
                        Log.e(e.getClass().getName(), e.getMessage(), e.getCause());
                        e.printStackTrace();
                } catch (SecurityException e) {
                        Log.e(e.getClass().getName(), e.getMessage(), e.getCause());
                        e.printStackTrace();
                } catch (IllegalStateException e) {
                        Log.e(e.getClass().getName(), e.getMessage(), e.getCause());
                        e.printStackTrace();
                } catch (IOException e) {
                        Log.e(e.getClass().getName(), e.getMessage(), e.getCause());
                        e.printStackTrace();
                }
                mp.setSurface(mSurface);
                return mTexture;
        }
}
