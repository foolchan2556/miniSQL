#include "index.h"

index_node index_all[100000];

int index_type;
//the type for the value of the cell in node.
int index_used_num;
//the total num of the index_all[] which have been used.
int index_last_used;

int index_i_last_used;

string where_is_the_index_file;

//initialize the index.
int index_clear(void)
{
	index_all[FATHER].node_point[0] = 1;
	index_all[1].leaf = true;
	index_all[1].count = 0;
	index_all[1].node_point[0] = FATHER;
	index_last_used = -1;
	index_i_last_used = -1;
	index_used_num = 1;
	return 0;
}


//create a new file ./database/<database_name>/<table_name>/<index_name>_index.txt to save the index.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_catalog_file.
int index_creat_file(string database_name, string table_name, string index_name, int attrib_index)
{
	where_is_the_index_file = ".\\database\\";
	where_is_the_index_file += database_name;
	where_is_the_index_file += "\\";
	where_is_the_index_file += table_name;
	where_is_the_index_file += "\\";
	where_is_the_index_file += index_name;
	where_is_the_index_file += "_index.txt";
	ofstream fout(where_is_the_index_file.c_str());
	fout << endl;
	fout.close();
	index_clear();
	index_type = table_attrib_type[attrib_index];
	vector<record>::iterator iter;
	int i;

	for(iter = table.begin(), i = 0; iter != table.end(); iter++, i++) index_insert((*iter).member[attrib_index], i);

	catalog_attrib_all[attrib_index].catalog_attrib_ifhaveindex = true;
	return 0;
}


//open the file ./database/<database_name>/<table_name>/<index_name>_index.txt to read the index.
//return 0 for all right. -1 for something wrong.
//save the place of the file in where_is_the_index_file.
int index_open_file(string database_name, string table_name, string index_name, int attrib_index)
{
	where_is_the_index_file = ".\\database\\";
	where_is_the_index_file += database_name;
	where_is_the_index_file += "\\";
	where_is_the_index_file += table_name;
	where_is_the_index_file += "\\";
	where_is_the_index_file += index_name;
	where_is_the_index_file += "_index.txt";
//cout<<where_is_the_index_file<<endl;
	ifstream fin(where_is_the_index_file.c_str());

	if(fin == NULL)
	{
		cout << "No such file!" << endl;
		return -1;
	}

	string tmpstr;
	index_type = table_attrib_type[attrib_index];
	index_clear();
	fin >> tmpstr >> index_used_num;
//cout<<index_used_num<<endl;
	fin >> tmpstr >> index_all[FATHER].node_point[0];
	int i, j;

	for(i = 1; i <= index_used_num; i++)
	{
		fin >> tmpstr >> tmpstr >> index_all[i].leaf >> tmpstr >> index_all[i].count;

		for(j = 0; j < index_all[i].count; j++)
		{
			fin >> tmpstr >> index_all[i].node_point[j];

			switch(index_type)
			{
			case 0:
				fin >> tmpstr >> index_all[i].node_value[j]._intvalue;
				break;

			case 1:
				fin >> tmpstr >> index_all[i].node_value[j]._floatvalue;
				break;

			case 2:
				fin >> tmpstr >> index_all[i].node_value[j]._charvalue;
				break;
			}
		}

		fin >> tmpstr >> index_all[i].node_point[index_all[i].count];
	}

	fin.close();
	return 0;
}


//save and close the file ./database/<database_name>/<table_name>/<index_name>_index.txt
//return 0 for all right. -1 for something wrong.
int index_close_file(void)
{
	if(where_is_the_index_file == "") return -1;

	ofstream fout(where_is_the_index_file.c_str());
//    ofstream fout ("tmp_index.txt");
	fout << "used_index_node=\t" << index_used_num << endl;
	fout << "root_node=\t\t" << index_all[FATHER].node_point[0] << endl << endl;
	int i, j;

	for(i = 1; i <= index_used_num; i++)
	{
		fout << i << "_node:" << endl << "ifleaf=\t\t\t" << index_all[i].leaf << endl << "count=\t\t\t" << index_all[i].count << endl << endl;

		for(j = 0; j < index_all[i].count; j++)
		{
			fout << j << "_point=\t\t" << index_all[i].node_point[j] << endl << j << "_value=\t\t";

			switch(index_type)
			{
			case 0:
				fout << index_all[i].node_value[j]._intvalue << endl;
				break;

			case 1:
				fout << index_all[i].node_value[j]._floatvalue << endl;
				break;

			case 2:
				fout << index_all[i].node_value[j]._charvalue << endl;
				break;
			}
		}

		fout << "last_point=\t\t" << index_all[i].node_point[index_all[i].count] << endl << endl;
	}

	fout.close();
	where_is_the_index_file = "";
	return 0;
}


