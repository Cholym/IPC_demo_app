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

    // 1. Создание очереди

    /* file to key преобразует путь к файлу и ID в уникальный ключ:
    запрашивает stat, откуда объединяет побитого части proj_id, st_dev, st_ino.
    Способ ненадёжен, так как могут будет сгенерированы одинаковые ключи для разных файлов 
    при совпадении битов в разной последовательности */
    key = ftok("progfile", 65);
    
    /* IPC_CREAT — специальный флаг. Он говорит системе: «Если очереди с таким ключом еще нет, 
    создай её». Если она уже есть, просто верни её дескриптор. */
    msgid = msgget(key, 0666 | IPC_CREAT);

    // 2. Формируем первое сообщение
    system("ls /etc/*.conf 2>/dev/null | sed 's|.*/||' > /tmp/files.txt");

    // Читаем имена из временного файла и собираем в одну строку через запятую
    message.msg_text[0] = '\0';
    fp = fopen("/tmp/files.txt", "r");
    if (fp != NULL) {
        int first = 1;
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (!first) strcat(message.msg_text, ",");
            strcat(message.msg_text, buffer);
            first = 0;
        }
        fclose(fp);
    }

    char first_msg[MAX];
    sprintf(first_msg, "writer%s,%s", STUDENT_ID, message.msg_text);
    strcpy(message.msg_text, first_msg);
    message.msg_type = 1; // тип сообщения задания №1
    
    // Отправляем первое сообщение
    msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0);
    printf("Отправлено первое сообщение\n");
    
    // 3. Формируем второе сообщение (список каталогов в /dev)
    system("ls -d /dev/*/ 2>/dev/null | sed 's|/dev/||;s|/||' > /tmp/dirs.txt");
    
    message.msg_text[0] = '\0';
    fp = fopen("/tmp/dirs.txt", "r");
    if (fp != NULL) {
        int first = 1;
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0'; // сканируем строку, возвращаем индекс первого символа второй строки
            if (!first) strcat(message.msg_text, ",");
            strcat(message.msg_text, buffer);
            first = 0;
        }
        fclose(fp);
    }
    
    message.msg_type = 2; // тип сообщения задания №2
    msgsnd(msgid, &message, strlen(message.msg_text) + 1, 0);
    printf("Отправлено второе сообщение\n");
    
    return 0;
}