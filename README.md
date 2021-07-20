# HiveNightmare
aka SeriousSam.  Exploit allowing you to read any registry hives as non-admin.

# What is this?
An exploit for HiveNightmare, discovered by @jonasLyk, PoC by @GossiTheDog, powered by Porgs

# Scope
Appears to work on all supported versions of Windows 10, where System Protection is enabled (should be enabled by default in most configurations).

# What does it do?
Allows you to read SAM data (sensitive) in Windows 10

This file saves the SAM in current directory as SAM-haxx, for use with whatever cracking tools, or whatever, you want.

# Bugs and issues
- Currently only looks for the first four system recovery snapshots.
- Haven't added support for dumping SECURITY, SYSTEM etc registry hives yet so I can't be bothered. 

![Alt Image text](Capture.PNG?raw=true "PoC on Windows 10 21H1 as non-admin")
