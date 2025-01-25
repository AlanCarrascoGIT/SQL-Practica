#include "consultas.h"

char* copiar(char* palabra) {
	char* tmp = malloc(sizeof(char) * strlen(palabra) + 1);
	memcpy(tmp, palabra, strlen(palabra));
	tmp[strlen(palabra)] = '\0';
	return tmp;
}

void mostrar(sqlite3_stmt** stmt) {
	int col = 0;
	while (sqlite3_step(*stmt) == SQLITE_ROW) {
		for (; col < sqlite3_column_count(*stmt); col++) {
			printf("%-20s", sqlite3_column_name(*stmt, col));
		}
		printf("\n");
		for (int i = 0; i < sqlite3_column_count(*stmt); i++) {
			char* data = sqlite3_column_text(*stmt, i);
			printf("%-20s", data);
		}
	}
	printf("\n");
}
//Count = cantidad de filas de la consulta
char** guardar_columna(sqlite3** db, const char* consulta, int columna,int count) {
	sqlite3_stmt* stmt;
	char** data = malloc(sizeof(char*) * count);//liberarlo despues
	int err;
	err=sqlite3_prepare_v2(*db, consulta, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
	}

	for (int i = 0; sqlite3_step(stmt) == SQLITE_ROW;i++ ) {
		data[i] = copiar(sqlite3_column_text(stmt, columna));
	}

	sqlite3_finalize(stmt);
	return data;
}
void mostrar_tablas(sqlite3** db) {
	sqlite3_stmt* stmt;
	char* tablas[10];
	int err;
	int j = 1;
	int col = 0;
	err = sqlite3_prepare_v2(*db, SHOW_TABLES, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
	}
	printf("TABLAS DE LA BASE DE DATOS\n");
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		for (int i = 0; i < sqlite3_column_count(stmt); i++) {
			char* data = sqlite3_column_text(stmt, i);
			printf("%d  %-20s",j, data);
		}
		j++;
		printf("\n");
	}

	sqlite3_finalize(stmt);
}
int cantidad_tablas(sqlite3** db) {
	sqlite3_stmt* stmt;
	int err;
	int count = 0;
	err = sqlite3_prepare_v2(*db, COUNT_TABLES, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
		sqlite3_finalize(stmt);
	}
	err = sqlite3_step(stmt);
	if (err == SQLITE_ERROR) {
		printf("%s\n", sqlite3_errmsg(*db));
		sqlite3_finalize(stmt);
	}
	count = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	return count;
}

char* dar_tabla(sqlite3**db) {
	int s=-1;
	int count = cantidad_tablas(db);
	char buff[22];
	//int len = 0;
	char**tables=NULL;
	char* tabla;
	mostrar_tablas(db);
	printf("Elegir el numero de la tabla que desea ver\n");
	tables = guardar_columna(db, SHOW_TABLES, 0, count);
	leer_entero(buff, &s, sizeof(buff));
	s--;
	printf("TABLA ELEGIDA: %s\n", tables[s]);
	tabla = copiar(tables[s]);
	
	for (int i = 0; i < count; i++) {
		free(tables[i]);
	}

	free(tables);
	return tabla;
}

int cantidad_campos(sqlite3** db,char* table) {
	sqlite3_stmt* stmt;
	int err;
	int count = 0;
	char consulta[128];
	sprintf_s(consulta, sizeof(consulta), CAMPOS_TABLA, table);
	err = sqlite3_prepare_v2(*db, consulta, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
		sqlite3_finalize(stmt);
	}
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		count++;
	}
	sqlite3_finalize(stmt);
	printf("CANTIDAD CAMPOS: %d\n", count);
	return count;
}

char** dar_campo(sqlite3** db, char* table) {
	sqlite3_stmt* stmt;
	char buff[22];
	int s = -1;
	int count = cantidad_campos(db,table);
	char** campos;
	char* campo;
	char consulta[200];
	sprintf_s(consulta, sizeof(consulta), CAMPOS_TABLA, table);
	sqlite3_prepare_v2(*db,consulta,-1,&stmt,NULL);
	mostrar(&stmt);
	sqlite3_finalize(stmt);
	campos = guardar_columna(db, consulta, 1, count);

	printf("Elegir el campo de la tabla que desea\n");
	leer_entero(buff, &s, sizeof(buff));
	s--;
	campo = copiar(campos[s]);
	printf("CAMPO ELEGIDO: %s\n", campo);
	for (int i = 0; i < count; i++) {
		free(campos[i]);
	}

	free(campos);

	return campo;
}

void mostrar_por(sqlite3** db) {
	sqlite3_stmt* stmt;
	int err;
	char buff[22];
	int s = 0;
	char consulta[254] = { '\0' };
	char* tabla = dar_tabla(db);
	char* campo = dar_campo(db, tabla);
	sprintf_s(consulta, sizeof(consulta), BUSCAR_POR, tabla, campo);
	err = sqlite3_prepare_v2(*db, consulta, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
		sqlite3_finalize(stmt);
	}
	//ENLAZAR SEGUN EL TIPO (HACER)
	leer_entero(buff, &s, sizeof(buff));
	sqlite3_bind_int(stmt,1,s);
	mostrar(&stmt);
	sqlite3_finalize(stmt);
	free(tabla);
	free(campo);
}

void insert(sqlite3** db) {
	char buff[81];
	char buff2[81];
	int err;

	sqlite3_stmt* stmt;
	err = sqlite3_prepare_v2(*db, INSERT, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
		sqlite3_close(*db);
	}

	printf("CategoryName "); leer_input_recortado(buff, sizeof(buff));
	printf("Description "); leer_input_recortado(buff2, sizeof(buff2));

	sqlite3_bind_text(stmt, 1, buff,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, buff2,-1, SQLITE_STATIC);
	mostrar(&stmt);
	sqlite3_finalize(stmt);

}

void delete(sqlite3** db) {
	char buff[81];
	int err;
	int id;
	sqlite3_stmt* stmt;
	err = sqlite3_prepare_v2(*db, DELETE_ID, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
		sqlite3_close(*db);
	}
	
	printf("CategoryID "); 
	leer_entero(buff, &id, sizeof(buff));

	sqlite3_bind_int(stmt, 1, id);
	mostrar(&stmt);
	sqlite3_finalize(stmt);

}

void update(sqlite3** db) {
	char buff[81];
	char buff2[81];
	char buffInt[21];
	int err;
	int id;
	sqlite3_stmt* stmt;
	err = sqlite3_prepare_v2(*db, UPDATE, -1, &stmt, NULL);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(*db));
		sqlite3_close(*db);
	}


	printf("CategoryID "); leer_entero(buffInt, &id, sizeof(buffInt));
	printf("CategoryName "); leer_input_recortado(buff, sizeof(buff));
	printf("Description "); leer_input_recortado(buff2, sizeof(buff2));

	sqlite3_bind_text(stmt, 1, buff, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, buff2, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 3, id);
	int rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		printf("Error ejecutando el UPDATE: %s\n", sqlite3_errmsg(*db));
	}
	else {
		int changes = sqlite3_changes(*db);
		if (changes == 0) {
			printf("No se actualizo ninguna fila. Verificar el CategoryID.\n");
		}
		else {
			printf("Actualizacion exitosa.\n");
		}
	}
	sqlite3_finalize(stmt);

}