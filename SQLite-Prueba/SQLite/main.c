#include <stdio.h>
#include "consultas.h"

int main() {
	sqlite3* db;
	int err;
	err = sqlite3_open("../../../Test-SQLITE/Prueba_SQL.db", &db);
	if (err != SQLITE_OK) {
		printf("%s\n",sqlite3_errmsg(db));
	}
	mostrar_por(&db);
	//cantidad_campos(&db);
	//mostrar_id(&db);
	err = sqlite3_close(db);
	if (err != SQLITE_OK) {
		printf("%s\n", sqlite3_errmsg(db));
	}
	return 0;
}