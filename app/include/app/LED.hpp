#pragma once
#include <cstdint>

enum {
    NONE,
    WRITE,
    TOGGLE,
} LED;

union LEDConfig {
    struct {
        uint8_t shine;
    } __attribute__((__packed__));
    uint8_t code[1];
} __attribute__((__packed__));