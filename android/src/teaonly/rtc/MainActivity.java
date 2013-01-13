package teaonly.rtc;
import teaonly.rtc.*;

import java.io.IOException;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStream;
import java.io.InputStream;
import java.io.ByteArrayInputStream;
import java.lang.System;
import java.lang.Thread;
import java.util.concurrent.locks.ReentrantLock;
import java.util.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import  org.apache.http.conn.util.InetAddressUtils;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.ProgressDialog;
import android.content.res.Resources;
import android.content.res.AssetManager;
import android.content.res.AssetFileDescriptor;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.PictureCallback;
import android.graphics.PixelFormat;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.Paint;
import android.graphics.YuvImage;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.media.AudioFormat;
import android.media.MediaRecorder;
import android.media.AudioRecord;
import android.text.format.Formatter;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.SurfaceView;
import android.util.Log;
import android.widget.LinearLayout; 
import android.widget.ImageButton;
import android.widget.Button;
import android.widget.TextView;


public class MainActivity extends Activity 
    implements View.OnTouchListener, CameraView.CameraReadyCallback, OverlayView.UpdateDoneCallback{
    private static final String TAG = "TEAONLY";

    private CameraView cameraView_;
    private OverlayView overlayView_;
    private ReentrantLock previewLock = new ReentrantLock(); 
    
    private Button btnStart;
    private Button btnStop;
    private TextView tvMessage;
   
    boolean inStreaming = false; 
    private static native int nativeStart(String url, String description);
    private static native int nativeStop();
    private static native int nativePushFrame(byte[] frame);

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        Window win = getWindow();
        win.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);    
        //win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN); 

        setContentView(R.layout.main);

        btnStart = (Button)findViewById(R.id.btn_start);
        btnStart.setOnClickListener(startAction);
        btnStop = (Button)findViewById(R.id.btn_stop);
        btnStop.setOnClickListener(stopAction);
        tvMessage = (TextView)findViewById(R.id.tv_message);
        
        initCamera();
        
        System.loadLibrary("jingle"); 
        System.loadLibrary("app"); 
    }
    
    @Override
    public void onCameraReady() {
        int wid = cameraView_.Width();
        int hei = cameraView_.Height();
        cameraView_.StopPreview();
        cameraView_.setupCamera(wid, hei, 3, previewCb_);
        cameraView_.StartPreview();
    }
 
    @Override
    public void onUpdateDone() {
    }

    @Override
    public void onDestroy(){
        super.onDestroy();
    }   

    @Override
    public void onStart(){
        super.onStart();
    }   

    @Override
    public void onResume(){
        super.onResume();
    }   
    
    @Override
    public void onPause(){  
        super.onPause();
        
        previewLock.lock();    
        cameraView_.StopPreview(); 
        previewLock.unlock();

        //System.exit(0);
        finish();
    }  
    
    @Override
    public void onBackPressed() {
        super.onBackPressed();
    }

    @Override 
    public boolean onTouch(View v, MotionEvent evt) { 
        return false;
    }
  
    private void initCamera() {
        SurfaceView cameraSurface = (SurfaceView)findViewById(R.id.surface_camera);
        cameraView_ = new CameraView(cameraSurface);        
        cameraView_.setCameraReadyCallback(this);

        overlayView_ = (OverlayView)findViewById(R.id.surface_overlay);
        overlayView_.setOnTouchListener(this);
        overlayView_.setUpdateDoneCallback(this);
    }
    
    private OnClickListener startAction = new OnClickListener() {
        @Override
        public void onClick(View v) {
            if ( inStreaming == true)
                return;

            cameraView_.StopPreview();
            cameraView_.setupCamera(640, 480, 3, previewCb_);
            cameraView_.StartPreview();

            int wid = cameraView_.Width();
            int hei = cameraView_.Height();
            
            String url = "udp://192.168.0.101:1979";
            String media = "video:h.264:" + Integer.toString(wid) + ":" + Integer.toString(hei);

            if ( nativeStart(url, media) == 0) {
                Log.d(TAG, "Start streaming is OK");
                inStreaming = true;
            }
        }   
    };
    private OnClickListener stopAction = new OnClickListener() {
        @Override
        public void onClick(View v) {
        }   
    };

    private PreviewCallback previewCb_ = new PreviewCallback() {
        public void onPreviewFrame(byte[] frame, Camera c) {
            
            previewLock.lock();
            
            if ( inStreaming )
                nativePushFrame(frame);

            c.addCallbackBuffer(frame);
            previewLock.unlock();
        }
    };
    
}    

