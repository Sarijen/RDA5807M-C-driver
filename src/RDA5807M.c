#include "RDA5807M.h"

static rda_status_t validate_handle(const rda5807m_t* handle);
static void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits);
static void reg_get_bits(uint16_t reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t* buf);

static rda_status_t reg_write_direct(rda5807m_t* handle, uint8_t reg_addr, uint16_t reg_data);
static rda_status_t reg_read_direct(rda5807m_t* handle, uint8_t reg_addr, uint16_t* buff);


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


rda_status_t rda5807m_get_raw_rds(rda5807m_t* handle, rds_group_t* new_group) {
  if (new_group == NULL) {return RDA_ERR_INVALID_ARG;}
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}


  r = reg_read_direct(handle, 0x0C, &new_group->block_a);
  if (r != RDA_OK) {return r;}

  r = reg_read_direct(handle, 0x0D, &new_group->block_b);
  if (r != RDA_OK) {return r;}

  r = reg_read_direct(handle, 0x0E, &new_group->block_c);
  if (r != RDA_OK) {return r;}

  r = reg_read_direct(handle, 0x0F, &new_group->block_d);
  if (r != RDA_OK) {return r;}
  
  new_group->type = rds_get_group_type(new_group);
  new_group->variant = rds_get_group_variant(new_group);

  return r;
}


rda_status_t rda5807m_is_rds_ready(rda5807m_t* handle, bool* is_ready) {
  if (is_ready == NULL) {return RDA_ERR_INVALID_ARG;}
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  uint16_t temp_reg;
  uint16_t rds_ready_bit;

  r = reg_read_direct(handle, 0x0A, &temp_reg);
  if (r != RDA_OK) {return r;}

  reg_get_bits(temp_reg, REG_0AH_RDS_READY_SHIFT, REG_0AH_RDS_READY_MASK, &rds_ready_bit);

  *is_ready = rds_ready_bit;

  return r;
}


rda_status_t rda5807m_get_rssi(rda5807m_t* handle, uint8_t* rssi_value) {
  if (rssi_value == NULL) {return RDA_ERR_INVALID_ARG;}
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  uint16_t temp_reg;
  uint16_t rssi_bits;

  r = reg_read_direct(handle, 0x0B, &temp_reg);
  if (r != RDA_OK) {return r;}

  reg_get_bits(temp_reg, REG_0BH_RSSI_SHIFT, REG_0BH_RSSI_MASK, &rssi_bits);

  *rssi_value = (uint8_t)rssi_bits;

  return r;
}

rda_status_t rda5807m_is_station(rda5807m_t* handle, bool* is_station) {
  if (is_station == NULL) {return RDA_ERR_INVALID_ARG;}
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  uint16_t temp_reg;
  uint16_t station_bit;

  r = reg_read_direct(handle, 0x0B, &temp_reg);
  if (r != RDA_OK) {return r;}

  reg_get_bits(temp_reg, REG_0BH_IS_STATION_SHIFT, REG_0BH_IS_STATION_MASK, &station_bit);

  *is_station = station_bit;

  return r;
}


rda_status_t rda5807m_enable_rds(rda5807m_t* handle, bool enabled) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  reg_set_bits(&handle->reg_02H, REG_02H_RDS_EN_SHIFT, REG_02H_RDS_EN_MASK, enabled);
  r = reg_write_direct(handle, 0x02, handle->reg_02H);

  if (r == RDA_OK) {
    handle->rds_enabled = true;
  }

  return r;
}


rda_status_t rda5807m_mute_audio(rda5807m_t* handle, bool enabled) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  reg_set_bits(&handle->reg_02H, REG_02H_MUTE_SHIFT, REG_02H_MUTE_MASK, enabled);
  r = reg_write_direct(handle, 0x02, handle->reg_02H);

  return r;
}


rda_status_t rda5807m_enable_softmute(rda5807m_t* handle, bool enabled) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  reg_set_bits(&handle->reg_04H, REG_04H_SOFTMUTE_SHIFT, REG_04H_SOFTMUTE_MASK, enabled);
  r = reg_write_direct(handle, 0x04, handle->reg_04H);

  return r;
}


