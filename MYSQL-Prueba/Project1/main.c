#include <WinSock2.h>
#include <stdio.h>
#include <string.h>
#include <mysql.h>
#include "backend\utils.h"

int main() {

	SOCKET s;
	mysql_library_init(0, NULL, NULL);

	MYSQL* conexion = NULL;
	conectar(&conexion);

	//getAll(&conexion);
	GetById(&conexion);
	InsertValue(&conexion);
	UpdateNombre(&conexion);
	DeleteNombre(&conexion);
	GetAll(&conexion);
	mysql_close(conexion);
	mysql_library_end();
	return 777;
}

