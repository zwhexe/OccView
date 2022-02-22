﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Media;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Input;

namespace OccView
{
    public enum CurrentAction3d
    {
        CurAction3d_Nothing,
        CurAction3d_DynamicZooming,
        CurAction3d_WindowZooming,
        CurAction3d_DynamicPanning,
        CurAction3d_GlobalPanning,
        CurAction3d_DynamicRotation
    }
    public enum CurrentPressedKey
    {
        CurPressedKey_Nothing,
        CurPressedKey_Ctrl,
        CurPressedKey_Shift
    }
    public enum ModelFormat
    {
        BREP,
        STEP,
        IGES,
        VRML,
        STL,
        IMAGE
    }

    public enum DisplayMode
    {
        Wireframe,
        Shading
    }
    //Tools object for output OCCT rendering with Direct3D
    public class D3dViewer
    {
        private D3DImage mD3dImg = new D3DImage();
        private IntPtr mColorSurf;
        public CurrentAction3d currentMode { get; private set; }
        public float mCurZoom;
        public OCCProxy mProxy { get; set; }

        public D3dViewer(OCCProxyer proxyer)
        {
            //register OnIsFrontBufferAvailabelChanged as DependencyPropertyChangedEventHandler to 
            //D3DImage.IsFrontBufferAvailableChanged which is an event(an instaniate delegate)
            mD3dImg.IsFrontBufferAvailableChanged
              += new DependencyPropertyChangedEventHandler(OnIsFrontBufferAvailableChanged);
            //once D3DViewer be initialized, attach FrontBuffer method to D3DImage
            //thus D3DImage Front Buffer changed it will notice OnIsFrontBufferAvailableChanged
            if (proxyer != null)
            {
                mProxy = proxyer.Proxy;
                BeginRenderingScene();
            }
        }

        private void OnIsFrontBufferAvailableChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            // If the front buffer is available, then WPF has just created a new
            // Direct3D device, thus we need to start rendering our custom scene
            if (mD3dImg.IsFrontBufferAvailable)
            {
                BeginRenderingScene();
            }
            else
            {
                // If the front buffer is no longer available, then WPF has lost Direct3D
                // device, thus we need to stop rendering until the new device is created
                StopRenderingScene();
            }
        }

        private bool mIsFailed = false;

        private void BeginRenderingScene()
        {
            if (mIsFailed)
            {
                return;
            }

            if (mD3dImg.IsFrontBufferAvailable)
            {
                //Proxyer = new OCCProxyer();
                if (!mProxy.occView.InitViewer())
                {
                    MessageBox.Show("Failed to initialize OpenGL-Direct3D interoperability!",
                      "Error", MessageBoxButton.OK, MessageBoxImage.Error);

                    mIsFailed = true;
                    return;
                }

                // Leverage the Rendering event of WPF composition
                // target to update the our custom Direct3D scene
                CompositionTarget.Rendering += OnRendering;
            }
        }

        public void StopRenderingScene()
        {
            CompositionTarget.Rendering -= OnRendering;
            mColorSurf = IntPtr.Zero;
        }

        private void OnRendering(object sender, EventArgs e)
        {
            UpdateScene();
        }

        private void UpdateScene()
        {
            if (!mIsFailed
              && mD3dImg.IsFrontBufferAvailable
              && mColorSurf != IntPtr.Zero
              && (mD3dImg.PixelWidth != 0 && mD3dImg.PixelHeight != 0))
            {
                mD3dImg.Lock();
                {
                    // Update the scene (via a call into our custom library)
                    mProxy.occView.RedrawView();

                    // Invalidate the updated region of the D3DImage
                    mD3dImg.AddDirtyRect(new Int32Rect(0, 0, mD3dImg.PixelWidth, mD3dImg.PixelHeight));
                }
                mD3dImg.Unlock();
            }
        }

        public void Resize(int theSizeX, int theSizeY)
        {
            if (!mIsFailed && mD3dImg.IsFrontBufferAvailable)
            {
                // Set the back buffer for Direct3D WPF image
                mD3dImg.Lock();
                {
                    mD3dImg.SetBackBuffer(D3DResourceType.IDirect3DSurface9, IntPtr.Zero);
                    mColorSurf = mProxy.occView.ResizeBridgeFBO(theSizeX, theSizeY);
                    mD3dImg.SetBackBuffer(D3DResourceType.IDirect3DSurface9, mColorSurf);
                }
                mD3dImg.Unlock();
            }
        }

        public void ImportModel(ModelFormat theFormat)
        {
            int aFormat = 10;
            System.Windows.Forms.OpenFileDialog anOpenDialog = new System.Windows.Forms.OpenFileDialog();
            string aDataDir = Environment.GetEnvironmentVariable("CSF_OCCTDataPath");
            string aFilter = "";

            switch (theFormat)
            {
                case ModelFormat.BREP:
                    anOpenDialog.InitialDirectory = (aDataDir + "\\occ");
                    aFormat = 0;
                    aFilter = "BREP Files (*.brep *.rle)|*.brep; *.rle";
                    break;
                case ModelFormat.STEP:
                    anOpenDialog.InitialDirectory = (aDataDir + "\\step");
                    aFormat = 1;
                    aFilter = "STEP Files (*.stp *.step)|*.stp; *.step";
                    break;
                case ModelFormat.IGES:
                    anOpenDialog.InitialDirectory = (aDataDir + "\\iges");
                    aFormat = 2;
                    aFilter = "IGES Files (*.igs *.iges)|*.igs; *.iges";
                    break;
                default:
                    break;
            }

            anOpenDialog.Filter = aFilter + "|All files (*.*)|*.*";
            if (anOpenDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string aFileName = anOpenDialog.FileName;
                if (aFileName == "")
                {
                    return;
                }
            }
        }

        public D3DImage Image
        {
            get => mD3dImg;
        }

        public void FitAll()
        {
            mProxy.occView.ZoomAllView();
        }

        public void SetDisplayMode(int theMode = 1)
        {
            mProxy.occView.SetDisplayMode(theMode);
        }

        public void ZoomWindow()
        {
            currentMode = CurrentAction3d.CurAction3d_WindowZooming;
        }

        public void DynamicZooming()
        {
            currentMode = CurrentAction3d.CurAction3d_DynamicZooming;
        }

        public void DynamicPanning()
        {
            currentMode = CurrentAction3d.CurAction3d_DynamicPanning;
        }

        public void GlobalPanning()
        {
            mCurZoom = mProxy.occView.Scale();
            currentMode = CurrentAction3d.CurAction3d_GlobalPanning;
        }

        public void OnMouseMove(System.Windows.IInputElement sender, System.Windows.Input.MouseEventArgs e)
        {
            System.Drawing.Point p = new System.Drawing.Point((int)e.GetPosition(sender).X, (int)e.GetPosition(sender).Y);
            if (e.MiddleButton == MouseButtonState.Pressed)
            {
                mProxy.occView.Rotation(p.X, p.Y);
                mProxy.occView.RedrawView();
            }

        }

        public void OnMouseDown(System.Windows.IInputElement sender, MouseButtonEventArgs e)
        {

        }

        public void OnMouseUp(System.Windows.IInputElement sender, MouseButtonEventArgs e)
        {

        }
    }
}
