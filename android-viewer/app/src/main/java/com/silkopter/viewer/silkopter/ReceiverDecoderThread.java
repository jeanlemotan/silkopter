package com.silkopter.viewer.silkopter;

import android.content.Context;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;
import java.nio.ByteBuffer;

//import android.media.MediaCodecInfo;
//import android.media.MediaCodecList;

/*
receives raw h.264 byte stream on udp port 5000,parses the data into NALU units,and passes them into a MediaCodec Instance.
Original: https://bitbucket.org/befi/h264viewer
Edited by Constantin Geier at 28.12.2015;
For Testing, replace "receiveFromUDP" by "receiveFromFile" , and add the path to your file. (Has to be a raw h.264 file)
 */
public class ReceiverDecoderThread extends Thread {
    //SharedPreferences settings;
    private int m_port;
    private Context m_context;
    private boolean m_running = true;
    private long m_timeB = 0;
    private long m_timeA = 0;
    private long m_presentationTimeMs = 0;
    private long m_averageHWDecoderLatency = 0;
    private long m_HWDecoderLatencySum = 0;
    private int m_outputCount = 0;
    //time we have to wait for an Buffer to fill
    private long m_averageWaitForInputBufferLatency = 0;
    private long m_waitForInputBufferLatencySum = 0;
    private long m_naluCount = 0;
    private ByteBuffer[] m_inputBuffers;
    //ByteBuffer[] m_outputBuffers;
    private MediaCodec.BufferInfo m_info;
    private int m_naluSearchState = 0;
    private byte[] m_naluData;
    //String s = "Time between output buffers: ";
    private int m_naluDataPosition;
    private int NALU_MAXLEN = 1024 * 1024;
    private MediaCodec m_decoder;


