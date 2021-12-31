//
// Created by flassabe on 23/11/2021.
//

#include "expand.h"
#include "table.h"

void expand(query_result_t *query) {
    if (query->query_type == QUERY_SELECT) {
        expand_select(&query->query_content.select_query);
    } else if (query->query_type == QUERY_INSERT) {
        expand_insert(&query->query_content.insert_query);
    }
}

void expand_select(update_or_select_query_t *query) {
    bool etoile = false;
    table_definition_t table_definition;
    table_definition_t *retour_definition;

    for (int i=0; i<query->set_clause.fields_count; i++) {
        if (strcmp(query->set_clause.fields[i].column_name, "*") == 0) { //Si le champ est une étoile
            etoile = true;
            retour_definition = get_table_definition(&query->table_name, &table_definition);
            query->set_clause.fields_count = table_definition.fields_count;
            for (int j=0; j<table_definition.fields_count; j++) {
                strcpy(query->set_clause.fields[j].column_name, table_definition.definitions[j].column_name); //On rempli les champs par tous les champs de la table
            }
            break;
        }
    }
}

void expand_insert(insert_query_t *query) {
    table_definition_t table_definition;

    if (get_table_definition(&query->table_name, &table_definition) != NULL) {
        for (int i=0; i<table_definition.fields_count; i++) {
            if (!is_field_in_record(&query->fields_names, table_definition.definitions[i].column_name)) {
                strcpy(query->fields_names.fields[query->fields_names.fields_count].column_name, table_definition.definitions[i].column_name); //On rajoute le champ qui n'a pas été trouvé
                query->fields_names.fields[query->fields_names.fields_count].field_type = table_definition.definitions[i].column_type; //On met à jour le type de valeur
                make_default_value(&query->fields_names.fields[query->fields_names.fields_count], query->table_name); //On met la valeur par défaut
                query->fields_names.fields_count++;
            }
        }
    }
}

bool is_field_in_record(table_record_t *record, char *field_name) {
    for (int i = 0; i<record->fields_count; ++i) {
        if (strcmp(record->fields[i].column_name, field_name) == 0) {
            return true;
        }
    }
    return false;
}

void make_default_value(field_record_t *field, char *table_name) {
    switch(field->field_type) {
        case TYPE_INTEGER:
            field->field_value.int_value = 0;
            break;
        case TYPE_FLOAT:
            field->field_value.float_value = 0.0;
            break;
        case TYPE_TEXT:
            strcpy(field->field_value.text_value, "");
            break;
        case TYPE_PRIMARY_KEY:
            field->field_value.primary_key_value = get_next_key(table_name); // s'il n'y a pas de clé primaire la valeur sera 0
            break;
        default:
            break;
    }
}
