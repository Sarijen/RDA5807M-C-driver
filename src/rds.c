/*

This code follows the official RDS specifications (IEC 62106) maintained by the RDS Forum.
Note that this document is protected by a password, I cannot (and will not)
upload it here for copyright reasons. I recommend getting it yourself, if you are interested in RDS. More information can be found here:

https://www.rdsforum.org/2010/RDS-Specification.htm

*/


#include "rds.h"


bool rds_get_ps(const rds_group_t* g, rds_ps_t* ps) {
  bool text_updated = false;

  uint8_t seg = (g->block_b & 0x03) << 1;

  char new_chars[2] = {
    g->block_d >> 8,
    g->block_d & 0xFF,
  };


// Every char has to be received twice to get written to the final string
// Skip if the same char already was written
  for (uint8_t ch = 0; ch < 2; ch++) {
    if (new_chars[ch] == ps->str_buff[seg + ch] &&
        ps->str[seg + ch] != ps->str_buff[seg + ch]) {

      text_updated = true;
      ps->str[seg + ch] = ps->str_buff[seg + ch];
    } else {
      ps->str_buff[seg + ch] = new_chars[ch];
    }
  }

  if (text_updated) {
    ps->str[8] = '\0';
  }
  
  return text_updated;
}


bool rds_get_radiotext(const rds_group_t* g, rds_radiotext_t* rt) {
  bool text_updated = false;
  bool ab_flag_new = (g->block_b >> 4) & 0x01;

  if (rt->ab_flag != ab_flag_new) {
    rt->ab_flag = ab_flag_new;

    memset(rt->str_buff, ' ', RT_MAX_STRING_SIZE);
    memset(rt->str, ' ', RT_MAX_STRING_SIZE);
    return text_updated;
  }


  if (g->variant == GROUP_A) { // GROUP A (4 chars)
    uint8_t seg = (g->block_b & 0x0F) << 2;

    char new_chars[4] = {
      g->block_c >> 8,
      g->block_c & 0xFF,
      g->block_d >> 8,
      g->block_d & 0xFF,
    };


// Every char has to be received twice to get written to the final string
// Skip if the same char already was written
    for (uint8_t ch = 0; ch < 4; ch++) {
      if (new_chars[ch] == rt->str_buff[seg + ch] &&
          rt->str[seg + ch] != rt->str_buff[seg + ch]) {

        text_updated = true;
        rt->str[seg + ch] = rt->str_buff[seg + ch];
      } else {
        rt->str_buff[seg + ch] = new_chars[ch];
      }
    }

  } else { // GROUP B (2 chars)
// TO-DO
// Data is wrong every time for some reason
  }


  if (text_updated) {
    rt->str[RT_MAX_STRING_SIZE] = '\0';
  }

  return text_updated;
}


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


PTY rds_get_pty(const rds_group_t* g) {
  return (g->block_b >> 5) & 0x1F;
}


uint8_t rds_get_group_type(const rds_group_t* g) {
  return (g->block_b >> 12) & 0x0F;
}


bool rds_get_group_variant(const rds_group_t* g) {
  return (g->block_b >> 11) & 0x01;
}
