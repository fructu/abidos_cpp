/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : symbols_table.h
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 21/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#ifndef symbols_table_h
#define symbols_table_h

#include <string>
#include <map>
#include <vector>
#include <stdio.h>

#include "tokens.h"
#include "lex_yacc.h"


using namespace std;

void ts_tests(void);

struct c_token {
    int id;
    string text;

    string file;
    int line;

    c_token() {
        id = 0;
        text = "";

        file = "";
        line = 0;
    }

    c_token(int t, const char *s) {
        id = t;
        text = s;

        file = lex_file_name;
        line = yylineno;
    }

    void clear(void) {
        id = 0;
        text = "";

        file = "";
        line = 0;
    }
    /*
        void save(c_token & token) {
            id = token.id;
            text = token.text;

            file = token.file;
            line = token.line;
        }
    */
    void save(c_token token) {
        id = token.id;
        text = token.text;

        file = token.file;
        line = token.line;
    }

    void save(int t, const char *s) {
        id = t;
        text = s;

        file = lex_file_name;
        line = yylineno;
    }

    void print(const char *tab) {
        printf("%sc_ctoken::print id[%d]->[%s] text[%s] file[%s] line[%d]\n"
               ,tab
               ,id
               ,yytokens[id]
               ,text.c_str()
               ,file.c_str()
               ,line
              );
    }
};

/*
 * when a token is analised it can become to symbols_table
 *
 * class A
 *
 * class -> does not become a symbol A -> become a symbol
 */
struct c_base_class {
    string text;
    int access_specifier;

    c_base_class() {
        text = "";
        access_specifier = 0;
    }

    c_base_class(string t, int access) {
        text = t;
        access_specifier = access;
    }
};

typedef map < string, c_base_class > t_map_base_class;
typedef vector < c_base_class > t_vector_base_class;

/*
 * when a token is analised it can become to symbols_table
 *
 * class A
 *
 * class -> does not become a symbol A -> become a symbol
 */
struct c_friend_class {
    string text;

    c_friend_class() {
        text = "";
    }

    c_friend_class(string t) {
        text = t;
    }
};

typedef map < string, c_friend_class > t_map_friend_class;
typedef vector < c_friend_class > t_vector_friend_class;


/*
 * decl_specifier: storage_class_specifier | type_specifier |
 * function_specifier | FRIEND | TYPEDEF ;
 *
 * ## todo vector<c_decl_specifier> t_vector_decl_specifier
 */
struct c_decl_specifier {
    c_token token;

    int storage_class_specifier;
    int type_specifier;
    int has_colon_colon_after; // A:: ...

    c_decl_specifier() {
        token.clear();

        storage_class_specifier = 0;
        type_specifier = 0;
        has_colon_colon_after = 0;
    }

    c_decl_specifier(c_token t) {
        token = t;

        storage_class_specifier = 0;
        type_specifier = 0;
        has_colon_colon_after = 0;
    }

    void clear(void) {
        storage_class_specifier = 0;
        type_specifier = 0;
        has_colon_colon_after = 0;
    }
};

typedef vector < c_decl_specifier > t_vector_decl_specifier;
enum t_out_identifier {
    OUT_IDENTIFIER_NO,
    OUT_IDENTIFIER_YES
};
struct c_parameter {
    t_vector_decl_specifier vector_decl_specifier;

    c_token token;

    c_parameter() {

    } c_parameter(c_token t, t_vector_decl_specifier v) {
        token = t;
        vector_decl_specifier = v;
    }
    void print(const char *tab);
    string get_string(t_out_identifier sw_out_identifier);
};

typedef vector < c_parameter > t_vector_parameter;
typedef map < string, c_parameter > t_map_parameter;

/*----------------------------------------------------------------------------*/
/*
  templates
  ej:
    template <class T, int N>

    t_vector_template_parameter[0]
      vector_decl_specifier = [class]
      token = T

    t_vector_template_parameter[1]
      vector_decl_specifier = [int]
      token = N
*/
struct c_template_parameter {
    t_vector_decl_specifier vector_decl_specifier;
    c_token token;

