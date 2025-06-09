#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <direct.h> // Para _mkdir no Windows
#include <windows.h> // Para verificar atributos de arquivos
// Caminho padrão para backup
#define CAMINHO_PADRAO "C:\\MeusBackups
#define TAM_MAX_CAMINHO 260

void criarDiretorioSeNaoExistir(const char *caminho) {
    if (_access(caminho, 0)) { // Verifica se o diretório não existe
        if (_mkdir(caminho)) { // Tenta criar o diretório
            perror("Erro ao criar diretorio %s\n", caminho);
            exit(1);
        }
    }
}
int main () {
  char resposta[10];
 char caminhoBackup[TAM_MAX_CAMINHO];
  char caminhoOrigem[TAM_MAX_CAMINHO];
  printf("Deseja fazer o Backup dos arquivos? (s/n):");
  // Lê a resposta do usuário
     fgets(resposta, sizeof(resposta), stdin);
           resposta[strcspn(resposta, "\n")] = 0;
      if (strcmp(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {
        printf("Digite o caminho de origem onde quer
        salvar o backup (PRESSIONE ENTER para usar '%s'):\n " , CAMINHO_PADRAO);
        fgets(caminhoOrigem, sizeof(caminhoOrigem), stdin);
        caminhoBackup[strcspn(caminhoOrgiem, "\n")] = '\0';
        } 
      }
