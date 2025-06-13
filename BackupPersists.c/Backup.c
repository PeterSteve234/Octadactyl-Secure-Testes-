#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h> // Para _mkdir no Windows
#include <windows.h> // Para verificar atributos de arquivos
#include <openssl/sha.h>

void  calcularHashArquivo(const char *path, unsigned char *outputhash) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Erro ao abrir arquivo do Hash!");
            exit(1);
    }
}


SHA256_CTX sha256;
SHA256_init(&sha256);


    const int bufsize = 32768;
    unsigned char *buffer = malloc(bufSize);
        if(!buffer) {
            perror("falha ao alocar buffer\n");
            fclose(file);
            exit(1);
        }


while((bytesread  = fread(buffer, 1, bufsize, file) > 0) {
SHA256_Final(&sha256, buffer, bytesRead);

    
    SHA256_Final(outpushHash, &sha256);

    fclose(file);
    free(buffer);
}


     
