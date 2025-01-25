#ifndef _UTILS_
#define _UTILS_

#include <mysql.h>
#define HOST "localhost"
#define USER "root"
#define DATABASE "prueba"
#define PORT 3306


void conectar(MYSQL** conexion);
void GetById(MYSQL** conexion);
void GetAll(MYSQL** conexion);
void DeleteNombre(MYSQL** conexion);
void InsertValue(MYSQL** conexion);
void UpdateNombre(MYSQL** conexion);
void mostrar_consulta(MYSQL_STMT** result, MYSQL** conexion);


#endif // _UTILS_

