#include <EEPROM.h>


//======== EEPROM begin ===========
void writeSettingsESP(struct eeprom_data_t *eeprom_data);
void readSettingsESP(struct eeprom_data_t *eeprom_data);

#define EEPROM_START 0
boolean setEEPROM = false;

uint32_t memcrc; uint8_t *p_memcrc = (uint8_t*)&memcrc;

const PROGMEM prog_uint32_t crc_table[16] = {
  0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
  0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

void setupMemory(struct eeprom_data_t *eeprom_data) {
  readSettingsESP(eeprom_data);
}

boolean isWrited(struct eeprom_data_t *eeprom_data) {
  int i;
  uint32_t datacrc;
  byte eeprom_data_tmp[sizeof(eeprom_data)];
  EEPROM.begin(sizeof(eeprom_data) + sizeof(memcrc));

  for (i = EEPROM_START; i < sizeof(eeprom_data); i++) {
    eeprom_data_tmp[i] = EEPROM.read(i);
  }

  p_memcrc[0] = EEPROM.read(i++);
  p_memcrc[1] = EEPROM.read(i++);
  p_memcrc[2] = EEPROM.read(i++);
  p_memcrc[3] = EEPROM.read(i++);

  datacrc = crc_byte(eeprom_data_tmp, sizeof(eeprom_data_tmp));
  return memcrc == datacrc;
}
// ----------------------------------- readSettingsESP -----------------------------------
void readSettingsESP(struct eeprom_data_t *eeprom_data)
{
  byte eeprom_data_tmp[sizeof(eeprom_data)];
  if (isWrited(eeprom_data)) {
    setEEPROM = true;
    memcpy(&eeprom_data, eeprom_data_tmp,  sizeof(eeprom_data));
  }
  //  else {
  //    strncpy(eeprom_data.ssid, STA_SSID_DEFAULT, sizeof(STA_SSID_DEFAULT));
  //    strncpy(eeprom_data.pass, STA_PASSWORD_DEFAULT, sizeof(STA_PASSWORD_DEFAULT));
  //    strncpy(eeprom_data.from, from_email, sizeof(from_email));
  //    strncpy(eeprom_data.to, to_email, sizeof(to_email));
  //    strncpy(eeprom_data.smtphost, DEFAULT_SMTP_HOST, sizeof(DEFAULT_SMTP_HOST));
  //    strncpy(eeprom_data.to, to_email, sizeof(to_email));
  //    eeprom_data.hour = 25;
  //    eeprom_data.minutes = 0;
  //    eeprom_data.timezone = 2;
  //    eeprom_data.timeWatering = 15;
  //    eeprom_data.detect = true;
  //  }
}

// ----------------------------------- writeSettingsESP -----------------------------------
void writeSettingsESP(struct eeprom_data_t *eeprom_data) {
  int i;
  byte eeprom_data_tmp[sizeof(eeprom_data)];

  EEPROM.begin(sizeof(eeprom_data) + sizeof(memcrc));

  memcpy(eeprom_data_tmp, &eeprom_data, sizeof(eeprom_data));

  for (i = EEPROM_START; i < sizeof(eeprom_data); i++) {
    EEPROM.write(i, eeprom_data_tmp[i]);
  }
  memcrc = crc_byte(eeprom_data_tmp, sizeof(eeprom_data_tmp));

  EEPROM.write(i++, p_memcrc[0]);
  EEPROM.write(i++, p_memcrc[1]);
  EEPROM.write(i++, p_memcrc[2]);
  EEPROM.write(i++, p_memcrc[3]);
  EEPROM.commit();
}
// ----------------------------------- crc_update -----------------------------------
unsigned long crc_update(unsigned long crc, byte data)
{
  byte tbl_idx;
  tbl_idx = crc ^ (data >> (0 * 4));
  crc = pgm_read_dword_near(crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
  tbl_idx = crc ^ (data >> (1 * 4));
  crc = pgm_read_dword_near(crc_table + (tbl_idx & 0x0f)) ^ (crc >> 4);
  return crc;
}
// ----------------------------------- crc_byte -----------------------------------
unsigned long crc_byte(byte * b, int len)
{
  unsigned long crc = ~0L;
  uint8_t i;

  for (i = 0 ; i < len ; i++) {
    crc = crc_update(crc, *b++);
  }
  crc = ~crc;
  return crc;
}
//======== EEPROM End =============
