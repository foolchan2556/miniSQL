#ifndef MAIN_H_k23j23k2n34n445b6h5h4jk2k
#define MAIN_H_k23j23k2n34n445b6h5h4jk2k

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>
using namespace std;

#define	MAX_ATTRIBUTE    255
//define the max number of the attrib.

//////////////////////////for all used//////////////////////////////////////////
class cell{
public:
	int     _intvalue;
	double  _floatvalue;
	string  _charvalue;
};

extern string database_name;

extern string table_name;
//save the int,float,char in one same place.
////////////////////////////////////////////////////////////////////////////////


//////////////////////////for buffer and record used////////////////////////////
class record{
public:
	cell member[MAX_ATTRIBUTE];
};
//save one record.

extern vector<record> table;
//save the all record of the table.

extern string where_is_the_table_file;
//where is the table saved.
////////////////////////////////////////////////////////////////////////////////


//////////////////////////for record and interpreter used///////////////////////
extern set<int> record_select_result;
//save the index in the table for the records select out.

extern int print_len[MAX_ATTRIBUTE];
//for print format.

extern bool if_this_attrib_print[MAX_ATTRIBUTE];
//save for select to define which attrib to be print.
////////////////////////////////////////////////////////////////////////////////


//////////////////////////for catalog mainly used///////////////////////////////
class catalog_attrib{
public:
	string  catalog_attrib_name;
	int     catalog_attrib_type;
	int     catalog_attrib_stringlen;
	bool    catalog_attrib_ifprime;
	bool    catalog_attrib_ifunique;
	bool    catalog_attrib_ifhaveindex;
};

extern int table_attrib_type[MAX_ATTRIBUTE];
//save the type for every attrib. 0 for int, 1 for float, 2 for string.

extern int attrib_total_num;
//save the num of the attrib for the table.

extern catalog_attrib catalog_attrib_all[MAX_ATTRIBUTE];

extern string where_is_the_catalog_file;
//where is the catalog saved.
////////////////////////////////////////////////////////////////////////////////

//////////////////////////for index mainly used/////////////////////////////////
class index_node{
public:
	bool leaf;
	short count;
	cell node_value[10];
	int node_point[11];
};

extern index_node index_all[];
//for everynode int node_point[i]:
//if node_point[i]>0 : node_point[i] is the index in index_all[] which value is smaller than node_value[i];
//if node_point[i]<0 : node_point[i] is the index in vector<record> table value equal to node_value[i].
//index_all[0] is the root, so node_point[i]==0(FATHER) means end.
//and node_point[i]>1000 means useless for already delete.

extern int index_type;
//the type for the value of the cell in node.

extern int index_used_num;
//the total num of the index_all[] which have been used.

extern int index_last_used;

extern int index_i_last_used;
//the last index_search result.

extern string where_is_the_index_file;
//where is the index saved.
////////////////////////////////////////////////////////////////////////////////

#endif
