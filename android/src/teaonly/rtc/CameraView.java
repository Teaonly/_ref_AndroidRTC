package teaonly.rtc;

import android.graphics.ImageFormat;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.AutoFocusCallback;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

import java.util.*;

public class CameraView implements SurfaceHolder.Callback{
    public static interface CameraReadyCallback { 
        public void onCameraReady(); 
    }  

    private Camera camera_ = null;
    private SurfaceHolder surfaceHolder_ = null;
    private SurfaceView	  surfaceView_;
    CameraReadyCallback cameraReadyCb_ = null;
 
    private List<Camera.Size> supportedSizes; 
    private Camera.Size procSize_;
    private boolean inProcessing_ = false;

    public CameraView(SurfaceView sv){
        surfaceView_ = sv;

        surfaceHolder_ = surfaceView_.getHolder();
        surfaceHolder_.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
        surfaceHolder_.addCallback(this); 
    }

    public List<Camera.Size> getSupportedPreviewSize() {
        return supportedSizes;
    }

    public int Width() {
        return procSize_.width;
    }

    public int Height() {
        return procSize_.height;
    }

    public void setCameraReadyCallback(CameraReadyCallback cb) {
        cameraReadyCb_ = cb;
    }

    public void StartPreview(){
        if ( camera_ == null)
            return;
        camera_.startPreview();
    }
    
    public void StopPreview(){
        if ( camera_ == null)
            return;
        camera_.stopPreview();
    }

    public void AutoFocus() {
        camera_.autoFocus(afcb);
    }

    public void Release() {
        if ( camera_ != null) {
            camera_.stopPreview();
            camera_.release();
            camera_ = null;
        }
    }
    
    public void setupCamera(int wid, int hei, int bufNumber, PreviewCallback cb) {
        
        int diff = Math.abs(supportedSizes.get(0).width - wid);
        int targetIndex = 0;
        for(int i = 0; i < supportedSizes.size(); i++) {
            int newDiff = Math.abs(supportedSizes.get(i).width - wid);
            if ( newDiff < diff) {
                diff = newDiff;
                targetIndex = i;
            }
        }

        procSize_.width = supportedSizes.get(targetIndex).width;
        procSize_.height = supportedSizes.get(targetIndex).height;

        PixelFormat pixelFormat = new PixelFormat();
        PixelFormat.getPixelFormatInfo(ImageFormat.NV21, pixelFormat);  

        Camera.Parameters p = camera_.getParameters();        
        p.setPreviewSize(procSize_.width, procSize_.height);
        p.setPreviewFormat(ImageFormat.NV21);
        camera_.setParameters(p);

        int bufSize = procSize_.width * procSize_.height * pixelFormat.bitsPerPixel / 8;
        byte[] buffer = null;
        for(int i = 0; i < bufNumber; i++) {
            buffer = new byte[ bufSize ];
            camera_.addCallbackBuffer(buffer);
        }
        camera_.setPreviewCallbackWithBuffer(cb);
    }

    private void setupCamera() {
        camera_ = Camera.open();
        procSize_ = camera_.new Size(0, 0);
        Camera.Parameters p = camera_.getParameters();        
       
        supportedSizes = p.getSupportedPreviewSizes();
        procSize_ = supportedSizes.get( supportedSizes.size()/2 );
        p.setPreviewSize(procSize_.width, procSize_.height);
        
        camera_.setParameters(p);
        //camera_.setDisplayOrientation(90);
        try {
            camera_.setPreviewDisplay(surfaceHolder_);
        } catch ( Exception ex) {
            ex.printStackTrace(); 
        }
        camera_.setPreviewCallbackWithBuffer(null);
        camera_.startPreview();    
    }  
    
    private Camera.AutoFocusCallback afcb = new Camera.AutoFocusCallback() {
        @Override
        public void onAutoFocus(boolean success, Camera camera) {
        }
    };

    @Override
    public void surfaceChanged(SurfaceHolder sh, int format, int w, int h){
    }
    
	@Override
    public void surfaceCreated(SurfaceHolder sh){        
        setupCamera();        
        if ( cameraReadyCb_ != null)
            cameraReadyCb_.onCameraReady();
    }
    
	@Override
    public void surfaceDestroyed(SurfaceHolder sh){
        Release();
    }
}
