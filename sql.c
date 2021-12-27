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
    sql = get_sep_space(sql); //On passe tous les espaces devant le caract
    if (sql == NULL) {
        printf("Erreur sql=NULL dans get_sep_space_and_char\n");
    } else if (*sql == c) {
        //Si le caract est celui demandé, on passe tous les espaces après
        sql = get_sep_space(sql+1);
    } else {
        //Si le caractere n'est pas celui demandé, on retourne NULL
        sql = NULL;
    }

    return sql;
}

char *get_keyword(char *sql, char *keyword) {
    if (sql == NULL) {
        return NULL;
    }
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

    //Si les deux mots sont differents ou si keyword est plus grand que sql, il y a erreur
    if (different) {
        sql = NULL;
    }

    return sql;
}

char *get_field_name(char *sql, char *field_name) {
    if (sql == NULL) {
        return NULL;
    }
    sql = get_sep_space(sql);

    int i = 0;
    if (*sql == '\'') {
        sql++;
        while ((i++ < (TEXT_LENGTH-1)) && (*sql != '\'') && (*sql != '\0')) {
            *(field_name++) = *(sql++);
        }
        if ((*sql == '\0') || (i >= TEXT_LENGTH)) { //Erreur si on trouve pas la cote de fermeture
            return NULL;
        }
        sql++;
    } else {
        while ((i++ < (TEXT_LENGTH-1)) && (*sql != ' ') && (*sql != ',') && (*sql != '\0')) {
            *(field_name++) = *(sql++);
        }
    }
    *(field_name++) = '\0';

    return sql;
}

bool has_reached_sql_end(char *sql) {
    int response = false;

    if (sql == NULL) {
        response = true;
    }
    sql = get_sep_space(sql);

    if (*sql == ';') {
        response = true;
    }

    return response;
}


char *parse_fields_or_values_list(char *sql, table_record_t *result) {
    if (sql == NULL) {
        return NULL;
    }
    sql = get_sep_space(sql);

    char buffer[TEXT_LENGTH];
    char *sql_next = sql;
    do {
        sql = sql_next;
        sql = get_field_name(sql, buffer);
        strcpy(result->fields[result->fields_count].field_value.text_value, buffer); //On rempli text_value avec le champ récupéré par get_field_name
        result->fields_count++; //On incrémente le compteur de champs

        sql_next = get_sep_space_and_char(sql, ',');
    } while ((sql != sql_next) && (result->fields_count < MAX_FIELDS_COUNT));

    if ((sql != sql_next) && (result->fields_count >= MAX_FIELDS_COUNT)) {
        sql = NULL;
    }
    return sql;
}


char *parse_create_fields_list(char *sql, table_definition_t *result) {
    if (sql == NULL) {
        return NULL;
    }
    sql = get_sep_space(sql);
    sql = get_sep_space_and_char(sql, '(');

    char buffer[TEXT_LENGTH];
    char *sql_next = sql;
    do {
        sql = sql_next;
        sql = get_field_name(sql, buffer);
        strcpy(result->definitions[result->fields_count].column_name, buffer); //On rempli column_name avec le champ récupéré par get_field_name
        sql = get_field_name(sql, buffer);

        if (buffer[strlen(buffer)-1] == ')') {
            buffer[strlen(buffer)-1] = '\0';
        }
        for (int i=0; i<strlen(buffer); i++) {
            buffer[i] = toupper(buffer[i]);
        }
        if (strcmp(buffer, "INT") == 0) {
            result->definitions[result->fields_count].column_type = TYPE_INTEGER; //On rempli column_type avec le champ récupéré par get_field_name    
        } else if (strcmp(buffer, "PRIMARY KEY") == 0) {
            result->definitions[result->fields_count].column_type = TYPE_PRIMARY_KEY; //On rempli column_type avec le champ récupéré par get_field_name
        } else if (strcmp(buffer, "FLOAT") == 0) {
            result->definitions[result->fields_count].column_type = TYPE_FLOAT; //On rempli column_type avec le champ récupéré par get_field_name
        } else if (strcmp(buffer, "TEXT") == 0) {
            result->definitions[result->fields_count].column_type = TYPE_TEXT; //On rempli column_type avec le champ récupéré par get_field_name
        } else {
            result->definitions[result->fields_count].column_type = TYPE_UNKNOWN; //On rempli column_type avec le champ récupéré par get_field_name
        }
        result->fields_count++; //On incrémente le compteur de champs

        sql_next = get_sep_space_and_char(sql, ',');
    } while ((sql != sql_next) && (result->fields_count < MAX_FIELDS_COUNT));

    if ((sql != sql_next) && (result->fields_count >= MAX_FIELDS_COUNT)) {
        sql = NULL;
    }
    sql = get_sep_space_and_char(sql, ')');

    return sql;
}

char *parse_equality(char *sql, field_record_t *equality) {
    if (sql == NULL) {
        return NULL;
    }
    sql = get_sep_space(sql);

    char buffer[TEXT_LENGTH];
    sql = get_field_name(sql, buffer);
    strcpy(equality->column_name, buffer);

    char *sql_next = get_sep_space_and_char(sql, '=');
    if (sql == sql_next) { //Si le pointeur sql n'a pas bougé, le '=' n'a pas été trouvé
        return NULL;
    } else {
        sql = sql_next;
    }

    sql = get_field_name(sql, buffer);
    strcpy(equality->field_value.text_value, buffer);
    equality->field_type = TYPE_UNKNOWN;

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