rda_status_t rda5807m_enable_mono(rda5807m_t* handle, bool enabled) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  reg_set_bits(&handle->reg_02H, REG_02H_MONO_SHIFT, REG_02H_MONO_MASK, enabled);
  r = reg_write_direct(handle, 0x02, handle->reg_02H);

  return r;
}


// Frequency uses fixed point representation
// For e.g. when tuning 82.1MHz, 821 has to be the argument
rda_status_t rda5807m_tune_frequency(rda5807m_t* handle, uint16_t new_frequency_mhz) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}


  uint16_t freq_band_start = handle->current_freq_band.freq_start;
  uint16_t freq_band_end = handle->current_freq_band.freq_end;
  uint16_t chan_spacing_khz = handle->current_chan_spacing.value_khz;


  if (new_frequency_mhz < freq_band_start) {
    new_frequency_mhz = freq_band_start;
  } else if (new_frequency_mhz > freq_band_end) {
    new_frequency_mhz = freq_band_end;
  }

  uint16_t channel_number = ((new_frequency_mhz - freq_band_start)*100) / chan_spacing_khz;

  reg_set_bits(&handle->reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, channel_number);


  // Tune to apply the frequency
  reg_set_bits(&handle->reg_03H, REG_03H_TUNE_SHIFT, REG_03H_TUNE_MASK, 1);
  r = reg_write_direct(handle, 0x03, handle->reg_03H);


  reg_set_bits(&handle->reg_03H, REG_03H_TUNE_SHIFT, REG_03H_TUNE_MASK, 0);
  if (r != RDA_OK) {return r;}


  handle->current_freq = new_frequency_mhz;

  return r;
}


rda_status_t rda5807m_set_frequency_band(rda5807m_t* handle, rda5807m_freq_band_t new_freq_band) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}


  reg_set_bits(&handle->reg_03H, REG_03H_BAND_SELECT_SHIFT, REG_03H_BAND_SELECT_MASK, new_freq_band.reg_bits);
  r = reg_write_direct(handle, 0x03, handle->reg_03H);
  if (r != RDA_OK) {return r;}

  bool uses_65m_50m_mode = (
    new_freq_band.freq_start == rda5807m_band_50_65.freq_start &&
    new_freq_band.freq_end == rda5807m_band_50_65.freq_end
  );


  reg_set_bits(&handle->reg_07H, REG_07H_65M_50M_MODE_SHIFT, REG_07H_65M_50M_MODE_MASK, !uses_65m_50m_mode);
  r = reg_write_direct(handle, 0x07, handle->reg_07H);
  if (r != RDA_OK) {return r;}

  handle->current_freq_band = new_freq_band;


  // Retune if current frequency is not within the new band frequency range
  if (handle->current_freq < new_freq_band.freq_start) {
    r = rda5807m_tune_frequency(handle, new_freq_band.freq_start);
  } else if (handle->current_freq > new_freq_band.freq_end) {
    r = rda5807m_tune_frequency(handle, new_freq_band.freq_end);
  }

  return r;
}


rda_status_t rda5807m_set_chan_spacing(rda5807m_t* handle, chan_spacing_t new_chan_spacing) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}


  reg_set_bits(&handle->reg_03H, REG_03H_CHAN_SELECT_SHIFT, REG_03H_CHAN_SELECT_MASK, new_chan_spacing.reg_bits);
  r = reg_write_direct(handle, 0x03, handle->reg_03H);
  if (r != RDA_OK) {return r;}

  handle->current_chan_spacing = new_chan_spacing;

  return r;
}


rda_status_t rda5807m_set_volume(rda5807m_t* handle, uint8_t volume_level) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK) {return r;}

  // 8th bit in register 0x05 has to be set, otherwise the audio output will be very noisy
  // when changing the volume state. There's no information about this in the public datasheet
  reg_set_bits(&handle->reg_05H, 7,    1,   1);
  //                     SHIFT MASK VALUE

  reg_set_bits(&handle->reg_05H, REG_05H_VOLUME_SHIFT, REG_05H_VOLUME_MASK, volume_level);
  r = reg_write_direct(handle, 0x05, handle->reg_05H);

  return r;
}


