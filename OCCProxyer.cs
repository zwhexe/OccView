using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Drawing;
using System.ComponentModel;

namespace OccView
{
    public class OCCProxyer
    {
        public static OccProxy Proxy { get; set; }

        public OCCProxyer()
        {
            Proxy = new OccProxy();
        }

        public void TestClick()
        {
            Proxy.makeTest();
        }

        public void MakeBox()
        {

        }

        public void MakeCone()
        {
            Proxy.makeCone();
        }

        public void MakeTorus()
        {
            Proxy.makeTorus();
        }

        public void MakeWedge()
        {
            Proxy.makeWedge();
        }

        public void MakeSphere()
        {

        }

        public void MakeCylinder()
        {

        }

        public void LoadJson()
        {
            System.Windows.Forms.OpenFileDialog anOpenDialog = new System.Windows.Forms.OpenFileDialog();
            string aDataDir = Environment.GetEnvironmentVariable("CSF_OCCTDataPath");
            string aFilter = "";

            anOpenDialog.InitialDirectory = (aDataDir + "\\json");
            aFilter = "JSON Files (*.json)|*.json";
            anOpenDialog.Filter = aFilter + "|All files (*.*)|*.*";

            if (anOpenDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string aFileName = anOpenDialog.FileName;
                if (aFileName == "")
                {
                    return;
                }
                if (!OCCProxyer.Proxy.loadJson(aFileName))
                {
                    Console.WriteLine("Failed to Open File!");
                }
            }
        }
    }
}
