# ActionNote
Integrated task/action tracking with note taking

#Usage
actionnote.exe to launch, actionnote.exe <16 byte encryption key> to run in encrypted mode

#Building
My CMake is lazy and assumes that a copy of tiny-AES-c (https://github.com/kokke/tiny-AES-c) is located at ../Common/tiny-AES-c relative to the root.

Once in place, run CMake. Only tested on Windows. Note - Google Test integration hasn't been done with CMake yet. Currently, to execute a separate solution is needed.