package org.silkopter;

import android.graphics.SurfaceTexture;
import android.util.Log;

public class MySurfaceTexture extends SurfaceTexture {
        public MySurfaceTexture(int texName) {
                super(texName);
                Log.d(TAG, "SurfaceTexture received!");

                setOnFrameAvailableListener(new OnFrameAvailableListener() {
                        @Override
                        public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                                //Log.d(TAG, "Frame received!");
                        }
                });
        }

        @Override
        public void updateTexImage() {
                super.updateTexImage();
                //Log.d(TAG, "Updating tex image.");
        }

        private static final String TAG = "Skptr";
}
