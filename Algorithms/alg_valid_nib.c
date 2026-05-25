#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "alg_valid_nib.h"

int verificarNum(const char *str) {
    if (!str) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

int modulo97(const char *nib) {
    int mod = 0;
    for (int i = 0; nib && nib[i] != '\0'; i++) {
        int digit = nib[i] - '0';
        mod = (mod * 10 + digit) % 97;
    }
    return mod;
}

int validarNIBDetalhado(const char *nib) {
    if (!nib) return 1; // tratar como tamanho incorreto
    // 1) Tamanho exato 21
    if (strlen(nib) != 21) return 1;
    // 2) Numérico
    if (!verificarNum(nib)) return 2;
    // 3) Módulo 97 deve ser 1
    if (modulo97(nib) != 1) return 3;
    return 0;
}