//delete the index of the index_name.
//return 0 for all right. -1 for something wrong.
int index_delete_file(string database_name, string table_name, string index_name, int attrib_index)
{
	where_is_the_index_file = ".\\database";
	where_is_the_index_file += database_name;
	where_is_the_index_file += "\\";
	where_is_the_index_file += table_name;
	where_is_the_index_file += "\\";
	where_is_the_index_file += index_name;
	where_is_the_index_file += "_index.txt";
	where_is_the_index_file = "del " + where_is_the_index_file;
	system(where_is_the_index_file.c_str());
	catalog_attrib_all[attrib_index].catalog_attrib_ifhaveindex = false;
	return 0;
}


//return the index in vector<record> table by value is cell.
//return -1 for something wrong.
int index_find(cell& temp_cell)
{
	int i, j;
	bool flag;
	i = index_all[FATHER].node_point[0];

	if(index_all[i].count == 0) return -1;

	while(!index_all[i].leaf)
	{
		for(j = 0; j < index_all[i].count; j++)
		{
			switch(index_type)
			{
			case 0:
				flag = index_all[i].node_value[j]._intvalue <= temp_cell._intvalue;
				break;

			case 1:
				flag = index_all[i].node_value[j]._floatvalue <= temp_cell._floatvalue;
				break;

			case 2:
				flag = index_all[i].node_value[j]._charvalue <= temp_cell._charvalue;
				break;
			}

			if(!flag) break;
		}

		i = index_all[i].node_point[j];
	}

	for(j = 0; j < index_all[i].count; j++)
	{
		switch(index_type)
		{
		case 0:
			flag = index_all[i].node_value[j]._intvalue < temp_cell._intvalue;
			break;

		case 1:
			flag = index_all[i].node_value[j]._floatvalue < temp_cell._floatvalue;
			break;

		case 2:
			flag = index_all[i].node_value[j]._charvalue < temp_cell._charvalue;
			break;
		}

		if(!flag) break;
	}

	index_last_used = i;
	index_i_last_used = j;

	switch(index_type)
	{
	case 0:
		flag = index_all[i].node_value[j]._intvalue == temp_cell._intvalue;
		break;

	case 1:
		flag = index_all[i].node_value[j]._floatvalue == temp_cell._floatvalue;
		break;

	case 2:
		flag = index_all[i].node_value[j]._charvalue == temp_cell._charvalue;
		break;
	}

	if(flag && index_all[i].node_point[j] <= 0) return -index_all[i].node_point[j];
	else return -1;
}


