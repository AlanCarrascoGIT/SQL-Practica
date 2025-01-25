#ifndef _UTILS_
#define _UTILS_
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

void limpiarBuffer();
void leer_entero(char* buffer, int* id, size_t buffTamanio);
void leer_input_recortado(char* buffer, size_t tamanio);

#endif