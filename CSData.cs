using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Data;
using System.Diagnostics;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.Runtime.InteropServices;

namespace OccView
{
    public struct HighLowTemps
    {
        public int High { get; set; }
        public int Low { get; set; }
        public HighLowTemps(int hi, int lo)
        {
            High = hi;
            Low = lo;
        }
    }

    public class CSData
    {
        public CSData()
        {
            occModel = new global::OCCModel();
        }

        public string JsonPath()
        {
            OpenFileDialog anOpenFileDialog = new OpenFileDialog();
            string aDataDir = Environment.GetEnvironmentVariable("JsonDataPath");
            string aFilter = "";

            anOpenFileDialog.InitialDirectory = (aDataDir + "\\json");
            aFilter = "JSON Files (*.json)|*.json";

            anOpenFileDialog.Filter = aFilter + "|All files(*.*)|*.*";

            if (anOpenFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                return anOpenFileDialog.FileName;
            }
            return null;
        }

        public void ToOccJson()
        {
            string filename = JsonPath();
            if (filename == "" || filename == null)
                return;

            if (!occModel.LoadJson(filename))
            {
                MessageBox.Show("Can't read this file", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        public void LoadJson()
        {
            string filename = JsonPath();
            if (filename == "" || filename == null)
            {
                return;
            }
            //record analyze json file in C# time elapsed
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();

            //read json object from json file
            StreamReader fileReader = File.OpenText(filename);
            JsonTextReader reader = new JsonTextReader(fileReader);
            csJson = (JObject)JToken.ReadFrom(reader);

            stopwatch.Stop();
            string dur = stopwatch.ElapsedMilliseconds.ToString();
            string num = csJson.Count.ToString();
            Console.WriteLine("CSData::LoadJson() size " + num + " cost " + dur + " ms");
            stopwatch.Reset();

            //write json object to json file
            stopwatch.Start();            
            string output = Newtonsoft.Json.JsonConvert.SerializeObject(csJson, Newtonsoft.Json.Formatting.Indented);
            File.WriteAllText("D:\\Test\\Layout.json", output);
            stopwatch.Stop();
            dur = stopwatch.ElapsedMilliseconds.ToString();
            Console.WriteLine("CSData::WriteJson() cost " + dur + " ms");
            stopwatch.Reset();
        }

        public void AnalyzeJson()
        {
            foreach(var x in csJson as JObject)
            {
                Console.WriteLine("{0} : {1}", x.Key, x.Value);
            }
        }

        //Test_Click to test data pass between C# and C++
        public void TestConvert()
        {
            //create C# struct instance
            HighLowTemps temp = new HighLowTemps(35, 15);

            //get instance size and allocate memory pointer
            int size = Marshal.SizeOf(temp);
            byte[] bytes = new byte[size];
            IntPtr structPtr = Marshal.AllocHGlobal(size);

            //convert struct temp to pointer structPtr
            Marshal.StructureToPtr(temp, structPtr, false);
            Marshal.Copy(structPtr, bytes, 0, size);

            //pass structPtr to C++ function
            HighLowTemp tp = new HighLowTemp();
            tp.high = temp.High;
            tp.low = temp.Low;

            //pass struct to C++
            occModel.TestTemp(tp);

            //pass structPtr to C++
            occModel.TestTempPtr(structPtr);

            //get structPtrr from C++
            //HighLowTemp tt = occModel.TestTempRet();

            //release structPtr memory
            Marshal.FreeHGlobal(structPtr);
        }

        //declare json object and file path
        private global::OCCModel occModel;
        private JObject csJson;
    }
}
