#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MAX_MSG_SIZE 4096
#define STUDENT_ID "5239"

struct msg_buffer {
    long msg_type;
    char msg_text[MAX_MSG_SIZE];
};


static key_t get_key() {
    return ftok("progfile", 65);
    /* file to key преобразует путь к файлу и ID в уникальный ключ:
    запрашивает stat, откуда объединяет побитого части proj_id, st_dev, st_ino.
    Способ ненадёжен, так как могут будут сгенерированы одинаковые ключи для разных файлов 
    при совпадении битов в разной последовательности */
}

static inline int create_queue(key_t key) {
    return msgget(key, 0666 | IPC_CREAT);
}

#endif