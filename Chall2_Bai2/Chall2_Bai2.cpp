#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <string>
using namespace std;
unsigned long show_module(MEMORY_BASIC_INFORMATION info) {
    unsigned long usage = 0;

    std::cout << info.BaseAddress << "(" << info.RegionSize / 1024 << ")\t";

    switch (info.State) {
    case MEM_COMMIT:
        std::cout << "Committed";
        break;
    case MEM_RESERVE:
        std::cout << "Reserved";
        break;
    case MEM_FREE:
        std::cout << "Free";
        break;
    }
    std::cout << "\t";
    switch (info.Type) {
    case MEM_IMAGE:
        std::cout << "Code Module";
        break;
    case MEM_MAPPED:
        std::cout << "Mapped     ";
        break;
    case MEM_PRIVATE:
        std::cout << "Private    ";
    }
    std::cout << "\t";

    int guard = 0, nocache = 0;

    if (info.AllocationProtect & PAGE_NOCACHE)
        nocache = 1;
    if (info.AllocationProtect & PAGE_GUARD)
        guard = 1;

    info.AllocationProtect &= ~(PAGE_GUARD | PAGE_NOCACHE);

    if ((info.State == MEM_COMMIT) && (info.AllocationProtect == PAGE_READWRITE || info.AllocationProtect == PAGE_READONLY))
        usage += info.RegionSize;

    switch (info.Protect) {
    case PAGE_READONLY:
        std::cout << "Read Only";
        break;
    case PAGE_READWRITE:
        std::cout << "Read/Write";
        break;
    case PAGE_WRITECOPY:
        std::cout << "Copy on Write";
        break;
    case PAGE_EXECUTE:
        std::cout << "Execute only";
        break;
    case PAGE_EXECUTE_READ:
        std::cout << "Execute/Read";
        break;
    case PAGE_EXECUTE_READWRITE:
        std::cout << "Execute/Read/Write";
        break;
    case PAGE_EXECUTE_WRITECOPY:
        std::cout << "COW Executable";
        break;
    }
   // if (info.AllocationProtect == PAGE_EXECUTE_READWRITE) {
   //     std::cout << "Execute/Read/Write";
   // }

    if (guard)
        std::cout << "\tguard page";
    if (nocache)
        std::cout << "\tnon-cacheable";
    std::cout << "\n";
    return usage;
}
unsigned long Check_module(MEMORY_BASIC_INFORMATION info) {
    unsigned long usage = 0;
    if ((info.State == MEM_COMMIT) && (info.AllocationProtect == PAGE_READWRITE || info.AllocationProtect == PAGE_READONLY))
        usage += info.RegionSize;
    if (info.AllocationProtect == PAGE_EXECUTE_READWRITE) {
        std::cout << info.BaseAddress << "(" << info.RegionSize / 1024 << ")\t";
        switch (info.State) {
        case MEM_COMMIT:
            std::cout << "Committed";
            break;
        case MEM_RESERVE:
            std::cout << "Reserved";
            break;
        case MEM_FREE:
            std::cout << "Free";
            break;
        }
        std::cout << "\t";
        switch (info.Type) {
        case MEM_IMAGE:
            std::cout << "Code Module";
            break;
        case MEM_MAPPED:
            std::cout << "Mapped     ";
            break;
        case MEM_PRIVATE:
            std::cout << "Private    ";
        }
        std::cout << "\t";
        cout << "PAGE_EXECUTE_READWRITE" << endl;
        int guard = 0, nocache = 0;

        if (info.AllocationProtect & PAGE_NOCACHE)
            nocache = 1;
        if (info.AllocationProtect & PAGE_GUARD)
            guard = 1;
        info.AllocationProtect &= ~(PAGE_GUARD | PAGE_NOCACHE);
        //if ((info.State == MEM_COMMIT) && (info.AllocationProtect == PAGE_READWRITE || info.AllocationProtect == PAGE_READONLY))
           // usage += info.RegionSize;
        // if (info.AllocationProtect == PAGE_EXECUTE_READWRITE) {
        //     std::cout << "Execute/Read/Write";
        // }
        if (guard)
            std::cout << "\tguard page";
        if (nocache)
            std::cout << "\tnon-cacheable";
        std::cout << "\n";
    }

    return usage;
}
unsigned long show_modules(HANDLE process) {

    unsigned long usage = 0;

    unsigned char* p = NULL;
    MEMORY_BASIC_INFORMATION info;

    for (p = NULL;VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info);p += info.RegionSize)
    {
        usage += Check_module(info);
    }
    return usage;
}

int main(int argc, char** argv) {

    /*int pid;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <process ID>\n";
        return EXIT_FAILURE;
    }
    
    pid = std::stoi(argv[1]);
    //
    */
    while (true) {

        int PID;
        cout << "Process ID : ";
        cin >> PID;
        HANDLE process = OpenProcess(
            PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
            false,
            PID);

        unsigned long mem_used = show_modules(process);
        std::cout << "Total memory used: " << mem_used / 10224 << "KB\n";
    }
    return 0;
}