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
