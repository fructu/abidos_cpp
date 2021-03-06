/*------------------------------------------------------------------------------

    Proyecto            : abidos
    Codigo              : symbols_table.cpp
    Descripcion         :
    Version             : 0.1
    Autor               : F. Manuel Hevia Garcia
    Fecha creacion      : 21/03/2011
    Fecha modificacion  :

    Observaciones :


------------------------------------------------------------------------------*/
#include "symbols_table.h"
#include "parser_descent.h"
#include "parser.h"
#include "options.h"
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void
c_parameter::print(const char *tab)
{
    unsigned i_decl = 0;
    for (i_decl = 0; i_decl < vector_decl_specifier.size(); ++i_decl) {
        printf(" [%s]", vector_decl_specifier[i_decl].token.text.c_str());
    }

    printf(" [%s]", token.text.c_str());
}

/*----------------------------------------------------------------------------*/
string c_parameter::get_string(t_out_identifier sw_out_identifier)
{
    string str = "";
    unsigned i_decl = 0;
    int have_text = 1;
    for (i_decl = 0; i_decl < vector_decl_specifier.size(); ++i_decl) {
        str = str + vector_decl_specifier[i_decl].token.text;
        if ((i_decl + 1) < vector_decl_specifier.size()) {
            str = str + " ";
        }
        switch (vector_decl_specifier[i_decl].token.id) {
        case VOID:
            have_text = 0;
            break;
        case ELLIPSIS:
            have_text = 0;
            break;
        default:
            have_text = 1;
        }
    }

    if (1 == have_text) {
        // if parameter dont have name i puted @IDENTIFIER#
        if (NO_IDENTIFIER != token.text) {
            if (OUT_IDENTIFIER_YES == sw_out_identifier) {
                str = str + " " + token.text;
            }
        }
    }

    return str;
}
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void
c_template_parameter::print(const char *tab)
{
    unsigned i_decl = 0;
    for (i_decl = 0; i_decl < vector_decl_specifier.size(); ++i_decl) {
        printf(" [%s]", vector_decl_specifier[i_decl].token.text.c_str());
    }

    printf(" [%s]", token.text.c_str());
}

