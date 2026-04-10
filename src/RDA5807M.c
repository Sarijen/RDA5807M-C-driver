#include "RDA5807M.h"

static void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits);
static void reg_write_direct(rda5807m_t *handle, uint8_t reg_addr, uint16_t reg_data);


// FIXED POINT REPRESENTATION
void rda5807m_tune_frequency(rda5807m_t *handle, uint16_t fm_frequency_mhz) {
  uint16_t band_start_mhz = 870; 
  uint16_t chan_spacing_khz = 100;

  uint16_t channel_number = ((fm_frequency_mhz - band_start_mhz) * chan_spacing_khz);
  channel_number /= 100;

  reg_set_bits(&handle->reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, channel_number);


  // Tune to apply the frequency
  reg_set_bits(&handle->reg_03H, REG_03H_TUNE_SHIFT, REG_03H_TUNE_MASK, 1);

  reg_write_direct(handle, 0x03, handle->reg_03H);
}


void rda5807m_init(rda5807m_t *handle) {
  // Enable audio output
  reg_set_bits(&handle->reg_02H, REG_02H_AUDIO_OUTPUT_SHIFT, REG_02H_AUDIO_OUTPUT_MASK, 1);

  // Disable mute
  reg_set_bits(&handle->reg_02H, REG_02H_MUTE_SHIFT, REG_02H_MUTE_MASK, 1);

  // Enable the IC
  reg_set_bits(&handle->reg_02H, REG_02H_ENABLE_SHIFT, REG_02H_ENABLE_MASK, 1);

  rda5807m_software_reset(handle);

  reg_write_direct(handle, 0x02, handle->reg_02H);
  handle->delay_ms(0.5); // 50us
}


void rda5807m_software_reset(rda5807m_t *handle) {
  reg_set_bits(&handle->reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 1);

  reg_write_direct(handle, 0x02, handle->reg_02H);
  handle->delay_ms(0.5); // 50us

  reg_set_bits(&handle->reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 0);
}


static void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits) {
  bits &= bit_mask; // Clamp

  *reg &= ~(bit_mask << bit_shift); // Clear

  *reg |= (bits << bit_shift); // Write
}


static void reg_write_direct(rda5807m_t *handle, uint8_t reg_addr, uint16_t reg_data) {
  uint8_t temp_buf[3] = {
    reg_addr,
    (uint8_t)(reg_data >> 8), // High byte
    (uint8_t)(reg_data & 0xFF), // Low byte
  };

  handle->i2c_write(RDA5807M_I2C_ADDR, temp_buf, sizeof(temp_buf));
}
