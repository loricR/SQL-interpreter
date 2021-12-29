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
    create_table(&query->table_definition);
}

void execute_insert(insert_query_t *query) {
    FILE *index = open_index_file(query->table_name, "rb+"); //ouverture en lecture/écriture sans écraser le contenu
    
    table_definition_t *definition;
    definition = get_table_definition(query->table_name, definition); //On récupère la définition de la table
    uint8_t active = 1;
    uint32_t offset = 0;
    uint16_t length = compute_record_length(definition); //On récupère la somme des longueurs des champs

    char buffer[7];
    buffer[0] = active; //Active = 1
    memcpy(buffer[1], &offset, 32); //Offset = 00000000
    memcpy(buffer[5], &length, 16); //Length = la somme des longueurs des champs

    do {
        fread(buffer, 7, 1, index); //Si le fichier est vide ou qu'on est à la fin du fichier le buffer n'est pas modifié
        active = buffer;
        offset = buffer+1;
        length = buffer+5;
    } while ((!feof(index)) && (active != 0));

    if (feof(index) != 0) {
        buffer[0] = 1; //Active = 1
        offset = offset + length; //Offset = offset précédent + taille précédent
        memcpy(buffer[1], &offset, 32); 
        fwrite(buffer, 7, 1, index);
    } else if (active == 0) {
        buffer[0] = 1; //Active = 1
        fseek(index, -7, SEEK_CUR); //On se décale de 7 octets avant
        fwrite(buffer, 7, 1, index);
    }

    fclose(index);

    FILE *data = open_content_file(query->table_name, "rb+");
    FILE *key = open_key_file(query->table_name, "rb+");
    fseek(data, offset, SEEK_SET); //On se positionne à l'offset défini dans index
    for (int i=0; i<query->fields_values.fields_count; i++) {
        switch (query->fields_names.fields[i].field_type) {
        case TYPE_INTEGER:
            fwrite(&query->fields_values.fields[i].field_value.int_value, sizeof(long long), 1, data);
            break;
        case TYPE_FLOAT:
            fwrite(&query->fields_values.fields[i].field_value.float_value, sizeof(double), 1, data);
            break;
        case TYPE_TEXT:
            fwrite(query->fields_values.fields[i].field_value.text_value, TEXT_LENGTH, 1, data);
            break;
        case TYPE_PRIMARY_KEY:
            fseek(key, 0, SEEK_END); //On se positionne à la fin du fichier key
            fwrite(&query->fields_values.fields[i].field_value.primary_key_value, sizeof(unsigned long long), 1, data);
            fwrite(&query->fields_values.fields[i].field_value.primary_key_value, sizeof(unsigned long long), 1, key);
            break;
        default:
            break;
        }
        
    }
    fclose(data);
    if (key != NULL) {
        fclose(key);
    }
}

void execute_select(update_or_select_query_t *query) {}

void execute_update(update_or_select_query_t *query) {}

void execute_delete(delete_query_t *query) {
}

void execute_drop_table(char *table_name) {
}

void execute_drop_database(char *db_name) {
}