    public ReceiverDecoderThread(Surface surface, int port, Context context) {
        m_context = context;
        m_port = port;
        m_naluData = new byte[NALU_MAXLEN];
        m_naluDataPosition = 0;
        try {
            m_decoder = MediaCodec.createDecoderByType("video/avc");
            //This Decoder Seems to exist on most android devices,but is pretty slow
            //decoder=MediaCodec.createByCodecName("OMX.google.h264.decoder");
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("Error creating decoder");
            return;
        }

        System.out.println("Codec Info: " + m_decoder.getCodecInfo().getName());

        MediaFormat format = MediaFormat.createVideoFormat("video/avc", 1280, 720);

        try {
            m_decoder.configure(format, surface, null, 0);
            if (m_decoder == null) {
                System.out.println("Can't configure decoder!");
                return;
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("error config decoder");
        }
        //decoder.setVideoScalingMode(MediaCodec.VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING);
        m_info = new MediaCodec.BufferInfo();

        m_decoder.start();
    }

    @Override
    public void interrupt() {
        m_running = false;
        writeLatencyFile();
    }

    public void run() {
        setPriority(Thread.MAX_PRIORITY);

        new Thread(new Runnable() {
            public void run() {
                setPriority(Thread.MAX_PRIORITY);
                System.out.println("Thread Priority: " + getPriority()); //Thread priority:5
                while (m_running) {
                    checkOutput();
                }
            }
        }).start();

        receiveFromUDP();
    }

    private void feedDecoder(byte[] n, int len) {
        for (; ; ) {
            try {
                m_inputBuffers = m_decoder.getInputBuffers();
                int inputBufferIndex = m_decoder.dequeueInputBuffer(0);
                if (inputBufferIndex >= 0) {
                    ByteBuffer inputBuffer = m_inputBuffers[inputBufferIndex];
                    inputBuffer.put(n, 0, len);
                    //decoder.queueInputBuffer(inputBufferIndex, 0, len, 0, 0);
                    m_presentationTimeMs = System.currentTimeMillis();
                    m_decoder.queueInputBuffer(inputBufferIndex, 0, len, m_presentationTimeMs, 0);
                    break;
                }
            } catch (Exception e) {
                //e.printStackTrace();
                System.out.println("Error Qeueing in/out Buffers");
            }
            /*
            int count1=0;
            try{for(int i=0;i<100;i++){ByteBuffer xyz=inputBuffers[i];count1=i;}}catch(Exception e){e.printStackTrace();}
            System.out.println("number of inputBuffers:"+count1+"");
            try{for(int i=0;i<100;i++){ByteBuffer xyz=outputBuffers[i];count1=i;}}catch(Exception e){e.printStackTrace();}
            System.out.println("number of outputBuffers:"+count1+"");
            */
        }


    }

    private void interpretNalu(byte[] n, int len) {
        //Here is the right place to do some changes to the data (f.e sps fix up )
        m_timeB = System.currentTimeMillis();
        feedDecoder(n, len); //takes beteen 2 and 40ms (1ms,1ms,46ms,1ms,1ms,40ms,... in this order),
        // beacause there isn't always an input buffer available immediately;
        long time = System.currentTimeMillis() - m_timeB;
        if (time >= 0 && time <= 200) {
            m_naluCount++;
            m_waitForInputBufferLatencySum += time;
            m_averageWaitForInputBufferLatency = (m_waitForInputBufferLatencySum / m_naluCount);
            //Log.w("1","Time spent waiting for an input buffer:"+time);
            //Log.w("2","average Time spent waiting for an input buffer:"+averageWaitForInputBufferLatency);
        }

        /*TESTINGS SPECIFIC FOR RASPBERRY PI CAMERA OUTPUT
        __________________________________________________________________________;
        //This is from moonlight;but for rpi it doesn't work
        if (n[4] == 0x67){Log.w("interpretNalu","SPS" );} //SPS 0x67=103
        if (n[4] == 0x68){Log.w("interpretNalu","PPS" );} //PPS      //0x68=104
        if (n[4] == 0x40){Log.w("interpretNalu","VPS" );} //VPS        //0x40=64
        if (n[4] == 0x42){Log.w("interpretNalu","SPS" );} //SPS too    //0x42=66
        if (n[4] == 0x44){Log.w("interpretNalu","PPS" );} //PPS too  //0x44=68
        __________________________________________________________________________;
        n[4]==constant 33  ,ausser ersten paar (0,39,40,37,,33...)
        n[5]== constant -102 ausser ersten paar  (0,66,-50,-120,...)
        n[6] steigt andauernd um 2 an ?!!
        n[7]= regelmäßigkeiten erkennbar,aber nicht wirklich regelmäßig
        it seems like when rpi
        1)puts 39,40 or 37 in the n[4] byte, if it's configuration data or
        2)puts       33 in the n[4] byte if it's a non I -frame
        by using -ih in the raspivid pipeline the rpi puts this config data not only in the beginning,but in intervalls between the
        non-i-(==33)frames (which makes sence,that's exactely the purpose of -ih )
        //
        String x="";
        for(int i=0;i<60;i++){
            x+=n[i]+",";
        }
        Log.w("interpretNAlU: ", x);
        /*if(n[4]==33 || n[4]==39 || n[4]==40 || n[4]==37){ feedDecoder(n,len);}
        what happens,when packets get filtered out
        1)no n[4]==33 packets: only about 4 frames (15 sec video ) get displayed (häh ?? )
        I think: eather n[4]==39,40,oder 37 are key frames
        2)no n[4]==39 packets: nothing gets decoded
        3) no n[4]==40 packets: nothing gets displayed
        4) no n[4]==37 packets: stream works,but has some artefacts;
        _______________________________________________________________________
        I can definitely say: (for rpi)
        1)when n[4]==39 || n[4]==40 it is decoder configuration data (SPS or PPS)
        2)when n[4]==33 it is not configuration data, probably a b-frame
        3)when n[4]==37 it is an key frame
        but it's weird that i can decode the stream even without key-frames with just some Artefakts
        if((n[4] & 0x1f)==0x07){ it is definitely an sps frame (Info from stackoverflow) ;should be true general,too
        this is true for n[4]==39 }
        _________________________________________________________________________
        */


    }

    private void parseDatagram(byte[] p, int plen) {
        //Maybe: use System.arraycopy ...
        try {
            for (int i = 0; i < plen; ++i) {
                m_naluData[m_naluDataPosition++] = p[i];
                if (m_naluDataPosition == NALU_MAXLEN - 1) {
                    Log.w("parseDatagram", "NALU Overflow");
                    m_naluDataPosition = 0;
                }
                switch (m_naluSearchState) {
                    case 0:
                    case 1:
                    case 2:
                        if (p[i] == 0)
                            m_naluSearchState++;
                        else
                            m_naluSearchState = 0;
                        break;
                    case 3:
                        if (p[i] == 1) {
                            //nalupacket found
                            m_naluData[0] = 0;
                            m_naluData[1] = 0;
                            m_naluData[2] = 0;
                            m_naluData[3] = 1;
                            interpretNalu(m_naluData, m_naluDataPosition - 4);
                            m_naluDataPosition = 4;
                        }
                        m_naluSearchState = 0;
                        break;
                    default:
                        break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("error parsing");
        }
    }

    private void checkOutput() {
        //try {Thread.sleep(200,0);} catch (InterruptedException e) {e.printStackTrace();}
        try {
            //outputBuffers = decoder.getOutputBuffers();
            int outputBufferIndex = m_decoder.dequeueOutputBuffer(m_info, 0);
            if (outputBufferIndex >= 0) {
                long latency = System.currentTimeMillis() - m_info.presentationTimeUs;
                if (latency >= 0 && latency <= 200) {
                    m_outputCount++;
                    m_HWDecoderLatencySum += latency;
                    m_averageHWDecoderLatency = m_HWDecoderLatencySum / m_outputCount;
                    //Log.w("checkOutput 2","hw decoder latency:"+latency);
                    //Log.w("checkOutput 1","Average HW decoder latency:"+averageHWDecoderLatency);
                }
                m_decoder.releaseOutputBuffer(outputBufferIndex, true);
                //decoder.releaseOutputBuffer(outputBufferIndex,(long) 0); //needs api 21

            } else if (outputBufferIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                Log.d("UDP", "output format changed");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void receiveFromUDP() {
        byte[] message = new byte[1024];
        DatagramPacket packet = new DatagramPacket(message, message.length);
        DatagramSocket socket = null;
        try {
            socket = new DatagramSocket(m_port);
        } catch (SocketException e) {
            System.out.println("error opening port");
        }

        while (!Thread.interrupted() && socket != null) {
            try {
                socket.receive(packet);
            } catch (IOException e) {
            }
            parseDatagram(message, packet.getLength());
        }
        if (socket != null) {
            socket.close();
        }
        m_decoder.flush();
        m_decoder.stop();
        m_decoder.release();
    }

    public void writeLatencyFile() {
        java.io.PrintWriter out;
        String lf = "everything in ms:";
        lf += "\n Average measured app Latency: " + (m_averageWaitForInputBufferLatency + m_averageHWDecoderLatency);
        lf += "\n Average time waiting for an input Buffer:" + m_averageWaitForInputBufferLatency;
        lf += "\n Average time HW encoding:" + m_averageHWDecoderLatency;
        lf += "\n .";
        //Todo: measure time between releasing output buffer and rendering it onto Screen
        try {
            out = new java.io.PrintWriter(Environment.getExternalStorageDirectory() + "/latencyFile.txt");
            out.println(lf);
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}