#include "Catalog.h"

catalog_attrib catalog_attrib_all[MAX_ATTRIBUTE];

string where_is_the_catalog_file;

int attrib_total_num;

int table_attrib_type[MAX_ATTRIBUTE];

//create a new file ./database/<database_name>/<table_name>/catalog.txt to save the information.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_catalog_file.
int catalog_create_attrib_file(string database_name, string table_name)
{
	where_is_the_catalog_file = ".\\database\\";
	where_is_the_catalog_file += database_name;
	where_is_the_catalog_file += "\\";
	where_is_the_catalog_file += table_name;
	where_is_the_catalog_file += "\\";
	where_is_the_catalog_file += "catalog.txt";
	ofstream fout(where_is_the_catalog_file.c_str());
	fout << "0" << endl;
	fout.close();
	attrib_total_num = 0;
	return 0;
}

//get all attrib information for the ./database/<database_name>/<table_name>/catalog.txt, and save into catalog_attrib_all.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_catalog_file.
int catalog_open_file(string database_name, string table_name)
{
	where_is_the_catalog_file = ".\\database\\";
	where_is_the_catalog_file += database_name;
	where_is_the_catalog_file += "\\";
	where_is_the_catalog_file += table_name;
	where_is_the_catalog_file += "\\";
	where_is_the_catalog_file += "catalog.txt";
	ifstream fin(where_is_the_catalog_file.c_str());
	attrib_total_num = 0;

	if(fin == NULL)
	{
		cout << "No such file!" << endl;
		return -1;
	}

	fin >> attrib_total_num;

	for(int i = 0; i < attrib_total_num; i++)
	{
		fin >> catalog_attrib_all[i].catalog_attrib_name;
		fin >> catalog_attrib_all[i].catalog_attrib_type;
		fin >> catalog_attrib_all[i].catalog_attrib_stringlen;
		fin >> catalog_attrib_all[i].catalog_attrib_ifprime;
		fin >> catalog_attrib_all[i].catalog_attrib_ifunique;
		fin >> catalog_attrib_all[i].catalog_attrib_ifhaveindex;
		table_attrib_type[i] = catalog_attrib_all[i].catalog_attrib_type;
	}

	fin.close();
	return 0;
}

//save catalog_attrib_all into ./database/<database_name>/<table_name>/catalog.txt. close the file.
//return 0 for all right. -1 for something wrong.
//int catalog_close_file(string database_name, string table_name){
int catalog_close_file(void)
{
	if(where_is_the_catalog_file == "") return 0;

	/*  where_is_the_catalog_file=".\\"";
	  where_is_the_catalog_file+=database_name;
	  where_is_the_catalog_file+="\\";
	  where_is_the_catalog_file+=table_name;
	  where_is_the_catalog_file+="\\";
	  where_is_the_catalog_file+="catalog.txt";*/
	ofstream fout(where_is_the_catalog_file.c_str());
	fout << attrib_total_num << endl;

	for(int i = 0; i < attrib_total_num; i++)
	{
		fout << catalog_attrib_all[i].catalog_attrib_name << ' ';
		fout << catalog_attrib_all[i].catalog_attrib_type << ' ';
		fout << catalog_attrib_all[i].catalog_attrib_stringlen << ' ';
		fout << catalog_attrib_all[i].catalog_attrib_ifprime << ' ';
		fout << catalog_attrib_all[i].catalog_attrib_ifunique << ' ';
		fout << catalog_attrib_all[i].catalog_attrib_ifhaveindex << endl;
	}

	fout.close();
	where_is_the_catalog_file = "";
	return 0;
}

//delete the file ./database/<database_name>/<table_name>/catalog.txt.
//return 0 for all right. -1 for something wrong.

