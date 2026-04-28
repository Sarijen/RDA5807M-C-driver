# Raspberry Pi Pico example code
## Setup
1. Make sure you have [pico SDK](https://github.com/raspberrypi/pico-sdk) installed and working on your machine (git submodules are not required).

2. Download or make sure you have `cmake python3 build-essential gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib` installed.  

3. Copy the `pico-sdk/external/pico_sdk_import.cmake` file to `/platform/raspberry-pico/`


## Building
- Ensure you are in `/platform/raspberry-pico/`
```
cmake -B build -DPICO_SDK_PATH="path/to/your/pico-sdk" .

cd build && make -j$(nproc)
```

`ST7567_pico_example.uf2` should now be located in the `build/` directory.

- `-j$(nproc)` distributes the compilation process into as many threads as the number of physical threads in your CPU. If this is not desired, you can omit it.

- Sometimes building throws linker errors, removing the `build/` directory and starting over usually fixes this.
