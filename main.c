#include <stdio.h>

void output(const int value)
{
    int tmp = 28;
    while (tmp >= 0) {
        printf("%x", (value >> tmp) & 0xF);
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
void array_first_task()
{
    int N, i, j;
    printf("N = ");
    scanf("%d", &N);
    int a[N * N];
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            a[i * N + j] = i * N + j + 1;
            printf("%d ", a[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}
void array_second_task()
{
    int N, i;
    printf("N = ");
    scanf("%d", &N);
    int a[N];
    for (i = N - 1; i >= 0; i--)
        scanf("%d", &a[i]);
    for (i = 0; i < N; i++)
        printf("%d ", a[i]);
    printf("\n\n");
}
void array_third_task()
{
    int N, i, j;
    printf("N = ");
    scanf("%d", &N);
    int a[N * N];
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i < N - j - 1 && j < N - i - 1)
                a[i * N + j] = 0;
            else
                a[i * N + j] = 1;
            printf("%d ", a[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}
void array_fourth_task()
{
    int N, count_of_steps, step, i = 0, j = 0, value = 0, iter = 1;
    char who = 'j';
    printf("N = ");
    scanf("%d", &N);
    int a[N * N];
    count_of_steps = N;
    while(value < N * N) {
        step = 0;
        while (step < count_of_steps) {
            a[i * N + j] = ++value, step++;
            if (who == 'i')
                i += iter;
            else
                j += iter;
        }
        if (who == 'i') {
            who = 'j';
            i -= iter;
            iter = -1 * iter;
            j += iter; 
        } else if (who == 'j') {
            who = 'i';
            count_of_steps--;
            j -= iter, i += iter;   
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d\t", a[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}
int main()
{
    int task;
    printf("Which task you wanna test:\n");
    printf("\t1) Byte shift\n");
    printf("\t2) Array tasks\n");
    printf("\t3) Ptr byte change\n");
    printf("Enter number of test\n> ");
    scanf("%d", &task);
    if (task == 1) {
        int a = 0x00DDAABB;
        output(a);
        set_reg_hex(&a, 0xC, 3);
        output(a);
        set_reg_hex(&a, 0xC, 4);
        output(a);
    } else if (task == 2) {
        printf("\nFirst task\n\n");
        array_first_task();
        printf("Second task\n\n");
        array_second_task();
        printf("Third task\n\n");
        array_third_task();
        printf("Fourth task\n\n");
        array_fourth_task();
    } else if (task == 3) {
        int a = 270533154;
        char *ptr= &a;
        printf("%d\n", a);
        printf("%d|%d|%d|%d\n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
        *(ptr + 2) = 5;
        printf("%d\n", a);
        printf("%d|%d|%d|%d\n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
    } else {
        printf("Wrong number of test\n");
    }
    return 0;
}