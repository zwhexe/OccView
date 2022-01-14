using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace OccView
{
    //Tools object for output OCCT rendering with Direct3D
    public class D3dViewer
    {
        private D3DImage mD3dImg = new D3DImage();
        private IntPtr mColorSurf;
        public OCCViewer Viewer;

        public D3dViewer()
        {
            //register OnIsFrontBufferAvailabelChanged as DependencyPropertyChangedEventHandler to 
            //D3DImage.IsFrontBufferAvailableChanged which is an event(an instaniate delegate)
            mD3dImg.IsFrontBufferAvailableChanged
              += new DependencyPropertyChangedEventHandler(OnIsFrontBufferAvailableChanged);
            //once D3DViewer be initialized, attach FrontBuffer method to D3DImage
            //thus D3DImage Front Buffer changed it will notice OnIsFrontBufferAvailableChanged
            BeginRenderingScene();
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
                Viewer = new OCCViewer();

                if (!Viewer.InitViewer())
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
                    Viewer.View.RedrawView();

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
                    mColorSurf = Viewer.View.ResizeBridgeFBO(theSizeX, theSizeY);
                    mD3dImg.SetBackBuffer(D3DResourceType.IDirect3DSurface9, mColorSurf);
                }
                mD3dImg.Unlock();
            }
        }

        public D3DImage Image
        {
            get => mD3dImg;
        }
    }
}
