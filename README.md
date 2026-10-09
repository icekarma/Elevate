# Elevate

A lightweight C++20 Win32 command-line utility that launches executables with elevated privileges (Administrator rights).

Similar to a basic `sudo` for Windows, `Elevate` uses the Windows Shell API and the `runas` verb to trigger a User Account Control (UAC) prompt, allowing the launch of administrative tasks, installers, or scripts from a standard-privilege command prompt or PowerShell window.

## Features
* **Seamless Argument Passing:** Automatically handles and quotes arguments containing spaces.
* **Windows Shell API:** Uses `ShellExecuteEx` for native UAC integration.
* **Fire-and-Forget:** Spawns the elevated process and immediately returns control to your current console.
* **Modern C++:** Written in clean C++20 that compiles with no warnings, even with `-Wall`.

## Usage

```cmd
elevate <executable> [args...]
```

### Examples

Open a privileged Command Prompt:
```cmd
elevate cmd.exe
```

Edit a protected system file in Notepad:
```cmd
elevate notepad.exe C:\Windows\System32\drivers\etc\hosts
```

Run a script that requires admin rights:
```cmd
elevate powershell.exe -ExecutionPolicy Bypass -File setup_environment.ps1
```

## Exit codes

The application returns specific exit codes to help with script integration:

| Code | Status | Description |
| :--- | :--- | :--- |
| `0`  | **Success** | The program was successfully launched. |
| `1`  | **Failure** | Missing arguments or a general process execution failure. |
| `2`  | **Cancelled** | The UAC elevation prompt was declined by the user. |

*(Note: Because this is a fire-and-forget utility, an exit code of `0` means the process was successfully **started**, not that the elevated process itself finished successfully).*

## Building from source

This project uses the modern Visual Studio `.slnx` solution format and targets the C++20 standard.

### Prerequisites

* Windows SDK
* Visual Studio 2026

### Build instructions

1. Open `Elevate.slnx` in Visual Studio.
2. Select your desired configuration (`Release` or `Debug`) and platform (`Win32` [x86] or `x64`).
3. Build the solution.

The resulting executable will be in the folder `Build/<configuration>-<platform>/Elevate/bin/`.

Alternatively, you can build from the command line using MSBuild:
```cmd
msbuild Elevate\Elevate.vcxproj /p:Configuration=Release /p:Platform=x64
```
