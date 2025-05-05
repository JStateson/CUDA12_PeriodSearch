using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;

namespace TimeTests
{
    internal class Program
    {
        static void Main(string[] args)
        {
            string nl = Environment.NewLine;
            string filePath = "stderr.txt";
            //string filePath = @"C:\Users\josep\source\repos\PeriodSearch\period_search_petri\x64_v12.8\Release\i7-9800x_2080s\stderr.txt";

            List<string> TimeEntries = new List<string>();
            List<string> AppName = new List<string>();
            List<string> CPUName = new List<string>();
            List<string> TGTName = new List<string>();

            if (!File.Exists(filePath))
            {
                Console.WriteLine("Error: File 'stderr.txt' not found.");
                return;
            }

            try
            {
                string[] lines = File.ReadAllLines(filePath);
                string[] sLine;
                foreach (string line in lines)
                {
                    if (line.Contains("Application"))
                    {
                        AppName.Add(line);
                    }
                    if (line.Contains("CPU:"))// || line.Contains("CUDA"))
                    {
                        CPUName.Add(line);
                    }
                    if (line.Contains("Target") || line.Contains("Using") || line.Contains("NVIDIA"))
                    {
                        TGTName.Add(line);
                    }
                    if (line.Contains("standalone") || line.Contains("boinc_finish"))
                    {
                        sLine = line.Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                        if (sLine.Length > 1)
                        {
                            string timeEntry = sLine[0];
                            if (timeEntry.Contains(':')) TimeEntries.Add(timeEntry);
                            else
                            {
                                timeEntry = sLine[1];
                                TimeEntries.Add(timeEntry);
                            }
                        }
                    }
                }
                int n = TimeEntries.Count - 1;
                if (n >= 1)
                {
                    TimeSpan TimeStart = TimeSpan.Parse(TimeEntries[n - 1]);
                    TimeSpan TimeEnd = TimeSpan.Parse(TimeEntries[n]);
                    TimeSpan TimeDiff = TimeEnd - TimeStart;
                    string sName = "App did not provide its name";
                    if (AppName.Count > 0) sName = AppName[AppName.Count - 1];
                    Console.WriteLine(sName);

                    string tName = "APP did not provide GPU name";
                    if (TGTName.Count > 0) tName = TGTName[TGTName.Count - 1];
                    else Console.WriteLine(tName);

                    string cName = "APP did not provide CPU name";
                    if (CPUName.Count > 0) cName = CPUName[CPUName.Count - 1];
                    else Console.WriteLine(cName);

                    string sTimeDiff = TimeDiff.ToString(@"hh\:mm\:ss");

                    if (CPUName.Count > 0)
                        Console.WriteLine(cName);
                    if (TGTName.Count > 0)
                        Console.WriteLine(tName);
                    Console.WriteLine(TimeEntries[n] + nl + TimeEntries[n - 1] + " Minutes " + sTimeDiff);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"An error occurred while reading the file:\n{ex.Message}");
            }
        }
    }
}
