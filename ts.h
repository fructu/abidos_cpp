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
//expecific vendor
//#include <ext/hash_map>
//#include <unordered_map>
#include <map>

//const string path_root = "./";
#define PATH_ROOT "./"

using namespace std;

struct c_cell
{
	string path;
	string name;

	c_cell();
	~c_cell();
	void init(void);
	void print(void);
	void fill(char *f);
	char * full(void);
};

typedef map<string, c_cell> t_files_included;
//typedef tt_files_included::value_type t_files_included;

typedef map<string, t_files_included> t_files;

class c_ts
{
	private:
		c_cell  file;

		t_files files;

	public:
		c_ts();
		~c_ts();

		void print(void);
		void generate(void);

		void file_begin(char *f);
		void file_end(void);

		void file_included(char *f);
};

extern c_ts ts;
#endif
/*----------------------------------------------------------------------------*/

