#ifndef CATALOG_H_k3kmv8vd7vdfsjfkjfds99
#define CATALOG_H_k3kmv8vd7vdfsjfkjfds99

#include "main.h"



int catalog_create_attrib_file(string database_name, string table_name);
//create a new file ./database/<database_name>/<table_name>/catalog.txt to save the information.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_catalog_file.


int catalog_open_file(string database_name, string table_name);
//get all attrib information for the ./database/<database_name>/<table_name>/catalog.txt, and save into catalog_attrib_all.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_catalog_file.


int catalog_close_file(void);
//save catalog_attrib_all into ./database/<database_name>/<table_name>/catalog.txt. close the file.
//return 0 for all right. -1 for something wrong.


void catalog_add_attrib(string attrib_name, int attrib_type, int attrib_stringlen);
//add a new attrib into catalog_attrib_all.
//return 0 for all right. -1 for something wrong.


int catalog_get_attrib_index(string attrib_name);
//retrun the index in the catalog_attrib_all for the arrtib with attrib_name as name.
//return -1 for not find.


int catalog_set_prime(int attrib_index);
//set the prime atrrib by the index.
//return 0 for all right. -1 for something wrong.


int catalog_set_unique(int attrib_index);
//set the unique atrrib by the index.
//return 0 for all right. -1 for something wrong.


int catalog_set_haveindex(int attrib_index);
//set the have-index atrrib by the index.
//return 0 for all right. -1 for something wrong.


int catalog_unset_haveindex(int attrib_index);
//unset the have-index atrrib by the index.
//return 0 for all right. -1 for something wrong.


int catalog_get_type(int attrib_index);
//retrun the type for the attrib by the index.
//return -1 for something wrong.


int catalog_get_stringlen(int attrib_index);
//retrun the stringlen for the attrib by the index.
//return -1 for something wrong.


bool catalog_get_ifprime(int attrib_index);
//retrun ifprime for the attrib by the index.


bool catalog_get_ifunique(int attrib_index);
//retrun ifunique for the attrib by the index.


#endif
