#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define HELP printf("\n./svmq [S/C]\n\n S -- Server\n C -- Client\n\n")
#define Q_NAME "/tmp/svmq"
#define BUF_SIZE 256

struct message_text {
    int qid;
    char buf[BUF_SIZE];
};

struct message {
    long message_type;
    struct message_text message_text;
};

int check(const int result, const int expected, const char* opt)
{
    if (result == expected) { perror(opt), exit(EXIT_FAILURE); }
    return result;
}

void client()
{
    struct message msg;
    msg.message_type     = 1;
    msg.message_text.qid = check(msgget(IPC_PRIVATE, 0660), -1, "msgget");

    key_t key    = check(ftok(Q_NAME, 97), -1, "ftok");
    int serv_qid = check(msgget(key, 0), -1, "msgget");
    for (; msg.message_text.buf[0] != 'q';) {
        printf("> "), fflush(stdout);
        fgets(msg.message_text.buf, BUF_SIZE, stdin);
        check(msgsnd(serv_qid, &msg, sizeof(struct message_text), 0), -1, "msgsnd");
    }
}
void server()
{
    struct message msg;
    key_t key = check(ftok(Q_NAME, 97), -1, "ftok");
    int qid   = check(msgget(key, IPC_CREAT | 0660), -1, "msgget");
    for (; msg.message_text.buf[0] != 'q';) {
        check(msgrcv(qid, &msg, sizeof(struct message_text), 0, 0), -1, "msgrcv");
        printf(":%d -- %s", msg.message_text.qid, msg.message_text.buf);
    }
    check(msgctl(qid, IPC_RMID, NULL), -1, "msgctl");
}

int main(int argc, char* argv[])
{
    system("touch /tmp/svmq");
    if (argc == 2) {
        if (argv[1][0] == 'S') {
            server();
        } else if (argv[1][0] == 'C') {
            client();
        } else {
            HELP;
        }
    } else {
        HELP;
    }
    return 0;
}