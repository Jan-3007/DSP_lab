
# General Information

This is a basic VS Code sample project for the DSP course at H-KA in Karlsruhe, Germany.  
This sample project is for Windows only. 
Find all documentation under "docs/". Most downloads should be available under "downloads/" and all [download links](#download-links) are listed below.



# Prerequisites for this sample project

1. Unpack GCC
    - recommended location: "C:/GCC" 
    - recommended release: 13.3 rel1
1. Unpack OpenOCD 
    - recommended location: "C:/OpenOCD"
1. Unpack CMake
    - recommended location: "C:/CMake"
1. Install "CMSIS-DAP Driver.msi", see "downloads/FM4S6E2GMKitSetup_RevSB"
1. Check if board identifies as "FM-Link/CMSIS-DAP Cypress FM Communications Port" in Device Manager, if not see [Flash CMSIS-DAP FW on MB9AF312K](#flash-cmsis-dap-fw-on-mb9af312k)
1. launch "hello_world.code-workspace"
    - all required extensions should load automatically





# Usage of the project

- add each required source file to the "target_sources" command in "CMakeLists.txt"
- set a breakpoint at the entry point of main before debugging
- select "GDB debugger CMSIS-DAP" in the built-in "Run and Debug" extension before debugging

Use the CMake extension to:
1. Configure/Reconfigure
1. Build the executable
1. Debug the program

- open the built-in Serial Monitor with the correct COM port before starting debugging or running the program to receive strings transferred via UART


 



# Flash CMSIS-DAP FW on MB9AF312K

1. Install "FLASH USB DIRECT Programmer.msi"
    - go to "downloads/FM4S6E2GMKitSetup_RevSB/"
    - run the "FLASH USB DIRECT Programmer.msi"
1. Download "mb9af31xk_cmsis_dap_v25.srec"
1. Set J4 = 1-2
1. Set J1 closed
1. Use USB CN2
1. Plug device in, check COM Port in Device Manager
1. Run USBDirect, select MB9AF312K, downloaded .srec file and COM port
1. click "Full Operation" (note: when reset required, unplug and replug USB and quickly press OK)
1. Unplug USB
1. Open J1
1. Plug device in again
1. Check Device Manager for "FM-Link/CMSIS-DAP Cypress FM Communications Port" device
1. manually install the drivers ...
    - right click on device node in device manager, select "Update Driver"
    - "Browse...", "Let me pick...", "Have Disk", Select above dir





# Flash J-Link FW on MB9AF312K

These instructions should not be needed, but are included for completeness.  
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
10. Unplug USB
11. Open J1
12. Plug device in again
13. Check Device Manager for J-Link device
14. Download latest "J-Link Software and Documentation pack" and install
15. manually install the drivers on J-Link and CDC device from C:\Program Files\SEGGER\JLink_V810c\USBDriver\x64
instructions: 
    - right click on device node in device manager, select "Update Driver"
    - "Browse...", "Let me pick...", "Have Disk", Select above dir

	









# Download-Links

check the "downloads/" folder of this sample project before downloading

GCC toolchain:  
https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads

OpenOCD:  
https://github.com/openocd-org/openocd/releases/tag/v0.12.0

CMSIS-DAP:  
https://www.infineon.com/cms/en/product/gated-document/cmsis-dap-firmware-update-8ac78c8c7d0d8da4017d0f8b794075f4/

J-Link-OB-MB9AF312K-Spansion:  
https://www.segger.com/products/debug-probes/j-link/models/other-j-links/j-link-ob-spansion/

J-Link Software and Documentation pack:  
https://www.segger.com/downloads/jlink/

CMakle
https://cmake.org/download/#latest
