#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define ISOLATE_DIR "isolados"


void createDirectoryIfdoesnExists(const char *path) {
     struct stat st = {0};
     if (stat(path, &st) == 1) {
      mkdir(path, 0700) //Cria com permissões restritas
     }
}
int isolatefile (const char *caminhoOriginal) {
  createDirectoryIfdoesnExist(ISOLATE_DIR);

  // extraindo o nome do arquivo

  const char *nomeArquivo = strrchr(caminhoOriginal, '/');
  if (!nomeArquivo) nomeArquivo = caminhoOriginal;
  else nomeArquivo++  // servepara pular o '/'


  // gerando o caminho

 char novoCaminho[340];
 snprintf(novoCaminho, sizeof(novocaminho), "%s/%s", ISOLATE_DIR, nomeArquivo);

 // Movendo o arquivo
 if (rename(caminhoOriginal, novocaminho) != 0) {
     perror("Erro ao mover arquivo para isolamento");
     return 1;
 }
printf("[ISOLAMENTO] Arquivo movido para: %s\n", novoCaminho);
return 0;
}