//temp faction for insert at every node.
//if the node need to split, return the new larger part node index.
//return 0 for nothing need to do.
int index_insert_tmp(int now_index, cell& temp_cell, int index_in_table)
{
	int j;
	int ret;
	bool flag;

	if(!index_all[now_index].leaf) //not leaf_node
	{
		for(j = 0; j < index_all[now_index].count; j++)
		{
			switch(index_type)
			{
			case 0:
				flag = index_all[now_index].node_value[j]._intvalue <= temp_cell._intvalue;
				break;

			case 1:
				flag = index_all[now_index].node_value[j]._floatvalue <= temp_cell._floatvalue;
				break;

			case 2:
				flag = index_all[now_index].node_value[j]._charvalue <= temp_cell._charvalue;
				break;
			}

			if(!flag) break;
		}

		int tmp = index_all[now_index].node_point[j];
		ret = index_insert_tmp(tmp, temp_cell, index_in_table);

		if(ret > 0)
		{
			int jj;
			index_all[now_index].count++;

			for(jj = index_all[now_index].count - 1; jj >= j + 1; jj--)
			{
				switch(index_type)
				{
				case 0:
					index_all[now_index].node_value[jj]._intvalue = index_all[now_index].node_value[jj - 1]._intvalue;
					break;

				case 1:
					index_all[now_index].node_value[jj]._floatvalue = index_all[now_index].node_value[jj - 1]._floatvalue;
					break;

				case 2:
					index_all[now_index].node_value[jj]._charvalue = index_all[now_index].node_value[jj - 1]._charvalue;
					break;
				}

				index_all[now_index].node_point[jj + 1] = index_all[now_index].node_point[jj];
			}

			switch(index_type)
			{
			case 0:
				index_all[now_index].node_value[j]._intvalue = index_all[tmp].node_value[5]._intvalue;
				break;

			case 1:
				index_all[now_index].node_value[j]._floatvalue = index_all[tmp].node_value[5]._floatvalue;
				break;

			case 2:
				index_all[now_index].node_value[j]._charvalue = index_all[tmp].node_value[5]._charvalue;
				break;
			}

			index_all[now_index].node_point[j + 1] = ret;
		}
	}
	else  //leaf_node
	{
		for(j = 0; j < index_all[now_index].count; j++)
		{
			switch(index_type)
			{
			case 0:
				flag = index_all[now_index].node_value[j]._intvalue < temp_cell._intvalue;
				break;

			case 1:
				flag = index_all[now_index].node_value[j]._floatvalue < temp_cell._floatvalue;
				break;

			case 2:
				flag = index_all[now_index].node_value[j]._charvalue < temp_cell._charvalue;
				break;
			}

			if(!flag) break;
		}

		int jj;
		index_all[now_index].count++;
		index_all[now_index].node_point[index_all[now_index].count] = index_all[now_index].node_point[index_all[now_index].count - 1];

		for(jj = index_all[now_index].count - 1; jj > j; jj--)
		{
			switch(index_type)
			{
			case 0:
				index_all[now_index].node_value[jj]._intvalue = index_all[now_index].node_value[jj - 1]._intvalue;
				break;

			case 1:
				index_all[now_index].node_value[jj]._floatvalue = index_all[now_index].node_value[jj - 1]._floatvalue;
				break;

			case 2:
				index_all[now_index].node_value[jj]._charvalue = index_all[now_index].node_value[jj - 1]._charvalue;
				break;
			}

			index_all[now_index].node_point[jj] = index_all[now_index].node_point[jj - 1];
		}

		index_all[now_index].node_point[j] = -index_in_table;

		switch(index_type)
		{
		case 0:
			index_all[now_index].node_value[j]._intvalue = temp_cell._intvalue;
			break;

		case 1:
			index_all[now_index].node_value[j]._floatvalue = temp_cell._floatvalue;
			break;

		case 2:
			index_all[now_index].node_value[j]._charvalue = temp_cell._charvalue;
			break;
		}
	}

	if(index_all[now_index].count == 10)
	{
		index_used_num++;
		int tmp2 = index_used_num;
		int i;
		index_all[tmp2].leaf = index_all[now_index].leaf;
		index_all[now_index].count = 5;

		if(!index_all[tmp2].leaf)
		{
			for(i = 6; i < 10; i++)
			{
				switch(index_type)
				{
				case 0:
					index_all[tmp2].node_value[i - 6]._intvalue = index_all[now_index].node_value[i]._intvalue;
					break;

				case 1:
					index_all[tmp2].node_value[i - 6]._floatvalue = index_all[now_index].node_value[i]._floatvalue;
					break;

				case 2:
					index_all[tmp2].node_value[i - 6]._charvalue = index_all[now_index].node_value[i]._charvalue;
					break;
				}

				index_all[tmp2].node_point[i - 6] = index_all[now_index].node_point[i];
			}

			index_all[tmp2].node_point[4] = index_all[now_index].node_point[10];
			index_all[tmp2].count = 4;
		}
		else
		{
			for(i = 5; i < 10; i++)
			{
				switch(index_type)
				{
				case 0:
					index_all[tmp2].node_value[i - 5]._intvalue = index_all[now_index].node_value[i]._intvalue;
					break;

				case 1:
					index_all[tmp2].node_value[i - 5]._floatvalue = index_all[now_index].node_value[i]._floatvalue;
					break;

				case 2:
					index_all[tmp2].node_value[i - 5]._charvalue = index_all[now_index].node_value[i]._charvalue;
					break;
				}

				index_all[tmp2].node_point[i - 5] = index_all[now_index].node_point[i];
			}

			index_all[tmp2].node_point[5] = index_all[now_index].node_point[10];
			index_all[tmp2].count = 5;
			index_all[now_index].node_point[5] = tmp2;
		}

		return tmp2;
	}
	else return 0;
}


