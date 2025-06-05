/*
 * This file is part of [your project name].
 *
 * Copyright (C) [year] [your name or organization]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */




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
