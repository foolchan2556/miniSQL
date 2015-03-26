#include "Interpreter.h"

bool if_this_attrib_print[MAX_ATTRIBUTE];
int print_len[MAX_ATTRIBUTE];
string database_name = "";
string table_name = "";
set<string> needtoprint;

int Interpreter()
{
	string cmd;
	cmd = read_input();
	cout << cmd << endl;

	//如果没有输入，则返回重来
	if(cmd == "") return 1;

	istringstream sin(cmd);
	string temp;
	sin >> temp;

	if(temp == "quit")
	{
		quit_clause();
		return 0;
	}
	else if(temp == "create")
	{
		if(create_clause(sin) == -1) do_error();
	}
	else if(temp == "use")
	{
		if(use_clause(sin) == -1) do_error();
	}
	else if(temp == "insert")
	{
		if(insert_clause(sin) == -1) do_error();
	}
	else if(temp == "delete")
	{
		if(delete_clause(sin) == -1) do_error();
	}
	else if(temp == "select")
	{
		if(select_clause(sin, true) == -1) do_error();
	}
	else if(temp == "execfile")
	{
		if(execfile_clause(sin) == -1) do_error();
	}
	else if(temp == "drop")
	{
		if(drop_clause(sin) == -1) do_error();
	}
	else if(temp == "test")
	{
		if(do_test(sin) == -1) do_error();
	}
	else do_error();

	index_close_file();
	buffer_close_table();
	catalog_close_file();
	table.clear();
	where_is_the_index_file = "";
	where_is_the_catalog_file = "";
	where_is_the_table_file = "";
	return 1;
}

int create_clause(istringstream &sin)
{
	string temp;
	string temp2 = "md .\\database\\";
	string temp3 = ".\\database\\";
	sin >> temp;
	int primaryindex = -1;

/*
create database testdb;
create database testdb;
*/
	if(temp == "database")
	{
		sin >> temp;
		temp3 += temp;

		//判断是否存在
		if((_access(temp3.c_str(), 2)) == 0)
		{
			cout << "NOTICE: database " << temp << " already exist" << endl;
			return 0;
		}
		
		temp2 += temp;
		system(temp2.c_str());
		database_name = temp;
		cout << "NOTICE: A new database " << database_name << " has been created" << endl;
		return 0;
	}
/*
create table book3 ( bin int primary , bprice float , bname char (10) unique , );
create table t1 ( id int , name char(20) unique, age int , salary float, primary key(id) ) ;
create table t1
( 
id int ,
name char(20) unique,
age int ,
salary float,
primary key(id),
);
*/
	else if(temp == "table")
	{
		if(database_name == "")
		{
			cout << "NOTICE: No available database!" << endl;
			return 0;
		}
		else
		{
			temp2 += database_name;
			temp2 += "\\";
			sin >> temp;
			temp2 += temp;
			system(temp2.c_str());
			table_name = temp;
			catalog_create_attrib_file(database_name, table_name);
			buffer_create_table(database_name, table_name);
			sin >> temp;

			if(temp != "(") return -1;

			string name, type;
			int strlen, tmptype;
			int count = 0;
			string special;

			while(sin >> name)
			{
				if(name == ")") break;

				sin >> type;
				strlen = 0;

				if(type == "int") tmptype = 0;
				else if(type == "float") tmptype = 1;
				else if(type == "char")
				{
					tmptype = 2;
					sin >> temp;

					if(temp != "(") return -1;

					sin >> strlen;
					sin >> temp;

					if(temp != ")") return -1;
				}

				catalog_add_attrib(name, tmptype, strlen);
				sin >> special;

				if(special == "unique")
				{
					catalog_set_unique(count);
					sin >> temp;

					if(temp != ",") return -1;
				}
				else if(special == "primary")
				{
					if(primaryindex != -1)
					{
						cout << "NOTICE: Only one attribute can be set as primary key" << endl;
						return -1;
					}

					catalog_set_prime(count);
					primaryindex = count;
					sin >> temp;

					if(temp != ",") return -1;
				}
				else if(special == ",");
				else return -1;

				count++;
			}

			if(primaryindex != -1) index_creat_file(database_name, table_name, catalog_attrib_all[primaryindex].catalog_attrib_name, primaryindex);

			cout << "NOTICE: A new table " << table_name << " of database " << database_name << " has been created" << endl;
			return 0;
		}
	}
	else if(temp == "index")
	{
		if(database_name == "")
		{
			cout << "NOTICE: No available database!" << endl;
			return 0;
		}
		else
		{
			sin >> temp >> temp2;

			if(temp2 != "on") return -1;

			sin >> table_name;

			if(!catalog_open_file(database_name, table_name) && !buffer_open_table(database_name, table_name))
			{
				int tmp_index = catalog_get_attrib_index(temp);

				if(tmp_index >= 0)
				{
					index_creat_file(database_name, table_name, temp, tmp_index);
					cout << "NOTICE: A index on " << temp << " of table " << table_name << " has been created" << endl;
					return 0;
				}
			}
			else
			{
				cout << "NOTICE: No such index!" << endl;
				return 0;
			}
		}
	}

	return -1;
}

