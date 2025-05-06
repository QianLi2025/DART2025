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

// ==== 新增: 通用位域提取函数 ====
/**
 * @brief 从 var 的 low 位到 high 位提取位域值
 * @param var   输入值
 * @param high  高位索引（0-31）
 * @param low   低位索引（0-31），且 low <= high
 * @return 提取后的无符号整型值
 */
static inline uint32_t get_bits_u32(uint32_t var, uint8_t high, uint8_t low) {
    uint8_t width = high - low + 1;
    uint32_t mask = ((1U << width) - 1U) << low;
    return (var & mask) >> low;
}

/**
 * @brief 从 uint16_t 类型 var 的 low 位到 high 位提取位域值
 * @param var   输入值
 * @param high  高位索引（0-15）
 * @param low   低位索引（0-15），且 low <= high
 * @return 提取后的无符号整型值
 */
static inline uint16_t get_bits_u16_var(uint16_t var, uint8_t high, uint8_t low) {
    uint8_t width = high - low + 1;
    uint16_t mask = (uint16_t)(((1U << width) - 1U) << low);
    return (var & mask) >> low;
}

/**
 * @brief 从 uint8_t 类型 var 的 low 位到 high 位提取位域值
 * @param var   输入值
 * @param high  高位索引（0-7）
 * @param low   低位索引（0-7），且 low <= high
 * @return 提取后的无符号整型值
 */
static inline uint8_t get_bits_u8_var(uint8_t var, uint8_t high, uint8_t low) {
    uint8_t width = high - low + 1;
    uint8_t mask = (uint8_t)(((1U << width) - 1U) << low);
	  return (var & mask) >> low;
}

#ifdef __cplusplus
}
#endif

#endif // BIT_UTILS_H
