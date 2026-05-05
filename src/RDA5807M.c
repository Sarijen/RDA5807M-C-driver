#include "RDA5807M.h"

static void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits);
static void reg_write_direct(rda5807m_t *handle, uint8_t reg_addr, uint16_t reg_data);

//////////////////////////////////////
// - FREQUENCY BANDS
//////////////////////////////////////

const rda5807m_freq_band_t rda5807m_band_50_65 = {
  .reg_bits = 0x03,
  .freq_start = 500,
  .freq_end = 650,
};

const rda5807m_freq_band_t rda5807m_band_65_76 = {
  .reg_bits = 0x03,
  .freq_start = 650,
  .freq_end = 760,
};

const rda5807m_freq_band_t rda5807m_band_76_91 = {
  .reg_bits = 0x01,
  .freq_start = 760,
  .freq_end = 910,
};

const rda5807m_freq_band_t rda5807m_band_76_108 = {
  .reg_bits = 0x02,
  .freq_start = 760,
  .freq_end = 1080,
};

const rda5807m_freq_band_t rda5807m_band_87_108 = {
  .reg_bits = 0x00,
  .freq_start = 870,
  .freq_end = 1080,
};


//////////////////////////////////////
// - CHANNEL SPACING
//////////////////////////////////////

const chan_spacing_t CHAN_SPACING_100 = {
  .reg_bits = 0x00,
  .value_khz = 100,
};

const chan_spacing_t CHAN_SPACING_200 = {
  .reg_bits = 0x01,
  .value_khz = 200,
};

const chan_spacing_t CHAN_SPACING_50 = {
  .reg_bits = 0x02,
  .value_khz = 50,
};

const chan_spacing_t CHAN_SPACING_25 = {
  .reg_bits = 0x03,
  .value_khz = 25,
};


//////////////////////////////////////
// - FUNCTION DEFINITIONS
//////////////////////////////////////

void rda5807m_set_frequency_band(rda5807m_t* handle, rda5807m_freq_band_t new_freq_band) {
  handle->current_freq_band = new_freq_band;

  reg_set_bits(&handle->reg_03H, REG_03H_BAND_SELECT_SHIFT, REG_03H_BAND_SELECT_MASK, new_freq_band.reg_bits);
  reg_write_direct(handle, 0x03, handle->reg_03H);


  bool uses_65m_50m_mode = (
    new_freq_band.freq_start == rda5807m_band_50_65.freq_start &&
    new_freq_band.freq_end == rda5807m_band_50_65.freq_end
  );

  reg_set_bits(&handle->reg_07H, REG_07H_65M_50M_MODE_SHIFT, REG_07H_65M_50M_MODE_MASK, uses_65m_50m_mode);
  reg_write_direct(handle, 0x07, handle->reg_07H);

}


void rda5807m_set_chan_spacing(rda5807m_t* handle, chan_spacing_t new_chan_spacing) {
  handle->current_chan_spacing = new_chan_spacing;

  reg_set_bits(&handle->reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, new_chan_spacing.reg_bits);
  reg_write_direct(handle, 0x03, handle->reg_03H);
}


void rda5807m_set_volume(rda5807m_t *handle, uint8_t volume_level) {
  reg_set_bits(&handle->reg_05H, REG_05H_VOLUME_SHIFT, REG_05H_VOLUME_MASK, volume_level);
  reg_write_direct(handle, 0x05, handle->reg_05H);
}


// Frequency uses fixed point representation
// For e.g. when tuning 82.1MHz, 821 has to be the argument
void rda5807m_tune_frequency(rda5807m_t *handle, uint16_t fm_frequency_mhz) {
  uint16_t band_start_mhz = 870; 
  uint16_t chan_spacing_khz = handle->current_chan_spacing.value_khz;

  uint16_t channel_number = ((fm_frequency_mhz - band_start_mhz) * chan_spacing_khz);
  channel_number /= 100;

  reg_set_bits(&handle->reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, channel_number);


  // Tune to apply the frequency
  reg_set_bits(&handle->reg_03H, REG_03H_TUNE_SHIFT, REG_03H_TUNE_MASK, 1);

  reg_write_direct(handle, 0x03, handle->reg_03H);
}


void rda5807m_init(rda5807m_t *handle) {
  // Set default values
  rda5807m_set_chan_spacing(handle, CHAN_SPACING_100);


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