int use_clause(istringstream &sin)
{
	sin >> database_name;
	cout << "NOTICE: Database changed: " << database_name << endl;
	return 0;
}

int insert_clause(istringstream &sin)
{
	sin >> table_name;
	catalog_open_file(database_name, table_name);
	buffer_open_table(database_name, table_name);
	string temp;
	record tmprecord;
	int i, j;

	for(i = 0; i < attrib_total_num; i++)
	{
		sin >> temp;

//cout<<"1 "<<temp<<":"<<table_attrib_type[i]<<endl;
		if(temp != "(" && temp != ",") break;

		switch(table_attrib_type[i])
		{
		case 0:
			sin >> tmprecord.member[i]._intvalue;
			break;

		case 1:
			sin >> tmprecord.member[i]._floatvalue;
			break;

		case 2:
			sin >> temp;
			temp[0] = ' ';
			temp[temp.length() - 1] = ' ';
			istringstream ssin(temp);
			ssin >> tmprecord.member[i]._charvalue;
			break;
		}
	}

	if(i != attrib_total_num) return -1;

	if(record_insert(tmprecord) != 0) return -1;

	int pos = table.size() - 1;

	for(j = 0; j < attrib_total_num; j++)
		if(catalog_attrib_all[j].catalog_attrib_ifhaveindex)
		{
			index_open_file(database_name, table_name, catalog_attrib_all[j].catalog_attrib_name, j);
			index_insert(tmprecord.member[j], pos);
//            index_creat_file(database_name,table_name,catalog_attrib_all[j].catalog_attrib_name,j);
			index_close_file();
		}

	buffer_close_table();
	catalog_close_file();
	return 0;
}

int delete_clause(istringstream &sin)
{
	select_clause(sin, false);
	int i, j;
	vector<record>::iterator iter;

	for(i = table.size() - 1; i >= 0; i--)
		if(record_select_result.find(i) != record_select_result.end())
		{
			for(j = 0, iter = table.begin(); j < i; j++, iter++);

			table.erase(iter);
		}

	for(j = 0; j < attrib_total_num; j++)
		if(catalog_attrib_all[j].catalog_attrib_ifhaveindex)
		{
			index_creat_file(database_name, table_name, catalog_attrib_all[j].catalog_attrib_name, j);
			index_close_file();
		}

	cout << "NOTICE: Delete all right. Now there are " << table.size() << " records" << endl;
	return 0;
}

int drop_clause(istringstream &sin)
{
	string temp;
	string temp2 = ".\\database\\";
	string tmp3 = "rmdir /S ";
	string tmp4 = "del ";
	sin >> temp;

	if(temp == "database")
	{
		sin >> temp;
		temp2 += temp;
		tmp3 += temp2;
		tmp4 += temp2;
		system(tmp4.c_str());
		system(tmp3.c_str());
		database_name = temp;
		cout << "NOTICE: Database " << database_name << " has been deleted" << endl;
		database_name = "";
		return 0;
	}
	else if(temp == "table")
	{
		if(database_name == "")
		{
			cout << "NOTICE: No available database!" << endl;
			return 0;
		}
		else
		{
			temp2 += database_name;
			temp2 += "\\";
			sin >> temp;
			temp2 += temp;
			tmp3 += temp2;
			tmp4 += temp2;
			system(tmp4.c_str());
			system(tmp3.c_str());
			table_name = temp;
			cout << "NOTICE: Table " << table_name << " of database " << database_name << " has been deleted" << endl;
			return 0;
		}
	}
	else if(temp == "index")
	{
		if(database_name == "")
		{
			cout << "NOTICE: No available database!" << endl;
			return 0;
		}
		else
		{
			string tmp3;
			sin >> temp >> tmp3;

			if(tmp3 != "on") return -1;

			sin >> table_name;
			temp2 += database_name;
			temp2 += "\\";
			temp2 += table_name;
			temp2 += "\\";
			temp2 += temp;
			temp2 += "_index.txt";
			tmp4 += temp2;
			system(tmp4.c_str());
			cout << "NOTICE: Index " << temp << " of table " << table_name << " has been deleted" << endl;
			return 0;
		}
	}

	return -1;
}

