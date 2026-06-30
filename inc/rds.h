/*

This code follows the official RDS specifications from the RDS Forum.
Note that this document is protected by a password, I cannot (and it will not be
uploaded it here for copyright reasons. I recommend getting it yourself, if you are interested in RDS. More information can be found here:

https://www.rdsforum.org/2010/RDS-Specification.htm

*/

#ifndef RDS
#define RDS

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint16_t block_a;
  uint16_t block_b;
  uint16_t block_c;
  uint16_t block_d;

  uint8_t type;
  bool variant;
} rds_group_t;

uint8_t rds_get_group_type(const rds_group_t* g);
bool rds_get_group_variant(const rds_group_t* g);

#endif
