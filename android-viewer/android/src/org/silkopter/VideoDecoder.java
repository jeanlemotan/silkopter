package org.silkopter;

import java.io.IOException;

import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.media.MediaPlayer;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.util.Log;
import android.view.Surface;
import android.os.Environment;
import java.nio.ByteBuffer;


public class VideoDecoder {
        private static Surface m_surface;
        private static MySurfaceTexture m_texture;
        //private static MediaPlayer m_mp;
        private static MediaCodec.BufferInfo m_info;
        private static MediaCodec m_decoder;
        private static MediaFormat m_format;
        private static boolean m_isRunning = true;

        public static ByteBuffer getCsd0(){
            byte[] csd0={0,0,0,1,39,66,-128,40,-107,-96,60,6,127,-110,1,-30,68,-44};
            ByteBuffer BBcsd0=ByteBuffer.wrap(csd0);
            return BBcsd0;
        }

        public static ByteBuffer getCsd1(){
            byte[] csd1={0,0,0,1,40,-50,2,92,-128};
            ByteBuffer BBcsd1=ByteBuffer.wrap(csd1);
            return BBcsd1;
        }

        public static void feedDecoder(byte[] n, int len) {
            while (m_isRunning) {
                try {
                    int inputBufferIndex = m_decoder.dequeueInputBuffer(10000);
                    if (inputBufferIndex >= 0) {
                        ByteBuffer inputBuffer = m_decoder.getInputBuffer(inputBufferIndex);
                        inputBuffer.put(n, 0, len);
                        //long presentationTimeMs = System.nanoTime();
                        m_decoder.queueInputBuffer(inputBufferIndex, 0, len, 0, 0);
                        break;

                    }else if (inputBufferIndex != MediaCodec.INFO_TRY_AGAIN_LATER){
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        public static SurfaceTexture setupDecoder(/* final Context c */ int textureId) {
                m_texture = new MySurfaceTexture(textureId);
                m_surface = new Surface(m_texture);


                m_info = new MediaCodec.BufferInfo();
                try {
                    m_decoder = MediaCodec.createDecoderByType("video/avc");
                } catch (Exception e) {
                    System.out.println("Error creating decoder");
                    e.printStackTrace();
                    return null;
                }
                System.out.println("Codec Info: " + m_decoder.getCodecInfo().getName());
                m_format = MediaFormat.createVideoFormat("video/avc", 1920, 1080);
                m_format.setByteBuffer("csd-0", getCsd0());
                m_format.setByteBuffer("csd-1", getCsd1());
                try {
                    //This configuration will be overwritten anyway when we put an sps into the buffer
                    //But: My decoder agrees with this,but some may not; to be improved
                    m_decoder.configure(m_format, m_surface, null, 0);
                    if (m_decoder == null) {
                        System.out.println("Can't configure decoder!");
                        return null;
                    }
                } catch (Exception e) {
                    System.out.println("error config decoder");
                    e.printStackTrace();
                }
                m_decoder.start();

                Thread outputThread = new Thread(){
                   @Override
                   public void run()
                   {
                       while(m_isRunning) {
                           try {
                               int outputBufferIndex = m_decoder.dequeueOutputBuffer(m_info, 10000);
                               if (outputBufferIndex >= 0) {
                                   //on my device this code snippet from Moonlight is not needed,after testing I doubt if it is really working at all;
                                   //if(decoder.dequeueOutputBuffer(info, 0) >= 0){ Log.w("...","second available");}
                                   //for GLSurfaceView,to drop the latest frames except the newest one,the timestamp has to be near the VSYNC signal.
                                   //requires android 5
                                   m_decoder.releaseOutputBuffer(outputBufferIndex, true); //needs api 21
                                   //decoder.releaseOutputBuffer(outputBufferIndex,true);
                               } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED || outputBufferIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                                   Log.d("UDP", "output format / buffers changed");
                               } else if(outputBufferIndex != MediaCodec.INFO_TRY_AGAIN_LATER) {
                                   Log.d("dequeueOutputBuffer", "not normal");
                               }
                           } catch(Exception e) {
                               e.printStackTrace();
                           }
                       }
                   }
                };
                outputThread.setPriority(Thread.MAX_PRIORITY);
                outputThread.start();

                return m_texture;
        }
}
