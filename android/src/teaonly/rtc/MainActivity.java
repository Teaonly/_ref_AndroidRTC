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

    boolean inProcessing = false;
    final int maxVideoNumber = 3;
    VideoFrame[] videoFrames = new VideoFrame[maxVideoNumber];
    byte[] preFrame = new byte[1024*1024*8];
    
    TeaServer webServer = null;
    private CameraView cameraView_;
    private OverlayView overlayView_;
    private Button btnExit;
    private TextView tvMessage1;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        Window win = getWindow();
        win.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);    
        //win.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN); 

        setContentView(R.layout.main);

        btnExit = (Button)findViewById(R.id.btn_exit);
        btnExit.setOnClickListener(exitAction);
        tvMessage1 = (TextView)findViewById(R.id.tv_message1);
        
        for(int i = 0; i < maxVideoNumber; i++) {
            videoFrames[i] = new VideoFrame(1024*1024*2);        
        }    

        initCamera();
    }
    
    @Override
    public void onCameraReady() {
        if ( initWebServer() ) {
            int wid = cameraView_.Width();
            int hei = cameraView_.Height();
            cameraView_.StopPreview();
            cameraView_.setupCamera(wid, hei, previewCb_);
            cameraView_.StartPreview();
        }
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
        inProcessing = true;
        if ( webServer != null)
            webServer.stop();
        cameraView_.StopPreview(); 
        
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
    
    public String getLocalIpAddress() {
        try {
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) {
                NetworkInterface intf = en.nextElement();
                for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) {
                    InetAddress inetAddress = enumIpAddr.nextElement();
                    //if (!inetAddress.isLoopbackAddress() && !inetAddress.isLinkLocalAddress() && inetAddress.isSiteLocalAddress() ) {
                    if (!inetAddress.isLoopbackAddress() && InetAddressUtils.isIPv4Address(inetAddress.getHostAddress()) ) {
                        String ipAddr = inetAddress.getHostAddress();
                        return ipAddr;
                    }
                }
            }
        } catch (SocketException ex) {
            Log.d(TAG, ex.toString());
        }
        return null;
    }   

    private boolean initWebServer() {
        String ipAddr = getLocalIpAddress();
        if ( ipAddr != null ) {
            try{
                webServer = new TeaServer(8080, this); 
                webServer.registerCGI("/cgi/start", doStart);
                webServer.registerCGI("/cgi/stop", doStop);
            }catch (IOException e){
                webServer = null;
            }
        }
        if ( webServer != null) {
            tvMessage1.setText( getString(R.string.msg_access_local) + " http://" + ipAddr  + ":8080" );
            return true;
        } else {
            tvMessage1.setText( getString(R.string.msg_error) );
            return false;
        }
          
    }
   
    private OnClickListener exitAction = new OnClickListener() {
        @Override
        public void onClick(View v) {
            onPause();
        }   
    };
   
    private PreviewCallback previewCb_ = new PreviewCallback() {
        public void onPreviewFrame(byte[] frame, Camera c) {
            if ( !inProcessing ) {
                inProcessing = true;
           
                int picWidth = cameraView_.Width();
                int picHeight = cameraView_.Height(); 
                ByteBuffer bbuffer = ByteBuffer.wrap(frame); 
                bbuffer.get(preFrame, 0, picWidth*picHeight + picWidth*picHeight/2);

                inProcessing = false;
            }
        }
    };
    
    private TeaServer.CommonGatewayInterface doStop = new TeaServer.CommonGatewayInterface () {
        @Override
        public String run(Properties parms) {
            return "OK";
        }
        
        @Override 
        public InputStream streaming(Properties parms) {
            return null;
        }    
    }; 

    private TeaServer.CommonGatewayInterface doStart = new TeaServer.CommonGatewayInterface () {
        @Override
        public String run(Properties parms) {
            String remote = parms.getProperty("remote"); 
            
            int wid = 640;
            int hei = 480; 
            cameraView_.StopPreview();
            cameraView_.setupCamera(wid, hei, previewCb_);
            cameraView_.StartPreview();
            
            return "OK";
        }   
 
        @Override 
        public InputStream streaming(Properties parms) {
            return null;
        }    
    }; 

}    

