#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "alg_valid_nif.h"

// Retorna 1 se o dígito de controlo for válido; 0 caso contrário.
int verif(const char *nif) {
    // Pré-condição: string com 9 chars, todos dígitos.
    int soma = 0;
    int peso = 9;
    for (int i = 0; i < 8; i++) {
        soma += (nif[i] - '0') * peso;
        peso--;
    }
    int resto = soma % 11;
    int digito_controlo = (resto == 0 || resto == 1) ? 0 : (11 - resto);
    return digito_controlo == (nif[8] - '0');
}

int validarNIFDetalhado(const char *nif) {
    if (!nif) return 1; // tratar como tamanho incorreto
    // 1) Tamanho
    if (strlen(nif) != 9) return 1;
    // 2) Numérico
    for (int i = 0; i < 9; i++) {
        if (!isdigit((unsigned char)nif[i])) return 2;
    }
    // 3) Primeiro dígito permitido
    if (nif[0] != '1' && nif[0] != '2' && nif[0] != '5' &&
        nif[0] != '6' && nif[0] != '8' && nif[0] != '9') {
        return 3;
    }
    // 4) Dígito de controlo
    if (!verif(nif)) return 4;
    return 0;
}
