#ifndef BIT_UTILS_H
#define BIT_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// ---------- uint8_t 位掩码 ----------
#define BIT_0   (1U << 0)
#define BIT_1   (1U << 1)
#define BIT_2   (1U << 2)
#define BIT_3   (1U << 3)
#define BIT_4   (1U << 4)
#define BIT_5   (1U << 5)
#define BIT_6   (1U << 6)
#define BIT_7   (1U << 7)

// ---------- uint16_t 位掩码 ----------
#define BIT_8    (1U << 8)
#define BIT_9    (1U << 9)
#define BIT_10   (1U << 10)
#define BIT_11   (1U << 11)
#define BIT_12   (1U << 12)
#define BIT_13   (1U << 13)
#define BIT_14   (1U << 14)
#define BIT_15   (1U << 15)

// ---- uint8_t 操作 ----
static inline void set_bit_u8(uint8_t* var, uint8_t mask) {
    *var |= mask;
}

static inline void clear_bit_u8(uint8_t* var, uint8_t mask) {
    *var &= ~mask;
}

static inline void toggle_bit_u8(uint8_t* var, uint8_t mask) {
    *var ^= mask;
}

static inline bool is_bit_set_u8(uint8_t var, uint8_t mask) {
    return (var & mask) != 0;
}

// ---- uint16_t 操作 ----
static inline void set_bit_u16(uint16_t* var, uint16_t mask) {
    *var |= mask;
}

static inline void clear_bit_u16(uint16_t* var, uint16_t mask) {
    *var &= ~mask;
}

static inline void toggle_bit_u16(uint16_t* var, uint16_t mask) {
    *var ^= mask;
}

static inline bool is_bit_set_u16(uint16_t var, uint16_t mask) {
    return (var & mask) != 0;
}

// ---- 特定：检查 uint16_t 的第6和第7位是否都为1 ----
static inline bool is_bit6_and_bit7_set_u16(uint16_t var) {
    return (var & (BIT_6 | BIT_7)) == (BIT_6 | BIT_7);
}
// ---- 提取 uint16_t 中第6、7位，组合成 0~3 的值 ----
static inline uint8_t get_bits_6_7_u16(uint16_t var) {
    return (uint8_t)((var >> 6) & 0x03);  // 提取第6和7位（右移6位后取最低两位）
}

#ifdef __cplusplus
}
#endif

#endif // BIT_UTILS_H
