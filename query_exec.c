//
// Created by flassabe on 22/11/2021.
//

#include "query_exec.h"

#include <dirent.h>
#include <unistd.h>

#include "database.h"
#include "table.h"

void execute(query_result_t *query) {
    switch (query->query_type) {
        case QUERY_CREATE_TABLE:
            execute_create(&query->query_content.create_query);
            break;
        case QUERY_INSERT:
            execute_insert(&query->query_content.insert_query);
            break;
        case QUERY_SELECT:
            execute_select(&query->query_content.select_query);
            break;
        case QUERY_UPDATE:
            execute_update(&query->query_content.update_query);
            break;
        case QUERY_DELETE:
            execute_delete(&query->query_content.delete_query);
            break;
        case QUERY_DROP_TABLE:
            execute_drop_table(query->query_content.table_name);
            break;
        case QUERY_DROP_DB:
            execute_drop_database(query->query_content.database_name);
            break;
        default:
            break; 
    }
}

void execute_create(create_query_t *query) {
    if (!directory_exists("db")) {
        create_db_directory("db");
    }
    create_table(query);
}

void execute_insert(insert_query_t *query) {
    add_row_to_table(query->table_name, &query->fields_names);
}

void execute_select(update_or_select_query_t *query) {
    record_list_t *record_list = malloc(sizeof(record_list_t));
    clear_list(record_list);
    get_filtered_records(query->table_name, &query->set_clause, &query->where_clause, record_list);
    display_table_record_list(record_list);
    clear_list(record_list);
    free(record_list);
}

void execute_update(update_or_select_query_t *query) {
    //mettre à jour les lignes qui correspondes au where
    char buffer_data[MAX_FIELDS_COUNT*TEXT_LENGTH];
    index_record_t buffer_index;
    record_list_t record_list;
    table_definition_t definition;
    table_record_t record_data;
    int longueur_lu = 0;

    FILE *data = open_content_file(query->table_name, "rb+");
    if (data == NULL) {
        printf("Erreur d'accès au fichier data\n");
        return;
    }
    FILE *index = open_index_file(query->table_name, "rb");
    if (index == NULL) {
        printf("Erreur d'accès au fichier index\n");
        fclose(data);
        return;
    }
    get_table_definition(query->table_name, &definition);
    record_data.fields_count = definition.fields_count;
    while (!feof(index)) {
        fread(&buffer_index, sizeof(buffer_index), 1, index);
        if (buffer_index.is_active == 1) {
            fread(buffer_data, sizeof(buffer_data), 1, data);
            longueur_lu = 0;

            for (int i=0; i<definition.fields_count; i++) {
                switch(definition.definitions[i].column_type) {
                    case TYPE_INTEGER:
                        record_data.fields[i].field_value.int_value = *(buffer_data+longueur_lu);
                        longueur_lu += sizeof(long long);
                        break;
                    case TYPE_FLOAT:
                        record_data.fields[i].field_value.float_value = *(buffer_data+longueur_lu);
                        longueur_lu += sizeof(double);
                        break;
                    case TYPE_PRIMARY_KEY:
                        record_data.fields[i].field_value.primary_key_value = *(buffer_data+longueur_lu);
                        longueur_lu += sizeof(unsigned long long);
                        break;
                    case TYPE_TEXT:
                        strcpy(record_data.fields[i].field_value.text_value, buffer_data+longueur_lu);
                        longueur_lu += TEXT_LENGTH;
                        break;
                    default:
                        break;
                }
            }

            /*for (int j=0; j<query->where_clause.values.fields_count; j++) {
                if (record_data.fields[j].field_value == query->where_clause.values.fields[j].field_value) {

                }
            }*/
        }
    }
    get_filtered_records(query->table_name, &query->where_clause.values, &query->where_clause, &record_list);
    
}

void execute_delete(delete_query_t *query) {
    //met active à 0 sur les lignes où il y a la condition du where
    //lire le index et le data en même temps et si une ligne correspond au where on met active à 0
    record_list_t test;
 //   add_record();
}

void execute_drop_table(char *table_name) {
    drop_table(table_name);
}

void execute_drop_database(char *db_name) {
    recursive_rmdir(db_name);
}
