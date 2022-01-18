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
    public class WeatherForecast
    {
        public DateTimeOffset Date { get; set; }
        public int TemperatureCelsius { get; set; }
        public string Summary { get; set; }
        public string SummaryField;
        public IList<DateTimeOffset> DatesAvailable { get; set; }
        public Dictionary<string, HighLowTemps> TemperatureRanges { get; set; }
        public string[] SummaryWords { get; set; }
        public WeatherForecast()
        {
            Date = DateTimeOffset.UtcNow;
            TemperatureCelsius = 105;
            Summary = "Weather Forecast Success";
            
            DatesAvailable = new List<DateTimeOffset>();
            DatesAvailable.Add(DateTimeOffset.MaxValue);
            DatesAvailable.Add(DateTimeOffset.MinValue);

            TemperatureRanges = new Dictionary<string, HighLowTemps>();
            TemperatureRanges["Hot"] = new HighLowTemps(100, 88);
            TemperatureRanges["Cold"] = new HighLowTemps(55, 49);

            SummaryWords = new string[2];
            SummaryWords[0] = "Warm";
            SummaryWords[1] = "Cool";
        }
    }

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
            occJson = new OCCJson();
        }
        public void InitWeather()
        {
            WeatherForecast weather = new WeatherForecast();

            string jsonString = JsonConvert.SerializeObject(weather, Formatting.Indented);
            File.WriteAllText(filepath + "CSJson.json", jsonString);
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

            if (!occJson.LoadJson(filename))
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

        public void TestArray()
        {
            int N = 0;
            int[] n;
            n = new int[10];
            for (int i = 0; i < 10; i++)
            {
                n[i] = i;
            }

            HighLowTemps temp = new HighLowTemps(35, 10);

            int size = Marshal.SizeOf(temp);
            byte[] bytes = new byte[size];
            IntPtr structPtr = Marshal.AllocHGlobal(size);

            Marshal.StructureToPtr(temp, structPtr, false);
            Marshal.Copy(structPtr, bytes, 0, size);

            HighLowTemp tp = new HighLowTemp();
            tp.high = temp.High;
            tp.low = temp.Low;
            occJson.TestTemp(tp);
            //occJson.TestTempByt(bytes);
            occJson.TestTempPtr(structPtr);

            Marshal.FreeHGlobal(structPtr);
        }

        //This is OCC defined json which use nlohmann 
        private OCCJson occJson;
        private JObject csJson;
        private string filepath = "D:\\";
    }
}
