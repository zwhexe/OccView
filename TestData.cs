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

    public class TestData
    {
        public TestData()
        {
            jsData = new JsonData();
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

            if (!jsData.LoadJson(filename))
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
            //0.create C# struct instance
            HighLowTemps temp = new HighLowTemps(35, 15);
            
            //1.create CLI struct and pass to CLI
            HighLowTemp tp = new HighLowTemp();
            tp.high = temp.High;
            tp.low = temp.Low;
            jsData.TestTemp(tp);

            //2.alloc IntPtr and pass to CLI
            int size = Marshal.SizeOf(temp);
            IntPtr structPtr = Marshal.AllocHGlobal(size);
            Marshal.StructureToPtr(temp, structPtr, false);
            jsData.TestTempPtr(structPtr);

            //3.alloc bytes and pass to CLI
            //byte[] bytes = new byte[size];
            //Marshal.Copy(structPtr, bytes, 0, size);
            //cppData.TestTempByt(bytes);

            //4.get structPtr from CLI
            IntPtr pt = jsData.TestTempRet();
            HighLowTemps tps = (HighLowTemps)Marshal.PtrToStructure(pt, typeof(HighLowTemps));
            Console.WriteLine("{0} {1}", tps.High, tps.Low);
            
            //5.release structPtr memory
            Marshal.FreeHGlobal(structPtr);
            Marshal.FreeHGlobal(pt);
        }

        //declare json object and file path
        private JsonData jsData;
        private JObject csJson;
    }
}
