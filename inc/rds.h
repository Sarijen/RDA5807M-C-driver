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
#include <string.h>

#define GROUP_A 0
#define GROUP_B 1

#define RT_MAX_STRING_SIZE 64

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

typedef struct {
  char str[RT_MAX_STRING_SIZE + 1];
  char str_buff[RT_MAX_STRING_SIZE + 1];
  bool ab_flag;
} rds_radiotext_t;


typedef enum {
  PTY_UNDEFINED = 0,
  NEWS,
  CURRENT_AFFAIRS,
  INFORMATION,
  SPORT,
  EDUCATION,
  DRAMA,
  CULTURE,
  SCIENCEC,
  VARIED,
  POP_MUSIC,
  ROCK_MUSIC,
  EASY_LISTENING_MUSIC,
  LIGHT_CLASSICAL_MUSIC,
  SERIOUS_CLASSICAL_MUSIC,
  OTHER_MUSIC,
  WEATHER,
  FINANCE,
  CHILDRENS_PROGRAMMES,
  SOCIAL_AFFAIRS,
  RELIGION,
  PHONE_IN,
  TRAVEL,
  LEISURE,
  JAZZ_MUSIC,
  COUNTRY_MUSIC,
  NATIONAL_MUSIC,
  OLDIES_MUSIC,
  FOLK_MUSIC,
  DOCUMENTARY,
  ALARM_TEST,
  ALARM
} PTY;


bool rds_get_radiotext(const rds_group_t* g, rds_radiotext_t* rt);
void rds_get_datetime(const rds_group_t* g, rds_datetime_t* dt);
bool rds_get_tp(const rds_group_t* g);
PTY rds_get_pty(const rds_group_t* g);

uint8_t rds_get_group_type(const rds_group_t* g);
bool rds_get_group_variant(const rds_group_t* g);

#endif
