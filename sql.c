//
// Created by flassabe on 16/11/2021.
//

#include "sql.h"

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

char *get_sep_space(char *sql) {
    if(sql == NULL){
        printf("Erreur sql=NULL dans get_sep_space\n");
    }
    else{
        //Tant que le caract est un espaces on passe au suivant
        while(*sql == ' '){
            sql++;
        }
    }
    return sql;
}

char *get_sep_space_and_char(char *sql, char c) {
    char *backup = sql;
    sql = get_sep_space(sql); //On passe tous les espaces devant le caract
    if(sql == NULL){
        printf("Erreur sql=NULL dans get_sep_space_and_char\n");
    }
    else if(*sql == c){
        //Si le caract est celui demandé, on passe tous les espaces après
        sql = get_sep_space(sql+1);
    }
    else{
        //Si le caract n'est pas celui demandé, on rétabli sql comme initialement
        printf("Erreur rencontree dans fonction get_sep_space_and_char\n");
        sql = backup;
    }
    return sql;
}

char *get_keyword(char *sql, char *keyword) {
    char *backup = sql;
    int different = 0;
    int longueur = strlen(keyword);
    if (longueur <= strlen(sql)) { //Keyword ne doit pas être plus grand que sql
        for (int i=0; i<longueur; i++) { //Pour chaque caractere
            if (toupper(*(sql++)) != toupper(*(keyword++))) { //Si ce caract est different d'un mot à l'autre
                different = 1; //On défini la variable different a vrai
            }
        }
    } else {
        different = 1;
    }

    //Si les deux mots sont differents ou que keyword est plus grand que sql, on rétablit sql comme initialement
    if (different) {
        sql = backup;
    }

    return sql;
}

char *get_field_name(char *sql, char *field_name) {
    if(*sql == ' '){
        printf("ERREUR dans fonction get_field_name\n");
    }
    else if(*sql == '\''){
        sql++;
        while(*sql != '\''){
            *field_name = *sql;
            sql++;
            field_name++;
        }
    }
    else{
        while(*sql != ' '){
            *field_name = *sql;
            sql++;
            field_name++;
        }
    }
    return sql;
}

bool has_reached_sql_end(char *sql) {
    sql = get_sep_space(sql);
    int response=false;

    if(*sql==';'){
        response=true;
    }

    return response;
}

char *parse_fields_or_values_list(char *sql, table_record_t *result) {
    sql = get_sep_space(sql);
    char *temp;

    if(*sql == '('){
        sql++;
    }

    do{
        if(*sql == ','){
            sql++; //On passe à l'espace suivant la virgule
            sql++; //On passe au caract suivant l'espace
        }

        while(*sql != ',' && *sql != ' '){
            //Stocker chaque caractère quelque part dans la structure table_record_t mais jsp où
        }
        result->fields_count++; //On incrémente le compteur de champs
    }while(*sql == ',');
    return sql;
}

char *parse_create_fields_list(char *sql, table_definition_t *result) {
    return sql;
}

char *parse_equality(char *sql, field_record_t *equality) {
    return sql;
}

char *parse_set_clause(char *sql, table_record_t *result) {
    return sql;
}

char *parse_where_clause(char *sql, filter_t *filter) {
    return sql;
}

query_result_t *parse(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_select(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_create(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_insert(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_update(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_delete(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_drop_db(char *sql, query_result_t *result) {
    return NULL;
}

query_result_t *parse_drop_table(char *sql, query_result_t *result) {
    return NULL;
}
