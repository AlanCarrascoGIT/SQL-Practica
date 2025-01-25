#pragma once
#ifndef _CONSULTAS_
#define _CONSULTAS

#include <sqlite3.h>
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>
#define SELECT_ID "SELECT * FROM Categories WHERE CategoryID = ?1"
#define SELECT_TABLE "SELECT * FROM %s"
#define BUSCAR_POR "SELECT * FROM %s WHERE %s = ?1"
#define CAMPOS_TABLA "PRAGMA table_info(%s)"
#define SHOW_TABLES "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'"
#define COUNT_TABLES "SELECT COUNT(name) FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%'"
#define INSERT "INSERT INTO	Categories (CategoryName, Description) VALUES (?1, ?2)" 
#define TABLA(n) printf("Tabla Ingresada \"%s\"\n", (char*)n);
#define DELETE_ID "DELETE FROM Categories WHERE CategoryID = ?1"
#define UPDATE "UPDATE Categories SET CategoryName = ?1, Description = ?2 WHERE CategoryID = ?3"

typedef enum {
	Categories=1,
	Customers,
	Employees,
	Shippers,
	Suppliers,
	Products,
	Orders,
	OrderDetails
}op_code;


void insert(sqlite3** db);
int cantidad_campos(sqlite3** db,char* table);
void mostrar_tablas(sqlite3** db); 
void delete(sqlite3** db);
char* dar_tabla(sqlite3** db);
void mostrar_por(sqlite3** db);
char** dar_campo(sqlite3** db,char* table);
void update(sqlite3** db);
int cantidad_tablas(sqlite3** db);
#endif
