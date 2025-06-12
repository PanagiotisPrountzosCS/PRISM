# PRISM (Predictive realtime industrial sensor monitor)

## This project implements a simple live sensor monitoring system using mqtt for data transmission. It is used in conjunction with DATAFLUX, for the sensor nodes.

# Installation

To install PRISM, simply run the following commands

```
git clone https://github.com/PanagiotisPrountzosCS/PRISM
cd PRISM
git submodule update --recursive --init
chmod +x install_dependencies.sh
./install_dependencies.sh

cmake -S . -B build
cmake --build build -j$(nproc)
```

The executables will be placed in the build directory
