#include <stdio.h>
#include "..\backend\utils.h"
#include <limits.h>
/*result = mysql_list_dbs(conexion, NULL);
while (fila = mysql_fetch_row(result))printf("database: %s\n", fila[0]);*/

#define PASSWORD "****"
#define MOSTRAR_TABLA "SELECT * FROM muestra"
#define BUSCAR_ID "SELECT * FROM muestra where idmuestra = ?"
#define BUFFER_SIZE 51 //el tamanio permitido - 1 para borrar el '\n'
#define INSERT_TABLA "INSERT INTO muestra (nombre) VALUES (?)"
#define UPDATE_NOMBRE "UPDATE muestra SET nombre = ? WHERE idmuestra = ?"
#define DELETE_USER "DELETE FROM muestra WHERE nombre = ?"

void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {
		// Vacía el buffer leyendo todos los caracteres hasta encontrar un salto de línea o EOF
	}
}

void leer_entero(char* buffer, int* id,size_t tamanio) {
	errno = 0;
	char* endptr; // Puntero para verificar errores de conversión
	printf("Ingresa el numero: ");

	// Leer la entrada
	if (fgets(buffer, tamanio, stdin) == NULL) {
		printf("Error al leer la entrada\n"); // Error al leer la entrada
		return;
	}

	// Verificar si se ingresó más texto del que cabe en el buffer
	if (strchr(buffer, '\n') == NULL) {
		limpiarBuffer(); // Limpiar caracteres sobrantes del buffer
		printf("La entrada excedio el limite del buffer y ha sido limpiada.\n");
		return;
	}
	else {
		buffer[strcspn(buffer, "\n")] = '\0'; // Reemplazar el '\n' por '\0'
	}

	// Convertir la cadena a un entero usando strtol
	long resultado = strtol(buffer, &endptr, 10);

	// Verificar si la conversión contiene caracteres no numéricos
	if (*endptr != '\0') {
		printf("No valida: contiene caracteres no numericos.\n");
		return;
	}

	// Validar si el resultado está fuera del rango de un entero
	if (errno == ERANGE || resultado > INT_MAX || resultado < INT_MIN) {
		printf("El valor ingresado esta fuera del rango permitido para un entero.\n");
		return;
	}

	// Asignar el resultado si es válido
	*id = (int)resultado;
	printf("Numero ingresado correctamente: %d\n", *id);
}



