#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE 1024

// typedef simplifica usar "struct passwd_entry_t" a solo "passwd_entry_t"
typedef struct {
    char *login_name;
    char *optional_encrypted_passwd;
    int uid;
    int gid;
    char *user_name;
    char *user_home;
    char *user_shell;
} passwd_entry_t;

// Parsea una línea separada por ':' (estándar /etc/passwd)
passwd_entry_t parse_passwd(char *line) {
    passwd_entry_t entry;
    char *token;
    
    // strsep() busca el delimitador ":", divide el string y avanza el puntero de line. Devuelve la porción.
    token = strsep(&line, ":");
    // strdup() reserva memoria (malloc subyacente) y copia el string (requiere free() posterior).
    entry.login_name = (token != NULL) ? strdup(token) : NULL;
    
    token = strsep(&line, ":");
    entry.optional_encrypted_passwd = (token != NULL) ? strdup(token) : NULL;
    
    token = strsep(&line, ":");
    entry.uid = (token != NULL) ? atoi(token) : 0; // atoi extrae int de string (ASCII to Integer)
    
    token = strsep(&line, ":");
    entry.gid = (token != NULL) ? atoi(token) : 0;
    
    token = strsep(&line, ":");
    entry.user_name = (token != NULL) ? strdup(token) : NULL;
    
    token = strsep(&line, ":");
    entry.user_home = (token != NULL) ? strdup(token) : NULL;
    
    token = strsep(&line, ":");
    if (token != NULL) {
        // Elimina el salto de línea al final pisándolo con el terminador '\0'
        if (token[strlen(token)-1] == '\n') {
            token[strlen(token)-1] = '\0'; 
        }
        entry.user_shell = strdup(token);
    } else {
        entry.user_shell = NULL;
    }
    
    return entry;
}

void print_passwd_entry(passwd_entry_t *entry, int csv) {
    if (csv == 1) {
        printf("%s,%s,%d,%d,%s,%s,%s\n", 
            entry->login_name ? entry->login_name : "",
            entry->optional_encrypted_passwd ? entry->optional_encrypted_passwd : "",
            entry->uid, entry->gid,
            entry->user_name ? entry->user_name : "",
            entry->user_home ? entry->user_home : "",
            entry->user_shell ? entry->user_shell : "");
    } else {
        printf("Login: %s\nPassword: %s\nUID: %d\nGID: %d\nName: %s\nHome: %s\nShell: %s\n----------------------\n",
            entry->login_name, entry->optional_encrypted_passwd, entry->uid, entry->gid, entry->user_name, entry->user_home, entry->user_shell);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    char *input_file = "/etc/passwd";
    int csv_mode = 0;

    // "i:c": -i requiere argumento (input file), -c es un flag sin argumento.
    while ((opt = getopt(argc, argv, "i:c")) != -1) {
        switch (opt) {
            case 'i': input_file = optarg; break;
            case 'c': csv_mode = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-i input_file] [-c]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // fopen en modo lectura ("r").
    FILE *file = fopen(input_file, "r");
    if (!file) {
        perror("Error opening file"); // perror imprime string más razón del error del sistema
        return EXIT_FAILURE;
    }

    char line[MAX_LINE];
    // fgets lee hasta el final de la línea o MAX_LINE y lo mete en line.
    while (fgets(line, sizeof(line), file)) {
        char *line_ptr = line; // Copia del puntero porque strsep() lo altera!
        passwd_entry_t entry = parse_passwd(line_ptr);
        print_passwd_entry(&entry, csv_mode);
        
        // Es imperativo liberar los strdup para no tener fugas de memoria (memory leaks).
        free(entry.login_name);
        free(entry.optional_encrypted_passwd);
        free(entry.user_name);
        free(entry.user_home);
        free(entry.user_shell);
    }

    fclose(file); // Cerrar archivo abierto con fopen.
    return EXIT_SUCCESS;
}
