#pragma once
#include <cstdint>

enum
{
    none,
    write,
    toggle,
} Led;

union LED_Config
{
    struct
    {
        uint8_t shine;
    } __attribute__((__packed__));
    uint8_t code[1];
} __attribute__((__packed__));