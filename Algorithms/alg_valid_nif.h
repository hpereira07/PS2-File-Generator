#ifndef ALG_VALID_NIF_H
#define ALG_VALID_NIF_H

/**
 * Validação de NIF (Número de Identificação Fiscal - Portugal)
 *
 * Funções expostas:
 *  - verif(const char*): valida o dígito de controlo (retorna 1 válido, 0 inválido).
 *  - validarNIFDetalhado(const char*): validações completas com códigos de erro.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Retorna 1 se o NIF for válido segundo o algoritmo do dígito de controlo,
 * caso contrário 0. Assume uma string com 9 dígitos numéricos.
 */
int verif(const char *nif);

/**
 * Validação completa do NIF (sem side-effects e sem I/O):
 * 0 -> OK
 * 1 -> tamanho incorreto (deve ter 9 dígitos)
 * 2 -> contém caracteres não numéricos
 * 3 -> primeiro dígito inválido
 * 4 -> dígito de controlo inválido
 */
int validarNIFDetalhado(const char *nif);

#ifdef __cplusplus
}
#endif

#endif // ALG_VALID_NIF_H
