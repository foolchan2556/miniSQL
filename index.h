#ifndef INDEX_H_n83n8c84noiwwcrn483nc8n3
#define INDEX_H_n83n8c84noiwwcrn483nc8n3

#include "main.h"

#define FATHER 0

int index_clear(void);
//initialize the index.

int index_creat_file(string database_name, string table_name, string index_name, int attrib_index);
//create a new file ./database/<database_name>/<table_name>/index/<index_name>.txt to save the index.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_catalog_file.

int index_open_file(string database_name, string table_name, string index_name, int attrib_index);
//open the file ./database/<database_name>/<table_name>/index/<index_name>.txt to read the index.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_index_file.

int index_close_file(void);
//save and close the file ./database/<database_name>/<table_name>/index/<index_name>.txt
//return 0 for all right. -1 for something wrong.

int index_delete_file(string database_name, string table_name, string index_name, int attrib_index);
//delete the index of the index_name.
//return 0 for all right. -1 for something wrong.

int index_find(cell& temp_cell);
//return the index in vector<record> table by value is cell.
//return -1 for something wrong.

int index_insert(cell& temp_cell, int index_in_table);
//insert the index in vector<record> table by value is cell.
//return 0 for all right. -1 for something wrong.

int index_delete(cell& temp_cell, int index_in_table);
//delete the index in vector<record> table by value is cell.
//return 0 for all right. -1 for something wrong.

int index_smallest(void);
//return the smallest cell index in the vector<record> table.
//return -1 for something wrong.

int index_next(void);
//return the next value index in the vector<record> table.
//return -1 for something wrong.

#endif
