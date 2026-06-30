#include "rds.h"

uint8_t rds_get_group_type(const rds_group_t* g) {
  return (g->block_b >> 12) & 0x0F;
}

bool rds_get_group_variant(const rds_group_t* g) {
  return (g->block_b >> 11) & 0x01;
}
