void main();
int strcmp(const char *str1, const char *str2);
void read_command(char *buffer);
void process_command(char *buffer);
void help();
void hello();
void mailbox();

typedef struct {
    const char *command;
    void (*function)();
} Command;

