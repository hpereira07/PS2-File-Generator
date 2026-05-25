#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "alg_valid_nib.h"
#include "alg_valid_nif.h"

#define MAX_CLIENTES 100
#define MAX_CONSUMOS 500
#define MAX_PERIODOS 50

// --- Configuração do cabeçalho simplificado ---
#define DIA_PROC 26
#define HEADER_PAD "                   "  // espaços entre o nome da empresa e os totais

// ---------------------- STRUCTS ----------------------
typedef struct {
    int id;
    char nome[50];
    // Aumentamos buffers para conseguir ler valores inválidos (p.ex. NIF com 10 dígitos)
    char nif[32];   // permite >9 p/ detetar tamanho incorreto; validação trata disso
    char nib[64];   // permite >21 p/ detetar tamanho incorreto; validação trata disso
} Cliente;

typedef struct {
    int idCliente;
    int ano;
    int mes;
    float consumo;
} Consumo;

typedef struct {
    int ano;
    int mes;
    float preco_kWh;
} Periodo;

// ---------------------- ERROS DE VALIDAÇÃO ----------------------
static void printNIFErro(int code, int idCliente) {
    switch (code) {
        case 1: printf("NIF inválido - Tamanho incorreto (Cliente %d)\n", idCliente); break;
        case 2: printf("NIF inválido - Contém caracteres não numéricos (Cliente %d)\n", idCliente); break;
        case 3: printf("NIF inválido - Primeiro dígito inválido (Cliente %d)\n", idCliente); break;
        case 4: printf("NIF inválido - Dígito de controlo inválido (Cliente %d)\n", idCliente); break;
        default: printf("NIF inválido - Erro desconhecido (Cliente %d)\n", idCliente); break;
    }
}

static void printNIBErro(int code, int idCliente) {
    switch (code) {
        case 1: printf("NIB inválido - Tamanho incorreto (Cliente %d)\n", idCliente); break;
        case 2: printf("NIB inválido - Contém caracteres não numéricos (Cliente %d)\n", idCliente); break;
        case 3: printf("NIB inválido - Falha no módulo 97 (Cliente %d)\n", idCliente); break;
        default: printf("NIB inválido - Erro desconhecido (Cliente %d)\n", idCliente); break;
    }
}

// ---------------------- LOADERS ----------------------
int carregarClientes(Cliente clientes[]) {
    FILE *f = fopen("clientes.txt", "r");
    if (!f) { printf("Erro ao abrir clientes.txt\n"); return 0; }
    int n = 0;
    // NOTA: Lemos campos com larguras MAIORES para NÃO truncar valores inválidos
    // id ; nome ; nif ; nib
    while (fscanf(f, "%d;%49[^;];%31[^;];%63s", &clientes[n].id, clientes[n].nome, clientes[n].nif, clientes[n].nib) == 4) {
        n++;
        if (n >= MAX_CLIENTES) break;
        // consumir possível resto de linha (espaços) até '\n'
        int c; while ((c = fgetc(f)) != '\n' && c != EOF) { /* skip */ }
    }
    fclose(f);
    return n;
}

int carregarConsumos(Consumo consumos[]) {
    FILE *f = fopen("consumos.txt", "r");
    if (!f) { printf("Erro ao abrir consumos.txt\n"); return 0; }
    int n = 0;
    while (fscanf(f, "%d;%d;%d;%f", &consumos[n].idCliente, &consumos[n].ano, &consumos[n].mes, &consumos[n].consumo) == 4) {
        n++;
        if (n >= MAX_CONSUMOS) break;
        int c; while ((c = fgetc(f)) != '\n' && c != EOF) { /* skip */ }
    }
    fclose(f);
    return n;
}

int carregarPeriodos(Periodo periodos[]) {
    FILE *f = fopen("periodos.txt", "r");
    if (!f) { printf("Erro ao abrir periodos.txt\n"); return 0; }
    int n = 0;
    while (fscanf(f, "%d;%d;%f", &periodos[n].ano, &periodos[n].mes, &periodos[n].preco_kWh) == 3) {
        n++;
        if (n >= MAX_PERIODOS) break;
        int c; while ((c = fgetc(f)) != '\n' && c != EOF) { /* skip */ }
    }
    fclose(f);
    return n;
}

