#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/sha.h>
#include <yara.h>
#include <curl/curl.h>
#include <jansson.h>
#include <time.h>
#include <sqlite3.h>

// Configurações atualizadas
#define MAX_SMART_CHUNK_SIZE (2 * 1024 * 1024) // 2MB
#define MIN_ANALYSIS_CHUNKS 3
#define MAX_ANALYSIS_CHUNKS 30
#define MAX_FILE_SIZE_TO_FULL_SCAN (100 * 1024 * 1024) // 100MB limite
#define DYNAMIC_CHUNK_RATIO 0.1f // 10% do arquivo

// Estrutura para análise inteligente
typedef struct {
    const char* filename;
    size_t total_size;
    int chunks_to_analyze;
    size_t chunk_size;
    int min_chunks;
    float coverage_ratio;
} FileAnalysisPlan;

FileAnalysisPlan get_analysis_plan(const char* filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("Erro ao obter estatísticas do arquivo");
        exit(EXIT_FAILURE);
    }
    
 FileAnalysisPlan plan = {
        .filename = filename,
        .total_size = st.st_size,
        .chunk_size = MAX_SMART_CHUNK_SIZE,
        .chunks_to_analyze = MIN_ANALYSIS_CHUNKS,
        .min_chunks = MIN_ANALYSIS_CHUNKS,
        .coverage_ratio = DYNAMIC_CHUNK_RATIO
    };

// Ajuste dinâmico baseado no tamanho
    if (plan.total_size > MAX_FILE_SIZE_TO_FULL_SCAN) {
        plan.coverage_ratio = 0.05f;  // Cobre apenas 5% de arquivos muito grandes
        plan.chunks_to_analyze = MAX_ANALYSIS_CHUNKS;
    } else if (plan.total_size > 50 * 1024 * 1024) {
         plan.coverage_ratio = 0.1f;   // Cobre 10% para arquivos grandes
        plan.chunks_to_analyze = 15;
    }

// Calcula tamanho do chunk baseado na cobertura desejada
  plan.chunk_size = (size_t)(plan.total_size * plan.coverage_ratio) / plan.chunks_to_analyze;
    
    // Garante que o chunk não seja muito pequeno nem muito grande
 if (plan.chunk_size < 128 * 1024) {  // Mínimo 128KB
        plan.chunk_size = 128 * 1024;
         plan.chunks_to_analyze = (int)(plan.total_size * plan.coverage_ratio) / plan.chunk_size;
    } else if (plan.chunk_size > 5 * 1024 * 1024) {  // Máximo 5MB
        plan.chunk_size = 5 * 1024 * 1024;
         plan.chunks_to_analyze = (int)(plan.total_size * plan.coverage_ratio) / plan.chunk_size;
    }

    return plan;
}

void analyze_file_chunks(const FileAnalysisPlan* plan) {
    FILE* file = fopen(plan->filename, "rb");
    if (!file) {
         perror("Erro ao abrir arquivo para análise");
        return;
    }

 unsigned char* buffer = malloc(plan->chunk_size);
    if (!buffer) {
         perror("Erro ao alocar buffer");
        fclose(file);
        return;
    }

 printf("Analisando %s (%.2f MB) com %d chunks de %.2f KB cada\n",
           plan->filename,
           (float)plan->total_size / (1024 * 1024),
           plan->chunks_to_analyze,
           (float)plan->chunk_size / 1024);

// Pontos estratégicos para análise (incluindo posições aleatórias)
srand(time(NULL));
  for (int i = 0; i < plan->chunks_to_analyze; i++) {
 size_t offset;
        
 // Garante análise do início, meio e fim
 if (i == 0) offset = 0;  // Primeiro chunk
else if (i == 1) offset = plan->total_size / 2;  // Meio
else if (i == 2) offset = plan->total_size - plan->chunk_size;  // Fim
else offset = rand() % (plan->total_size - plan->chunk_size);  // Aleatório

    fseek(file, offset, SEEK_SET);
   size_t bytes_read = fread(buffer, 1, plan->chunk_size, file);
        
   if (bytes_read > 0) {
    printf("Chunk %d: Offset %zu (%.2f MB), Tamanho %zu bytes\n",
   i+1, offset, (float)offset/(1024*1024), bytes_read);
            
   // Aqui você implementaria a análise real do chunk
   // Ex: check_for_malware(buffer, bytes_read, offset);
        }
    }

  free(buffer);
  fclose(file);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
     return EXIT_FAILURE;
    }
FileAnalysisPlan plan = get_analysis_plan(argv[1]);
    analyze_file_chunks(&plan);

return EXIT_SUCCESS;
}
