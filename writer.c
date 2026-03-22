#include "common.h"

int main() {
    key_t key;
    int msgid;
    struct msg_buffer msg;
    FILE *fp;
    char buffer[256];
    
    // 1. СОЗДАЁМ ОЧЕРЕДЬ
    key = get_key();
    msgid = create_queue(key);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }
    
    // 2. ФОРМИРУЕМ ПЕРВОЕ СООБЩЕНИЕ (список .conf файлов)
    system("ls /etc/*.conf 2>/dev/null | sed 's|.*/||' > /tmp/files.txt");
    
    msg.msg_text[0] = '\0';
    fp = fopen("/tmp/files.txt", "r");
    if (fp != NULL) {
        int first = 1;
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (!first) strcat(msg.msg_text, ",");
            strcat(msg.msg_text, buffer);
            first = 0;
        }
        fclose(fp);
    }
    
    {
        char list_copy[MAX_MSG_SIZE];
        strncpy(list_copy, msg.msg_text, sizeof(list_copy));
        list_copy[sizeof(list_copy) - 1] = '\0';
        int plen = snprintf(msg.msg_text, sizeof(msg.msg_text), "writer%s,", STUDENT_ID);
        if (plen > 0 && (size_t)plen < sizeof(msg.msg_text))
            strncat(msg.msg_text, list_copy, sizeof(msg.msg_text) - (size_t)plen - 1U);
    }
    msg.msg_type = 1;
    
    if (msgsnd(msgid, &msg, strlen(msg.msg_text) + 1, 0) == -1) {
        perror("msgsnd (1)");
        exit(1);
    }
    printf("Отправлено первое сообщение (тип %ld)\n", (long)msg.msg_type);
    
    // 3. ФОРМИРУЕМ ВТОРОЕ СООБЩЕНИЕ (список каталогов в /dev)
    system("ls -d /dev/*/ 2>/dev/null | sed 's|/dev/||;s|/||' > /tmp/dirs.txt");
    
    msg.msg_text[0] = '\0';
    fp = fopen("/tmp/dirs.txt", "r");
    if (fp != NULL) {
        int first = 1;
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0';
            if (!first) strcat(msg.msg_text, ",");
            strcat(msg.msg_text, buffer);
            first = 0;
        }
        fclose(fp);
    }
    
    msg.msg_type = 2;
    if (msgsnd(msgid, &msg, strlen(msg.msg_text) + 1, 0) == -1) {
        perror("msgsnd (2)");
        exit(1);
    }
    printf("Отправлено второе сообщение (тип %ld)\n", (long)msg.msg_type);
    
    return 0;
}