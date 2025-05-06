
# General Information

This is a basic VS Code sample project for the DSP course at H-KA in Karlsruhe, Germany.  
The original source code is provided by Prof. Dr.-Ing. Christian Langen.  
This sample project is for __Windows__ and __Linux__.
Find all documentation under "docs/". Some downloads are available under "downloads/". All [download links](#download-links) are listed below.



# Prerequisites for this sample project

## Windows

1. GCC
    - recommended release: 13.3 rel1
    - download from [website](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
        - section: "Windows (mingw-w64-i686) hosted cross toolchains"
        - subsection: "AArch32 bare-metal target (arm-none-eabi)"
        - file: "arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi.zip"
    - unzip and copy/move to recommended location: "C:/GCC/arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi/" 
    - if deviating, edit path in "{project_name}/.vscode/launch.json" and "{project_name}/cmake/toolchain_arm-none-eabi-gcc-13.3.Rel1.cmake"
1. OpenOCD 
    - recommended version: 0.12.0
    - download from [website](https://github.com/openocd-org/openocd/releases/tag/v0.12.0)
        - file: "openocd-v0.12.0-i686-w64-mingw32.tar.gz"
    - unzip and copy/move to recommended location: "C:/OpenOCD/"
    - if deviating, edit path in "{project_name}/.vscode/launch.json"
2. CMake
    - recommended version: 3.30.5, at least 3.20
    - download from [website](https://cmake.org/download/#latest)
        - file: "cmake-3.30.5-windows-x86_64.zip"
    - unzip and copy/move to recommended location: "C:/CMake/"
    - if deviating, edit path in "{project_name}/.vscode/settings.windows.json"
3. Ninja
    - if not installed yet, run `winget install Ninja-build.Ninja`
4. Install "CMSIS-DAP Driver.msi", see "downloads/FM4S6E2GMKitSetup_RevSB"
5. Check if board identifies as "FM-Link/CMSIS-DAP Cypress FM Communications Port" in Device Manager, if not see [Flash CMSIS-DAP FW on MB9AF312K](#flash-cmsis-dap-fw-on-mb9af312k)

>[!NOTE]
> all recommended versions have been tested, using different versions may result in errors

## Linux

The following is specifically for the apt package manager (tested with Ubuntu 22.04 LTS).  
Required packages are: GCC, OpenOCD, CMake and Ninja.
Run the following commands:

```
sudo apt update
sudo apt install openocd
sudo apt install cmake
sudo apt install ninja-build
sudo apt install libncursesw5
```

To install GCC:
- download from [website](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
    - section: "x86_64 Linux hosted cross toolchains"
    - subsection: "AArch32 bare-metal target (arm-none-eabi)"
    - file: "arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi.tar.xz"
- unzip and copy/move folder to "/usr/bin/" using `cp -R arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi /usr/bin/`
- if deviating, edit path in "{project_name}/.vscode/launch.json" and "{project_name}/cmake/toolchain_arm-none-eabi-gcc-13.3.Rel1.cmake"


Arch Linux, e.g.:
```
pacman -Ss openocd
pacman -Ss cmake
pacman -Ss ninja
pacman -Ss arm-none-eabi-gcc
pacman -Ss arm-none-eabi-newlib
```


Check for connected USB devices using `lsusb`.




# Getting started

- jumper configuration:
	- J1, J2 open
	- J4: 1-2
    - J3: 2-3 
- plug your board into your PC using the USB cable
- launch "hello_world.code-workspace" (with VS Code)
- install all recommended extensions, they will be displayed as a notification by VS Code in the bottom right
- go to "{project_name}/.vscode/"
  - depending on your OS choose between settings.windows.json or settings.linux.json
  - rename the corresponding file to settings.json
- switch to the CMake extension
  - at the same level of each tab to the right is the button to run the corresponding process
  - in each tab you can choose between different options by selecting the button next to it
  - in the _Configure_ tab, select DebugNoOpt as the option and run Configure afterwards
    - all temporary files for the build process are stored in the "{project_name}/build/" folder, which can be deleted anytime
  - in the _Build_ tab, select DebugNoOpt as the option and run Build afterwards
- switch to Run and Debug
  - select "GDB debugger CMSIS-DAP ({project_name})
  - clicking the green play button will flash the program and start the debugger
  - an automatic breakpoint will be set at main()
    - due to a limited number of breakpoints, it can be necessary to disable this
    - you can do so in "{project_name}/.vscode/launch.jason" by commenting the line: `"runToEntryPoint": "main"`,
  - press F5 or click the green button in the debugger window
- when playing audio to the input jack, the board should pass the audio undistorted to the output jack
- now you are ready to set up your own project


# Create your own project

- to use this sample project for your own project you have to make some adjustments
1. copy the entire sample project
2. rename the folder containing all files with your project name (referred to as {project_name})
3. go into the folder
4. delete "{project_name}/.git" (it might be a hidden folder)
5. rename the folder "hello_world" to "{project_name}"
6. rename "hello_world.code-workspace" to "{project_name}.code-workspace"
7. edit "{project_name}.code-workspace" with e.g. Notepad++
   - search for the "path" variable
   - replace "hello_world" with "{project_name}"
8. go into the folder {project_name}
9. step out of the current folder and launch {project_name}.code-workspace (with VS Code)
The following steps can be made within in VS Code
10. rename all "hello_world".cpp and "hello_world".h files with your {project_name} if necessary, if you do so, pay attention to the next step
11. every C/C++ source file (.cpp or .cc) needs to be listed in the `target_sources()` command inside the "CMakeLists.txt" file, see EOF (end of file)
12. open "CMakeLists.txt"
    - search for the `project()` command (line 21) and replace "hello_world" with {project_name}

>[!WARNING] editing CMake files
> after changing a CMake file, it is important to Reconfigure using the CMake extension

You are ready to start your own project!




# More information

- use `IF_DEBUG(debug_printf())` to send messages to the Serial Port Monitor
    - with `IF_DEBUG()`, `debug_printf()` is automatically disabled in Release build
    - `debug_printf()` can be used exactly as `printf()`
    - to view messages sent via `debug_printf()` use the "Serial Monitor" VS Code extension by Microsoft
- after compiling, a memory map is printed to the OUTPUT window of VS Code
  - it is absolutely fine for the "STACK %age Used" to be 100%
  - to increase the stack size, open and edit "{project_name}/device/device_config.h"




# How to create your own project

1. copy the entire project folder and rename it to your project
2. enter your project folder
3. open the project workspace by double-clicking "hello_world.code-workspace"
4. open the file "CMakeLists.txt" and replace "hello_world" with your project name in the command "project()" (should be line 16)
5. close VS Code
6. edit the folder "hello_world" and replace the name with your project name
7. open the file "hello_world.code-workspace" in an editor and change the value of the variable "path" to the same name of the folder from step 6
8. edit the file name "hello_world.code-workspace" and replace "hello_world" with your own project name
9. you are set to work on your project




# Flash CMSIS-DAP FW on MB9AF312K

1. Install "FLASH USB DIRECT Programmer.msi"
    - go to "downloads/FM4S6E2GMKitSetup_RevSB/"
    - run "FLASH USB DIRECT Programmer.msi"
1. find "mb9af31xk_cmsis_dap_v25.srec" in "downloads/MB9AF312K_FW/"
1. Set J4 = 1-2
1. Set J1 closed
1. Use USB CN2
1. Plug device in, check COM Port in Device Manager
    - if not present, run "USBVCOM Driver Installer.msi" from "downloads/FM4S6E2GMKitSetup_RevSB/"
1. Run USBDirect, select MB9AF312K, .srec file and COM port
1. click "Full Operation" (note: when reset required, unplug and replug USB and quickly press OK)
1. when done, unplug USB
1. Open J1
1. Plug device in again
1. Check Device Manager for "FM-Link/CMSIS-DAP Cypress FM Communications Port" device





# Flash J-Link FW on MB9AF312K

These instructions should not be needed, but are included for the sake of completeness.  
Refer to https://www.segger.com/products/debug-probes/j-link/models/other-j-links/j-link-ob-spansion/ for more information.
1. Install "USBVCOM Driver Installer.msi"
2. Install "FLASH USB DIRECT Programmer.msi"
    - go to "downloads/FM4S6E2GMKitSetup_RevSB/"
    - run the "FLASH USB DIRECT Programmer.msi"
3. Download "J-Link-OB-MB9AF312K-Spansion_150917.srec"
4. Set J4 = 1-2
5. Set J1 closed
6. Use USB CN2
7. Plug device in, check COM Port in Device Manager
8. Run USBDirect, select MB9AF312K, downloaded .srec file and COM port
9. click Full Operation (note: when reset required, unplug and replug USB and quickly press OK)
10. when done, unplug USB
11. Open J1
12. Plug device in again
13. Check Device Manager for J-Link device
14. Download latest "J-Link Software and Documentation pack" and install
15. manually install the drivers on J-Link and CDC device from C:\Program Files\SEGGER\JLink_V810c\USBDriver\x64
instructions: 
    - right click on device node in device manager, select "Update Driver"
    - "Browse...", "Let me pick...", "Have Disk", Select above dir

	


# How to use Semihosting

These instructions should not be needed, but are included for the sake of completeness.  
Refer to the file "downloads/FM4_FilterLab_1_3_Template/Eclipse_Application_Notes_Semihosting.pdf" for more information.
1. add following code in the file "hello_world/.vscode/launch.json" to the "GDB debugger CMSIS-DAP" configuration
    ```JSON
        "postLaunchCommands": [
            "monitor arm semihosting enable"
        ],
    ```
1. add following code before the main function
    ```C
    // needed for semihosting
    extern "C" void initialise_monitor_handles(void);
    ```
1. add following code before calling printf() for the first time
    ```C
    // needed for semihosting
    initialise_monitor_handles();
    ```
1. Modify the libraries included by the linker in "hello_world/ldscripts/s6e2cc_rom.ld"
    ```
    GROUP(libgcc.a libc.a librdimon.a libm.a libnosys.a)
    ```
1. use printf() to send messages via the debugger






# Download-Links

Check the "downloads/" folder of this sample project before downloading.

GCC toolchain:  
https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

OpenOCD:  
https://github.com/openocd-org/openocd/releases/tag/v0.12.0

CMake:  
https://cmake.org/download/#latest

CMSIS-DAP:  
https://www.infineon.com/cms/en/product/gated-document/cmsis-dap-firmware-update-8ac78c8c7d0d8da4017d0f8b794075f4/

Infineon FM4S6E2GMKitSetup_RevSB:  
https://www.infineon.com/cms/en/product/evaluation-boards/fm4-176l-s6e2gm/#!designsupport

J-Link-OB-MB9AF312K-Spansion:  
https://www.segger.com/products/debug-probes/j-link/models/other-j-links/j-link-ob-spansion/

J-Link Software and Documentation pack:  
https://www.segger.com/downloads/jlink/

CMSIS-DSP:  
https://github.com/ARM-software/CMSIS-DSP

CMSIS:  
https://github.com/ARM-software/CMSIS_6