int select_clause(istringstream &sin, bool ifprint)
{
	string temp;
	needtoprint.clear();

	while(sin >> temp)
	{
		if(temp == "from") break;

		needtoprint.insert(temp);
	}

	if(temp != "from") return -1;

	sin >> table_name;
	catalog_open_file(database_name, table_name);
	buffer_open_table(database_name, table_name);

	if(attrib_total_num == 0) return -1;

	int i;
	bool flag = false;

	if(needtoprint.size() == 0) flag = true;

	for(i = 0; i < attrib_total_num; i++) if_this_attrib_print[i] = flag;

	set<string>::iterator iter;

	for(iter = needtoprint.begin(); iter != needtoprint.end(); iter++)
	{
		temp = *iter;

		if(temp == "*")
		{
			for(i = 0; i < attrib_total_num; i++) if_this_attrib_print[i] = true;

			break;
		}
		else
		{
			i = catalog_get_attrib_index(temp);

			if(i >= 0) if_this_attrib_print[i] = true;
		}
	}

	record_select_result.clear();
	sin >> temp;

	if(temp == ";") record_select_all();
	else if(temp == "where")
	{
		cell tmpcell;
		bool ifand = false;
		int tmpindex;

		while(true)
		{
			sin >> temp;
			tmpindex = catalog_get_attrib_index(temp);

			if(tmpindex < 0) return -1;

			sin >> temp;

			if(temp == "<")
			{
				switch(table_attrib_type[tmpindex])
				{
				case 0:
					sin >> tmpcell._intvalue;
					break;

				case 1:
					sin >> tmpcell._floatvalue;
					break;

				case 2:
					sin >> temp;
					temp[0] = ' ';
					temp[temp.length() - 1] = ' ';
					istringstream ssin(temp);
					ssin >> tmpcell._charvalue;
					break;
				}

				if(ifand) record_select_small_and_the_last_result(tmpindex, tmpcell);
				else record_select_small_or_the_last_result(tmpindex, tmpcell);

				sin >> temp;

				if(temp == ";") break;
				else if(temp == "and") ifand = true;
				else if(temp == "or") ifand = false;
				else return -1;
			}
			else if(temp == "=")
			{
				switch(table_attrib_type[tmpindex])
				{
				case 0:
					sin >> tmpcell._intvalue;
					break;

				case 1:
					sin >> tmpcell._floatvalue;
					break;

				case 2:
					sin >> temp;
					temp[0] = ' ';
					temp[temp.length() - 1] = ' ';
					istringstream ssin(temp);
					ssin >> tmpcell._charvalue;
					break;
				}

				if(ifand) record_select_equal_and_the_last_result(tmpindex, tmpcell);
				else record_select_equal_or_the_last_result(tmpindex, tmpcell);

				sin >> temp;

				if(temp == ";") break;
				else if(temp == "and") ifand = true;
				else if(temp == "or") ifand = false;
				else return -1;
			}
			else if(temp == ">")
			{
				switch(table_attrib_type[tmpindex])
				{
				case 0:
					sin >> tmpcell._intvalue;
					break;

				case 1:
					sin >> tmpcell._floatvalue;
					break;

				case 2:
					sin >> temp;
					temp[0] = ' ';
					temp[temp.length() - 1] = ' ';
					istringstream ssin(temp);
					ssin >> tmpcell._charvalue;
					break;
				}

				if(ifand) record_select_large_and_the_last_result(tmpindex, tmpcell);
				else record_select_large_or_the_last_result(tmpindex, tmpcell);

				sin >> temp;

				if(temp == ";") break;
				else if(temp == "and") ifand = true;
				else if(temp == "or") ifand = false;
				else return -1;
			}
			else if(temp == "<=")
			{
				switch(table_attrib_type[tmpindex])
				{
				case 0:
					sin >> tmpcell._intvalue;
					break;

				case 1:
					sin >> tmpcell._floatvalue;
					break;

				case 2:
					sin >> temp;
					temp[0] = ' ';
					temp[temp.length() - 1] = ' ';
					istringstream ssin(temp);
					ssin >> tmpcell._charvalue;
					break;
				}

				if(ifand) record_select_smallequal_and_the_last_result(tmpindex, tmpcell);
				else record_select_smallequal_or_the_last_result(tmpindex, tmpcell);

				sin >> temp;

				if(temp == ";") break;
				else if(temp == "and") ifand = true;
				else if(temp == "or") ifand = false;
				else return -1;
			}
			else if(temp == ">=")
			{
				switch(table_attrib_type[tmpindex])
				{
				case 0:
					sin >> tmpcell._intvalue;
					break;

				case 1:
					sin >> tmpcell._floatvalue;
					break;

				case 2:
					sin >> temp;
					temp[0] = ' ';
					temp[temp.length() - 1] = ' ';
					istringstream ssin(temp);
					ssin >> tmpcell._charvalue;
					break;
				}

				if(ifand) record_select_largeequal_and_the_last_result(tmpindex, tmpcell);
				else record_select_largeequal_or_the_last_result(tmpindex, tmpcell);

				sin >> temp;

				if(temp == ";") break;
				else if(temp == "and") ifand = true;
				else if(temp == "or") ifand = false;
				else return -1;
			}
			else if(temp == "<>")
			{
				switch(table_attrib_type[tmpindex])
				{
				case 0:
					sin >> tmpcell._intvalue;
					break;

				case 1:
					sin >> tmpcell._floatvalue;
					break;

				case 2:
					sin >> temp;
					temp[0] = ' ';
					temp[temp.length() - 1] = ' ';
					istringstream ssin(temp);
					ssin >> tmpcell._charvalue;
					break;
				}

				if(ifand) record_select_notequal_and_the_last_result(tmpindex, tmpcell);
				else record_select_notequal_or_the_last_result(tmpindex, tmpcell);

				sin >> temp;

				if(temp == ";") break;
				else if(temp == "and") ifand = true;
				else if(temp == "or") ifand = false;
				else return -1;
			}
			else return -1;
		}
	}
	else return -1;

	if(ifprint) print_result();

	return 0;
}

