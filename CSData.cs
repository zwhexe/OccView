using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;
using System.Data;
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

    public class HighLowTemps
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
        public void ToOccJson()
        {
            OpenFileDialog anOpenFileDialog = new OpenFileDialog();
            string aDataDir = Environment.GetEnvironmentVariable("JsonDataPath");
            string aFilter = "";

            anOpenFileDialog.InitialDirectory = (aDataDir + "\\json");
            aFilter = "JSON Files (*.json)|*.json";

            anOpenFileDialog.Filter = aFilter + "|All files(*.*)|*.*";
            if (anOpenFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string fileName = anOpenFileDialog.FileName;
                if (fileName == "")
                {
                    return;
                }
                //analyze json file in OCCProxy
                unsafe
                {
                    sbyte* cptr = stackalloc sbyte[26];
                    
                    if (!occJson.LoadJson(fileName))
                    {
                        MessageBox.Show("Cann't read this file", "Error!", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                }

            }
        }

        public void LoadJson()
        {
            OpenFileDialog anOpenFileDialog = new OpenFileDialog();
            string aDataDir = Environment.GetEnvironmentVariable("JsonDataPath");
            string aFilter = "";

            anOpenFileDialog.InitialDirectory = (aDataDir + "\\json");
            aFilter = "JSON Files (*.json)|*.json";

            anOpenFileDialog.Filter = aFilter + "|All files(*.*)|*.*";
            if (anOpenFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                string fileName = anOpenFileDialog.FileName;
                if (fileName == "")
                {
                    return;
                }
                //analyze json file in C#
                string jsonString = File.ReadAllText(fileName);
                StreamReader fileReader = File.OpenText(fileName);
                JsonTextReader reader = new JsonTextReader(fileReader);
                JObject job = (JObject)JToken.ReadFrom(reader);

                foreach(var x in job as JObject)
                {
                    Console.WriteLine("{0} : {1}", x.Key, x.Value);
                }
            }
        }

        //This is OCC defined json which use nlohmann 
        private OCCJson occJson;
        private string filepath = "D:\\Test\\";
    }
}
