#include "RDA5807M.h"

static uint16_t reg_02H = 0x0000;
static uint16_t reg_03H = 0x0000;
static uint16_t reg_04H = 0x0000;
static uint16_t reg_05H = 0x0000;

void (*rda5807m_i2c_write)(uint8_t, uint8_t*, uint8_t);
void (*rda5807m_delay_ms)(uint8_t);

static void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits);
static void reg_write_direct(uint8_t reg_addr, uint16_t reg_data);


// FIXED POINT REPRESENTATION
void rda5807m_tune_frequency(uint16_t fm_frequency_mhz) {
  uint16_t band_start_mhz = 870; 
  uint16_t chan_spacing_khz = 100;

  uint16_t channel_number = ((fm_frequency_mhz - band_start_mhz) * chan_spacing_khz);
  channel_number /= 100;

  reg_set_bits(&reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, channel_number);


  // Tune to apply the frequency
  reg_set_bits(&reg_03H, REG_03H_TUNE_SHIFT, REG_03H_TUNE_MASK, 1);

  reg_write_direct(0x03, reg_03H);
}


void rda5807m_init() {
  // Enable audio output
  reg_set_bits(&reg_02H, REG_02H_AUDIO_OUTPUT_SHIFT, REG_02H_AUDIO_OUTPUT_MASK, 1);

  // Disable mute
  reg_set_bits(&reg_02H, REG_02H_MUTE_SHIFT, REG_02H_MUTE_MASK, 1);

  // Enable the IC
  reg_set_bits(&reg_02H, REG_02H_ENABLE_SHIFT, REG_02H_ENABLE_MASK, 1);

  rda5807m_software_reset();

  reg_write_direct(0x02, reg_02H);
  rda5807m_delay_ms(0.5); // 50us
}


void rda5807m_software_reset() {
  reg_set_bits(&reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 1);

  reg_write_direct(0x02, reg_02H);
  rda5807m_delay_ms(0.5); // 50us

  reg_set_bits(&reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 0);
}


static void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits) {
  bits &= bit_mask; // Clamp

  *reg &= ~(bit_mask << bit_shift); // Clear

  *reg |= (bits << bit_shift); // Write
}


static void reg_write_direct(uint8_t reg_addr, uint16_t reg_data) {
  uint8_t temp_buf[3] = {
    reg_addr,
    (uint8_t)(reg_data >> 8), // High byte
    (uint8_t)(reg_data & 0xFF), // Low byte
  };

  rda5807m_i2c_write(RDA5807M_I2C_ADDR, temp_buf, sizeof(temp_buf));
}
