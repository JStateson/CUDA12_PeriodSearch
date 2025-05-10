// TimeTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
#include <cmath>
#include <iomanip>

double timeToMinutes(const std::string& time_str) {
    int hours, minutes, seconds;
    char colon; // to ignore the ':' characters

    // Parse the time string using a stringstream
    std::stringstream ss(time_str);
    ss >> hours >> colon >> minutes >> colon >> seconds;

    // Convert time to total minutes
    return hours * 60 + minutes + static_cast<double>(seconds) / 60.0;
}


std::string minutesToTime(double totalMinutes) {
    int totalSeconds = static_cast<int>(std::round(totalMinutes * 60));
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;

    return oss.str();
}


// no need to use vectors, just wanted to use them at first

int main() {
    int  m, n = 0;
    std::vector<std::string> TimeEntries;
    std::vector<std::string> AppName;
    std::vector<std::string> CPUName;
    std::vector<std::string> TGTName;
    std::vector<std::string> OVRName;
    std::vector<double> TimeSpan;

    std::string nl = "\n";
    std::string filePath = "stderr.txt";

	std::ifstream file(filePath);

    if (!file)
    {
		std::cout << "Error: File 'stderr.txt' not found." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {

		if (line.find("Applications") != std::string::npos) {
			AppName.push_back(line);
		}
		if (line.find("CPU:") != std::string::npos) {
			CPUName.push_back(line);
		}
        if (line.find("Using") != std::string::npos) {
            TGTName.push_back(line);
        }
        if (line.find("NVIDIA") != std::string::npos) {
            TGTName.push_back(line);
        }
		if (line.find("Target") != std::string::npos) {
			TGTName.push_back(line);
		}
		if (line.find("Override") != std::string::npos) {
			OVRName.push_back(line);
		}
        

        if ((line.find("standalone") != std::string::npos) ||
            (line.find("boinc_finish") != std::string::npos)) {
            
            std::vector<std::string> words;
            std::string current;

            for (char ch : line) {
                if (std::isspace(static_cast<unsigned char>(ch))) {
                    if (!current.empty()) {
                        words.push_back(current);
                        current.clear();
                    }
                }
                else {
                    current += ch;
                }
            }

            // With the following line:
            const char* ch = words[0].c_str();
			if (strchr(ch, ':'))           // some apps haved dates in the first column
                TimeEntries.push_back(words[0]);           
            else 
                TimeEntries.push_back(words[1]);
            if (line.find("boinc_finish") != std::string::npos)
            {
                n = TimeEntries.size() - 1;
				m = AppName.size() + CPUName.size() + TGTName.size() + OVRName.size();  
				if (n > 0 && m > 0)
				{
                    double endTime = timeToMinutes(TimeEntries[n]);
                    double startTime = timeToMinutes(TimeEntries[n - 1]);
                    double timeSpan = endTime - startTime;
                    std::string sTimeDiff = minutesToTime(timeSpan);

                    std::string sName = "App did not provide its name";
                    if (AppName.size()) sName = AppName[0];
                    std::cout << sName << std::endl;

                    std::string cName = "APP did not provide CPU name";
                    if (CPUName.size())cName = CPUName[0];
                    std::cout << cName << std::endl;

                    std::string tName = "APP did not provide GPU name";
                    if (TGTName.size()) tName = TGTName[0];
                    std::cout << tName << std::endl;

                    std::string oName = "APP was not overridden";
                    if (OVRName.size()) oName = OVRName[0];
                    std::cout << oName << std::endl;

                    std::cout << TimeEntries[n] << nl << TimeEntries[n - 1] <<
                        " Minutes " << sTimeDiff << nl << std::endl;
				}
				TimeEntries.clear();
				AppName.clear();
				CPUName.clear();
				TGTName.clear();
				OVRName.clear();
			}
        }   
    }


    file.close();
    return 0;
}

