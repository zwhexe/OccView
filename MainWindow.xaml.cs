using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Windows.Forms.Integration;
using System.Windows.Interop;
using System.Runtime.InteropServices;

namespace OccView
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        //declare an event/instaniate a delegate which type is PropertyChangedEventHandler
        /*PropertyChangedEventHandler(object sender, PropertyChangedEventArgs e)*/
        //PropertyChanged takes 2 param: object sender, PropertyChangedEventArgs e
        public event PropertyChangedEventHandler PropertyChanged;

        protected void RaisePropertyChanged(string thePropertyName)
        {//call observers registered method when property changed
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(thePropertyName));
            }
        }

        public TestData mData { get; set; }
        public D3dViewer mViewer { get; set; }
        public OCCProxyer mProxyer { get; set; }
        public MainWindow()
        {
            InitializeComponent();

            //init occproxyer include view and model
            mProxyer = new OCCProxyer();
            mData = new TestData();
            InitViewer();
            mProxyer.Proxy.SetView(mViewer.mView);
        }

        public void InitViewer()
        {
            //0.create Grid with Viewer
            mViewer = new D3dViewer();
            Grid g = new Grid();
            Map.Add(g, mViewer);
            
            //1.use D3DImage to initialize imgBrush
            ImageBrush imgBrush = new ImageBrush(mViewer.Image);

            //2.use ImageBrush to fill grid background
            g.Background = imgBrush;
            g.MouseWheel += new MouseWheelEventHandler(g_MouseWheel);
            g.MouseMove += new MouseEventHandler(g_MouseMove);
            g.MouseDown += new MouseButtonEventHandler(g_MouseDown);
            g.MouseUp += new MouseButtonEventHandler(g_MouseUp);
            g.HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch;

            //3.assign grid to TabItem content
            TabItem aNewTab = new TabItem();
            aNewTab.Content = g;  //assign grid to TabItem
            aNewTab.HorizontalAlignment = System.Windows.HorizontalAlignment.Stretch;
            aNewTab.HorizontalContentAlignment = System.Windows.HorizontalAlignment.Stretch;
            aNewTab.VerticalContentAlignment = System.Windows.VerticalAlignment.Stretch;

            g.SizeChanged += new SizeChangedEventHandler(g_SizeChanged);
            aNewTab.IsSelected = true;
            aNewTab.Header = "View " + mViewCounter.ToString();
            mViewCounter++;

            //4.add TabItem to xaml TabControl
            ViewPanel.Items.Add(aNewTab);
            ViewPanel.Focus();

            //5.update XAML property
            RaisePropertyChanged("IsDocumentOpen");
        }

        private void Import_Click(object sender, RoutedEventArgs e)
        {
            if (d3dviewer != null)
            {
                d3dviewer.ImportModel(ModelFormat.STEP);
            }
        }

        private void Occ_Click(object sender, RoutedEventArgs e)
        {
            if (mData != null)
            {
                mData.ToOccJson();
            }
        }

        private void CSharp_Click(object sender, RoutedEventArgs e)
        {
            if (mData != null)
            {
                mData.LoadJson();
            }
        }

        private void Box_Click(object sender, RoutedEventArgs e)
        {
            if (d3dviewer != null)
            {
                mProxyer.MakeBox();
                d3dviewer.SetDisplayMode();
                d3dviewer.FitAll();
            }
        }

        private void Cone_Click(object sender, RoutedEventArgs e)
        {
            if (d3dviewer != null)
            {
                mProxyer.MakeCone();
                d3dviewer.SetDisplayMode();
                d3dviewer.FitAll();
            }
        }

        private void Torus_Click(object sender, RoutedEventArgs e)
        {
            if (d3dviewer != null)
            {
                mProxyer.MakeTorus();
                d3dviewer.SetDisplayMode();
                d3dviewer.FitAll();
            }
        }

        private void Wedge_Click(object sender, RoutedEventArgs e)
        {
            if (d3dviewer != null)
            {
                mProxyer.MakeWedge();
                d3dviewer.SetDisplayMode();
                d3dviewer.FitAll();
            }
        }

        private void Sphere_Click(object sender, RoutedEventArgs e)
        {
            if (d3dviewer != null)
            {
                mProxyer.MakeSphere();
                d3dviewer.SetDisplayMode();
                d3dviewer.FitAll();
            }
        }

        private void Cylinder_Click(object sender, RoutedEventArgs e)
        {
            if (d3dviewer != null)
            {
                mProxyer.MakeCylinder();
                d3dviewer.SetDisplayMode();
                d3dviewer.FitAll();
            }
        }

        private void Test_Click(object sender, RoutedEventArgs e)
        {
            if (mData != null)
            {
                mData.TestConvert();
            }

        }

        private D3dViewer d3dviewer
        {
            get
            {
                if (!IsDocumentOpen)
                {
                    return null;
                }

                Grid grid = (ViewPanel.SelectedContent) as Grid;
                if (grid == null)
                {
                    return null;
                }
                return Map[grid];
            }
        }

        public Boolean IsDocumentOpen
        {
            get
            {
                return ViewPanel.Items.Count > 0;
            }
        }

        void g_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            if (!IsDocumentOpen)
                return;
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;

            if (aGrid == null)
                return;

            Map[aGrid].Resize(Convert.ToInt32(e.NewSize.Width),
                               Convert.ToInt32(e.NewSize.Height));
        }

        void g_MouseWheel(object sender, MouseWheelEventArgs e)
        {
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;
            if (aGrid != null)
            {
                d3dviewer.OnMouseWheel(aGrid, e);
            }
        }

        void g_MouseMove(object sender, MouseEventArgs e)
        {
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;
            if (aGrid != null)
            {
                d3dviewer.OnMouseMove(aGrid, e);
            }
        }

        void g_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;
            if (aGrid != null)
            {
                d3dviewer.OnMouseDown(ViewPanel, e);
            }
        }

        void g_MouseUp(object sender, MouseButtonEventArgs e)
        {
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;

            if (aGrid != null)
            {
                d3dviewer.OnMouseUp(aGrid, e);
            }
        }

        private int mViewCounter = 1;
        Dictionary<Grid, D3dViewer> Map = new Dictionary<Grid, D3dViewer>();

        private void ViewPanel_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
