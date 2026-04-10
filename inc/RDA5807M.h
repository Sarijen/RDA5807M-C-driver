#ifndef RDA5807M
#define RDA5807M

#include "stdint.h"

typedef struct {
  void (*i2c_write)(uint8_t, uint8_t*, uint8_t); 
  void (*delay_ms)(uint8_t);

  uint16_t reg_02H;
  uint16_t reg_03H;
  uint16_t reg_04H;
  uint16_t reg_05H;
} rda5807m_t;

void rda5807m_init(rda5807m_t *handle);
void rda5807m_software_reset(rda5807m_t *handle);
void rda5807m_tune_frequency(rda5807m_t *handle, uint16_t fm_frequency_mhz);

#define RDA5807M_I2C_ADDR (0x11 << 1)

// ==== ==== ==== Register bit SHIFTS ==== ==== ====

// Register 0x02
#define REG_02H_ENABLE_SHIFT 0
#define REG_02H_RESET_SHIFT 1
#define REG_02H_NEW_METHOD_SHIFT 2
#define REG_02H_RDS_EN_SHIFT 3
#define REG_02H_CLK_MODE_SHIFT 4
#define REG_02H_SK_MODE_SHIFT 7
#define REG_02H_SEEK_EN_SHIFT 8
#define REG_02H_SEEK_DIR_SHIFT 9
#define REG_02H_RCLK_MODE_SHIFT 11
#define REG_02H_BASS_BOOST_SHIFT 12
#define REG_02H_MONO_SHIFT 13
#define REG_02H_MUTE_SHIFT 14
#define REG_02H_AUDIO_OUTPUT_SHIFT 15

// Register 0x03
#define REG_03H_CHAN_SPACING_SHIFT 0
#define REG_03H_BAND_SELECT_SHIFT  2
#define REG_03H_TUNE_SHIFT  4
#define REG_03H_CHAN_SELECT_SHIFT  6

// Register 0x04
#define REG_04H_AFCD_SHIFT  8
#define REG_04H_SOFTMUTE_SHIFT  9
#define REG_04H_DE_EMPHASIS_SHIFT  11

// Register 0x05
#define REG_05H_VOLUME_SHIFT  0
#define REG_05H_SNR_THRESHOLD_SHIFT  8
#define REG_05H_INTERRUPT_SHIFT  15

// Register 0x07 (WRITE)
#define REG_07H_FREQ_MODE_SHIFT  0
#define REG_07H_SOFTBLEND_SHIFT  1
#define REG_07H_SEEK_MODE_SHIFT  2
#define REG_07H_65M_50M_MODE_SHIFT  9
#define REG_07H_SOFTBLEND_TRESHOLD_SHIFT  10


// Register 0x0A (READ)
#define REG_0AH_CHAN_READ_SHIFT  0
#define REG_0AH_STEREO_SHIFT  10
#define REG_0AH_RDS_BLK_E_SHIFT  11
#define REG_0AH_RDS_SYNC_SHIFT  12
#define REG_0AH_SEEK_FAIL_SHIFT  13
#define REG_0AH_SEEK_DONE_SHIFT  14
#define REG_0AH_RDS_READY_SHIFT  15

// Register 0x0B (READ)
#define REG_0BH_BLKB_ERR_SHIFT  0
#define REG_0BH_BLKA_ERR_SHIFT  2
#define REG_0BH_ABCD_E_SHIFT  4
#define REG_0BH_FM_READY_SHIFT  7
#define REG_0BH_IS_STATION_SHIFT  8
#define REG_0BH_RSSI_SHIFT  9


// ==== ==== ==== Register bit MASKS ==== ==== ====

// Register 0x02
#define REG_02H_ENABLE_MASK  0x01
#define REG_02H_RESET_MASK  0x01
#define REG_02H_NEW_METHOD_MASK  0x01
#define REG_02H_RDS_EN_MASK  0x01
#define REG_02H_CLK_MODE_MASK  0x07
#define REG_02H_SK_MODE_MASK  0x01
#define REG_02H_SEEK_EN_MASK  0x01
#define REG_02H_SEEK_DIR_MASK  0x01
#define REG_02H_RCLK_MODE_MASK  0x01
#define REG_02H_BASS_BOOST_MASK  0x01
#define REG_02H_MONO_MASK  0x01
#define REG_02H_MUTE_MASK  0x01
#define REG_02H_AUDIO_OUTPUT_MASK  0x01

// Register 0x03
#define REG_03H_CHAN_SPACING_MASK  0x07
#define REG_03H_BAND_SELECT_MASK  0x07
#define REG_03H_TUNE_MASK  0x01
#define REG_03H_CHAN_SELECT_MASK  0x3FF

// Register 0x04
#define REG_04H_AFCD_MASK  0x01
#define REG_04H_SOFTMUTE_MASK  0x01
#define REG_04H_DE_EMPHASIS_MASK  0x01

// Register 0x05
#define REG_05H_VOLUME_MASK  0x0F
#define REG_05H_SNR_THRESHOLD_MASK  0x0F
#define REG_05H_INTERRUPT_MASK  0x01

// Register 0x07 (WRITE)
#define REG_07H_FREQ_MODE  0x01
#define REG_07H_SOFTBLEND_MASK  0x01
#define REG_07H_SEEK_MODE_MASK  0x06
#define REG_07H_65M_50M_MODE_MASK  0x01
#define REG_07H_SOFTBLEND_TRESHOLD_MASK  0x05

// Register 0x0A (READ)
#define REG_0AH_CHAN_READ_MASK  0x3FF
#define REG_0AH_STEREO_MASK  0x01
#define REG_0AH_RDS_BLK_E_MASK  0x01
#define REG_0AH_RDS_SYNC_MASK  0x01
#define REG_0AH_SEEK_FAIL_MASK  0x01
#define REG_0AH_SEEK_DONE_MASK  0x01
#define REG_0AH_RDS_READY_MASK  0x01

// Register 0x0B (READ)
#define REG_0BH_BLKB_ERR_MASK  0x03
#define REG_0BH_BLKA_ERR_MASK  0x03
#define REG_0BH_ABCD_E_MASK  0x01
#define REG_0BH_FM_READY_MASK  0x01
#define REG_0BH_IS_STATION_MASK  0x01
#define REG_0BH_RSSI_MASK  0x77

#endif
