// test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <cstdio>
#include<timezoneapi.h>
#define INFO_BUFFER_SIZE 32767
#define DIV 1048576
#define WIDTH 7
#pragma comment(lib, "User32.lib")
using namespace std;
int main() 
{

    TCHAR  infoBuf[INFO_BUFFER_SIZE];
    DWORD  bufCharCount = INFO_BUFFER_SIZE;
    // Get and display the name of the computer.
    if (GetComputerName(infoBuf, &bufCharCount))
        _tprintf(TEXT("\nComputer name:      %s"), infoBuf);
    // Get and display the user name.
    if (GetUserName(infoBuf, &bufCharCount))
        _tprintf(TEXT("\nUser name:          %s"), infoBuf);
    // Get and display the system directory. 
    if (GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE))
        _tprintf(TEXT("\nSystem Directory:   %s"), infoBuf);
    // Get and display the Windows directory. 
    if (GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE))
        _tprintf(TEXT("\nWindows Directory:  %s"), infoBuf);
    SYSTEM_INFO siSysInfo;
    // Copy the hardware information to the SYSTEM_INFO structure. 
    GetSystemInfo(&siSysInfo);
    // Display the contents of the SYSTEM_INFO structure. 
    //printf("Hardware information: \n");
    printf("OEM ID: %u\n", siSysInfo.dwOemId);
    printf("Number of processors: %u\n",
        siSysInfo.dwNumberOfProcessors);
    printf("Page size: %u\n", siSysInfo.dwPageSize);
    printf("Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("Minimum application address: %lx\n",
        siSysInfo.lpMinimumApplicationAddress);
    printf("Maximum application address: %lx\n",
        siSysInfo.lpMaximumApplicationAddress);
    printf("Active processor mask: %u\n",
        siSysInfo.dwActiveProcessorMask);
    MEMORYSTATUSEX statex;

    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    _tprintf(TEXT("percent of memory in use. : %*ld\n"), WIDTH, statex.dwMemoryLoad);
    _tprintf(TEXT("total Mbytes of physical memory : %*I64d.\n"), WIDTH, statex.ullTotalPhys / DIV);
    _tprintf(TEXT("free Mbytes of physical memory : %*I64d.\n"), WIDTH, statex.ullAvailPhys / DIV);
    _tprintf(TEXT("total Mbytes of paging file. : %*I64d\n"), WIDTH, statex.ullTotalPageFile / DIV);
    _tprintf(TEXT("free Mbytes of paging file. : %*I64d\n"), WIDTH, statex.ullAvailPageFile / DIV);
    _tprintf(TEXT("total Mbytes of virtual memory. : %*I64d\n"), WIDTH, statex.ullTotalVirtual / DIV);
    _tprintf(TEXT("free Mbytes of virtual memory. : %*I64d\n"), WIDTH, statex.ullAvailVirtual / DIV);
    _tprintf(TEXT("free Mbytes of extended memory.: %*I64d\n"), WIDTH, statex.ullAvailExtendedVirtual / DIV);
    cout << "\nUsing GetAdaptersAddresses";
    DYNAMIC_TIME_ZONE_INFORMATION time;

    

    return 0;
}
*/
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")


#include <Windows.h>
#include <iostream>
#include <string.h>
#include <iomanip>
#include <string> 

#include <Iphlpapi.h>
#include <chrono>
#include <sstream>
#include <time.h>
#include <locale.h>

#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define INFO_BUFFER_SIZE 32767
#define in_label std::cout << std::left << std::setw(30) 
#define in_data std::cout << std::right << std::setw(40)

void displayDataFromRegistry(const char* label, const char* path, const char* queryPath);
void displayData_TIME_FromRegistry(const char* label, const char* path, const char* queryPath);

#pragma comment(lib, "wbemuuid.lib")

void displayDataFromRegistry(const char* label, const char* path, const char* queryPath) {
	char data[INFO_BUFFER_SIZE];
	memset(data, 0, sizeof(data));
	HKEY hkey;
	DWORD buffer = INFO_BUFFER_SIZE;
	in_label << label;
	RegOpenKeyA(HKEY_LOCAL_MACHINE, path, &hkey);
	RegQueryValueExA(hkey, queryPath, NULL, NULL, (LPBYTE)data, &buffer);
	in_data << data << std::endl;
	RegCloseKey(hkey);

}


