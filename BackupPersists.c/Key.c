int main(void)
{
    FileIDGenerator gen;

    // Carrega o estado do gerador (opcional)
    load_generator_state(&gen, "estado.bin");

    // Escolha o diretório inicial (exemplo: "C:/Users/rdafa")
    const char *diretorio_inicial = "C:/Users/rdafa";

    // Vasculha as pastas e gera IDs
    scan_and_generate(diretorio_inicial, &gen);

    // Salva o estado do gerador ao final (opcional)
    save_generator_state(&gen, "estado.bin");

    return 0;
}