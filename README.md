# C.A.V.O. <span style="font-size: 16px;"> (**C**hroma with **A**rtificial **V**ision using **O**penCV)</span>

## Requisites
- OpenCV 4.5.2
- Qt SDK 5.15.2
- CMake 3.16.3
- Visual Studio Code (Optional)

## Pre-Build
Make sure that you have already installed OpenCV, Qt SDK, CMake and have environment variables added.

### OpenCV-Linux

```bash
export OpenCV_DIR="/path/to/opencv/install/lib/cmake/opencv4"
```

### OpenCV-Windows

Using PowerShell:

```ps
$env:Path += ";C:\path\to\opencv\install\x64\vc16\bin"
$env:Path += ";C:\path\to\opencv\install\x64\vc16\lib"
```

You can also use the GUI.

## To Build with Visual Studio Code

1. Install these extensions to build the application with Visual Studio Code:

    - CMake
    - CMake Tools
    - C/C++ (Optional)

2. Configure CMake to do a Release type.

3. Press `CTRL+Shift+P` to configure the project.

4. You have two parameters to optional change of in `header_files/main_cavo_gui.hpp`:
    - To use or not 2 threads (Linux OS recommended)
        ```c++
        bool using2Threads = false;
        ```
    - To choose a camera number. Default laptop camera is `0`.
        ```c++ 
        int cameraNumber = 0;
        ```

5. Now press **build**. Wait for it and click **run** icon to show the GUI.

For more information visit the following website [C.A.V.O.](https://sites.google.com/view/cavo-project/home "Chroma with Artificial Vision Using OpenCV")