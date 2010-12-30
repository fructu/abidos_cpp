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
using namespace std;

typedef map<string, int> t_files_included;
typedef map<string, t_files_included> t_files;

class c_ts
{
	private:
		string file_name;
		t_files files;

	public:
		c_ts();
		~c_ts();

		void print(void);
		void file_begin(char *f);
		void file_end(void);
};

extern c_ts ts;
#endif
/*----------------------------------------------------------------------------*/