//insert the index in vector<record> table by value is cell.
//return 0 for all right. -1 for something wrong.
int index_insert(cell& temp_cell, int index_in_table)
{
//cout<<temp_cell._intvalue<<","<<index_in_table<<endl;
	int tmp = index_all[FATHER].node_point[0];
	int ret;
	ret = index_insert_tmp(tmp, temp_cell, index_in_table);

//cout<<index_all[1].count<<endl;
	if(ret > 0)
	{
		index_used_num++;
		int tmp2 = index_used_num;
		index_all[tmp2].leaf = 0;
		index_all[tmp2].count = 1;
		index_all[tmp2].node_point[0] = tmp;
		index_all[tmp2].node_point[1] = ret;
		index_all[FATHER].node_point[0] = tmp2;

		switch(index_type)
		{
		case 0:
			index_all[tmp2].node_value[0]._intvalue = index_all[tmp].node_value[5]._intvalue;
			break;

		case 1:
			index_all[tmp2].node_value[0]._floatvalue = index_all[tmp].node_value[5]._floatvalue;
			break;

		case 2:
			index_all[tmp2].node_value[0]._charvalue = index_all[tmp].node_value[5]._charvalue;
			break;
		}
	}

	return 0;
}


//delete the index in vector<record> table by value is cell.
//return 0 for all right. -1 for something wrong.
int index_delete(cell& temp_cell, int index_in_table)
{
	index_find(temp_cell);
	bool flag;
	int i, j;
	i = index_last_used;
	j = index_i_last_used;

	for(; j < 10; j++)
	{
		switch(index_type)
		{
		case 0:
			flag = index_all[i].node_value[j]._intvalue > temp_cell._intvalue;
			break;

		case 1:
			flag = index_all[i].node_value[j]._floatvalue > temp_cell._floatvalue;
			break;

		case 2:
			flag = index_all[i].node_value[j]._charvalue > temp_cell._charvalue;
			break;
		}

		if(flag) break;

		if(!(index_all[i].node_point[j] + index_in_table)) index_all[i].node_point[j] = 10000;
	}

	return 0;
}


//return the smallest cell index in the vector<record> table.
//return -1 for something wrong.
int index_smallest(void)
{
//cout<<"in"<<endl;
	int i, j;
	i = index_all[FATHER].node_point[0];

	while(!index_all[i].leaf)
	{
//if(i) cout<<i<<","<<endl;
		i = index_all[i].node_point[0];
	}

	index_last_used = i;
	index_i_last_used = 0;
	j = index_i_last_used;

	if(i > 0 && index_all[i].node_point[j] <= 0) return -index_all[i].node_point[j];

	index_next();
	i = index_last_used;
	j = index_i_last_used;

//cout<<i<<","<<j<<endl;
	if(i > 0 && index_all[i].node_point[j] <= 0) return -index_all[i].node_point[j];

	return -1;
}


//return the next value index in the vector<record> table.
//return -1 for something wrong.
int index_next(void)
{
	int i = index_last_used;
	int j = index_i_last_used;

	if(i < 0) return -1;
	else if(!index_all[i].leaf)
	{
		index_last_used = -1;
		return -1;
	}
	else if(j >= index_all[i].count)
	{
		index_last_used = -1;
		return -1;
	}

	if(j < index_all[i].count - 1) j++;
	else
	{
		i = index_all[i].node_point[j + 1];
		j = 0;
	}

	if(i == FATHER) return -1;

	while(index_all[i].node_point[j] > 0)
	{
		if(i == FATHER) break;

		if(j < index_all[i].count - 1) j++;
		else
		{
			i = index_all[i].node_point[j + 1];
			j = 0;
		}
	}

	index_last_used = i;
	index_i_last_used = j;

	if(i != FATHER) return -index_all[i].node_point[j];
	else return -1;
}
