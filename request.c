#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX 4096
#define STUDENT_ID "5239"

struct msg_buffer {
    long msg_type;
    char msg_text[MAX];
}

int main() {
    key_t key;
    int msgid;

    FILE *fp;
    char buffer[MAX];

    // file to key преобразует путь к файлу и ID в уникальный ключ
    // запрашивает stat, откуда объединяет побитого части proj_id, st_dev, st_ino
    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);


}