rda_status_t rda5807m_init(rda5807m_t* handle) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK && r != RDA_ERR_NOT_INITIALIZED) {return r;}


  // Enable audio output
  reg_set_bits(&handle->reg_02H, REG_02H_AUDIO_OUTPUT_SHIFT, REG_02H_AUDIO_OUTPUT_MASK, 1);

  // Disable mute
  reg_set_bits(&handle->reg_02H, REG_02H_MUTE_SHIFT, REG_02H_MUTE_MASK, 1);

  // Enable the IC
  reg_set_bits(&handle->reg_02H, REG_02H_ENABLE_SHIFT, REG_02H_ENABLE_MASK, 1);

  r = rda5807m_software_reset(handle);
  if (r != RDA_OK) {return r;}

  handle->initialized = true;

  // Set default values
  rda5807m_set_chan_spacing(handle, CHAN_SPACING_100);
  rda5807m_set_frequency_band(handle, rda5807m_band_76_108);

  handle->delay_ms(30);
  return r;
}


rda_status_t rda5807m_software_reset(rda5807m_t* handle) {
  rda_status_t r = validate_handle(handle);
  if (r != RDA_OK && r != RDA_ERR_NOT_INITIALIZED) {return r;}

  reg_set_bits(&handle->reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 1);
  r = reg_write_direct(handle, 0x02, handle->reg_02H);
  if (r != RDA_OK) {return r;}
  handle->delay_ms(1);


  reg_set_bits(&handle->reg_02H, REG_02H_RESET_SHIFT, REG_02H_RESET_MASK, 0);
  r = reg_write_direct(handle, 0x02, handle->reg_02H);
  handle->delay_ms(1);

  return r;
}


static rda_status_t validate_handle(const rda5807m_t* handle) {
  if (handle == NULL ||
    handle->i2c_write == NULL ||
    handle->i2c_read == NULL ||
    handle->delay_ms == NULL) {
    return RDA_ERR_INVALID_ARG;
  } else if (handle->initialized != true) {
    return RDA_ERR_NOT_INITIALIZED;
  } else {
    return RDA_OK;
  }
}


static void reg_get_bits(uint16_t reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t* buf) {
  uint16_t new_bits;

  new_bits = reg >> bit_shift; // Discard bits from right
  new_bits &= bit_mask; // Discard bits from left

  // Only bits left are those we want, so we're done
  *buf = new_bits;
}


static void reg_set_bits(uint16_t* reg, uint16_t bit_shift, uint16_t bit_mask, uint16_t bits) {
  bits &= bit_mask; // Clamp

  *reg &= ~(bit_mask << bit_shift); // Clear

  *reg |= (bits << bit_shift); // Write
}


static rda_status_t reg_read_direct(rda5807m_t* handle, uint8_t reg_addr, uint16_t* buff) {
  if (buff == NULL) {return RDA_ERR_INVALID_ARG;}

  rda_status_t r;
  uint8_t temp_buff[sizeof(uint16_t)];

  for (uint8_t attempt = 0; attempt < 3; attempt++) {
    r = handle->i2c_read(RDA5807M_I2C_ADDR, reg_addr, temp_buff, sizeof(temp_buff));

    if (r == RDA_OK) {
      *buff = ((uint16_t)temp_buff[0] << 8) | (temp_buff[1]);
      return r;
    }

    handle->delay_ms(1 + attempt);
  }

  return r;
}


static rda_status_t reg_write_direct(rda5807m_t* handle, uint8_t reg_addr, uint16_t reg_data) {
  uint8_t temp_buf[3] = {
    reg_addr,
    (uint8_t)(reg_data >> 8),   // High byte
    (uint8_t)(reg_data & 0xFF), // Low byte
  };

  rda_status_t r;

  for (uint8_t attempt = 0; attempt < 3; attempt++) {
    r = handle->i2c_write(RDA5807M_I2C_ADDR, temp_buf, sizeof(temp_buf));
    
    if (r == RDA_OK) {
      return r;
    }

    handle->delay_ms(1 + attempt);
  }

  return r;
}
