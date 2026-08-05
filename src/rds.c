/*

This code follows the official RDS specifications (IEC 62106) maintained by the RDS Forum.
Note that this document is protected by a password, I cannot (and will not)
upload it here for copyright reasons. I recommend getting it yourself, if you are interested in RDS. More information can be found here:

https://www.rdsforum.org/2010/RDS-Specification.htm

*/


#include "rds.h"

void rds_get_datetime(const rds_group_t* g, rds_datetime_t* dt) {
  // Formulas from ETSI EN 300 468 Annex C Figure C.1
  // Also described in the RDS spec IEC 62106-2 Annex B


  // Date
  float MJD = ((g->block_b & 0x03) << 15) | (g->block_c >> 1);

  uint16_t Yi = (int)((MJD - 15078.2) / 365.25);
  uint16_t Mi = (int)((MJD - 14956.1 - (int)(Yi * 365.25)) / 30.6001);
  uint8_t K;
  if (Mi == 14 || Mi == 15) {K = 1;} else {K = 0;}

  dt->year = (Yi + K) % 100;
  dt->month = Mi - 1 - K * 12;
  dt->day = MJD - 14956 - (int)(Yi * 365.25) - (int)(Mi * 30.6001);


  // Time
  uint8_t utc_hour = ((g->block_c & 0x01) << 4) | ((g->block_d >> 12) & 0b1111);
  uint8_t utc_minute = (g->block_d >> 6) & 0b111111;

  int8_t local_offset = (g->block_d & 0b11111);
  bool offset_dir = (g->block_d >> 5) & 0x01;
  local_offset = (offset_dir) ? -local_offset : local_offset;

  dt->hour = (utc_hour + local_offset / 2);
  dt->minute = (utc_minute + local_offset*30) % 60;
}


bool rds_get_tp(const rds_group_t* g) {
  return (g->block_b >> 10) & 0x01;
}


uint8_t rds_get_pty(const rds_group_t* g) {
  return (g->block_b >> 5) & 0x1F;
}


uint8_t rds_get_group_type(const rds_group_t* g) {
  return (g->block_b >> 12) & 0x0F;
}


bool rds_get_group_variant(const rds_group_t* g) {
  return (g->block_b >> 11) & 0x01;
}
