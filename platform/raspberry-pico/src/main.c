#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "RDA5807M.h"

#define I2C0_FREQUENCY_KHZ 100
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

#define EXAMPLE_TUNE_FREQUENCY 880

void my_i2c0_init();
rda_status_t rda5807m_i2c_write(uint8_t i2c_addr, uint8_t* data, uint8_t length);
void global_delay_ms(uint8_t ms);


int main() {
  static rda5807m_t handle;
  handle.i2c_write = rda5807m_i2c_write;
  handle.delay_ms = global_delay_ms;

  my_i2c0_init();
  rda5807m_init(&handle);
  rda5807m_tune_frequency(&handle, EXAMPLE_TUNE_FREQUENCY);

  return 0;
}


void my_i2c0_init() {
  i2c_init(i2c0, I2C0_FREQUENCY_KHZ*1000); 
  gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
}


rda_status_t rda5807m_i2c_write(uint8_t i2c_addr, uint8_t* data, uint8_t length) {
  int result = i2c_write_blocking(i2c0, i2c_addr, data, length, false);

  if (result == PICO_OK) {
    return RDA_OK;
  } else {
    return RDA_I2C_ERROR;
  }
}


void global_delay_ms(uint8_t ms) {
  sleep_ms(ms);
}
