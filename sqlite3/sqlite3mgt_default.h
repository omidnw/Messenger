// this file just created for test sqlite3 CRUD Control.
// you can delete this file.

#include "sqlite3mgt.h"

struct Sqlite3_MGT Sqlite3_Default =
{
	.condition = "1",
	.databasename = "test.db",
	.tablename = "Cars",
	.columnname = "taghi TEXT,naghi TEXT",
	.value = "paghi,jaghi",
	.column = "",
	.wherecondition = ""
};