#ifndef ALG_VALID_NIB_H
#define ALG_VALID_NIB_H

/**
 * Validação de NIB (Número de Identificação Bancária - 21 dígitos)
 *
 * Funções expostas:
 *  - verificarNum(const char*): verifica se a string é totalmente numérica.
 *  - modulo97(const char*): calcula (NIB mod 97) de forma incremental.
 *  - validarNIBDetalhado(const char*): validações completas com códigos de erro.
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Retorna 1 se todos os caracteres da string forem dígitos (0-9), senão 0.
 */
int verificarNum(const char *str);

/**
 * Retorna nib % 97, calculado incrementalmente.
 */
int modulo97(const char *nib);

/**
 * Validação completa do NIB (sem side-effects e sem I/O):
 * 0 -> OK
 * 1 -> tamanho incorreto (deve ter 21 dígitos)
 * 2 -> contém caracteres não numéricos
 * 3 -> módulo 97 inválido (esperado == 1)
 */
int validarNIBDetalhado(const char *nib);

#ifdef __cplusplus
}
#endif

#endif // ALG_VALID_NIB_H