void print_result(void)
{
	int i, j;
	set<int>::iterator iter;
	int sumlen = 0;

	for(j = 0; j < attrib_total_num; j++)
	{
		print_len[j] = 0;

		if(if_this_attrib_print[j])
		{
			print_len[j] = 10;

			if(catalog_attrib_all[j].catalog_attrib_name.length() > 10)
				print_len[j] = catalog_attrib_all[j].catalog_attrib_name.length();

			if(table_attrib_type[j] == 2 && catalog_attrib_all[j].catalog_attrib_stringlen > print_len[j])
				print_len[j] = catalog_attrib_all[j].catalog_attrib_stringlen;

			sumlen += print_len[j] + 2;
		}
	}

	cout << "NOTICE: Select result : Total " << record_select_result.size() << " records" << endl;
	cout << "-";

	for(j = 0; j < sumlen; j++) cout << "-";

	cout << endl;
	cout << "|";

	for(j = 0; j < attrib_total_num; j++)
		if(if_this_attrib_print[j])
			cout << setw(print_len[j]) << catalog_attrib_all[j].catalog_attrib_name << " |";

	cout << endl;
	cout << "-";

	for(j = 0; j < sumlen; j++) cout << "-";

	cout << endl;

	for(iter = record_select_result.begin(); iter != record_select_result.end(); iter++)
	{
		i = *iter;
		cout << "|";

		for(j = 0; j < attrib_total_num; j++)
			if(if_this_attrib_print[j])
				switch(table_attrib_type[j])
				{
				case 0:
					cout << setw(print_len[j]) << table[i].member[j]._intvalue << " |";
					break;

				case 1:
					cout << setw(print_len[j]) << table[i].member[j]._floatvalue << " |";
					break;

				case 2:
					cout << setw(print_len[j]) << table[i].member[j]._charvalue << " |";
					break;
				}

		cout << endl;
	}

	cout << "-";

	for(j = 0; j < sumlen; j++) cout << "-";

	cout << endl;
}