void leer_input_recortado(char* buffer, size_t tamanio) {
	printf("Ingrese el nombre: ");
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

void mostrar_consulta(MYSQL_STMT** stmt,MYSQL** conexion) {
	int cantColum;
	MYSQL_RES* result;
	MYSQL_BIND param2[2];
	MYSQL_FIELD* col;
	unsigned long length[2];
	int           int_data;
	char          str_data[BUFFER_SIZE];
	bool          is_null[2];
	unsigned int rows = 0;

	result = mysql_stmt_result_metadata(*stmt);

	if (!result)
	{
		fprintf(stderr,
			" mysql_stmt_result_metadata(), \
           returned no meta information\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(*stmt));
		exit(0);
	}

	while (col = mysql_fetch_field(result)) printf("%15s", col->name);
	printf("\n");

	memset(param2, 0, sizeof(param2));

	param2[0].buffer_type = MYSQL_TYPE_LONG;
	param2[0].buffer = &int_data;
	param2[0].is_null = &is_null[0];
	param2[0].length = &length[0];

	/* STRING COLUMN */
	param2[1].buffer_type = MYSQL_TYPE_STRING;
	param2[1].buffer = str_data;
	param2[1].buffer_length = BUFFER_SIZE;
	param2[1].is_null = &is_null[1];
	param2[1].length = &length[1];

	if (mysql_stmt_bind_result(*stmt, param2)) {
		fprintf(stderr, " mysql_stmt_bind_result() failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(*stmt));
		exit(0);
	}

	while (!mysql_stmt_fetch(*stmt))
	{
		rows++;
		/* column 1 */
		fprintf(stdout, "%15d", int_data);

		/* column 2 */
		fprintf(stdout, "%15s", str_data);

		fprintf(stdout, "\n");
	}
	if (rows == 0) {
		printf("No se encontraron resultados\n");
	}
	else
		printf("Filas afectadas: %u\n", rows);


	mysql_free_result(result);

	if (mysql_stmt_close(*stmt)) {
		/* mysql_stmt_close() invalidates stmt, so call          */
		/* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_error(*conexion));
		exit(0);
	}

	/*
	if (result) {
		cantColum = mysql_num_fields(result);

		while (columnas = mysql_fetch_field(result)) printf("%15s", columnas->name);
		printf("\n");

		while (fila = mysql_fetch_row(result)) {
			for (int i = 0; i < cantColum; i++) {
				printf("%15s", fila[i]);
			}
			printf("\n");
		}
		printf("CANTIDAD DE FILAS AFECTADAS %u", mysql_affected_rows(*conexion));
	}
	else {
		// Si no es una consulta SELECT, verificar si hubo un error
		if (mysql_field_count(*conexion) == 0) {
			printf("Consulta ejecutada correctamente. Filas afectadas: %lld\n", mysql_affected_rows(*conexion));
		}
		else {
			fprintf(stderr, "Error al obtener resultados: %s\n", mysql_error(*conexion));
		}
	}*/
	
}

void GetAll(MYSQL** conexion){
	MYSQL_BIND param[2];
	MYSQL_STMT* stmt;

	stmt = mysql_stmt_init(*conexion);

	if (mysql_stmt_prepare(stmt, MOSTRAR_TABLA, strlen(MOSTRAR_TABLA))) {
		fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	mostrar_consulta(&stmt, *conexion);
	

	/*
	if (mysql_query(*conexion, "SELECT * FROM muestra")) {
		fprintf(stderr, "Error en la consulta SQL: %s\n", mysql_error(*conexion));
		mysql_close(*conexion);
		exit(EXIT_FAILURE);
	}
	result = mysql_store_result(*conexion);*/
		
}


void GetById(MYSQL** conexion) {
	MYSQL_STMT* stmt;
	int id;
	MYSQL_RES* result;
	MYSQL_BIND param[1];
	char buff[20];/*
	MYSQL_BIND param2[2];
	MYSQL_FIELD* col;
	unsigned long length[2];
	int           int_data;
	char          str_data[BUFFER_SIZE];
	bool          is_null[2];
	bool          error[2];
	int cant;*/

	stmt = mysql_stmt_init(*conexion);

	if (!stmt) {
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}

	if (mysql_stmt_prepare(stmt, BUSCAR_ID, strlen(BUSCAR_ID))) {
		fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_LONG;
	param[0].buffer = &id;
	param[0].length = 0;
	param[0].is_null = 0;


	if (mysql_stmt_bind_param(stmt, param)) {
		fprintf(stderr, " mysql_stmt_bind_param(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	leer_entero(buff,&id,sizeof(buff));

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	mostrar_consulta(&stmt, conexion);
}

void InsertValue(MYSQL** conexion){
	MYSQL_STMT* stmt;
	MYSQL_BIND param[1];
	char nombre[BUFFER_SIZE];
	unsigned int len;
	stmt = mysql_stmt_init(*conexion);

	if (!stmt) {
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}

	if (mysql_stmt_prepare(stmt, INSERT_TABLA, strlen(INSERT_TABLA))) {
		fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	memset(param, 0, sizeof(param));

	param[0].buffer_type = MYSQL_TYPE_STRING;
	param[0].buffer = nombre;
	param[0].buffer_length = BUFFER_SIZE;
	param[0].length = &len;
	param[0].is_null = 0;

	if (mysql_stmt_bind_param(stmt, param)) {
		fprintf(stderr, " mysql_stmt_bind_param(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	printf("(INSERT) ");
	leer_input_recortado(nombre,sizeof(nombre));
	len = strlen(nombre);

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	

	int affected_rows;
	affected_rows = mysql_stmt_affected_rows(stmt);
	fprintf(stdout, "total affected rows: %d\n",affected_rows);

	if (affected_rows != 1) /* validate affected rows */
	{
		fprintf(stderr, " invalid affected rows by MySQL\n");
		exit(0);
	}

	if (mysql_stmt_close(stmt)) {
		/* mysql_stmt_close() invalidates stmt, so call          */
		/* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_error(*conexion));
		exit(0);
	}
}

void UpdateNombre(MYSQL** conexion) {
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[2];
	long id;
	char nombre[BUFFER_SIZE];
	char buff[20];
	unsigned long len;

	stmt = mysql_stmt_init(*conexion);
	if (!stmt) {
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}

	if (mysql_stmt_prepare(stmt, UPDATE_NOMBRE, strlen(UPDATE_NOMBRE))) {
		fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = nombre;
	bind[0].buffer_length = BUFFER_SIZE;
	bind[0].length = &len;
	bind[0].is_null = 0;

	bind[1].buffer_type = MYSQL_TYPE_LONG;
	bind[1].buffer = &id;
	bind[1].is_null = 0;
	bind[1].length = 0;

	if (mysql_stmt_bind_param(stmt, bind)) {
		fprintf(stderr, " mysql_stmt_bind_param(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}
	
	
	printf("(UPDATE) ");
	leer_input_recortado(nombre, sizeof(nombre));
	len = strlen(nombre);
	
	printf("(UPDATE) ");
	leer_entero(buff, &id, sizeof(buff));

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	int affected_rows;
	affected_rows = mysql_stmt_affected_rows(stmt);
	fprintf(stdout, "total affected rows: %d\n", affected_rows);

	if (affected_rows != 1) /* validate affected rows */
	{
		fprintf(stderr, " invalid affected rows by MySQL\n");
		exit(0);
	}

	if (mysql_stmt_close(stmt)) {
		/* mysql_stmt_close() invalidates stmt, so call          */
		/* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_error(*conexion));
		exit(0);
	}

}

void DeleteNombre(MYSQL** conexion) {
	MYSQL_STMT* stmt;
	MYSQL_BIND bind[1];
	char nombre[BUFFER_SIZE];
	unsigned long len;

	stmt = mysql_stmt_init(*conexion);
	if (!stmt) {
		fprintf(stderr, " mysql_stmt_init(), out of memory\n");
		exit(0);
	}

	if (mysql_stmt_prepare(stmt, DELETE_USER, strlen(DELETE_USER))) {
		fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	memset(bind, 0, sizeof(bind));

	bind[0].buffer_type = MYSQL_TYPE_STRING;
	bind[0].buffer = nombre;
	bind[0].buffer_length = BUFFER_SIZE;
	bind[0].length = &len;
	bind[0].is_null = 0;

	if (mysql_stmt_bind_param(stmt, bind)) {
		fprintf(stderr, " mysql_stmt_bind_param(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}


	printf("(DELETE) ");
	leer_input_recortado(nombre, sizeof(nombre));
	len = strlen(nombre);

	if (mysql_stmt_execute(stmt)) {
		fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
		fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
		exit(0);
	}

	int affected_rows;
	affected_rows = mysql_stmt_affected_rows(stmt);
	fprintf(stdout, "total affected rows: %d\n", affected_rows);

	if (affected_rows != 1) /* validate affected rows */
	{
		fprintf(stderr, " invalid affected rows by MySQL\n");
		exit(0);
	}

	if (mysql_stmt_close(stmt)) {
		/* mysql_stmt_close() invalidates stmt, so call          */
		/* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
		fprintf(stderr, " failed while closing the statement\n");
		fprintf(stderr, " %s\n", mysql_error(*conexion));
		exit(0);
	}
	
}

void conectar(MYSQL** conexion) {
	
	*conexion = mysql_init(NULL);
	if (*conexion == NULL) {
		fprintf(stderr, "Error al inicializar MySQL: %s\n", mysql_error(*conexion));
		exit(EXIT_FAILURE);
	}
	if (mysql_real_connect(*conexion, HOST, USER, PASSWORD, DATABASE, PORT, NULL, 0) == NULL) {
		fprintf(stderr, "Error al conectar a MySQL: %s\n", mysql_error(*conexion));
		mysql_close(*conexion);
		exit(EXIT_FAILURE);
	}

	printf("Se conecto a la base de datos \"%s\"\n", (*conexion)->db);
}