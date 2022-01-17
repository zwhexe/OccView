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
    public class OCCViewer
    {
        public OCCProxy View { get; private set; }
        public OCCJson Json { get; set; }
        public CurrentAction3d currentMode { get; private set; }
        public float mCurZoom;
        public OCCViewer()
        {
            View = new OCCProxy();
            View.InitOCCProxy();

            Json = new OCCJson();
        }
        
        public bool InitViewer()
        {
            return View.InitViewer();
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

                if (!View.TranslateModel(aFileName, aFormat, true))
                {
                    MessageBox.Show("Cann't read this file", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
            }

            View.SetDisplayMode(1);
            View.ZoomAllView();
        }
    
        public void FitAll()
        {
            View.ZoomAllView();
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
            mCurZoom = View.Scale();
            currentMode = CurrentAction3d.CurAction3d_GlobalPanning;
        }

        public void OnMouseMove(System.Windows.IInputElement sender, System.Windows.Input.MouseEventArgs e)
        {

        }

        public void OnMouseDown(System.Windows.IInputElement sender, MouseButtonEventArgs e)
        {

        }

        public void OnMouseUp(System.Windows.IInputElement sender, MouseButtonEventArgs e)
        {

        }
    }
}
