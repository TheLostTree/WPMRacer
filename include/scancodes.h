#ifndef SCANCODES_H
#define SCANCODES_H
#include <stdint.h>

#define INVALID_KEYCODE {0, 0}
// these gaps.. really make me feel like the keyboard spec is bad; why are the numbers not next to eachother
// why are the letters scattered :sob:
uint8_t keycodes[126][2] = {
    INVALID_KEYCODE, // 0x00
    INVALID_KEYCODE, // 0x01 
    INVALID_KEYCODE, // 0x02
    INVALID_KEYCODE, // 0x03
    INVALID_KEYCODE, // 0x04
    INVALID_KEYCODE, // 0x05
    INVALID_KEYCODE, // 0x06
    INVALID_KEYCODE, // 0x07
    INVALID_KEYCODE, // 0x08
    INVALID_KEYCODE, // 0x09
    INVALID_KEYCODE, // 0x0a
    INVALID_KEYCODE, // 0x0b
    INVALID_KEYCODE, // 0x0c
    INVALID_KEYCODE, // 0x0d
    { '`', '`' }, // 0x0e
    INVALID_KEYCODE, // 0x0f
    INVALID_KEYCODE, // 0x10
    INVALID_KEYCODE, // 0x11
    INVALID_KEYCODE, // 0x12
    INVALID_KEYCODE, // 0x13
    INVALID_KEYCODE, // 0x14 Cntrl
    { 'q', 'Q' }, // 0x15
    { '1', '!' }, // 0x16
    INVALID_KEYCODE, // 0x17
    INVALID_KEYCODE, // 0x18
    INVALID_KEYCODE, // 0x19
    { 'z', 'Z' }, // 0x1a
    { 's', 'S' }, // 0x1b
    { 'a', 'A' }, // 0x1c
    { 'w', 'W' }, // 0x1d
    { '2', '@' }, // 0x1e
    INVALID_KEYCODE, // 0x1f
    INVALID_KEYCODE, // 0x20
    { 'c', 'C' }, // 0x21
    { 'x', 'X' }, // 0x22
    { 'd', 'D' }, // 0x23
    { 'e', 'E' }, // 0x24
    { '4', '$' }, // 0x25
    { '3', '#' }, // 0x26
    INVALID_KEYCODE, // 0x27
    INVALID_KEYCODE, // 0x28
    { ' ', ' ' }, // 0x29
    { 'v', 'V' }, // 0x2a
    { 'f', 'F' }, // 0x2b
    { 't', 'T' }, // 0x2c
    { 'r', 'R' }, // 0x2d
    { '5', '%' }, // 0x2e
    INVALID_KEYCODE, // 0x2f
    INVALID_KEYCODE, // 0x30
    { 'n', 'N' }, // 0x31
    { 'b', 'B' }, // 0x32
    { 'h', 'H' }, // 0x33
    { 'g', 'G' }, // 0x34
    { 'y', 'Y' }, // 0x35
    { '6', '^' }, // 0x36
    INVALID_KEYCODE, // 0x37
    INVALID_KEYCODE, // 0x38
    { ',', 'L' }, // 0x39
    { 'm', 'M' }, // 0x3a
    { 'j', 'J' }, // 0x3b
    { 'u', 'U' }, // 0x3c
    { '7', '&' }, // 0x3d
    { '8', '*' }, // 0x3e
    INVALID_KEYCODE, // 0x3f
    INVALID_KEYCODE, // 0x40
    { ',', '<' }, // 0x41
    { 'k', 'K' }, // 0x42
    { 'i', 'I' }, // 0x43
    { 'o', 'O' }, // 0x44
    { '0', ')' }, // 0x45
    { '9', '(' }, // 0x46
    INVALID_KEYCODE, // 0x47
    INVALID_KEYCODE, // 0x48
    { '.', '>' }, // 0x49
    { '/', '?' }, // 0x4a
    { 'l', 'L' }, // 0x4b
    { ';', ':' }, // 0x4c
    { 'p', 'P' }, // 0x4d
    { '-', '_' }, // 0x4e
    INVALID_KEYCODE, // 0x4f
    INVALID_KEYCODE, // 0x50
    INVALID_KEYCODE, // 0x51
    { '`', '"' }, // 0x52
    INVALID_KEYCODE, // 0x53
    { '[', '[' }, // 0x54
    { '=', '+' }, // 0x55
    INVALID_KEYCODE, // 0x56
    INVALID_KEYCODE, // 0x57
    INVALID_KEYCODE, // 0x58
    INVALID_KEYCODE, // 0x59
    { '\r', '\r' }, // 0x5a
    { ']', ']' }, // 0x5b
    INVALID_KEYCODE, // 0x5c
    { '/', '|' }, // 0x5d
    INVALID_KEYCODE, // 0x5e
    INVALID_KEYCODE, // 0x5f
    INVALID_KEYCODE, // 0x60
    { '<', '>' }, // 0x61
    INVALID_KEYCODE, // 0x62
    INVALID_KEYCODE, // 0x63
    INVALID_KEYCODE, // 0x64
    INVALID_KEYCODE, // 0x65
    { '\b', '\b' }, // 0x66
    INVALID_KEYCODE, // 0x67
    INVALID_KEYCODE, // 0x68
    { '1', '1' }, // 0x69
    INVALID_KEYCODE, // 0x6a
    { '4', '4' }, // 0x6b
    { '7', '7' }, // 0x6c
    INVALID_KEYCODE, // 0x6d
    INVALID_KEYCODE, // 0x6e
    INVALID_KEYCODE, // 0x6f
    { '0', '0' }, // 0x70
    { ',', ',' }, // 0x71
    { '2', '2' }, // 0x72
    { '5', '5' }, // 0x73
    { '6', '6' }, // 0x74
    { '8', '8' }, // 0x75
    INVALID_KEYCODE, // 0x76
    INVALID_KEYCODE, // 0x77
    INVALID_KEYCODE, // 0x78
    { '+', '+' }, // 0x79
    { '3', '3' }, // 0x7a
    { '-', '-' }, // 0x7b
    { '*', '*' }, // 0x7c
    { '9', '9' }, // 0x7d
};

#endif /* SCANCODES_H_ */