float obterPreco(Periodo periodos[], int nPeriodos, int ano, int mes) {
    for (int i = 0; i < nPeriodos; i++) {
        if (periodos[i].ano == ano && periodos[i].mes == mes) {
            return periodos[i].preco_kWh; // usa a 1ª ocorrência
        }
    }
    return -1; // Não encontrado
}

// ---------------------- PRÉ-VALIDAÇÃO ----------------------
// Retorna 1 se existirem erros; 0 caso contrário.
static int validarTodosClientes(Cliente clientes[], int nClientes) {
    int temErros = 0;
    for (int i = 0; i < nClientes; ++i) {
        int eNIF = validarNIFDetalhado(clientes[i].nif);
        int eNIB = validarNIBDetalhado(clientes[i].nib);
        if (eNIF != 0) { printNIFErro(eNIF, clientes[i].id); temErros = 1; }
        if (eNIB != 0) { printNIBErro(eNIB, clientes[i].id); temErros = 1; }
    }
    if (temErros) {
        printf("Foram detetados erros de NIF/NIB em pelo menos um cliente. O ficheiro PS2 não será gerado.\n");
        fflush(stdout);
    }
    return temErros;
}

// ---------------------- GERAÇÃO PS2 (Formato Simplificado) ----------------------
void gerarFicheiroPS2(Cliente clientes[], Consumo consumos[], int nClientes, int nConsumos,
                      float preco, int ano, int mes)
{
    // 0) Validação prévia: se houver algum NIF/NIB inválido, aborta e NÃO cria ficheiro
    if (validarTodosClientes(clientes, nClientes)) {
        return;
    }

    // 1) Apurar totais e nº operações
    double total = 0.0;
    int numDebitos = 0;
    for (int i = 0; i < nClientes; i++) {
        for (int j = 0; j < nConsumos; j++) {
            if (consumos[j].idCliente == clientes[i].id &&
                consumos[j].ano == ano && consumos[j].mes == mes)
            {
                total += (double)consumos[j].consumo * (double)preco;
                numDebitos++;
            }
        }
    }

    // 2) Escrever ficheiro no formato simplificado
    char nomeFicheiro[64];
    sprintf(nomeFicheiro, "DD_%d_%02d.ps2", ano, mes);
    FILE *f = fopen(nomeFicheiro, "w");
    if (!f) { printf("Erro ao criar ficheiro PS2\n"); return; }

    double totalCent = total * 100.0; // cêntimos

    // Cabeçalho (Tipo 1) com totais à direita
    fprintf(f, "1%04d%02d%02dIPCA Energy%s501234567%010.0f%05d\n", // 501234567 - NIF da entidade (?)
            ano, mes, DIA_PROC,
            HEADER_PAD,
            totalCent,
            numDebitos);

    // Detalhes (Tipo 2)
    for (int i = 0; i < nClientes; i++) {
        for (int j = 0; j < nConsumos; j++) {
            if (consumos[j].idCliente == clientes[i].id &&
                consumos[j].ano == ano && consumos[j].mes == mes)
            {
                float valor = consumos[j].consumo * preco;
                fprintf(f, "2%010d%s%s%010.0fFatura Eletricidade %02d/%d\n",
                        clientes[i].id,
                        clientes[i].nib,
                        clientes[i].nif,
                        valor * 100.0f, // cêntimos
                        mes, ano);
            }
        }
    }

    // Trailer (Tipo 9)
    fprintf(f, "9%010.0f%05d\n", totalCent, numDebitos);

    fclose(f);
    printf("Ficheiro %s gerado com sucesso!\n", nomeFicheiro);
}

// ---------------------- MAIN ----------------------
int main() {
    // garantir que as mensagens aparecem logo
    setvbuf(stdout, NULL, _IONBF, 0);

    Cliente clientes[MAX_CLIENTES];
    Consumo consumos[MAX_CONSUMOS];
    Periodo periodos[MAX_PERIODOS];

    int nClientes = carregarClientes(clientes);
    int nConsumos = carregarConsumos(consumos);
    int nPeriodos = carregarPeriodos(periodos);

    int ano, mes;
    printf("Introduza o ano: ");
    scanf("%d", &ano);
    printf("Introduza o mes: ");
    scanf("%d", &mes);

    float preco = obterPreco(periodos, nPeriodos, ano, mes);
    if (preco < 0) {
        printf("Preço para %d/%d não encontrado.\n", ano, mes);
        return 1;
    }

    gerarFicheiroPS2(clientes, consumos, nClientes, nConsumos, preco, ano, mes);
    return 0;
}
