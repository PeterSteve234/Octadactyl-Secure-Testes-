

// utils/file_naming.c
#include <intmainGenerator.c>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>


#define MAX_ID_LEN 32 // Tamanho máximo do ID (ex.: "a-000-...-999")

// Estrutura para armazenar o estado atual do ID
typedef struct {
    char prefix;      // Prefixo (a-z)
    uint64_t counter; // Contador (0 até 99999999999999999999)
} FileIDGenerator;


// Gera o próximo ID
void generate_file_id(FileIDGenerator *gen, char *output) {
    // Formata o contador como 21 dígitos (preenchidos com zeros)
   snprintf(output, MAX_ID_LEN, "%c-%03llu-%03llu-%03llu-%03llu-%03llu-%03llu-%03llu",
    gen->prefix,
    (unsigned long long)((gen->counter / 1000000000000000000ULL) % 1000),
    (unsigned long long)((gen->counter / 1000000000000000ULL) % 1000),
    (unsigned long long)((gen->counter / 1000000000000ULL) % 1000),
    (unsigned long long)((gen->counter / 1000000000ULL) % 1000),
    (unsigned long long)((gen->counter / 1000000ULL) % 1000),
    (unsigned long long)((gen->counter / 1000ULL) % 1000),
    (unsigned long long)(gen->counter % 1000)
);
    // Incrementa o contador (e avança o prefixo se necessário)
    gen->counter++;
    if (gen->counter == 10000000000000000000) {
        gen->counter = 0;
        gen->prefix++;
    }
}
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

int is_system_folder(const char *path) {
    // Adicione aqui os caminhos que você quer ignorar
    return strstr(path, "Windows") || strstr(path, "Program Files");
}

void init_file_id_generator(FileIDGenerator *gen, char initial_prefix) {
    gen->prefix = initial_prefix;
    gen->counter = 0;
}

void scan_and_generate(const char *base_path, FileIDGenerator *gen) {
    DIR *dir = opendir(base_path);
    if (!dir) return;

    struct dirent *entry;
    char path[1024];

    while ((entry = readdir(dir)) != NULL) {
        // Ignora "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        struct stat st;
        if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            if (!is_system_folder(path)) {
                char id[MAX_ID_LEN];
                generate_file_id(gen, id);
                printf("Pasta: %s | ID: %s\n", path, id);
                scan_and_generate(path, gen); // Recursivo
            }
        }
    }
    closedir(dir);
}


// Um arquivo para armazenar caminhos de arquivos que já receberam ID
#define HISTORICO_ARQUIVOS "historico_ids.txt"
int ja_processado(const char *path) {
    FILE *file = fopen(HISTORICO_ARQUIVOS, "r");
    if (!file) return 0;

    char linha[1024];
    while (fgets(linha, sizeof(linha), file)) {
        linha[strcspn(linha, "\n")] = 0; // remove \n
        if (strcmp(linha, path) == 0) {
            fclose(file);
            return 1; // já existe
        }
    }

    fclose(file);
    return 0;
}

// Salva o estado do gerador em um arquivo (para persistência)
void save_generator_state(FileIDGenerator *gen, const char *path) {
    FILE *file = fopen(path, "wb");
    if (file) {
        fwrite(gen, sizeof(FileIDGenerator), 1, file);
        fclose(file);
    }
}

// Carrega o estado do gerador
void load_generator_state(FileIDGenerator *gen, const char *path) {
    FILE *file = fopen(path, "rb");
    if (file) {
        fread(gen, sizeof(FileIDGenerator), 1, file);
        fclose(file);
    } else {
        // Estado inicial (a-000-...-000)
        gen->prefix = 'a';
        gen->counter = 0;
    }
}
