using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Drawing;

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
    public class OCCProxyer
    {
        public OCCProxy Proxy { get; private set; }
        public CurrentAction3d currentMode { get; private set; }
        public float mCurZoom;
        public OCCProxyer()
        {
            Proxy = new OCCProxy();
        }
        
        public bool InitViewer()
        {
            return Proxy.occView.InitViewer();
        }

        public void MakeBox()
        {
            Proxy.occModel.MakeBox();
            Proxy.Display();
            SetDisplayMode();
            FitAll();
        }

        public void MakeSphere()
        {
            Proxy.occModel.MakeSphere();
            Proxy.Display();
            SetDisplayMode();
            FitAll();
        }

        public void MakeCylinder()
        {
            Proxy.occModel.MakeCylinder();
            Proxy.Display();
            SetDisplayMode();
            FitAll();
        }

        public void ImportModel(ModelFormat theFormat)
        {
            int aFormat = 10;
            OpenFileDialog anOpenDialog = new OpenFileDialog();
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
            if (anOpenDialog.ShowDialog() == DialogResult.OK)
            {
                string aFileName = anOpenDialog.FileName;
                if (aFileName == "")
                {
                    return;
                }

                if (!Proxy.occView.TranslateModel(aFileName, aFormat, true))
                {
                    MessageBox.Show("Cann't read this file", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }

            SetDisplayMode();
            FitAll();
        }
    
        public void FitAll()
        {
            Proxy.occView.ZoomAllView();
        }

        public void SetDisplayMode(int theMode = 1)
        {
            Proxy.occView.SetDisplayMode(theMode);
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
            mCurZoom = Proxy.occView.Scale();
            currentMode = CurrentAction3d.CurAction3d_GlobalPanning;
        }

        public void OnMouseMove(System.Windows.IInputElement sender, System.Windows.Input.MouseEventArgs e)
        {
            Point p = new Point((int)e.GetPosition(sender).X, (int)e.GetPosition(sender).Y);
            if (e.MiddleButton == MouseButtonState.Pressed)
            {
                Proxy.occView.Rotation(p.X, p.Y);
                Proxy.occView.RedrawView();
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
