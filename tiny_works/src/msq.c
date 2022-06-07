#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Q_NAME "/mq"
#define BUF_SIZE 256

int check(const int result, const int expected, const char* opt)
{
    if (result == expected) { perror(opt), exit(EXIT_FAILURE); }
    return result;
}

int main(int argc, char* argv[])
{
    if (argc == 2) {
        struct mq_attr attr;
        attr.mq_flags   = 0;
        attr.mq_maxmsg  = 10;
        attr.mq_msgsize = BUF_SIZE;
        attr.mq_curmsgs = 0;

        if (!strcmp(argv[1], "C")) {
            mqd_t mq = (mqd_t)check(
                mq_open(Q_NAME, O_CREAT | O_WRONLY, S_IRWXU, &attr), (mqd_t)-1, "OPEN");

            for (char buf[BUF_SIZE]; strncmp(buf, "q", 1);) {
                printf("> "), fflush(stdout);
                fgets(buf, BUF_SIZE, stdin);
                check(mq_send(mq, buf, BUF_SIZE, 0), -1, "SEND");
            }

            check(mq_close(mq), -1, "CLOSE");
        } else if (!strcmp(argv[1], "S")) {
            mqd_t mq = (mqd_t)check(
                mq_open(Q_NAME, O_CREAT | O_RDONLY, S_IRWXU, &attr), (mqd_t)-1, "OPEN");

            for (char buf[BUF_SIZE]; strncmp(buf, "q", 1) != 0;) {
                check(mq_receive(mq, buf, BUF_SIZE, NULL), -1, "RECV");
                printf(": %s", buf);
            }
            check(mq_close(mq), -1, "CLOSE");
            check(mq_unlink(Q_NAME), -1, "UNLINK");
        }
    } else {
        printf("\n./mq [S/C]\n S -- Server\n C -- Client\n\n");
    }
    return 0;
}