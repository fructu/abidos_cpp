/*------------------------------------------------------------------------------

	Proyecto			: show_includes
	Codigo				: ts.h
	Descripcion			:
	Version				: 0.1
	Autor				: F. Manuel Hevia Garcia
	Fecha creacion		: 30/12/2010
	Fecha modificacion	:

	Observaciones :


------------------------------------------------------------------------------*/
#ifndef ts_h
#define ts_h

#include <stdio.h>
#include <string>
#include <map>
#include <vector>

//const string path_root = "./";
#define PATH_ROOT "./"

#define LONG_STR (20000)

using namespace std;

struct c_config
{
	int help;
	int sharp;
	int follow;
	int test;
	int print_;
	int batch;
	int callers;

	char callers_file[LONG_STR];

	c_config()
	{
		help   = 0;
		sharp  = 1;
		follow = 0;
		test   = 0;
		print_ = 0;
		batch  = 0;

		callers = 0;

		callers_file[0] = '\0';
	}

	void print(void)
	{
		printf("  config batch[%d] callers[%d] help[%d] sharp[%d] follow[%d] test[%d] print[%d]\n",batch, callers ,help , sharp, follow, test, print_);
	}
};



extern c_config config;

typedef vector<string> t_files_to_process;
typedef map<string, int> t_all_files;

class c_files_to_process
{
	private:
		t_files_to_process files_to_process;
		t_all_files   all_files;

	public:
        char str[LONG_STR];

		bool empty(void);
		void push(char *);
		char * pop(void);
};


extern c_files_to_process files_to_process;

//to resolved dirs
typedef vector<string> t_dir_vector;

class c_cell
{
	private:
		string path;
		string name;
		string delimiter;

		void push_dirs(t_dir_vector & dir_vector);
		void pop_dirs(t_dir_vector & dir_vector, char * str);

		int  is_resolve(c_cell cell);
	public:
		int show;

        char str_path[LONG_STR];
        char str_name[LONG_STR];
        char str_full[LONG_STR];
        char str_delimiter[LONG_STR];

		c_cell();
		~c_cell();
		void init(void);
		void print(void);
		void fill(const char *f1, char * c_type);
		char * full(void);
		int get_number_dirs(void);
		void path_resolve(c_cell & cell);

		char * get_path();
		char * get_delimiter();
		//used only inside this class
		void path_set(char str[]);

		char * get_name();
		int is_sharp(void);
};

typedef map<string, c_cell> t_files_included;
//typedef tt_files_included::value_type t_files_included;

typedef map<string, t_files_included> t_files;

typedef map<string, c_cell> t_files_all;

class c_ts
{
	private:
		//root file of include's tree
		c_cell  file;

		//it's a tree
		t_files files;

		//it's like a list with all files
		t_files_all all_files;

	public:
		c_ts();
		~c_ts();

		void print(void);
		void generate(void);

		void file_begin(char *f);
		void file_end(void);

		//return path resolved
		c_cell resolve(const char *f, char * c_type);
		void file_included(char *f, char * c_type);
};

void str_drop_char(char *p_str, char c);
extern c_ts ts_includes;
#endif
/*----------------------------------------------------------------------------*/

