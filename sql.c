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
        while ((i++ < (TEXT_LENGTH-1)) && (*sql != ' ') && (*sql != ',') && (*sql != ';') && (*sql != ')') && (*sql != '=') && (*sql != '\0')) {
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
    } else {
        sql = get_sep_space(sql);

        if (*sql == ';') {
            response = true;
        }
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
    result->fields_count = 0;
    do {
        sql = sql_next;
        sql = get_field_name(sql, buffer);
        strcpy(result->fields[result->fields_count].field_value.text_value, buffer); //On rempli text_value avec le champ récupéré par get_field_name
        strcpy(result->fields[result->fields_count].column_name, buffer); //On rempli column_name avec le champ récupéré par get_field_name
        result->fields_count++; //On incrémente le compteur de champs

        sql_next = get_sep_space_and_char(sql, ',');
    } while ((sql_next != NULL) && (result->fields_count < MAX_FIELDS_COUNT));

    if ((sql_next != NULL) && (result->fields_count >= MAX_FIELDS_COUNT)) {
        sql = NULL;
    }
    return sql;
}


char *parse_create_fields_list(char *sql, table_definition_t *result) {
    sql = get_sep_space(sql);
    sql = get_sep_space_and_char(sql, '(');
    if (sql == NULL) {
        return NULL;
    }

    char buffer[TEXT_LENGTH];
    char *sql_next = sql;
    result->fields_count = 0;
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
        } else if (strcmp(buffer, "PRIMARY") == 0) {
            sql = get_field_name(sql, buffer);
            for (int i=0; i<strlen(buffer); i++) {
                buffer[i] = toupper(buffer[i]);
            }
            if (strcmp(buffer, "KEY") == 0) {
                result->definitions[result->fields_count].column_type = TYPE_PRIMARY_KEY; //On rempli column_type avec le champ récupéré par get_field_name
            } 
        } else if (strcmp(buffer, "FLOAT") == 0) {
            result->definitions[result->fields_count].column_type = TYPE_FLOAT; //On rempli column_type avec le champ récupéré par get_field_name
        } else if (strcmp(buffer, "TEXT") == 0) {
            result->definitions[result->fields_count].column_type = TYPE_TEXT; //On rempli column_type avec le champ récupéré par get_field_name
        } else {
            result->definitions[result->fields_count].column_type = TYPE_UNKNOWN; //On rempli column_type avec le champ récupéré par get_field_name
        }
        result->fields_count++; //On incrémente le compteur de champs

        sql_next = get_sep_space_and_char(sql, ',');
    } while ((sql_next != NULL) && (result->fields_count < MAX_FIELDS_COUNT));

    if ((sql_next != NULL) && (result->fields_count >= MAX_FIELDS_COUNT)) {
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
    if (sql_next == NULL) { //Si le pointeur sql n'a pas bougé, le '=' n'a pas été trouvé
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
    if (sql == NULL) {
        return NULL;
    }
    result->fields_count = 0;
    char *sql_next = sql;
    do {
        sql = sql_next;
        sql = parse_equality(sql, &result->fields[result->fields_count]);
        result->fields_count++; //On incrémente le compteur de champs
        sql_next = get_sep_space_and_char(sql, ',');
    } while ((sql_next != NULL) && (result->fields_count < MAX_FIELDS_COUNT)); //Tant qu'il y a encore des égalités, donc des ','

    if ((sql_next != NULL) && (result->fields_count >= MAX_FIELDS_COUNT)) {
        sql = NULL;
    }

    return sql;
}

char *parse_where_clause(char *sql, filter_t *filter) {
    if (sql == NULL) {
        return NULL;
    }

    sql = get_sep_space(sql);
    char *sql_next = sql;
    filter->values.fields_count = 0;
    operator_t operator = OP_ERROR;
    do {
        sql = parse_equality(sql, &filter->values.fields[filter->values.fields_count]);

        sql = get_sep_space(sql);
        sql_next = get_keyword(sql, "OR");
        if (sql_next != NULL) { //On vérifie si l'opérateur entre les égalités est un OR
            operator = OP_OR;
            sql = sql_next;
        } else {
            sql_next = get_keyword(sql, "AND"); 
            if (sql_next != NULL) { //On vérifie si l'opérateur entre les égalités est un AND
                operator = OP_AND;
                sql = sql_next;
            }
        }
        
        if (filter->values.fields_count == 0) {
            filter->logic_operator = operator; //Si c'est le premier opérateur on le met à jour
        } else if (filter->logic_operator != operator) {
            filter->logic_operator = OP_ERROR; //Si c'est pas le même opérateur que le premier, c'est une erreur
        }

        filter->values.fields_count++; //On incrémente le compteur de champs
    } while ((sql_next != NULL) && (filter->values.fields_count < MAX_FIELDS_COUNT)); //Tant qu'il y a encore des égalités, donc des OR ou AND

    if ((sql_next != NULL) && (filter->values.fields_count >= MAX_FIELDS_COUNT)) {
        sql = NULL;
    }

    return sql;
}

query_result_t *parse(char *sql, query_result_t *result) {
    if (sql == NULL) {
        return NULL;
    }

    sql = get_sep_space(sql);
    //On appelle les fonctions qui correspondent au keyword de la requête
    if (get_keyword(sql, "SELECT") != NULL) {
        result = parse_select(sql, result);
    } else if (get_keyword(sql, "CREATE") != NULL) {
        result = parse_create(sql, result);
    } else if (get_keyword(sql, "INSERT") != NULL) {
        result = parse_insert(sql, result);
    } else if (get_keyword(sql, "UPDATE") != NULL) {
        result = parse_update(sql, result);
    } else if (get_keyword(sql, "DELETE") != NULL) {
        result = parse_delete(sql, result);
    } else if (get_keyword(sql, "DROP DATABASE") != NULL) {
        result = parse_drop_db(sql, result);
    } else if (get_keyword(sql, "DROP TABLE") != NULL) {
        result = parse_drop_table(sql, result);
    }
    return result;
}

query_result_t *parse_select(char *sql, query_result_t *result) {
    char *temp = sql;
    //Chaque fonction retourne NULL si sql est déjà NULL on peut donc exécuter les fonctions en cascade
    //Si sql est NULL sur une des fonction, on rentrera dans la fonction pour rien
    //Par lisibilité on met une ligne par fonction plutôt que de tout faire en une seule
    sql = get_keyword(sql, "SELECT"); //On récupère le select
    sql = get_sep_space(sql);
    result->query_type = QUERY_SELECT;
    sql = parse_fields_or_values_list(sql, &result->query_content.select_query.set_clause); //On récupère la liste des champs du SELECT
    sql = get_sep_space(sql);
    sql = get_keyword(sql, "FROM");
    sql = get_sep_space(sql);
    sql = get_field_name(sql, result->query_content.select_query.table_name); //On récupère le nom de la table
    sql = get_sep_space(sql);
    temp = get_keyword(sql, "WHERE");
    if (temp != NULL) { //S'il y a une clause WHERE on la traite, sinon on a fini de parser la requête
        sql = temp;
        sql = get_sep_space(sql);
        sql = parse_where_clause(sql, &result->query_content.select_query.where_clause); //On récupère la clause WHERE
    }
    if ((!has_reached_sql_end(sql)) || (sql == NULL)) {
        return NULL;
    }

    return result;
}

query_result_t *parse_create(char *sql, query_result_t *result) {
    sql = get_keyword(sql, "CREATE");
    sql = get_sep_space(sql);
    sql = get_keyword(sql, "TABLE");
    sql = get_sep_space(sql);
    result->query_type = QUERY_CREATE_TABLE;
    sql = get_field_name(sql, result->query_content.create_query.table_name); //On récupère le nom de la table
    sql = parse_create_fields_list(sql, &result->query_content.create_query.table_definition); //On récupère les champs du create

    if ((!has_reached_sql_end(sql)) || (sql == NULL)) {
        return NULL;
    }

    return result;
}

query_result_t *parse_insert(char *sql, query_result_t *result) {
    sql = get_keyword(sql, "INSERT");
    sql = get_sep_space(sql);
    sql = get_keyword(sql, "INTO");
    sql = get_sep_space(sql);
    result->query_type = QUERY_INSERT;
    sql = get_field_name(sql, result->query_content.insert_query.table_name); //On récupère le nom de la table
    sql = get_sep_space_and_char(sql, '('); //On enlève la parenthèse de début
    sql = parse_fields_or_values_list(sql, &result->query_content.insert_query.fields_names); //On récupère les champs
    sql = get_sep_space_and_char(sql, ')'); //On enlève la parenthèse de fin
    sql = get_sep_space(sql);
    sql = get_keyword(sql, "VALUES");
    sql = get_sep_space_and_char(sql, '('); //On enlève la parenthèse de début
    sql = parse_fields_or_values_list(sql, &result->query_content.insert_query.fields_values); //On récupère les valeurs
    sql = get_sep_space_and_char(sql, ')'); //On enlève la parenthèse de fin

    if ((!has_reached_sql_end(sql)) || (sql == NULL)) {
            return NULL;
    }

    return result;
}

query_result_t *parse_update(char *sql, query_result_t *result) {
    char *temp;
    sql = get_keyword(sql, "UPDATE");
    sql = get_sep_space(sql);
    result->query_type = QUERY_UPDATE;
    sql = get_field_name(sql, result->query_content.update_query.table_name); //On récupère le nom de la table
    sql = get_sep_space(sql);
    sql = get_keyword(sql, "SET");
    sql = get_sep_space(sql);
    sql = parse_set_clause(sql, &result->query_content.update_query.set_clause);
    sql = get_sep_space(sql);
    temp = get_keyword(sql, "WHERE");
    if (temp != NULL) { //S'il y a une clause WHERE on la traite, sinon on a fini de parser la requête
        sql = temp;
        sql = get_sep_space(sql);
        sql = parse_where_clause(sql, &result->query_content.update_query.where_clause); //On récupère la clause WHERE
    }
    
    if ((!has_reached_sql_end(sql)) || (sql == NULL)) {
            return NULL;
    }

    return result;
}

query_result_t *parse_delete(char *sql, query_result_t *result) {
    printf("parse_delete\n");
    return NULL;
}

query_result_t *parse_drop_db(char *sql, query_result_t *result) {
    printf("parse_drop_db\n");
    return NULL;
}

query_result_t *parse_drop_table(char *sql, query_result_t *result) {
    printf("parse_drop_table\n");
    return NULL;
}