/*----------------------------------------------------------------------------*/
string c_template_parameter::get_string(void)
{
    string str = "";
    unsigned i_decl = 0;
    int have_text = 1;
    for (i_decl = 0; i_decl < vector_decl_specifier.size(); ++i_decl) {
        str = str + vector_decl_specifier[i_decl].token.text;
        if ((i_decl + 1) < vector_decl_specifier.size()) {
            str = str + " ";
        }

        switch (vector_decl_specifier[i_decl].token.id) {
            /*
            case VOID:
                have_text = 0;
                break;
            case ELLIPSIS:
                have_text = 0;
                break;
            */
        default:
            have_text = 1;
        }
    }

    if (1 == have_text) {
        // if parameter dont have name i puted @IDENTIFIER#
        if (NO_IDENTIFIER != token.text) {
            str = str + " " + token.text;
        }
    }

    return str;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void c_declarator::parameter_insert(c_parameter parameter)
{
    if (0 == is_function) {
        printf
        ("error c_declarator::parameter_insert(%s) not is_function \n",
         parameter.token.text.c_str());

        exit(-1);
    }
    map_parameter[parameter.token.text] = parameter;
    vector_parameter.push_back(parameter);
}

/*----------------------------------------------------------------------------*/
void c_declarator::print(const char *tab)
{
    unsigned i_decl = 0;

    printf("%s   ", tab);

    if ( 1 == is_typedef ) {
        printf("TYPEDEF_NAME ");
    }

    for (i_decl = 0; i_decl < vector_decl_specifier.size(); ++i_decl) {
        printf("[%s] ", vector_decl_specifier[i_decl].token.text.c_str());
    }

    if ( 1 == is_template_instantiation ) {
        size_t i = 0;
        printf("<");
        for ( i = 0; i < vector_template_argument.size(); ++i) {
            c_template_argument a = vector_template_argument[i];
            printf("%s is",a.token.text.c_str());

            size_t j = 0;
            for ( j = 0; j < a.vector_decl_specifier.size(); ++j) {
                c_decl_specifier decl = a.vector_decl_specifier[j];
                printf(" %s",decl.token.text.c_str());
            }

            if ( (i + 1) < vector_template_argument.size()) {
                printf(",");
            }
        }
        printf(">");
    }

    if ( 1 == is_extern ) {
        printf("  extern_points_token[");
        extern_points_token.print("");
        printf("  extern_points_token]");
    }

    if (1 == is_function) {
        if (1 == is_template) {
            printf("[%s]<>", token.text.c_str());
        } else {
            printf("[%s]", token.text.c_str());
        }

        if (vector_parameter.size() == 0) {
            printf("()\n");
            return;
        }

        printf("(");
        unsigned i_parameter = 0;
        for (i_parameter = 0; i_parameter < vector_parameter.size();
                ++i_parameter) {
            vector_parameter[i_parameter].print(tab);
            if ((i_parameter + 1) < vector_parameter.size()) {
                printf(",");
            }
        }
        printf(")\n");
    } else {
        printf("[%s]\n", token.text.c_str());
    }
}

/*----------------------------------------------------------------------------*/
string c_declarator::get_full_name(void)
{
    string name = token.text;

    if ( 1 == is_typedef ) {
        // ## typedef int t_int;
        // ## in this way in ts would be [typedef t_int]
        // name = "typedef " + token.text;
        // ## there is this information in
        //    vector_decl_specifier[0].token.id == TYPEDEF
        //
    }

    if ( 1 == is_constructor ) {
        //for now i dont need do nothing
        //name = token.text;
    }

    if ( 1 == is_destructor ) {
        name = "~" + token.text;
    }

    if (1 == is_function) {
        if (vector_parameter.size() == 0) {
            name = name + "()";
            return name;
        }

        if (1 == is_template) {
            name = name + "<" + ">" + "(";
        } else {
            name = name + "(";
        }
        unsigned i_parameter = 0;
        for (i_parameter = 0; i_parameter < vector_parameter.size();
                ++i_parameter) {
            name = name + vector_parameter[i_parameter].get_string(OUT_IDENTIFIER_NO);
            if ((i_parameter + 1) < vector_parameter.size()) {
                name = name + ",";
            }
        }
        name = name + ")";
    } else {
        string name = token.text;
    }

    return name;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void c_class_members::clear(void)
{
    map_class_member.clear();
    vector_class_member.clear();
}

/*----------------------------------------------------------------------------*/
void c_class_members::print(const char *tab)
{
    t_map_class_member::iterator i_map_member = map_class_member.begin();
    printf("%s  vector_class_member [%ld]\n", tab,
           (long) vector_class_member.size());
    printf("%s  {\n", tab);
    unsigned i_member = 0;
    for (i_member = 0; i_member < vector_class_member.size(); ++i_member) {
        vector_class_member[i_member]->print(tab);
    }
    printf("%s  }\n", tab);

    printf("%s  map_class_member [%ld]\n", tab,
           (long) map_class_member.size());
    printf("%s  {\n", tab);
    if (map_class_member.size() <= 0) {
        printf("%s    empty\n", tab);
    }

    for (i_map_member = map_class_member.begin();
            i_map_member != map_class_member.end(); ++i_map_member) {
        // printf("%s first[%s]->[%s][%s]\n"

        printf("%s   ", tab);

        printf("[%s]: ",
               yytokens[((*i_map_member).second).access_specifier]
              );

        unsigned i_decl = 0;

        for (i_decl = 0;
                i_decl < (i_map_member->second).vector_decl_specifier.size();
                ++i_decl) {
            printf("[%s] ",
                   (i_map_member->second).
                   vector_decl_specifier[i_decl].token.text.c_str());
        }

        if ( 1 == ((*i_map_member).second).is_template_instantiation ) {
            printf(" TEMPLATE INSTANTIATION ");
        }

        printf("first[%s]->[%s]\n"
               //, yytokens[((*i_map_member).second).access_specifier]
               , ((*i_map_member).first).c_str()
               , ((*i_map_member).second).token.text.c_str());
    }
}

/*----------------------------------------------------------------------------*/
void c_class_members::insert(c_class_member member)
{
    if ( 0 == member.token.text.size() ) {
        printf("%s:%d error c_class_members::insert() 0 == member.token.text.size()\n"
               , member.token.file.c_str()
               , member.token.line
              );
        exit(-1);
    }

    map_class_member[member.get_full_name()] = member;
    vector_class_member.push_back(&map_class_member
                                  [member.get_full_name()]);
}

/*----------------------------------------------------------------------------*/
c_class_member *c_class_members::get(string member)
{
    t_map_class_member::iterator i_member;

    i_member = map_class_member.find(member);

    if (i_member == map_class_member.end()) {
        printf("%s:%d warning c_class_member * c_class_members::get(%s)\n"
               , lex_file_name
               , yylineno
               , member.c_str());
//      exit(-1);
        return 0;
    }

    return (&(i_member->second));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*
  when a class is a template the name will be diferent
*/
void c_symbol::process_token_text()
{
    if ( 1 != is_template ) {
        return;
    }

    unsigned i = 0;
    string s = token.text + "<";

    for (i = 0; i < vector_template_parameter.size(); ++i) {
        //### todo maybe vector_decl_specifier[] have more than 1 :-S ???
        c_token token = vector_template_parameter[i].vector_decl_specifier[0].token;

        if ( CLASS == token.id ) {
            s += "typename";
        } else {
            s += token.text;
        }
        if ( (i + 1)  < vector_template_parameter.size() ) {
            s += ",";
        }
    }
    s += ">";
    text = s;
}

void c_symbol::print(const char *tab)
{
    printf("%s:%d id[%d]->[%s] text[%s] type[%d]->[%s] class_key[%d]->[%s]\n"
           /*tab*/
           ,token.file.c_str()
           ,token.line
           , token.id, yytokens[token.id], token.text.c_str(), type,
           yytokens[type], class_key, yytokens[class_key]);
    printf("%s{\n" , tab);

    if ( TYPEDEF_NAME == type) {
        printf("%s  is_typedef[%d] typedef_points_to[%s]\n"
               , tab
               , is_typedef
               , typedef_points_to.c_str()
              );
    }
    if ( 1 == is_template_instantiation ) {
        printf("%s   TEMPLATE_INSTANTIATION\n",tab);
    }

    // t_map_base_class map_base_class;

    t_map_base_class::iterator i_map_base = map_base_class.begin();

    printf("%s  map_base_class\n", tab);
    printf("%s  {\n", tab);

    if (map_base_class.size() <= 0) {
        printf("%s    empty\n", tab);
    }

    for (i_map_base = map_base_class.begin();
            i_map_base != map_base_class.end(); ++i_map_base) {
        printf("%s    first[%s]->[%s][%s]\n", tab,
               ((*i_map_base).first).c_str(),
               yytokens[((*i_map_base).second).access_specifier],
               ((*i_map_base).second).text.c_str());
    }

    printf("%s  }\n", tab);


    t_map_friend_class::iterator i_map_friend = map_friend_class.begin();
    printf("%s  map_friend_class\n", tab);
    printf("%s  {\n", tab);

    if (map_friend_class.size() <= 0) {
        printf("%s    empty\n", tab);
    }

    for (i_map_friend = map_friend_class.begin();
            i_map_friend != map_friend_class.end(); ++i_map_friend) {
        printf("%s    first[%s]->[%s]\n", tab,
               ((*i_map_friend).first).c_str(),
               ((*i_map_friend).second).text.c_str());
    }

    printf("%s  }\n", tab);

    members.print(tab);

    printf("%s  }\n", tab);


    printf("%s  free_declarator\n", tab);
    printf("%s  {\n", tab);
    if ( 1 == free_declarator ) {
        declarator.print(tab);
    }
    printf("%s  }\n", tab);
    printf("%s}\n" , tab);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
c_symbols_table::c_symbols_table()
{
    stack.clear();
}

/*----------------------------------------------------------------------------*/
c_symbols_table::~c_symbols_table()
{
    stack.clear();
}

/*----------------------------------------------------------------------------*/
void
c_symbols_table::set()
{
    t_symbols t;
    stack.push_back(t);
}

/*----------------------------------------------------------------------------*/
void c_symbols_table::unset()
{
    long i = stack.size() - 1;

    if (i < 0) {
        printf
        ("error void c_symbols_table::unset(c_token token) -> ( last < 0)\n");
        exit(-1);
    }

    unsigned last = stack.size() - 1;

    stack[last].clear();

    stack.pop_back();
}

/*----------------------------------------------------------------------------*/
void c_symbols_table::insert(c_symbol symbol)
{
    long i = stack.size() - 1;

    if (i < 0) {
        printf
        ("error void c_symbols_table::insert(c_token token) -> ( last < 0)\n");
        exit(-1);
    }

    unsigned last = stack.size() - 1;

    if (i < 0) {
        printf
        ("error void c_symbols_table::insert(c_token token) -> ( last < 0)\n");
        exit(-1);
    }
    // c_symbol symbol(token);
    if ( 0 != search_symbol(symbol.text) ) {
        printf
        ("%s:%d error c_symbols_table::insert(%s) symbol exists yet %s:%d\n"
         , lex_file_name
         , yylineno
         , symbol.text.c_str()
         , symbol.token.file.c_str()
         , symbol.token.line
        );
        return;
    }

    symbol.is_banned = options.ban_symbols_on;

    stack[last][symbol.text] = symbol;
}
/*----------------------------------------------------------------------------*/
/*
  ### todo
    this does not work, corrupts data in ts.
*/
void c_symbols_table::change_key(string key_old, string key_new)
{
    long i = stack.size() - 1;

    if (i < 0) {
        printf
        ("error c_symbols_table::change_key -> ( last < 0)\n");
        exit(-1);
    }

    unsigned last = stack.size() - 1;

    if (i < 0) {
        printf
        ("error c_symbols_table::change_key -> ( last < 0)\n");
        exit(-1);
    }

    if ( 0 == stack[last].count(key_old) ) {
        printf
        ("error c_symbols_table::change_key -> ! stack[last].count(key_old)\n");
        exit(-1);
    }

    if ( 0 == key_new.size() ) {
        printf
        ("error c_symbols_table::change_key -> 0 == key_new.size()\n");
        exit(-1);
    }

    stack[last][key_new] = stack[last][key_old];
    stack[last].erase(key_old);
    stack[last][key_new].token.text = key_new;
}
/*----------------------------------------------------------------------------*/
void c_symbols_table::print(void)
{
    unsigned i_stack = 0;

    t_symbols::iterator i_map;

    printf("c_symbols_table::print\n");
    printf("{\n");
    if (stack.size() <= 0) {
        printf("  empty\n");
    }

    for (i_stack = 0; i_stack < stack.size(); ++i_stack) {
        printf("  stack level[%d]\n", i_stack);
        printf("  {\n");
        for (i_map = stack[i_stack].begin(); i_map != stack[i_stack].end();
                ++i_map) {
            printf("    first[%s]", ((*i_map).first).c_str());
            ((*i_map).second).print("    ");
        }
        printf("  }\n");
    }
    printf("}\n");
}

/*----------------------------------------------------------------------------*/
c_symbol *c_symbols_table::search_symbol(string str)
{
    c_symbol *p_symbol = 0;

    unsigned i_stack = 0;

    t_symbols::iterator i_map;

    // printf("c_symbols_table::search_symbol\n");
    // printf("{\n");
    if (stack.size() <= 0) {
        printf("  empty\n");
    }

    for (i_stack = 0; i_stack < stack.size(); ++i_stack) {
        // printf(" stack level[%d]\n",i_stack);
        // printf(" {\n");
        i_map = stack[i_stack].find(str);
        if (i_map != stack[i_stack].end()) {
            p_symbol = &((*i_map).second);
            /*
             * printf(" first[%s] id[%d] text[%s]\n" ,
             * ((*i_map).first).c_str() , ((*i_map).second).token.id ,
             * ((*i_map).second).token.text.c_str() );
             */
        }
        // printf(" }\n");
    }
    // printf("}\n");

    return p_symbol;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
c_symbols_table ts;
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
void ts_tests_load_fixture_01(void)
{
    c_token t1(1, (char *) "level 0 ->1");
    // c_symbol s1(t1);

    ts.set();

    ts.insert(t1);

    ts.set();

    c_token t2(2, (char *) "level 1 -> 2");
    ts.insert(t2);

    c_token t3(3, (char *) "level 1 -> 3");
    ts.insert(t3);

    c_token t4(4, (char *) "level 1 -> 4");
    ts.insert(t4);

    ts.set();
    c_token t5(5, (char *) "level 2 -> 5");
    ts.insert(t5);
}

/*----------------------------------------------------------------------------*/
void ts_tests_01(void)
{
    ts_tests_load_fixture_01();

    printf("print [0-2]\n");
    ts.print();

    printf("print [0-1]\n");
    ts.unset();
    ts.print();

    printf("print [0-0]\n");
    ts.unset();
    ts.print();

    printf("print [empty]\n");
    ts.unset();
    ts.print();
}

/*----------------------------------------------------------------------------*/
void ts_tests_02(void)
{
    ts_tests_load_fixture_01();

    c_symbol *p_symbol = ts.search_symbol("level 1 -> 3");
    if (p_symbol) {
        p_symbol->print("  ");
    }

    ts.unset();
    ts.unset();
    ts.unset();
}

/*----------------------------------------------------------------------------*/
void ts_tests(void)
{
    printf("symbols_table testing\n");
    // c_symbols_table ts;
    // ts_tests_load_fixture_01();
    ts_tests_01();
    ts_tests_02();

    // this is useful to lock if stack detect empty state
    // ts.unset();
    printf("symbols_table testing end\n");
}

/*----------------------------------------------------------------------------*/
