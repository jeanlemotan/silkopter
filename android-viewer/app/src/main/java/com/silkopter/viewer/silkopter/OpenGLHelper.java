package com.silkopter.viewer.silkopter;

import android.opengl.GLES20;
import android.util.Log;

public class OpenGLHelper {
    public static int createProgram(String vertexSource, String fragmentSource) {
        int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) {
            return 0;
        }
        int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
        if (pixelShader == 0) {
            return 0;
        }

        int program = GLES20.glCreateProgram();
        if (program != 0) {
            GLES20.glAttachShader(program, vertexShader);
            checkGlError("glAttachShader");
            GLES20.glAttachShader(program, pixelShader);
            checkGlError("glAttachShader");
            GLES20.glLinkProgram(program);
            int[] linkStatus = new int[1];
            GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
            if (linkStatus[0] != GLES20.GL_TRUE) {
                GLES20.glDeleteProgram(program);
                program = 0;
            }
        }
        return program;
    }

    public static void checkGlError(String op) {
        int error;
        while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
            throw new RuntimeException(op + ": glError " + error);
        }
    }

    public static int loadShader(int shaderType, String source) {
        int shader = GLES20.glCreateShader(shaderType);
        if (shader != 0) {
            GLES20.glShaderSource(shader, source);
            GLES20.glCompileShader(shader);
            int[] compiled = new int[1];
            GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
            if (compiled[0] == 0) {
                GLES20.glDeleteShader(shader);
                shader = 0;
            }
        }
        return shader;
    }


    public static String getVertexShader() {
        String mVertexShader =
                "uniform mat4 uMVPMatrix;\n" +
                        "uniform mat4 uSTMatrix;\n" +
                        "attribute vec4 aPosition;\n" +
                        "attribute vec4 aTextureCoord;\n" +
                        "varying vec2 vTextureCoord;\n" +
                        "void main() {\n" +
                        "  gl_Position = uMVPMatrix * aPosition;\n" +
                        "  vTextureCoord = (uSTMatrix * aTextureCoord).xy;\n" +
                        "}\n";
        return mVertexShader;
    }

    public static String getFragmentShader() {
        String fragmentShader =
                "#extension GL_OES_EGL_image_external : require\n" +
                        "precision mediump float;\n" +
                        "varying vec2 vTextureCoord;\n" +
                        "uniform samplerExternalOES sTexture;\n" +
                        "void main() {\n" +
                        "  gl_FragColor = texture2D(sTexture, vTextureCoord);\n" +
                        "}\n";
        return fragmentShader;
    }

    /*public static float[] getTriangleVerticesData() {
        float[] TriangleVerticesData = {
                // X, Y, Z, U, V
                -1.0f, -0.75f, 0, 0.f, 0.f,
                0.0f, -0.75f, 0, 1.f, 0.f,
                -1.0f,  0.75f, 0, 0.f, 1.f,
                0.0f,  0.75f, 0, 1.f, 1.f,
                0.0f, -0.75f, 0, 0.f, 0.f,
                1.0f, -0.75f, 0, 1.f, 0.f,
                0.0f,  0.75f, 0, 0.f, 1.f,
                1.0f,  0.75f, 0, 1.f, 1.f
        };
        return TriangleVerticesData;
    }*/
    public static float[] getTriangleVerticesDataByFormat(float format) {
        //F.e format=4:3
        // (1/desired format) multiplied by the display faktor (in this case: a 16:9 Display)
        float y = (1.0f / format) * (8.0f / 9.0f);
        if (y <= 0 || y > 1) {
            y = 0.75f;
        }
        float[] TriangleVerticesData = {
                // X, Y, Z, U, V
                -1.0f, y, 0, 0.f, 0.f,
                0.0f, y, 0, 1.f, 0.f,
                -1.0f, -y, 0, 0.f, 1.f,
                0.0f, -y, 0, 1.f, 1.f,

                0.0f, y, 0, 0.f, 0.f,
                1.0f, y, 0, 1.f, 0.f,
                0.0f, -y, 0, 0.f, 1.f,
                1.0f, -y, 0, 1.f, 1.f
        };
        return TriangleVerticesData;
    }

}