/*int catalog_delete_file(string database_name, string table_name){
  where_is_the_catalog_file=".\\"";
  where_is_the_catalog_file+=database_name;
  where_is_the_catalog_file+="\\";
  where_is_the_catalog_file+=table_name;
  where_is_the_catalog_file+="\\";
  where_is_the_catalog_file+="catalog.txt";
  where_is_the_catalog_file="del "+where_is_the_catalog_file;
  system(where_is_the_catalog_file.c_str());
  return 0;
}*/

//add a new attrib into catalog_attrib_all.
//return 0 for all right. -1 for something wrong.
void catalog_add_attrib(string attrib_name, int attrib_type, int attrib_stringlen)
{
	catalog_attrib_all[attrib_total_num].catalog_attrib_name = attrib_name;
	catalog_attrib_all[attrib_total_num].catalog_attrib_type = attrib_type;
	catalog_attrib_all[attrib_total_num].catalog_attrib_stringlen = attrib_stringlen;
	catalog_attrib_all[attrib_total_num].catalog_attrib_ifprime = false;
	catalog_attrib_all[attrib_total_num].catalog_attrib_ifunique = false;
	catalog_attrib_all[attrib_total_num].catalog_attrib_ifhaveindex = false;
	attrib_total_num++;
}

//retrun the index in the catalog_attrib_all for the arrtib with attrib_name as name.
//return -1 for not find.
int catalog_get_attrib_index(string attrib_name)
{
	for(int i = 0; i < attrib_total_num; i++)
		if(catalog_attrib_all[i].catalog_attrib_name == attrib_name)
			return i;

	return -1;
}

//set the prime atrrib by the index.
//return 0 for all right. -1 for something wrong.
int catalog_set_prime(int attrib_index)
{
	if(attrib_index >= attrib_total_num) return -1;

	catalog_attrib_all[attrib_index].catalog_attrib_ifprime = true;
	catalog_attrib_all[attrib_index].catalog_attrib_ifunique = true;
	catalog_attrib_all[attrib_total_num].catalog_attrib_ifhaveindex = true;
	return 0;
}

//set the unique atrrib by the index.
//return 0 for all right. -1 for something wrong.
int catalog_set_unique(int attrib_index)
{
	if(attrib_index >= attrib_total_num) return -1;

	catalog_attrib_all[attrib_index].catalog_attrib_ifunique = true;
	return 0;
}

//set the have-index atrrib by the index.
//return 0 for all right. -1 for something wrong.
int catalog_set_haveindex(int attrib_index)
{
	if(attrib_index >= attrib_total_num) return -1;

	catalog_attrib_all[attrib_index].catalog_attrib_ifhaveindex = true;
	return 0;
}

//unset the have-index atrrib by the index.
//return 0 for all right. -1 for something wrong.
int catalog_unset_haveindex(int attrib_index)
{
	if(attrib_index >= attrib_total_num) return -1;

	catalog_attrib_all[attrib_index].catalog_attrib_ifhaveindex = false;
	return 0;
}


//retrun the type for the attrib by the index.
//return -1 for something wrong.
int catalog_get_type(int attrib_index)
{
	if(attrib_index >= attrib_total_num) return -1;

	return table_attrib_type[attrib_index];
}


//retrun the stringlen for the attrib by the index.
//return -1 for something wrong.
int catalog_get_stringlen(int attrib_index)
{
	if(attrib_index >= attrib_total_num) return -1;

	return catalog_attrib_all[attrib_index].catalog_attrib_stringlen;
}

//retrun ifprime for the attrib by the index.
bool catalog_get_ifprime(int attrib_index)
{
	return catalog_attrib_all[attrib_index].catalog_attrib_ifprime;
}

//retrun ifunique for the attrib by the index.
bool catalog_get_ifunique(int attrib_index)
{
	return catalog_attrib_all[attrib_index].catalog_attrib_ifunique;
}

//retrun ifhaveindex for the attrib by the index.
bool catalog_get_ifhaveindex(int attrib_index)
{
	return catalog_attrib_all[attrib_index].catalog_attrib_ifhaveindex;
}
