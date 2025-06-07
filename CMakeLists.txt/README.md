  A[Início] --> B[Carrega Assinaturas]
    B --> C{Arquivo/Alvo}
    C -->|Arquivo| D[Calcula Hash]
    C -->|Processo| E[Analisa Comportamento]
    D --> F[Compara com DB]
    E --> G[Checa Syscalls]
    F & G --> H[Gera Relatório]
