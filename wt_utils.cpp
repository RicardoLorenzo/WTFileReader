#include "include/wt_utils.h"

int unpackUInt(const uint8_t **data, uint64_t *upck) {
  const uint8_t *p_data;

  p_data = *data;
  switch (*p_data & 0xF0) {
    case POS_1BYTE_MARKER:
    case POS_1BYTE_MARKER | 0x10:
    case POS_1BYTE_MARKER | 0x20:
    case POS_1BYTE_MARKER | 0x30:
      *upck = *p_data & 0x3F;
      p_data++;
      break;
    case POS_2BYTE_MARKER:
    case POS_2BYTE_MARKER | 0x10:
      *upck = (*p_data & 0x1F) << 8;
      p_data++;

      *upck |= *p_data;
      p_data++;

      *upck += POS_1BYTE_MAX + 1;
      break;
    case POS_MULTI_MARKER:
      uint64_t i;
      uint8_t len;
      len = (*p_data & 0xF);
      p_data++;

      for (i = 0; len != 0; --len) {
        i = (i << 8) | *p_data;
        p_data++;
      }
      *upck = i + POS_2BYTE_MAX + 1;
      break;
    default:
  	   return -1;
  }

  *data = p_data;
  return (0);
}
