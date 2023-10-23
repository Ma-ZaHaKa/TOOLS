using SimpleJSON;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Security.Cryptography;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Web;

namespace srv
{
    public partial class Program
    {
        static string[] RemoveArgument(string[] args, string argument)
        {
            List<string> modifiedArgs = new List<string>(args);
            modifiedArgs.Remove(argument);
            return modifiedArgs.ToArray();
        }
        //-------TIME-----
        public static string TimeApi = "https://timeapi.io"; // without params = utc

        static (bool, DateTime) ConvertDateTimeZone(DateTime dateTime, string fromTimeZone = "Europe/Kyiv", string toTimeZone = "UTC")
        {
            string url = $"{TimeApi}/api/Conversion/ConvertTimeZone";
            HttpWebRequest httpRequest = (HttpWebRequest)WebRequest.Create(url);

            httpRequest.Method = "POST";
            httpRequest.ProtocolVersion = HttpVersion.Version11;
            //httpRequest.AllowAutoRedirect = false;
            httpRequest.ContentType = $"application/json";
            //httpRequest.Headers.Add("Cookie", $"PHPSESSID={psi}");
            //httpRequest.UserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/96.0.4664.110 Safari/537.36";

            string result = "";
            try
            {
                string data = $"{{\"fromTimeZone\": \"{fromTimeZone}\", \"dateTime\": \"{dateTime.ToString("yyyy-MM-dd HH:mm:ss")}\", \"toTimeZone\": \"{toTimeZone}\", \"dstAmbiguity\": \"\"}}";
                using (var streamWriter = new StreamWriter(httpRequest.GetRequestStream())) { streamWriter.Write(data); }

                using (var httpResponse = (HttpWebResponse)httpRequest.GetResponse())
                {
                    using (var streamReader = new StreamReader(httpResponse.GetResponseStream())) { result = streamReader.ReadToEnd(); }
                }
            }
            catch (Exception e) { Console.WriteLine(e.Message); return (false, new DateTime()); }
            finally { httpRequest?.Abort(); }
            try
            {
                JSONNode jres = JSON.Parse(result);
                JSONNode res = jres["conversionResult"];
                return (true, new DateTime(int.Parse(res["year"]), int.Parse(res["month"]), int.Parse(res["day"]), int.Parse(res["hour"]), int.Parse(res["minute"]), int.Parse(res["seconds"])));
            }
            catch { return (false, new DateTime()); }
        }
        public static long ConvertToUnixUTCTimestampFromUTCDT(DateTime dateTime) //UTC DT TO UTC UNIX
        {
            DateTime unixEpoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
            TimeSpan timeSpan = dateTime - unixEpoch;
            return (long)timeSpan.TotalSeconds;
        }


        /*public static (bool, long) ConvertToUnixUtcTimestampFromLocalDT(DateTime targetDT) // LC DT TO UTC UNIX
        { // ukr -> FLE Standard Time
          // https://stackoverflow.com/questions/50045322/converting-datetime-local-or-utc-to-moscow-and-kiev-timezone-issues-depending

            (bool, DateTime) ukrnow = GetUkrDT(); //59
            (bool, DateTime) utcnow = GetUTCDT(); //00 +1h bug
            if (!ukrnow.Item1 || !utcnow.Item1) { return (false, ConvertToUnixUTCTimestampFromUTCDT(DateTime.Now.ToUniversalTime())); }

            TimeSpan timeDifference = ukrnow.Item2 - utcnow.Item2; // local timezone offset utc + local
            //TimeSpan roundedtimeDifference = new TimeSpan(timeDifference.Hours, timeDifference.Minutes, 0); // api delay now offset err when 59 sec
            TimeSpan roundedtimeDifference = new TimeSpan(timeDifference.Hours, 0, 0); // api delay now offset
            DateTime utcDateTime = (targetDT - roundedtimeDifference);
            return (true, ConvertToUnixUTCTimestampFromUTCDT(utcDateTime));
        }
        public static (bool, DateTime) GetUkrDT()
        {
            string date = "", urlparam = "/api/Time/current/coordinate?latitude=50.27&longitude=30.31";
            try
            {
                using (WebClient wc = new WebClient()) { date = wc.DownloadString(TimeApi + urlparam); }
            }
            catch { return (false, DateTime.Now); }
            date = Regex.Match(date, "\"dateTime\":\"(.*?)\",\"date\":").Groups[1].Value;
            int y = 0, mon = 0, d = 0, h = 0, min = 0, s = 0;
            GroupCollection DATAGROUP = Regex.Match(date, "([0-9]*)-([0-9]*)-([0-9]*)T([0-9]*):([0-9]*):([0-9]*)").Groups;
            try { y = int.Parse(DATAGROUP[1].Value); mon = int.Parse(DATAGROUP[2].Value); d = int.Parse(DATAGROUP[3].Value); h = int.Parse(DATAGROUP[4].Value); min = int.Parse(DATAGROUP[5].Value); s = int.Parse(DATAGROUP[6].Value); }
            catch { return (false, DateTime.Now); }
            if ((y + mon + d + h + min + s) == 0) { return (false, DateTime.Now); }
            return (true, new DateTime(y, mon, d, h, min, s));
        }

        public static (bool, DateTime) GetUTCDT()
        {
            string date = "", urlparam = "/api/Time/current/coordinate";
            try
            {
                using (WebClient wc = new WebClient()) { date = wc.DownloadString(TimeApi); }
            }
            catch { return (false, DateTime.Now); }
            date = Regex.Match(date, "\"dateTime\":\"(.*?)\",\"date\":").Groups[1].Value;
            int y = 0, mon = 0, d = 0, h = 0, min = 0, s = 0;
            GroupCollection DATAGROUP = Regex.Match(date, "([0-9]*)-([0-9]*)-([0-9]*)T([0-9]*):([0-9]*):([0-9]*)").Groups;
            try { y = int.Parse(DATAGROUP[1].Value); mon = int.Parse(DATAGROUP[2].Value); d = int.Parse(DATAGROUP[3].Value); h = int.Parse(DATAGROUP[4].Value); min = int.Parse(DATAGROUP[5].Value); s = int.Parse(DATAGROUP[6].Value); }
            catch { return (false, DateTime.Now); }
            if ((y + mon + d + h + min + s) == 0) { return (false, DateTime.Now); }
            return (true, new DateTime(y, mon, d, h, min, s));
        }*/



