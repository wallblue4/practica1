
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>

#define SHM_KEY 12345
#define SHM_SIZE 1024
#define TABLE_SIZE 1000000



typedef struct hash_node {
    char* key;
    char* value;
    struct hash_node* next;
} hash_node;

hash_node* table[TABLE_SIZE];

unsigned int hash(char* key) {
    unsigned int hashval = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hashval = key[i] + (hashval << 5) - hashval;
    }
    return hashval % TABLE_SIZE;
}

void insert(char* key, char* value) {
    unsigned int hashval = hash(key);
    hash_node* new_node = (hash_node*) malloc(sizeof(hash_node));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = NULL;

    if (table[hashval] == NULL) {
        table[hashval] = new_node;
    } else {
        hash_node* current = table[hashval];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
}

char* get(char* key) {
    unsigned int hashval = hash(key);
    hash_node* current = table[hashval];

    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    return NULL;
}

int main(int argc, char *argv[]) {
   
    FILE* fp = fopen(argv[1], "r");

    if (fp == NULL) {
        fprintf(stderr, "Error: no se pudo abrir el archivo.\n");
        return 1;
    }

    char line[1024];
    char* tok;

    while (fgets(line, 1024, fp)) {
        // Obtiene la primera columna de origen
        tok = strtok(line, ",");
        char* origen = strdup(tok);

        // Obtiene la segunda columna de destino
        tok = strtok(NULL, ",");
        char* destino = strdup(tok);

        // Obtiene la tercera columna hora
        tok = strtok(NULL, ",");
        char* hora = strdup(tok);

        // Obtiene la cuarta columna tiempo medio
        tok = strtok(NULL, ",");
        char* tiempo = strdup(tok);

        // Inserta el valor en la tabla hash
        char* key = malloc(strlen(origen) + strlen(destino) + strlen(hora) + 1);
        sprintf(key, "%s%s%s", origen, destino, hora);
        insert(key, tiempo);

        free(origen);
        free(destino);
        free(hora);
        free(tiempo);
        free(key);
    }

    fclose(fp);

    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // Conectarse a la memoria compartida
    char *shm_ptr = shmat(shmid, NULL, 0);
    if (shm_ptr == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    // Obtener el valor de tiempo medio 
    *shm_ptr = get(shm_ptr);
     // Desconectarse de la memoria compartida
    shmdt(shm_ptr);
    

    return 0;
}
