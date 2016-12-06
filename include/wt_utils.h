#ifndef WT_UTILS_H_
#define WT_UTILS_H_

#include <stdint.h>

#define POS_1BYTE_MARKER (uint8_t)0x80
#define POS_2BYTE_MARKER (uint8_t)0xc0
#define POS_MULTI_MARKER (uint8_t)0xe0

#define POS_1BYTE_MAX ((1 << 6) - 1)
#define POS_2BYTE_MAX ((1 << 13) + POS_1BYTE_MAX)

int unpackUInt(const uint8_t** p_data, uint64_t* xp);

class WTUtils {
public:
    static int unpackUInt(const uint8_t** data, uint64_t* upck);
};

#endif  // WT_UTILS_H_
