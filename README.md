# SQL-Practica: bases de datos desde C

Dos aplicaciones de consola en C para practicar el acceso a bases de datos con sus APIs nativas:

- **MySQL** con la **MySQL C API** (`libmysqlclient`)
- **SQLite** con la biblioteca **`sqlite3`**

Las dos usan **consultas preparadas** (*prepared statements*) con parámetros en lugar de armar el SQL concatenando texto, que es la forma correcta de evitar la inyección SQL.

## Estructura

```
SQL-Practica/
├── MYSQL-Prueba/Project1/
│   ├── main.c              # Ejecuta las operaciones CRUD en orden
│   └── backend/
│       ├── utils.c         # Conexión, consultas preparadas y lectura de entrada
│       └── utils.h         # Configuración de la conexión
└── SQLite-Prueba/SQLite/
    ├── main.c              # Abre la base y lanza la búsqueda interactiva
    ├── consultas.c         # Listado de tablas/campos, búsqueda, INSERT/UPDATE/DELETE
    ├── consultas.h         # Consultas SQL
    └── utils/              # Lectura y validación de la entrada del usuario
```

---

## Proyecto MySQL

CRUD completo sobre una tabla `muestra` usando la MySQL C API:

| Operación | Función        | Consulta                                              |
|-----------|----------------|-------------------------------------------------------|
| Leer uno  | `GetById`      | `SELECT * FROM muestra WHERE idmuestra = ?`           |
| Crear     | `InsertValue`  | `INSERT INTO muestra (nombre) VALUES (?)`             |
| Modificar | `UpdateNombre` | `UPDATE muestra SET nombre = ? WHERE idmuestra = ?`   |
| Borrar    | `DeleteNombre` | `DELETE FROM muestra WHERE nombre = ?`                |
| Leer todo | `GetAll`       | `SELECT * FROM muestra`                               |

Los datos del usuario se leen con `fgets` y se validan antes de usarlos (largo máximo, solo números en los IDs y rango de `int`).

---

## Proyecto SQLite

Explorador interactivo de una base SQLite:

1. Lista las tablas de la base (consultando `sqlite_master`).
2. El usuario elige una tabla y el programa muestra sus columnas (`PRAGMA table_info`).
3. El usuario elige una columna e ingresa un valor numérico.
4. El programa muestra las filas que coinciden.

Las tablas y columnas se obtienen de la propia base en tiempo de ejecución, así que funciona con cualquier base sin cambiar el código. También incluye funciones de **INSERT**, **UPDATE** y **DELETE** con parámetros enlazados (`sqlite3_bind_*`) sobre la tabla `Categories`.

---

## Conceptos aplicados

SQL (SELECT, INSERT, UPDATE, DELETE) · consultas preparadas y enlace de parámetros · prevención de inyección SQL · uso de APIs nativas de bases de datos en C · lectura de metadatos (tablas y columnas) · validación de entrada del usuario · manejo manual de memoria.
