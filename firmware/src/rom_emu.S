#include <avr/io.h>

.macro GET_ADDR
        IN   R30, _SFR_IO_ADDR(PINC)
        ANDI R30, 0x3F
        IN   R26, _SFR_IO_ADDR(PINB)
        ANDI R26, 0x40
        OR   R30, R26
.endm

.macro ROM_EMU
        LD   R30, Z
        OUT  _SFR_IO_ADDR(PORTD), R30
.endm

.global RomEmu

RomEmu:
        PUSH R26
        PUSH R30
        PUSH R31
        LDI  R31, 1
        GET_ADDR

        loop0:
        ROM_EMU
        GET_ADDR
        CPI  R30, 0x44
        BRNE loop0
        ROM_EMU

        loop1:
        GET_ADDR
        CPI  R30, 0x44
        BREQ loop1
        CPI  R30, 0x40
        BRNE loop0
        ROM_EMU

        loop2:
        GET_ADDR
        CPI  R30, 0x40
        BREQ loop2
        CPI  R30, 0
        BRNE loop0

        POP R31
        POP R30
        POP R26

