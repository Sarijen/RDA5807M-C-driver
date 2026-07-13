#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "RDA5807M.h"

#define I2C0_FREQUENCY_KHZ 400
#define I2C0_SDA_PIN 0
#define I2C0_SCL_PIN 1

#define AUDIO_VOLUME 3

#define SEEK_UP 1 
#define SEEK_DOWN 0
#define SEEK_WRAP 0
#define SEEK_DONT_WRAP 1
#define SNR_THRESHOLD 16

void my_i2c0_init();
rda_status_t rda5807m_i2c_write(uint8_t i2c_addr, uint8_t* data, uint8_t length);
rda_status_t rda5807m_i2c_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t* buff, uint8_t length);
void global_delay_ms(uint8_t ms);


int main() {
  stdio_init_all();
  sleep_ms(1000);

  static rda5807m_t handle;
  handle.i2c_write = rda5807m_i2c_write;
  handle.i2c_read = rda5807m_i2c_read;
  handle.delay_ms = global_delay_ms;
  
  my_i2c0_init();

  rda_status_t r;
  r = rda5807m_init(&handle);
  if (r != RDA_OK) {
    printf("Init failed\n");
    return -1;
  }

  r = rda5807m_set_volume(&handle, AUDIO_VOLUME);
  if (r != RDA_OK) {
    printf("Failed to set volume!\n");
  }

  r = rda5807m_set_frequency_band(&handle, rda5807m_band_87_108);
  if (r != RDA_OK) {
    printf("Failed to set frequency band!\n");
  } else {
    rda5807m_tune_frequency(&handle, rda5807m_band_87_108.freq_start);
  }

  uint16_t tuned_frequency;
  bool is_station;
  uint8_t rssi;

  // Continously seek new stations every 10s
  sleep_ms(10);
  while (true) {
    r = rda5807m_seek(&handle, SEEK_UP, SEEK_WRAP, 1);
    if (r != RDA_OK) {
      printf("Failed to seek! Retrying...\n");
      sleep_ms(1000);
      continue;
    }

    sleep_ms(300);
    printf("========================\n");

    // Print basic information
    r = rda5807m_is_station(&handle, &is_station);
    if (r == RDA_OK) {
      if (is_station) {
        printf("Station found!\n");
      } else {
        printf("Not a station!\n");
      }
    } 

    r = rda5807m_get_frequency(&handle, &tuned_frequency);
    if (r != RDA_OK) {
      printf("Failed to get tuned frequency!\n");
    } else {
      printf("Tuned frequency: %d.%d MHz.\n", tuned_frequency / 10, tuned_frequency % 10);
    }

    r = rda5807m_get_rssi(&handle, &rssi);
    if (r != RDA_OK) {
      printf("Failed to get RSSI!\n");
    } else {
      printf("RSSI: %d\n", rssi);
    }

    sleep_ms(10*1000); // 10 seconds
  }

  return 0;
}


void my_i2c0_init() {
  i2c_init(i2c0, I2C0_FREQUENCY_KHZ*1000); 
  gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);

  gpio_pull_up(I2C0_SDA_PIN);
  gpio_pull_up(I2C0_SCL_PIN);
}


rda_status_t rda5807m_i2c_write(uint8_t i2c_addr, uint8_t* data, uint8_t length) {
  int result = i2c_write_blocking(i2c0, i2c_addr, data, length, false);

  if (result == length) {
    return RDA_OK;
  } else {
    return RDA_I2C_ERROR;
  }
}

rda_status_t rda5807m_i2c_read(uint8_t i2c_addr, uint8_t reg_addr, uint8_t* buff, uint8_t length) {
  i2c_write_blocking(i2c0, i2c_addr, &reg_addr, length, true);
  int result = i2c_read_blocking(i2c0, i2c_addr, buff, length, false);


  if (result == length) {
    return RDA_OK;
  } else {
    return RDA_I2C_ERROR;
  }
}

void global_delay_ms(uint8_t ms) {
  sleep_ms(ms);
}
