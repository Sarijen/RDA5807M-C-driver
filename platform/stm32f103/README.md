# STM32F103 series example code
## Setup
### STM32CUBEMX

### Git repo
```
 git clone --depth 1 https://github.com/STMicroelectronics/STM32CubeF

```

2. Download or make sure you have `cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib` installed.  



## Building
- Ensure you are in `/platform/stm32f103/`
```
cmake -B build --preset Debug

cd build && make -j$(nproc)
```

`stm32f103.elf` should now be located in the `build/` directory.

- `-j$(nproc)` distributes the compilation process into as many threads as the number of physical threads in your CPU. If this is not desired, you can omit it.
