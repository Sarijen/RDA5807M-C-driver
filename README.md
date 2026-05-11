# About the IC
The RDA5807M is a basic CMOS FM stereo tuner with RDS/RBDS support. It offers wide frequency range of 50MHz - 115MHz. Customizable volume output, de-emphasis, channel spacing, mono/stereo and more. Internal registers can be read/written to using the 2-wire I²C protocol.

- Supply voltage: 2.7 - 3.3V
- Supply current: ≈20mA
- Powerdown current: ≈15μA
- I²C SCL Frequency: 100 - 400kHz
- Max R/L Audio Output: 360mVpp 
- THD: ≈0.2 %
- More details in the [datasheet](docs/RDA5807M_datasheet.pdf)

## Getting started
### Premade example code
1. `git clone` this repository
2. Choose your preferred MCU in the [platform](platform/) directory
3. Follow the `README.md` instructions in the root directory of the selected MCU

### As a static library
#### Installation (CMake)
1. `git clone` this repo inside your project

2. Add `add_subdirectory(RDA5807M-C-driver)` to your CMakeLists.txt 

3. Add `RDA5807M` keyword to your `target_link_libraries`

#### Usage

This example shows tuning 88.0 MHz on the STM32F103

```c
#include "RDA5807M.h"

// Frequencies use fixed point representation ( FREQ_FLOAT*10 )
#define EXAMPLE_TUNE_FREQUENCY 880

void my_i2c1_write(uint8_t i2c_addr, uint8_t* data, uint8_t length) {
  HAL_I2C_Master_Transmit(&hi2c1, i2c_addr, data, length, HAL_MAX_DELAY);
}


void my_delay_ms(uint8_t ms) {
  HAL_Delay(ms);
}


int main(void) {
  static rda5807m_t handle;
  handle.i2c_write = my_i2c1_write;
  handle.delay_ms = my_delay_ms;

  rda5807m_init(&handle);
  rda5807m_tune_frequency(&handle, EXAMPLE_TUNE_FREQUENCY);

  return 0;
}
```

- Working examples for specific MCU platforms can be found in the [platform](platform/) directory
