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

#include <string>
#include <map>
#include <vector>

//const string path_root = "./";
#define PATH_ROOT "./"

#define LONG_STR (20000)

using namespace std;

//to resolved dirs
typedef vector<string> t_dir_vector;

class c_cell
{
	private:
		string path;
		string name;

		void push_dirs(t_dir_vector & dir_vector);
		void pop_dirs(t_dir_vector & dir_vector, char * str);

	public:
		string delimiter;

		c_cell();
		~c_cell();
		void init(void);
		void print(void);
		void fill(char *f1);
		char * full(void);
		int get_number_dirs(void);
		void path_resolve(c_cell & cell);

		char * get_path();
		//used only inside this class
		void path_set(char str[]);

		char * get_name();
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

		void file_included(char *f, char c_type);
};

extern c_ts ts;
#endif
/*----------------------------------------------------------------------------*/

