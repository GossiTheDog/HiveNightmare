# HiveNightmare
aka SeriousSam, or now CVE-2021–36934.  Exploit allowing you to read any registry hives as non-admin.

# What is this?
An zero day exploit for HiveNightmare, which allows you to retrieve all registry hives in Windows 10 as a non-administrator user.  For example, this includes hashes in SAM, which can be used to execute code as SYSTEM.

# Download
This is the direct download link for most recent version: https://github.com/GossiTheDog/HiveNightmare/raw/master/Release/HiveNightmare.exe

# Authors 
- Discovered by @jonasLyk.
- PoC by @GossiTheDog, powered by Porgs.
- Additions by @0xblacklight, @DHerls, @HynekPetrak

# Scope
Works on all supported versions of Windows 10, where System Protection is enabled (should be enabled by default in most configurations). 

# How does this work?
The permissions on key registry hives are set to allow all non-admin users to read the files by default, in most Windows 10 configurations.  This is an error.

# What does the exploit do?
Allows you to read SAM data (sensitive) in Windows 10, as well as the SYSTEM and SECURITY hives.

This exploit uses VSC to extract the SAM, SYSTEM, and SECURITY hives even when in use, and saves them in current directory as HIVENAME-haxx, for use with whatever cracking tools, or whatever, you want.

# Pulling Credentials out
```
python3 secretsdump.py -sam SAM-haxx -system SYSTEM-haxx -security SECURITY-haxx LOCAL
```

# More info?
I wrote a blog: https://doublepulsar.com/hivenightmare-aka-serioussam-anybody-can-read-the-registry-in-windows-10-7a871c465fa5

![Alt Image text](screenshot.PNG?raw=true "PoC on Windows 10 21H1 as non-admin")

Video of exploit:
https://www.youtube.com/watch?v=5zdIq6t3DOw
