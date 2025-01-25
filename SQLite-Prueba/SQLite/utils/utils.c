#include "utils.h"

void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		// Vacía el buffer leyendo todos los caracteres hasta encontrar un salto de línea o EOF
	}
}
void leer_entero(char* buffer, int* id, size_t buffTamanio) {
	errno = 0;
	char* endptr; 
	printf("Ingresa el numero: ");

	// Leer la entrada
	if (fgets(buffer, buffTamanio, stdin) == NULL) {
		printf("Error al leer la entrada\n"); 
		return;
	}

	if (strchr(buffer, '\n') == NULL) {
		limpiarBuffer(); 
		printf("La entrada excedio el limite del buffer y ha sido limpiada.\n");
		return;
	}
	else {
		buffer[strcspn(buffer, "\n")] = '\0'; 
	}

	long resultado = strtol(buffer, &endptr, 10);

	if (*endptr != '\0') {
		printf("No valida: contiene caracteres no numericos.\n");
		return;
	}

	if (errno == ERANGE || resultado > INT_MAX || resultado < INT_MIN) {
		printf("El valor ingresado esta fuera del rango permitido para un entero.\n");
		return;
	}

	*id = resultado;
	printf("Numero ingresado correctamente: %d\n", *id);
}
void leer_input_recortado(char* buffer, size_t tamanio) {
	printf("Ingrese el tabla: ");
	if (fgets(buffer, tamanio, stdin) != NULL) {
		size_t len = strlen(buffer);

		// Si el salto de línea está presente, lo eliminamos
		if (len > 0 && buffer[len - 1] == '\n') {
			buffer[len - 1] = '\0'; // Reemplaza el '\n' con '\0'
		}
		else {
			// Si no hay '\n', significa que la entrada excedió el tamaño del buffer
			limpiarBuffer(); // Vacía el resto del buffer
		}
	}
	else {
		printf("Error al leer la entrada.\n");
	}
}
