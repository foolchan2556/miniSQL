#include "buffer.h"

vector<record> table;
//save the total information.

//create a new file ./database/<database_name>/<table_name>/table.txt to save the information.
//return 0 for all right. -1 for something wrong.
string where_is_the_table_file;
int buffer_create_table(string database_name, string table_name){
    where_is_the_table_file=".\\database\\";
    where_is_the_table_file+=database_name;
    where_is_the_table_file+="\\";
    where_is_the_table_file+=table_name;
    where_is_the_table_file+="\\";
    where_is_the_table_file+="table.txt";
    ofstream fout (where_is_the_table_file.c_str());
    fout<<"0"<<endl;
    fout.close();
    table.clear();
    return 0;
}


//get all information for the ./database/<database_name>/<table_name>/table.txt, and save into vector<record> table.
//return 0 for all right. -1 for something wrong.
int buffer_open_table(string database_name, string table_name){
    where_is_the_table_file=".\\database\\";
    where_is_the_table_file+=database_name;
    where_is_the_table_file+="\\";
    where_is_the_table_file+=table_name;
    where_is_the_table_file+="\\";
    where_is_the_table_file+="table.txt";
    ifstream fin (where_is_the_table_file.c_str());
    if(fin==NULL){
        cout<<"No such file!"<<endl;
        return -1;
    }
    int num,i,j;
    record tmprecord;
    table.clear();
    fin>>num;
    for(i=0;i<num;i++){
        for(j=0;j<attrib_total_num;j++){
            switch(table_attrib_type[j]){
                case 0: fin>>tmprecord.member[j]._intvalue;break;
                case 1: fin>>tmprecord.member[j]._floatvalue;break;
                case 2: fin>>tmprecord.member[j]._charvalue;break;
            }
        }
        table.push_back(tmprecord);
    }
    fin.close();
    return 0;
}


//save vector<record> table into ./database/<database_name>/<table_name>/table.txt. close the file.
//return 0 for all right. -1 for something wrong.
int buffer_close_table(void){
	if(where_is_the_table_file=="") return 0;
    ofstream fout (where_is_the_table_file.c_str());
    int num,i,j;
    record tmprecord;
    fout<<table.size()<<endl;
    for(i=0;i<table.size();i++){
        for(j=0;j<attrib_total_num;j++){
            switch(table_attrib_type[j]){
                case 0: fout<<table[i].member[j]._intvalue<<endl;break;
                case 1: fout<<table[i].member[j]._floatvalue<<endl;break;
                case 2: fout<<table[i].member[j]._charvalue<<endl;break;
            }
        }
    }
    fout.close();
    return 0;
}