int execfile_clause(istringstream &sin)
{
	string filename;
	sin >> filename;
	ifstream fin(filename.c_str());
	string cmd;

	while(true)
	{
		cmd = file_input(fin);

		if(cmd == "") break;

		istringstream sin(cmd);
		string temp;
		sin >> temp;

		if(temp == "quit")
		{
			quit_clause();
			break;
		}
		else if(temp == "create")
		{
			if(create_clause(sin) == -1) do_error();
		}
		else if(temp == "use")
		{
			if(use_clause(sin) == -1) do_error();
		}
		else if(temp == "insert")
		{
			if(insert_clause(sin) == -1) do_error();
		}
		else if(temp == "delete")
		{
			if(delete_clause(sin) == -1) do_error();
		}
		else if(temp == "select")
		{
			if(select_clause(sin, true) == -1) do_error();
		}
		else if(temp == "execfile")
		{
			if(execfile_clause(sin) == -1) do_error();
		}
		else if(temp == "drop")
		{
			if(drop_clause(sin) == -1) do_error();
		}
		else do_error();

		index_close_file();
		buffer_close_table();
		catalog_close_file();
		table.clear();
		where_is_the_index_file = "";
		where_is_the_catalog_file = "";
		where_is_the_table_file = "";
	}

	return 0;
}

int do_error(void)
{
	cout << "NOTICE: Input format error" << endl;
	return 0;
}

string format_input(string cmd)
{
	transform(cmd.begin(), cmd.end(), cmd.begin(), tolower);
	
	string cmd2 = "";
	for(int i=0; i<cmd.length(); i++)
	{
		if(cmd[i] == '\(') cmd2 += " ( ";
		else if(cmd[i] == '\)') cmd2 += " ) ";
		else if(cmd[i] == '\;') cmd2 += " ; ";
		else if(cmd[i] == '\,') cmd2 += " , ";
		
		else if(cmd[i] == '\<' && cmd[i+1] == '\>') cmd2 += " <";
		else if(cmd[i] == '\>' && cmd[i-1] == '\<') cmd2 += "> ";
		
		else if(cmd[i] == '\<' && cmd[i+1] == '\=') cmd2 += " <";
		else if(cmd[i] == '\>' && cmd[i+1] == '\=') cmd2 += " >";
		else if(cmd[i] == '\=' && cmd[i-1] == '\<') cmd2 += "= ";
		else if(cmd[i] == '\=' && cmd[i-1] == '\>') cmd2 += "= ";
		
		else if(cmd[i] == '\<') cmd2 += " < ";
		else if(cmd[i] == '\>') cmd2 += " > ";
		else if(cmd[i] == '\=') cmd2 += " = ";
		
		else cmd2 += cmd[i];
	}
	return cmd2;
}
string read_input(void)
{
	string cmd;
	string temp;
	cmd = "";

	while(cin >> temp)
	{
		cmd += temp;
		if(temp[temp.length() - 1] == ';') break;
		cmd += " ";
	}
	
	cmd = format_input(cmd);
	return cmd;
}

string file_input(ifstream& fin)
{
	string cmd;
	string temp;
	cmd = "";

	while(fin >> temp)
	{
		cmd += temp;
		if(temp[temp.length() - 1] == ';') break;
		cmd += " ";
	}
	
	cmd = format_input(cmd);
	cout << cmd << endl;
	return cmd;
}

int do_test(istringstream &sin)
{
	string temp, temp2;
	sin >> temp;
	ofstream fout("test_index.txt");

	if(temp == "index")
	{
		if(database_name == "")
		{
			cout << "NOTICE: No available database!" << endl;
			return 0;
		}
		else
		{
			sin >> temp >> temp2;

			if(temp2 != "on") return -1;

			sin >> table_name;

			if(!catalog_open_file(database_name, table_name) && !buffer_open_table(database_name, table_name))
			{
				int tmp_index = catalog_get_attrib_index(temp);
				index_open_file(database_name, table_name, temp, tmp_index);
				int i = index_smallest();

				while(i >= 0)
				{
					switch(table_attrib_type[tmp_index])
					{
					case 0:
						fout << table[i].member[tmp_index]._intvalue << endl;;
						break;

					case 1:
						fout << table[i].member[tmp_index]._floatvalue << endl;;
						break;

					case 2:
						fout << table[i].member[tmp_index]._charvalue << endl;;
						break;
					}

					i = index_next();
				}

				cout << "NOTICE: Result has been print in test_index.txt" << endl;
				index_close_file();
				return 0;
			}
			else
			{
				cout << "NOTICE: No such index!" << endl;
				return 0;
			}
		}
	}

	return -1;
}

void quit_clause()
{
	catalog_close_file();
	buffer_close_table();
	index_close_file();
}