        /*static long ConvertToUnixUTCTimestampFromUTCDT(DateTime dateTime) //UTC DT TO UTC UNIX
         {
             DateTime unixEpoch = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc);
             TimeSpan timeSpan = dateTime - unixEpoch;
             return (long)timeSpan.TotalSeconds;
         }

         public static long ConvertToUnixUtcTimestampFromLocalDT(DateTime dateTime, string TimeRegion = "FLE Standard Time") // LC DT TO UTC UNIX
         { // ukr -> FLE Standard Time
             // https://stackoverflow.com/questions/50045322/converting-datetime-local-or-utc-to-moscow-and-kiev-timezone-issues-depending
             TimeZoneInfo timeZone;
             try { timeZone = TimeZoneInfo.FindSystemTimeZoneById(TimeRegion); }
             catch { return ConvertToUnixUTCTimestampFromUTCDT(DateTime.Now.ToUniversalTime()); }

             DateTime utcDateTime = TimeZoneInfo.ConvertTimeToUtc(dateTime, timeZone);
             return (long)(utcDateTime - new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc)).TotalSeconds;
         }*/



        static bool _IsMyIP(string myip)
          => new WebClient().DownloadString("http://domain/ip.php") == myip;
        static string ClearUrl(string url)
          => Regex.Match(url, "(p[0-9]*n[0-9]*)").Groups[1].Value;

        static bool IsLinux() => File.Exists("/bin/bash");

        static bool IsWindows() => Directory.Exists("C:\\Windows");
        static string RemoveDouble32(string str, bool is_trim = true)
        {
            bool is_find = false; // regex ( {2,}) for all double 32
            str = is_trim ? str.Trim() : str;
            for (int i = 0; i < str.Length; i++)
            {
                if (is_find && str[i] == ' ') { str = str.Remove(i, 1); i--; continue; }
                is_find = str[i] == ' ';
            }
            return str;
        }
        static void ZipAllFiles(string target_dir, string out_file) // return path to zip
        {
            if (File.Exists(out_file)) { try { File.Delete(out_file); } catch { } }
            if (!out_file.ToLower().EndsWith(".zip")) { return; }
            ZipFile.CreateFromDirectory(target_dir, out_file);
        }
        static void Sync(string test_code, string api = "http://just.com/tdb/api.php")
        {
            try
            {
                test_code = Regex.Match(test_code, "(p[0-9]*n[0-9]*)").Groups[1].Value;
                string teacher_code = Regex.Match(test_code, "p([0-9]*)n[0-9]*").Groups[1].Value;
                if ((teacher_code == "") || (test_code == "")) { return; }
                string zip = $"{test_db}/p{teacher_code}/{test_code}.zip";
                ZipAllFiles($"{test_db}/p{teacher_code}/{test_code}/", zip);
                new WebClient().UploadFile($"{api}?mode=upload&test_code={test_code}", zip);
                try { File.Delete(zip); } catch { }
            }
            catch { }
        }
        static string EncodeB64(string input)
        {
            byte[] bytesToEncode = Encoding.UTF8.GetBytes(input);
            return Convert.ToBase64String(bytesToEncode);
        }
        static string DecodeB64(string input)
        {
            byte[] bytesToDecode = Convert.FromBase64String(input);
            return Encoding.UTF8.GetString(bytesToDecode);
        }

        static string UrlEncode(string input)
        {
            return Uri.EscapeDataString(input);
        }
        static string UrlDecode(string input)
        {
            return Uri.UnescapeDataString(input);
        }

        static string Md5(string input)
        {
            using (var md5 = MD5.Create())
            {
                var byteHash = md5.ComputeHash(Encoding.UTF8.GetBytes(input));
                var hash = BitConverter.ToString(byteHash).Replace("-", "");
                return hash.ToLower();
            }
        }
        public static string XorString(string str)
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < str.Length; i++)
            {
                sb.Append((char)(str[i] ^ 255));
            }
            return sb.ToString();
        }
        static string XOREncrypt(string input, string key = "%KU")
        {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < input.Length; i++)
            {
                char c = (char)(input[i] ^ key[i % key.Length]);
                sb.Append(c);
            }
            return sb.ToString();
        }

        static string EncP(string pwd, string s = "vsau#")
          => Md5($"{pwd}{s}");

        static string HTMLCharsEncode(string input)
        {
            return HttpUtility.HtmlEncode(input);
        }
        static string HTMLCharsDecode(string input)
        {
            return HttpUtility.HtmlDecode(input);
        }


    }
}
