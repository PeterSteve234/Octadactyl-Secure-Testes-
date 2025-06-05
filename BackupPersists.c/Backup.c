#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Caminho padrão para backup
#define CAMINHO_PADRAO "C:\\MeusBackups

int main() {
    char resposta[10];
    char caminhoBackup[260];

        printf(" Deseja fazer Backup agora? (S/n)");
        fgets(resposta, sizeof(resposta), stdin);

       // remove o \n do final
       reposta[strcspn(resposta, "\n")] = 0;

       if  (strcspn(resposta, "s") == 0 || strcmp(resposta, "S") == 0) {

            printf("Digite o caminho onde quer salvar o Backup (pressione enter para usar '%s'):\n" , CAMINHO_PADRAO);
            fgets(caminhoBackup, sizeof(CaminhoBackup), stdin);
            caminhoBackup[strscpy(CaminhoBackup, "\n")] = 0;

        if (strlen(caminhoBackup) == 0) {
          strcpy(caminhoBackup, CAMINHOPADRAO)
        }

         printf("Backup sera salvo em: %s\n", CaminhoBackup);
         // codigo futuro será jogado aqui 
    } else {
         printf("Backup cancelado pelo usuario. \n");
    }
           
  return 0;
}
