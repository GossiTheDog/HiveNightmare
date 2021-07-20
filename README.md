# HiveNightmare
Exploit allowing you to read registry hives as non-admin

# What is this?
An exploit for HiveNightmare, discovered by @jonasLyk, PoC by @GossiTheDog, powered by Porgs

# Scope
Appears to work on all supported versions of Windows 10, where System Protection is enabled (should be enabled by default in most configurations).

# What does it do?
Allows you to read SAM data (sensitive) in Windows 10

This file saves the SAM in current directory as SAM-haxx, for use with whatever cracking tools, or whatever, you want.

# Bugs and issues
Hardcoded for snapshot #4, as this works on my PC - really, it should cycle through them all to find the latest.

![Alt Image text](Capture.png?raw=true "PoC on Windows 10 21H1 as non-admin")
