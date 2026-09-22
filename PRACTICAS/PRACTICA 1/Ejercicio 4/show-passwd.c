#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 1024

// Estructura para almacenar una entrada de /etc/passwd
typedef struct {
    char *login_name;
    char *optional_encrypted_passwd;
    int uid;
    int gid;
    char *user_name;
    char *user_home;
    char *user_shell;
} passwd_entry_t;

// Función original para clonar strings (reemplazada posteriormente por strdup)
/*
char* clone_string(const char* str) {
    if (str == NULL) return NULL;
    char* clone = malloc(strlen(str) + 1);
    if (clone) strcpy(clone, str);
    return clone;
}
*/

// Parsea una línea de /etc/passwd y rellena la estructura
passwd_entry_t parse_passwd(char *line) {
    passwd_entry_t entry;
    char *token;
    
    // Login name
    token = strsep(&line, ":");
    if (token != NULL) {
        entry.login_name = strdup(token);
    } else {
        entry.login_name = NULL;
    }
    
    // Password
    token = strsep(&line, ":");
    if (token != NULL) {
        entry.optional_encrypted_passwd = strdup(token);
    } else {
        entry.optional_encrypted_passwd = NULL;
    }
    
    // UID
    token = strsep(&line, ":");
    if (token != NULL) {
        entry.uid = atoi(token);
    } else {
        entry.uid = 0;
    }
    
    // GID
    token = strsep(&line, ":");
    if (token != NULL) {
        entry.gid = atoi(token);
    } else {
        entry.gid = 0;
    }
    
    // User name
    token = strsep(&line, ":");
    if (token != NULL) {
        entry.user_name = strdup(token);
    } else {
        entry.user_name = NULL;
    }
    
    // Home
    token = strsep(&line, ":");
    if (token != NULL) {
        entry.user_home = strdup(token);
    } else {
        entry.user_home = NULL;
    }
    
    // Shell
    token = strsep(&line, ":");
    if (token != NULL) {
        if (token[strlen(token)-1] == '\n') {
            token[strlen(token)-1] = '\0'; // Remove newline
        }
        entry.user_shell = strdup(token);
    } else {
        entry.user_shell = NULL;
    }
    
    return entry;
}

void print_passwd_entry(passwd_entry_t *entry, int csv) {
    if (csv == 1) {
        if (entry->login_name != NULL) {
            printf("%s,", entry->login_name);
        } else {
            printf(",");
        }
        
        if (entry->optional_encrypted_passwd != NULL) {
            printf("%s,", entry->optional_encrypted_passwd);
        } else {
            printf(",");
        }
        
        printf("%d,%d,", entry->uid, entry->gid);
        
        if (entry->user_name != NULL) {
            printf("%s,", entry->user_name);
        } else {
            printf(",");
        }
        
        if (entry->user_home != NULL) {
            printf("%s,", entry->user_home);
        } else {
            printf(",");
        }
        
        if (entry->user_shell != NULL) {
            printf("%s\n", entry->user_shell);
        } else {
            printf("\n");
        }
    } else {
        printf("Login: %s\n", entry->login_name);
        printf("Password: %s\n", entry->optional_encrypted_passwd);
        printf("UID: %d\n", entry->uid);
        printf("GID: %d\n", entry->gid);
        printf("Name: %s\n", entry->user_name);
        printf("Home: %s\n", entry->user_home);
        printf("Shell: %s\n", entry->user_shell);
        printf("----------------------\n");
    }
}

int main(int argc, char *argv[]) {
    int opt;
    char *input_file = "/etc/passwd";
    int csv_mode = 0;

    // Procesar opciones -i (input file) y -c (csv)
    while ((opt = getopt(argc, argv, "i:c")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                break;
            case 'c':
                csv_mode = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-i input_file] [-c]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        // Hacemos una copia local de la línea porque strsep la modifica
        char *line_ptr = line;
        passwd_entry_t entry = parse_passwd(line_ptr);
        print_passwd_entry(&entry, csv_mode);
        
        // Liberar la memoria de strdup
        free(entry.login_name);
        free(entry.optional_encrypted_passwd);
        free(entry.user_name);
        free(entry.user_home);
        free(entry.user_shell);
    }

    fclose(file);
    return EXIT_SUCCESS;
}
