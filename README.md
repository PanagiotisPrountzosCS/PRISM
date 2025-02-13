# PRISM
PRISM(Predictive Real-time Industrial Sensor Monitor) is a C++ application which monitors 1-D signals received from arbitrary sensors and makes predictions for future measurements.
This software is part of my Thesis at School of Electrical and Computer Engineering @ National Technical University of Athens

# Installation
This project has been tested only on debian 12 so far, but the code does not use any OS specific libraries, so it should be easy to port to windows or other operating systems.
- Step 1 : Clone repo
```
git clone git@github.com:PanagiotisPrountzosCS/PRISM.git
cd PRISM
```
- Step 2 : Dependencies
```
sudo chmod +x install_dependencies.sh
sudo ./install_dependencies.sh
```
- Step 3 : Configure and build project
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug (for debug build)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release (for release build)
cmake --build build -j$(nproc)
```
