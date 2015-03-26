//////////////////////////////////////////////////////////
///----------------------------------------------------///
///       Module: Interpreter                          ///
///       Produced by: Du Fan                          ///
///       Description: Read user's input and change it ///
///                    to another format that can be   ///
///                    recognized by Module API        ///
///       date: 2012/11/6                              ///
///----------------------------------------------------///
//////////////////////////////////////////////////////////
#if !defined(_INTERPRETER_H_)
#define _INTERPRETER_H_

#include "catalog.h"
#include "record.h"
#include "buffer.h"
#include "index.h"
#include <cctype>
#include <algorithm>
#include <io.h>

int Interpreter();
int create_clause(istringstream &sin);
int use_clause(istringstream &sin);
int insert_clause(istringstream &sin);
int delete_clause(istringstream &sin);
int drop_clause(istringstream &sin);
int select_clause(istringstream &sin, bool ifprint);
int execfile_clause(istringstream &sin);
void quit_clause();

int do_test(istringstream &sin);
int do_error();
void print_result();
string format_input(string cmd);
string read_input();
string file_input(ifstream& fin);

#endif
