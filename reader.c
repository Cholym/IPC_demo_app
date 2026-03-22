#include "common.h"

static void print_conf_filenames(const char *payload) {
    const char *comma = strchr(payload, ',');
    if (comma == NULL) {
        puts(payload);
        return;
    }
    const char *p = comma + 1;
    if (*p == '\0')
        return;

    for (;;) {
        const char *end = strchr(p, ',');
        if (end != NULL) {
            printf("%.*s\n", (int)(end - p), p);
            p = end + 1;
        } else {
            puts(p);
            break;
        }
    }
}

int main(void) {
    key_t key = get_key();
    int msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    struct msg_buffer message;
    ssize_t n;

    n = msgrcv(msgid, &message, sizeof(message.msg_text), 1, 0);
    if (n == -1) {
        perror("msgrcv (1)");
        exit(1);
    }
    print_conf_filenames(message.msg_text);

    n = msgrcv(msgid, &message, sizeof(message.msg_text), 2, 0);
    if (n == -1) {
        perror("msgrcv (2)");
        exit(1);
    }

    struct msqid_ds info;
    if (msgctl(msgid, IPC_STAT, &info) == -1)
        perror("msgctl IPC_STAT");
    else
        printf("Время последней записи в очередь: %s", ctime(&info.msg_stime));
    // удаляем очередь
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
        perror("msgctl IPC_RMID");

    return 0;
}
