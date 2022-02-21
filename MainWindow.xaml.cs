﻿using System;
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

        public CSData mData { get; set; }
        public D3dViewer mViewer { get; set; }
        public OCCProxyer mProxyer { get; set; }
        public MainWindow()
        {
            InitializeComponent();

            //init occproxyer include view and model
            mProxyer = new OCCProxyer();
            mData = new CSData();
            InitViewer();
        }

        public void InitViewer()
        {
            mViewer = new D3dViewer(mProxyer);
            Grid g = new Grid();
            Map.Add(g, mViewer);
            
            //1.use D3DImage to initialize imgBrush
            ImageBrush imgBrush = new ImageBrush(mViewer.Image);

            //2.use ImageBrush to fill grid background
            g.Background = imgBrush;
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
            if (proxyer != null)
            {
                proxyer.ImportModel(ModelFormat.STEP);
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

        private void Test_Click(object sender, RoutedEventArgs e)
        {
            if (mData != null)
            {
                mData.TestConvert();
            }

        }

        private void Box_Click(object sender, RoutedEventArgs e)
        {
            if (proxyer != null)
            {
                proxyer.MakeBox();
            }
        }

        private OCCProxyer proxyer
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
                return Map[grid].mProxyer;
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

        void g_MouseMove(object sender, MouseEventArgs e)
        {
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;
            if (aGrid != null)
            {
                proxyer.OnMouseMove(aGrid, e);
            }
        }

        void g_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;
            if (aGrid != null)
            {
                proxyer.OnMouseDown(ViewPanel, e);
            }
        }

        void g_MouseUp(object sender, MouseButtonEventArgs e)
        {
            Grid aGrid = (ViewPanel.SelectedContent) as Grid;

            if (aGrid != null)
            {
                proxyer.OnMouseUp(aGrid, e);
            }
        }

        private void MenuItem_Click(object sender, RoutedEventArgs e)
        {

        }

        private int mViewCounter = 1;
        Dictionary<Grid, D3dViewer> Map = new Dictionary<Grid, D3dViewer>();

        private void ViewPanel_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }
    }
}