    c_template_parameter() {

    } c_template_parameter(c_token t, t_vector_decl_specifier v) {
        token = t;
        vector_decl_specifier = v;
    }

    void clear(void) {
        vector_decl_specifier.clear();
        token.clear();
    }

    void save(c_template_parameter & template_parameter) {
        vector_decl_specifier = template_parameter.vector_decl_specifier;
        token.save(template_parameter.token);
    }

    void print(const char *tab);
    string get_string(void);
};

typedef vector < c_template_parameter > t_vector_template_parameter;
typedef map < string, c_template_parameter > t_map_template_parameter;
/*
template <class T>
class A3 {
};

class B {
  A3<int> a3;// template instantation
}

  symbol.name = a3
  symbol.is_template_instantation = 1;
  symbol.vector_template_argument[0]
    c_token.text = T
    vector_decl_specifier = int
*/
struct c_template_argument {
    t_vector_decl_specifier vector_decl_specifier;
    c_token token;

    c_template_argument() {
    }
    c_template_argument(c_token t, t_vector_decl_specifier v) {
        token = t;
        vector_decl_specifier = v;
    }
    void clear(void) {
        token.clear();
        vector_decl_specifier.clear();
    }
    void save( c_template_argument &  template_argument) {
        token.save(template_argument.token);
        vector_decl_specifier = template_argument.vector_decl_specifier;
    }
};

typedef vector <c_template_argument> t_vector_template_argument;
typedef map <string, c_template_argument> t_map_template_argument;

class c_declarator
{
protected:
    t_map_parameter map_parameter;
    t_vector_parameter vector_parameter;
public:

    //class A{ void a1(void);};
    c_token token;

    //void A::a1(void){...}
    c_token token_definition;

    t_vector_decl_specifier vector_decl_specifier;

    int is_function;
    int has_body;

    int access_specifier;
    int is_constructor;
    int is_destructor;
    int is_operator_overload;
    string operator_overload_sufix;

    void parameter_insert(c_parameter parameter);
    void print(const char *tab);

    int is_typedef;
    int is_template;

    int is_template_instantation;
    t_vector_template_argument vector_template_argument;
    t_map_template_argument map_template_argument;

    int is_extern;
    c_token extern_points_token;

    c_declarator() {
        is_function = 0;
        has_body = 0;

        access_specifier = 0;
        is_constructor = 0;
        is_destructor = 0;
        is_operator_overload = 0;
        operator_overload_sufix = "";

        is_typedef = 0;
        is_template = 0;

        is_template_instantation = 0;
        vector_template_argument.clear();
        map_template_argument.clear();

        is_extern = 0;
        extern_points_token.clear();
    }
    c_declarator(c_token t, t_vector_decl_specifier v) {
        token = t;

        is_typedef = 0;

        is_extern = 0;

        unsigned i_decl = 0;
        for ( i_decl = 0; i_decl < v.size(); ++i_decl ) {
            if ( TYPEDEF == v[i_decl].token.id ) {
                is_typedef = 1;
                continue;
            }
            if ( EXTERN == v[i_decl].token.id ) {
                is_extern = 1;
            }

            vector_decl_specifier.push_back(v[i_decl]);
        }

        is_function = 0;
        has_body = 0;

        access_specifier = 0;
        is_constructor = 0;
        is_destructor = 0;
        is_template = 0;
        is_operator_overload = 0;
        operator_overload_sufix = "";

        is_template_instantation = 0;
        vector_template_argument.clear();
        map_template_argument.clear();
        extern_points_token.clear();
    }
    void clear(void) {
        map_parameter.clear();
        vector_parameter.clear();
        token.clear();
        is_function = 0;
        has_body = 0;

        access_specifier = 0;
        is_constructor = 0;
        is_destructor = 0;
        is_operator_overload = 0;
        operator_overload_sufix = "";

        token_definition.clear();

        is_typedef = 0;
        is_template = 0;

        is_template_instantation = 0;
        vector_template_argument.clear();
        map_template_argument.clear();

        is_extern = 0;
        extern_points_token.clear();
    }
    string get_full_name(void);
};


/*
 * class A { long int a; }
 *
 * c_class_member has 'long int a' inside
 *
 * if is a function member can have parameters
 */

