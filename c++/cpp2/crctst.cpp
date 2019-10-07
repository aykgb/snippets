#include <string.h>

// ==========================================================================
// CRC Generation Unit - Linear Feedback Shift Register implementation
// (c) Kay Gorontzi, GHSi.de, distributed under the terms of LGPL
// ==========================================================================
char *MakeCRC(char *BitString)
{
:
    static char Res[9];                                 // CRC Result
    char CRC[8];
    int  i;
    char DoInvert;

    for (i=0; i<8; ++i)  CRC[i] = 0;                    // Init before calculation

    for (i=0; i<strlen(BitString); ++i) {
        DoInvert = ('1'==BitString[i]) ^ CRC[7];         // XOR required?

        CRC[7] = CRC[6];
        CRC[6] = CRC[5];
        CRC[5] = CRC[4];
        CRC[4] = CRC[3];
        CRC[3] = CRC[2];
        CRC[2] = CRC[1] ^ DoInvert;
        CRC[1] = CRC[0] ^ DoInvert;
        CRC[0] = DoInvert;
    }

    for (i=0; i<8; ++i)  Res[7-i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
    Res[8] = 0;                                         // Set string terminator

    return(Res);
}

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

uint8_t calc_crc8(uint8_t *data, uint16_t len)
{
    uint8_t crc = 0, i;
    while (len--) {
        for (i = 0x80; i != 0; i /= 2) {
            if((crc & 0x80) != 0) {
                crc *= 2;
                crc ^= 0x7;
            } else {
                crc *= 2;
            }
            if((*data & i) != 0) {
                crc ^= 0x7;
            }
        }
        data++;
    }

    return crc;
}

// A simple test driver:

#include <stdio.h>

int main()
{
    //   char *Data, *Result;                                   // Declare two strings

    //   Data = "1101000101000111";
    //   Result = MakeCRC(Data);                                // Calculate CRC

    //   printf("CRC of [2] is [%s] with P=[100000111]\n", Data, Result);

#define LEN  4
    uint8_t data[LEN] = {0x79, 0x00, 0xec}; // 0x16
    //  uint8_t data[LEN] = {0xb2, 0x00, 0x02}; // 0x32
    //  uint8_t data[LEN] = {0x00, 0x02, 0xb2}; // 0x3d
    //  uint8_t data[LEN] = {0x02, 0x00, 0xb2}; // 0xc1
    //  uint8_t data[LEN] = {0xb2, 0x79, 0x02, 0x00}; // 0xc3
    //  uint8_t data[LEN] = {0xb2, 0x79, 0x00, 0x02}; // 0xe7
    //  uint8_t data[LEN] = {0x00, 0x02, 0x79, 0xb2}; // 0xde
    //  uint8_t data[LEN] = {0x02, 0x00, 0x79, 0xb2}; // 0x24
    uint8_t res = calc_crc8(data, LEN);
    printf("0x%x\n", res);

    return(0);
}
