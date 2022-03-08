#ifndef UTILS
#define UTILS

char hexToAscii(char digH) {
    digH &= 0x0F;
    char ascii = digH + 0x30;
    return digH > 9 ? ascii + 0x07 : ascii;
}

#endif // UTILS