class c_class_member: public c_declarator
{
public:
    c_class_member() {

    }
    c_class_member(c_token t, t_vector_decl_specifier v):c_declarator(t, v) {
    }
};

typedef map < string, c_class_member > t_map_class_member;
typedef vector < c_class_member * >t_vector_class_member;

/*
 * i store members in map to have fast access i store members in vector to
 * haver the original order
 */
class c_class_members
{
private:
    t_map_class_member map_class_member;
    t_vector_class_member vector_class_member;
    friend class c_generator_class_diagram;
    friend class c_generator_original;
public:
    void clear(void);
    void print(const char *tab);
    void insert(c_class_member member);
    c_class_member *get(string member);
};

class c_symbol
{
private:
    t_map_base_class map_base_class;
    t_vector_base_class vector_base_class;

    t_map_friend_class map_friend_class;
    t_vector_friend_class vector_friend_class;

    friend class c_generator_class_diagram;
    friend class c_generator_original;

public:
    void insert_base_class(c_base_class base_class) {
        map_base_class[base_class.text] = base_class;
        vector_base_class.push_back(base_class);
    }

    void insert_friend_class(c_friend_class friend_class) {
        map_friend_class[friend_class.text] = friend_class;
        vector_friend_class.push_back(friend_class);
    }
    // int f(int 1); --> in text "f(int)" in token.text="f";
    string text;
    c_token token;
    int type;
    int class_key;
    int is_abstract;

    // t_map_class_member map_class_member;
    c_class_members members;

    int free_declarator;
    c_declarator declarator;

    int is_typedef;
    string typedef_points_to;

    //### todo it will need save the parameters of template here
    //###   it will bee good moment to do a objets to composite here
    int is_template;
    t_vector_template_parameter vector_template_parameter;
    t_map_template_parameter map_template_parameter;

    int is_template_instantation;
    t_vector_template_argument vector_template_argument;
    t_map_template_argument map_template_argument;

    int class_predeclaration;

    c_symbol() {
        text = "";
        type = 0;
        class_key = 0;
        is_abstract = 0;
        map_base_class.clear();
        members.clear();

        free_declarator = 0;
        is_typedef = 0;
        typedef_points_to = "";

        is_template = 0;
        vector_template_parameter.clear();
        map_template_parameter.clear();

        is_template_instantation = 0;
        vector_template_argument.clear();
        map_template_argument.clear();

        class_predeclaration = 0;
    }

    c_symbol(c_token token_1) {
        text = token_1.text;
        token = token_1;
        type = 0;
        class_key = 0;
        is_abstract = 0;
        map_base_class.clear();
        members.clear();

        free_declarator = 0;
        declarator.clear();

        is_typedef = 0;
        typedef_points_to = "";

        is_template = 0;
        vector_template_parameter.clear();
        map_template_parameter.clear();

        is_template_instantation = 0;
        vector_template_argument.clear();
        map_template_argument.clear();

        class_predeclaration = 0;
    }

    ~c_symbol() {
        text = "";
        type = 0;
        class_key = 0;
        is_abstract = 0;
        map_base_class.clear();
        members.clear();

        free_declarator = 0;
        declarator.clear();

        is_typedef = 0;
        typedef_points_to = "";

        is_template = 0;
        vector_template_parameter.clear();
        map_template_parameter.clear();

        is_template_instantation = 0;
        vector_template_argument.clear();
        map_template_argument.clear();

        class_predeclaration = 0;
    }
    void process_token_text();
    void print(const char *tab);
};

typedef map < string, c_symbol > t_symbols;
typedef vector < t_symbols > stack_symbols;
/*----------------------------------------------------------------------------*/
class c_symbols_table
{
private:
    stack_symbols stack;
    friend class c_generator_class_diagram;
    friend class c_generator_original;
public:
    c_symbols_table();
    ~c_symbols_table();
    void set();
    void unset();

    void insert(c_symbol symbol);
    void change_key(string key_old, string key_new);
    void print(void);

    c_symbol *search_symbol(string str);
};

extern c_symbols_table ts;
/*----------------------------------------------------------------------------*/
#endif
