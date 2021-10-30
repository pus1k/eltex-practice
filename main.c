#include <stdio.h>

void output(const int value)
{
    int tmp = 28;
    while (tmp >= 0) {
        printf("%x", (value » tmp) & 0xF);
        tmp -= 4;
    }
    printf("\n");
}
void set_reg_hex(int* value, const int reg_val, const int shift)
{
    if (0 <= shift || shift <= 8) {
        *value &= ~(0xF << (32 - shift * 4));
        *value |= reg_val << (32 - shift * 4);
    }
}
int main()
{
    int a = 0x00DDAABB;
    output(a);
    set_reg_hex(&a, 0xC, 3);
    set_reg_hex(&a, 0xC, 4);
    output(a);
    return 0;
}