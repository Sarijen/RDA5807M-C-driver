/*

This code follows the official RDS specifications (IEC 62106) maintained by the RDS Forum.
Note that this document is protected by a password, I cannot (and will not)
upload it here for copyright reasons. I recommend getting it yourself, if you are interested in RDS. More information can be found here:

https://www.rdsforum.org/2010/RDS-Specification.htm

*/

#ifndef RDS_H
#define RDS_H

#include <stdint.h>
#include <stdbool.h>

#define GROUP_A 0
#define GROUP_B 1

typedef struct {
  uint16_t block_a;
  uint16_t block_b;
  uint16_t block_c;
  uint16_t block_d;

  uint8_t type;
  bool variant;
} rds_group_t;


typedef struct {
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
} rds_datetime_t;

void rds_get_datetime(const rds_group_t* g, rds_datetime_t* dt);

uint8_t rds_get_group_type(const rds_group_t* g);
bool rds_get_group_variant(const rds_group_t* g);


#endif
