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
uint8_t rds_get_pty(const rds_group_t* g);

uint8_t rds_get_group_type(const rds_group_t* g);
bool rds_get_group_variant(const rds_group_t* g);


// PTY DESCRIPTIONS
#define PTY_UNDEFINED 0
#define NEWS 1
#define CURRENT_AFFAIRS 2
#define INFORMATION 3
#define SPORT 4
#define EDUCATION 5
#define DRAMA 6
#define CULTURE 7
#define SCIENCEC 8
#define VARIED 9
#define POP_MUSIC 10
#define ROCK_MUSIC 11
#define EASY_LISTENING_MUSIC 12
#define LIGHT_CLASSICAL_MUSIC 13
#define SERIOUS_CLASSICAL_MUSIC 14
#define OTHER_MUSIC 15
#define WEATHER 16
#define FINANCE 17
#define CHILDRENS_PROGRAMMES 18
#define SOCIAL_AFFAIRS 19
#define RELIGION 20
#define PHONE_IN 21
#define TRAVEL 22
#define LEISURE 23
#define JAZZ_MUSIC 24
#define COUNTRY_MUSIC 25
#define NATIONAL_MUSIC 26
#define OLDIES_MUSIC 27
#define FOLK_MUSIC 28
#define DOCUMENTARY 29
#define ALARM_TEST 30
#define ALARM 31


#endif
