# STM32F103 series example code
## Setup
Download or make sure you have `cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib` installed.  

### Using STM32CubeMX
Versions listed were tested, other may work but not guaranteed  
- CubeMX version 6.17.0  
- Firmware package v1.8.7  

1. Open the `stm32f103.ioc` file using STM32CubeMX

2. Press `Generate code` in the top right corner

3. Proceed to [building](#building)


### Using Git
1. Clone the official STM32F1 repo using the provided command below
```
 git clone --recursive --depth 1 --branch v1.8.7 https://github.com/STMicroelectronics/STM32CubeF1
```
2. Copy the `STM32CubeF1/Drivers` directory to the `platform/stm32f103` directory

3. Proceed to [building](#building)


## Building
- Ensure you are in `/platform/stm32f103/`
```
cmake -B build --preset Debug

cd build && make -j$(nproc)
```

`stm32f103.elf` should now be located in the `build/` directory.

- `-j$(nproc)` distributes the compilation process into as many threads as the number of physical threads in your CPU. If this is not desired, you can omit it.