void displayData_TIME_FromRegistry(const char* label, const char* path, const char* queryPath) {
	char data[INFO_BUFFER_SIZE];
	memset(data, 0, sizeof(data));
	HKEY hkey;
	DWORD dwType = REG_DWORD;
	DWORD buffer = INFO_BUFFER_SIZE;
	in_label << label;
	RegOpenKeyA(HKEY_LOCAL_MACHINE, path, &hkey);
	RegQueryValueExA(hkey, queryPath, NULL, NULL, (LPBYTE)data, &buffer);
#define len	strlen(data);
	int val[8];
	std::string hexVal;
	std::stringstream ss;

	for (int i = strlen(data) - 1; i >= 0; i--) {
		val[i] = (int)data[i];
		val[i] &= 0xff;

		ss << std::hex << val[i];

	}
	hexVal = ss.str();

	std::stringstream ss2;

	ss2 << std::hex << hexVal;

	long long a = 0;

	ss2 >> a;

	time_t epch = a;

	//in_data << asctime(gmtime(&epch));

	RegCloseKey(hkey);
}
int main() {
	in_label << "Host Name: ";
	CHAR host_name[INFO_BUFFER_SIZE];
	memset(host_name, 0, sizeof(host_name));
	DWORD buffer = INFO_BUFFER_SIZE;
	GetComputerNameA(host_name, &buffer);
	in_data << host_name << std::endl;

	displayDataFromRegistry("OS Name: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");
	displayDataFromRegistry("OS Version: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "BuildLab");
	displayDataFromRegistry("OS Manufacturer: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\OEM", "Manufacturer");
	displayDataFromRegistry("OS Build Type: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentType");

	std::cout << "Registered Organization: " << "\n";

	displayDataFromRegistry("Registered Owner: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "RegisteredOwner");
	displayDataFromRegistry("Product ID: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductId");
	displayData_TIME_FromRegistry("Original Install Date: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "InstallDate");

	displayDataFromRegistry("System Manufacturer: ", "HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardManufacturer");
	displayDataFromRegistry("System Model: ", "HARDWARE\\DESCRIPTION\\System\\BIOS", "SystemProductName");


	in_label << "Processor(s): ";
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	in_data << sysinfo.dwNumberOfProcessors << "\n";
	//free(sysinfo);


	displayDataFromRegistry("BIOS Version", "HARDWARE\\DESCRIPTION\\System\\BIOS", "BIOSVersion");
	displayDataFromRegistry("Windows Directory", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "SystemRoot");
	displayDataFromRegistry("System Director", "SOFTWARE\\Microsoft\\AppV\\Client", "InstallPath");


	//Boot device:
	in_label << "Boot Device: ";
	WCHAR  VolumeName[MAX_PATH] = L"";
	//HANDLE FindHandle           = INVALID_HANDLE_VALUE;
	FindFirstVolumeW(VolumeName, ARRAYSIZE(VolumeName));
	std::wcout << std::right << std::setw(35) << VolumeName << std::endl;

	//System Locale
	setlocale(LC_ALL, "");
	in_label << "SYstem Locale: ";
	in_data << setlocale(LC_ALL, NULL) << std::endl;

	in_label << "Input Locale: ";
	in_data << setlocale(LC_ALL, NULL) << std::endl;

	//RAM
	MEMORYSTATUSEX memStatus;
	memStatus.dwLength = sizeof(memStatus);
	GlobalMemoryStatusEx(&memStatus);


	in_label << "Total Physical Memory: ";
	in_data << memStatus.ullTotalPhys << std::endl;

	in_label << "Available Physical Memory: ";
	in_data << memStatus.ullAvailPhys << std::endl;

	in_label << "Virtual Memory: Max Size: ";
	in_data << memStatus.ullTotalPageFile << std::endl;

	in_label << "Virtual Memory: Available: ";
	in_data << memStatus.ullAvailPageFile << std::endl;

	in_label << "Virtual Memory: In Use: ";
	in_data << memStatus.ullTotalPageFile - memStatus.ullAvailPageFile << std::endl;

	//free(memStatus);

	displayDataFromRegistry("Page File Location(s)", "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Memory Management", "PagingFiles");
	displayDataFromRegistry("Logon Server: ", "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");

	//Adapter:
	in_label << "Network Card(s): ";

	PIP_ADAPTER_ADDRESSES  pAdapterInfo;
	PIP_ADAPTER_ADDRESSES  pAdapter = NULL;
	DWORD dwRetVal = 0;

	ULONG ulOutBufLen = sizeof(PIP_ADAPTER_ADDRESSES);

	pAdapterInfo = (IP_ADAPTER_ADDRESSES*)malloc(ulOutBufLen);

	dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAdapterInfo, &ulOutBufLen);

	if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_ADDRESSES*)malloc(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			std::cout << "Error";
		}
		if (dwRetVal = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, pAdapterInfo, &ulOutBufLen) == NO_ERROR) {
			int count = 0;
			PIP_ADAPTER_ADDRESSES backup = NULL;

			backup = pAdapterInfo;

			while (backup) {
				count += 1;
				backup = backup->Next;
			}

			pAdapter = pAdapterInfo;
			std::cout << std::right << std::setw(30 - 8) << count;
			std::cout << " NIC(s) Installed." << std::endl;

			while (pAdapter) {
				std::wcout << std::right << std::setw(70) << pAdapter->FriendlyName << std::endl;
				std::wcout << std::right << std::setw(70) << pAdapter->Description << std::endl;
				std::cout << std::right << std::setw(70) << pAdapter->AdapterName << std::endl;
				//std::cout << std::right << std::setw(70) << pAdapter->Dhcpv4Server  << std::endl;

				std::cout << "\n";

				pAdapter = pAdapter->Next;

			}
		}
	}
	return 0;
}