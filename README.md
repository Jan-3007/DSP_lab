
# General Information

This is a basic VS Code sample project for the DSP course at H-KA in Karlsruhe, Germany.  
The source code in "system/" and "ldscripts/" is provided by Prof. Dr.-Ing. Christian Langen.  
This sample project is __for Windows__. When using Linux (not tested yet), the paths need to be set.  
Find all documentation under "docs/". Most downloads should be available under "downloads/". All [download links](#download-links) are listed below.



# Prerequisites for this sample project

## Windows

1. GCC
    - recommended release: 13.3 rel1
    - download from website, select: Windows, AArch32 bare-metal target (arm-none-eabi)
    - unzip and copy/move to recommended location: "C:/GCC/arm-gnu-toolchain-13.3.rel1/" 
    - if deviating, edit path in ".vscode/launch.json" and "toolchain_arm-none-eabi-gcc-13.3.Rel1.cmake"
1. OpenOCD 
    - download from website
    - unzip and copy/move to recommended location: "C:/OpenOCD/"
    - if deviating, edit path in ".vscode/launch.json"
1. CMake
    - recommended version: at least 3.20
    - download from website
    - unzip and copy/move to recommended location: "C:/CMake/"
    - if deviating, edit path in VS Code Settings: Cmake Path = "C:/CMake/bin/cmake.exe"
1. Ninja
    - if not installed yet, run `winget install Ninja-build.Ninja`
1. Install "CMSIS-DAP Driver.msi", see "downloads/FM4S6E2GMKitSetup_RevSB"
1. Check if board identifies as "FM-Link/CMSIS-DAP Cypress FM Communications Port" in Device Manager, if not see [Flash CMSIS-DAP FW on MB9AF312K](#flash-cmsis-dap-fw-on-mb9af312k)


## Linux

The following is specifically for the apt package manager (tested with Ubuntu 22.04 LTS).  
Required packages are: GCC, OpenOCD, CMake and Ninja.
Run the following commands:

```
sudo apt update
sudo apt install openocd
sudo apt install cmake
sudo apt install ninja-build
```

To install GCC:
- download from website, select: x86_64 Linux, AArch32 bare-metal target (arm-none-eabi)
- unzip and copy/move folder to "/opt/" using `cp -R /opt/`
- if deviating, edit path in ".vscode/launch.json" and "toolchain_arm-none-eabi-gcc-13.3.Rel1.cmake"






# Usage of the project

- launch "hello_world.code-workspace"
- all required extensions should load automatically
- use "IF_DEBUG(debug_printf())" to send messages to the Serial Port Monitor
    - with "IF_DEBUG()", debug_printf() is automatically disabled in Release build
    - debug_printf() works exactly as printf()
- add each required source file to the "target_sources" commands in "CMakeLists.txt"
- Configure/Reconfigure and build the executable using the CMake extension
- select "GDB debugger CMSIS-DAP" in the "Run and Debug" extension before debugging
- start debugging with the "Run and Debug" extension.
- open the VS Code built-in Serial Port Monitor with the correct COM port to receive messages sent via debug_printf()


 



# Flash CMSIS-DAP FW on MB9AF312K

1. Install "FLASH USB DIRECT Programmer.msi"
    - go to "downloads/FM4S6E2GMKitSetup_RevSB/"
    - run "FLASH USB DIRECT Programmer.msi"
1. find "mb9af31xk_cmsis_dap_v25.srec" in "downloads/MB9AF312K_FW/"
1. Set J4 = 1-2
1. Set J1 closed
1. Use USB CN2
1. Plug device in, check COM Port in Device Manager
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

check the "downloads/" folder of this sample project before downloading.

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

