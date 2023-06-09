//
// Created by flassabe on 19/11/2021.
//

#include "table.h"

#include <sys/stat.h>
#include <unistd.h>

/*!
 * @brief function open_definition_file opens the table key file
 * @param table_name the name of the table whose definition file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_definition_file(char *table_name, char *mode) {
    char ext[] = ".def";
    char *full_path;
    size_t full_len = 2 * strlen(table_name) + 6;
    full_path = (char *) malloc(sizeof(char) * full_len);
    full_path = make_full_path(table_name, table_name);
    strcat(full_path,ext);
    FILE *fptr = fopen(full_path, mode);
    free(full_path);
    if (fptr != NULL) {
        return fptr;
    }
    return NULL;
}

/*!
 * @brief function open_index_file opens the table key file
 * @param table_name the name of the table whose index file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_index_file(char *table_name, char *mode) {
    char ext[] = ".idx";
    char *full_path;
    size_t full_len = 2 * strlen(table_name) + 6;
    full_path = (char *) malloc(sizeof(char) * full_len);
    full_path = make_full_path(table_name, table_name);
    strcat(full_path,ext);
    FILE *fptr = fopen(full_path, mode);
    free(full_path);
    if (fptr != NULL) {
        return fptr;
    }
    return NULL;
}

/*!
 * @brief function open_content_file opens the table key file
 * @param table_name the name of the table whose content file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_content_file(char *table_name, char *mode) {
    char ext[] = ".data";
    char *full_path;
    size_t full_len = 2 * strlen(table_name) + 7;
    full_path = (char *) malloc(sizeof(char) * full_len);
    full_path = make_full_path(table_name, table_name);
    strcat(full_path,ext);
    FILE *fptr = fopen(full_path, mode);
    free(full_path);
    if (fptr != NULL) {
        return fptr;
    }
    return NULL;
}

/*!
 * @brief function open_key_file opens the table key file
 * @param table_name the name of the table whose key file must be opened
 * @param mode the opening mode (as in fopen)
 * @return and pointer to a FILE type, resulting from the fopen function
 */
FILE *open_key_file(char *table_name, char *mode) {
    char ext[] = ".key";
    char *full_path;
    size_t full_len = 2 * strlen(table_name) + 6;
    full_path = (char *) malloc(sizeof(char) * full_len);
    full_path = make_full_path(table_name, table_name);
    strcat(full_path,ext);
    FILE *fptr = fopen(full_path, mode);
    free(full_path);
    if (fptr != NULL) {
        return fptr;
    }
    return NULL;
}

int table_exists(char *table_name) {
    FILE *f;
    if (directory_exists(table_name)) {
        f = open_definition_file(table_name, "r");
        if (f != NULL) {
            fclose(f);
            f = open_index_file(table_name, "r");
            if (f != NULL) {
                fclose(f);
                f = open_content_file(table_name, "r");
                if (f != NULL) {
                    fclose(f);
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

/*!
 * @brief function create_table creates the files for a new table. It checks if the table doesn't already exist,
 * creates its directory, its definition file, whose content is written based on the definition. It creates both
 * index and content empty files, and if there is a primary key, it creates the primary key file with a stored value
 * of 1.
 * @param table_definition a pointer to the definition of the new table
 */
void create_table(create_query_t *table_definition) {
    if (!table_exists(table_definition->table_name)) {
        mkdir(table_definition->table_name, S_IRWXU);
        FILE *fptr;
        fptr = open_index_file(table_definition->table_name, "wb");
        fclose(fptr);
        fptr = open_content_file(table_definition->table_name, "wb");
        fclose(fptr);
        fptr = open_definition_file(table_definition->table_name, "w");
        for (int i = 0; i<table_definition->table_definition.fields_count; i++) {
            fprintf(fptr, "%d %s\n", table_definition->table_definition.definitions[i].column_type, table_definition->table_definition.definitions[i].column_name);
            if (table_definition->table_definition.definitions[i].column_type == TYPE_PRIMARY_KEY) {
                FILE *fptr2 = open_key_file(table_definition->table_name, "wb");
                unsigned long long index = 1;
                fwrite(&index, sizeof(index), 1, fptr2);
                fclose(fptr2);
            }
        }
        fclose(fptr);
    }


}

/*!
 * @brief function drop_table removes all files and directory related to a table
 * @param table_name the name of the dropped table.
 */
void drop_table(char *table_name) {
    if (table_exists(table_name)) {
        recursive_rmdir(table_name);
    }
}

/*!
 * @brief function get_table_definition reads and returns a table definition
 * @param table_name the name of the target table
 * @param result a pointer to a previously allocated table_definition_t structure where the result will be stored.
 * @return the pointer to result, NULL if the function failed
 */
table_definition_t *get_table_definition(char *table_name, table_definition_t *result) {
    if (table_exists(table_name)) {
        char ext[] = ".def";
        char *full_path;
        size_t full_len = 2 * strlen(table_name) + 6;
        full_path = (char *) malloc(sizeof(char) * full_len);
        full_path = make_full_path(table_name, table_name);
        strcat(full_path,ext);
        FILE *fptr = open_definition_file(table_name, "r");
        if (fptr != NULL) {
            int type;
            char column_name[TEXT_LENGTH];
            result->fields_count = 0;
            while (fscanf(fptr, "%d %[^\n]", &type, column_name) == 2) {
                result->definitions[result->fields_count].column_type = type;
                strcpy(result->definitions[result->fields_count].column_name, column_name);
                result->fields_count += 1;
            }
            fclose(fptr);
        }
        return result;
    }
    return NULL;
}

/*!
 * @brief function compute_record_length computed the in-memory length of a table record. It sums all fields sizes
 * by their respective sizes. Text is always stored on TEXT_LENGTH chars so a text field in-memory size is always
 * TEXT_LENGTH, whatever its actual content strlen.
 * @param definition the table definition whose record length we need
 * @return the value of the record length.
 */
uint16_t compute_record_length(table_definition_t *definition) {
    uint16_t length = 0;
    for (int i = 0; i<definition->fields_count; ++i) {
        switch (definition->definitions[i].column_type) {
            case TYPE_PRIMARY_KEY:
                length += sizeof(unsigned long long);
                break;
            case TYPE_INTEGER:
                length += sizeof(long long);
                break;
            case TYPE_FLOAT:
                length += sizeof(double);
                break;
            case TYPE_TEXT:
                length += TEXT_LENGTH;
                break;
            default:
                break;
        }
    }
    return length;
}

/*!
 * @brief function find_first_free_record finds the first free record in the index file. The first free record is:
 * - the first index_record_t occurrence in the index file whose active field is 0. Its active field must be put to 1
 * before ending the function (replace value inplace)
 * - if none exists: a new field_record_t added to the end of the file, with active set to 1, and others fields to 0.
 * @param table_name the name of the target table
 * @return the offset of the free index in the index file.
 */
uint32_t find_first_free_record(char *table_name) {
    FILE *index = open_index_file(table_name, "rb+"); //ouverture en lecture sans écraser le contenu
    
    uint8_t active = 1;
    uint32_t offset = 0;

    index_record_t buffer;
    buffer.is_active = active; //Active = 1

    if (index != NULL) {
        do {
            fread(&buffer, sizeof(buffer), 1, index); //Si le fichier est vide ou qu'on est à la fin du fichier le buffer n'est pas modifié
            active = buffer.is_active;
        } while ((!feof(index)) && (active != 0));

        if (feof(index) != 0) {
            buffer.is_active = 1;
            buffer.record_length = 0;
            buffer.record_offset = 0;
            fwrite(&buffer, sizeof(buffer), 1, index);
        } else if (active == 0) {
            buffer.is_active = 1; //Active = 1
            fseek(index, -7, SEEK_CUR); //On se décale de 7 octets avant
            fwrite(&buffer, sizeof(buffer), 1, index);
        }
        fseek(index, -7, SEEK_CUR);
        offset = ftell(index);
        fclose(index);
    }
    return offset;
}

/*!
 * @brief function add_row_to_table adds a row to a table. Relies on format_row, compute_record_length,
 * find_first_free_record, get_table_definition, open_content_file, open_index_file
 * @param table_name the name of the target table
 * @param record the record to add
 */
void add_row_to_table(char *table_name, table_record_t *record) {
    FILE *index = open_index_file(table_name, "rb+"); //ouverture en lecture/écriture sans écraser le contenu
    if (index == NULL) {
        printf("Problème d'accès aux fichiers index de la table %s\n", table_name);
        return;
    }
    FILE *data = open_content_file(table_name, "rb+");
    if (data == NULL) {
        printf("Problème d'accès aux fichiers data de la table %s\n", table_name);
        fclose(index); //on ferme index car on a pu ouvrir le fichier
        return;
    }
    
    table_definition_t definition;
    if (get_table_definition(table_name, &definition) == NULL) { //On récupère la définition de la table
        printf("Inserttion impossible dans la table %s, probleme disque\n", table_name);
        return; //on n'a pas pu lire donc on sort de la fonction
    }
    index_record_t buffer;
    uint32_t offset_index = find_first_free_record(table_name);
    uint16_t taille_buffer = compute_record_length(&definition); //On récupère la somme des longueurs des champs

    fseek(index, offset_index, SEEK_SET); //On se place où on a trouvé le active à 0
    fread(&buffer, sizeof(buffer), 1, index);
    
    if (buffer.record_offset == 0) { //Si on a pas trouvé de active à 0 (donc début ou fin de fichier index)
        fseek(data, 0, SEEK_END); 
        buffer.record_offset = ftell(data); //On récupère l'offset à la fin du fichier data
        buffer.record_length = taille_buffer; //On récupère la somme des longueurs des champs
        fseek(index, offset_index, SEEK_SET); //On se place où on a trouvé le active à 0
        fwrite(&buffer, sizeof(buffer), 1, index);
    }
    
    char donnees[MAX_FIELDS_COUNT*TEXT_LENGTH]; //Pour réserver la mémoire sans faire de malloc
    char *donnees_retour;
    donnees_retour = format_row(table_name, donnees, &definition, record);
    fseek(data, buffer.record_offset, SEEK_SET); //On se positionne à l'offset défini dans index
    fwrite(donnees_retour, taille_buffer, 1, data);

    for (int i=0; i<record->fields_count; i++) {
        if (record->fields[i].field_type == TYPE_PRIMARY_KEY) {
            update_key(table_name, record->fields[i].field_value.primary_key_value); //La valeur est mise à jour si la valeur de la clé est supérieur à celle enregistrée
        }
    }

    fclose(data);
    fclose(index);
}

/*!
 * @brief function format_row prepares the buffer to be written to the content file
 * @param table_name the name of the target table
 * @param buffer the buffer where the row content is prepared
 * @param table_definition the table definition for fields ordering and formatting
 * @param record the record to write
 * @return a pointer to buffer in case of success, NULL else.
 */
char *format_row(char *table_name, char *buffer, table_definition_t *table_definition, table_record_t *record) {
    size_t longueur = 0;

    for (int i=0; i<table_definition->fields_count; i++) {
        for (int j=0; j<record->fields_count; j++) {
            if (strcmp(record->fields[j].column_name, table_definition->definitions[i].column_name) == 0) {
                switch (record->fields[j].field_type) {
                case TYPE_INTEGER:
                    memcpy(buffer + longueur, &record->fields[j].field_value.int_value, sizeof(long long));
                    longueur += sizeof(long long);
                    break;
                case TYPE_FLOAT:
                    memcpy(buffer + longueur, &record->fields[j].field_value.float_value, sizeof(double));
                    longueur += sizeof(double);
                    break;
                case TYPE_TEXT:
                    memcpy(buffer + longueur, record->fields[j].field_value.text_value, TEXT_LENGTH);
                    longueur += TEXT_LENGTH;
                    break;
                case TYPE_PRIMARY_KEY:
                    memcpy(buffer + longueur, &record->fields[j].field_value.primary_key_value, sizeof(unsigned long long));
                    longueur += sizeof(unsigned long long);
                    break;
                default:
                    buffer = NULL;
                    break;
                }  
                break; //On a traité la ligne, on continue
            }
        }
    }
    return buffer;
}

/*!
 * @brief function update_key updates the key value in the key file. Key value is updated if and only if the new value
 * is higher than the stored value. The value sent to the function is the last value inserted into the table, so the
 * function must increment it before comparing to the key file content.
 * @param table_name the name of the table whose key file must be updated
 * @param value the new key value
 */
void update_key(char *table_name, unsigned long long value) {
    if (table_exists(table_name)) {
        FILE *fptr = open_key_file(table_name, "rb+");
        if (fptr != NULL) {
            unsigned long long actual_key;
            fread(&actual_key, sizeof(unsigned long long), 1, fptr);
            if (value > actual_key) {
                rewind(fptr);
                fwrite(&value, sizeof(unsigned long long), 1, fptr);
            }
            fclose(fptr);
        }
    }
}

/*!
 * @brief function get_next_key extract the next key value from a table key file.
 * @param table_name the name of the table whose key value we need.
 * @return the next value of the key is it exists, 0 else
 */
unsigned long long get_next_key(char *table_name) {
    if (table_exists(table_name)) {
        FILE *fptr = open_key_file(table_name, "rb");
        if (fptr != NULL) {
            unsigned long long value;
            //fscanf(fptr, "%llu", &value);
            fread(&value, sizeof(unsigned long long), 1, fptr);
            fclose(fptr);
            return value+1;
        }
    }
    return 0;
}

/*!
 * @brief function find_field_in_table_record looks up for a field by its name in a table_record_t structure
 * @param field_name the name of the field to look for
 * @param record a pointer to the table_record_t structure
 * @return a pointer to the matching field_record_t if found, NULL else
 */
field_record_t *find_field_in_table_record(char *field_name, table_record_t *record) {
    for (int i=0; i<record->fields_count; i++) { //Pour chaque champs
        if (strcmp(field_name, record->fields[i].column_name) == 0) {
            return &record->fields[i];
        }
    }
    //Si aucun champs n'a mené à un return, on retourne NULL
    return NULL;
}

/*!
 * @brief function is_matching_filter tests a table record against a filter to check if the record matches the filter,
 * i.e. conditions in the filter are verified by the record. The filter contains one or more equalities to check.
 * Tests are all following a single AND or OR logical operator.
 * @param record the record to be checked
 * @param filter the filter to check against (a NULL filter means no filtering must be done -> returns 0)
 * @return true if the record matches the filter, false else
 */
bool is_matching_filter(table_record_t *record, filter_t *filter) {
    if (filter == NULL) {
        //Si NULL on retourne faux
        return false;
    } else if (filter->values.fields_count == 0) {
        //la clause where est vide, donc quelque soit les records ils correspondent
        return true;
    } else if (filter->logic_operator == OP_ERROR) {
        //Il y a au moins 1 clauses where mais l'opérateur est invalide
        return false;
    } else { //Sinon on test les conditions du filtre
        bool or = false; //Variable qui vérifie la condition or
        bool and = true; //Variable qui vérifie la condition and

        field_record_t *field_record;

        for (int i=0; i<filter->values.fields_count; i++) { //Pour chaque champs filter
            field_record = find_field_in_table_record(filter->values.fields[i].column_name, record);
            if (field_record != NULL) {
                    switch (field_record->field_type) {
                        //Si la valeur correspond au filtre alors on doit calcule or et and avec les valeurs: 
                        //   1 : or=or | true
                        //   2 : and=and & true
                        //Si la valeur ne correspond pas au filtre alors on doit calcule or et and avec les valeurs:
                        //   3 : or=or | false
                        //   4 : and=and & false
                        // On voit que les cas 2 et 3 ne sont pas utile
                        case TYPE_INTEGER:
                            if (field_record->field_value.int_value == filter->values.fields[i].field_value.int_value) {
                                or |= true;
                            } else {
                                and &= false;
                            }
                            break;
                        case TYPE_FLOAT:
                            if (field_record->field_value.float_value == filter->values.fields[i].field_value.float_value) {
                                or |= true;
                            } else {
                                and &= false;
                            }
                            break;
                        case TYPE_PRIMARY_KEY:
                            if (field_record->field_value.primary_key_value == filter->values.fields[i].field_value.primary_key_value) {
                                or |= true;
                            } else {
                                and &= false;
                            }
                            break;
                        case TYPE_TEXT:
                            if (strcmp(field_record->field_value.text_value, filter->values.fields[i].field_value.text_value) == 0) {
                                or |= true;
                            } else {
                                and &= false;
                            }
                            break;
                        default:
                                or |= false;
                                and &= false;
                            break;
                    }
            } else { //On ne devrait normalement pas arrivé dans ce cas, car filter a été testé et doit être valide
                return false;
            }
        }
         
        if (filter->logic_operator == OP_OR) {
            return or;
        }
        else if (filter->logic_operator == OP_AND) {
            return and;
        }
        else {
            //S'il y a une erreur on retourne faux
            return false;
        }
    }
}

/*!
 * @brief function get_filtered_records gets a list (as a linked list) of table records limited to the fields
 * specified to the function and filtered by the specified WHERE clause.
 * @param table_name table from which to fetch data
 * @param required_fields the list of fields to be returned (cannot be NULL)
 * @param filter the WHERE clause filter. NULL if no filter must be applied
 * @param result a pointer to a previously allocated record_list_t. List shall be empty
 * @return a pointer to the first element of the resulting linked list. Shall be freed by the calling function
 */
record_list_t *get_filtered_records(char *table_name, table_record_t *required_fields, filter_t *filter, record_list_t *result) {
    if (result->head != NULL || required_fields == NULL) { //La liste chainée result doit être vide et required_fields ne peut pas être NULL
        return NULL;
    } else {
        FILE *index = open_index_file(table_name, "rb");
        if (index == NULL) {
            return NULL;
        }

        index_record_t index_record;
        table_definition_t table_definition;
        table_record_t record_lu;
        table_record_t record_afficher;

        if (get_table_definition(table_name, &table_definition) == NULL) {
            fclose(index);
            return NULL;
        }
        fseek(index, 0, SEEK_SET);
        while(fread(&index_record, sizeof(index_record), 1, index)) {    
            if (index_record.is_active) {
                if (get_table_record(table_name, index_record.record_offset, &table_definition, &record_lu) != NULL) {
                    if (is_matching_filter(&record_lu, filter)) {
                        record_afficher.fields_count = 0;
                        for (int i=0; i<required_fields->fields_count; i++) {
                            for (int j=0; j<record_lu.fields_count; j++) { //on enregistre le record pour l'envoye dans un record_list
                                if (strcmp(record_lu.fields[j].column_name, required_fields->fields[i].column_name) == 0) {
                                    strcpy(record_afficher.fields[record_afficher.fields_count].column_name, record_lu.fields[j].column_name);
                                    record_afficher.fields[record_afficher.fields_count].field_type = record_lu.fields[j].field_type;
                                    switch (record_lu.fields[j].field_type) {
                                        case TYPE_INTEGER:
                                            record_afficher.fields[record_afficher.fields_count].field_value.int_value = record_lu.fields[j].field_value.int_value;
                                            break;
                                        case TYPE_FLOAT:
                                            record_afficher.fields[record_afficher.fields_count].field_value.float_value = record_lu.fields[j].field_value.float_value;
                                            break;
                                        case TYPE_PRIMARY_KEY:
                                            record_afficher.fields[record_afficher.fields_count].field_value.primary_key_value = record_lu.fields[j].field_value.primary_key_value;
                                            break;
                                        case TYPE_TEXT:
                                            strcpy(record_afficher.fields[record_afficher.fields_count].field_value.text_value, record_lu.fields[j].field_value.text_value);
                                            break;
                                        default:
                                            break;
                                    }
                                    record_afficher.fields_count++;
                                }
                            }
                        }
                        add_record(result, &record_afficher);
                    }
                }
            }
        }
    }
    return result;
}

/*!
 * @brief function get_table_record reads a table record from its content file
 * @param table_name the table whose record must be read from
 * @param offset the position of the beginning of the record in the content file (found in the index file)
 * @param def the table definition
 * @param result a pointer to a previously allocated table_record_t structure
 * @return the pointer to to result if succeeded, NULL else.
 */
table_record_t *get_table_record(char *table_name, uint32_t offset, table_definition_t *def, table_record_t *result) {
    if (table_exists(table_name)) {
        FILE *fptr_content = open_content_file(table_name, "rb");
        if (fptr_content != NULL) {
            fseek(fptr_content, offset, SEEK_SET);
            result->fields_count = def->fields_count;
            // Pour chaque champ :
            for (int i=0; i<def->fields_count; i++) {
                strcpy(result->fields[i].column_name, def->definitions[i].column_name);
                result->fields[i].field_type = def->definitions[i].column_type;
                // Valeur lue est assignée au champ correspondant
                switch (def->definitions[i].column_type) {
                    case TYPE_INTEGER:
                        fread(&result->fields[i].field_value.int_value, sizeof(long long), 1, fptr_content);
                        break;
                    case TYPE_FLOAT:
                        fread(&result->fields[i].field_value.float_value, sizeof(double), 1, fptr_content);
                        break;
                    case TYPE_TEXT:
                        fread(result->fields[i].field_value.text_value, TEXT_LENGTH, 1, fptr_content);
                        break;
                    case TYPE_PRIMARY_KEY:
                        fread(&result->fields[i].field_value.primary_key_value, sizeof(unsigned long long), 1, fptr_content);
                        break;
                    default:
                        break;
                }
            }
            fclose(fptr_content);
        }
        return result;
    }
    return NULL;
}

/*!
 * @brief function delete_row_to_table
 * @param table_name the name of the target table
 * @param filter the WHERE clause filter. NULL if no filter must be applied
 */
void delete_row_to_table(char *table_name, filter_t *filter) {
        //met active à 0 sur les lignes où il y a la condition du where
    //lire le index et le data en même temps et si une ligne correspond au where on met active à 0
    FILE *index = open_index_file(table_name, "rb+");
    if (index == NULL) {
        return;
    }

    index_record_t index_record;
    size_t longueur = sizeof(index_record);
    table_definition_t table_defintion;
    table_record_t record_lu;

    if (get_table_definition(table_name, &table_defintion) == NULL) {
        fclose(index);
        return;
    }

    fseek(index, 0, SEEK_SET);
    while(fread(&index_record, longueur, 1, index)) {    
        if (index_record.is_active) {
            if (get_table_record(table_name, index_record.record_offset, &table_defintion, &record_lu) != NULL) {
                if (is_matching_filter(&record_lu, filter)) {
                    index_record.is_active = 0; //On ne modifie que active
                    fseek(index, -longueur, SEEK_CUR); //On se repositionne au début du dernier index que l'on vient de lire
                    fwrite(&index_record, longueur, 1, index); //On réécrit tout mais offset et lenght n'ont pas changé
                }
            }
        }
    }
}

/*!
 * @brief function set_row_to_table 
 * @param table_name the name of the target table
 * @param set_fields the list of fields to be update
 * @param filter the WHERE clause filter. NULL if no filter must be applied
 */
void set_row_to_table(char *table_name, table_record_t  *set_fields, filter_t *filter) {
    if (set_fields == NULL) { //S'il n'y a rien à mettre à jour
        return;
    }

    FILE *index = open_index_file(table_name, "rb"); //ouverture en lecture 
    if (index == NULL) {
        printf("Problème d'accès aux fichiers index de la table %s\n", table_name);
        return;
    }

    FILE *data = open_content_file(table_name, "rb+");
    if (data == NULL) {
        printf("Problème d'accès aux fichiers data de la table %s\n", table_name);
        fclose(index); //on ferme index car on n'a pas pu ouvrir le fichier
        return;
    }

    table_definition_t table_definition;
    index_record_t index_record;
    table_record_t record_lu;
    uint16_t taille_buffer;
    char donnees[MAX_FIELDS_COUNT*TEXT_LENGTH]; //Pour réserver la mémoire sans faire de malloc
    int nb_maj = 0;

    if (get_table_definition(table_name, &table_definition) == NULL) {
        printf("Update impossible dans la table %s, probleme disque\n", table_name);
        fclose(index);
        fclose(data);
        return;
    }
    
    taille_buffer = compute_record_length(&table_definition); //On récupère la somme des longueurs des champs
    
    fseek(index, 0, SEEK_SET);
    while(fread(&index_record, sizeof(index_record), 1, index)) {    
        if (index_record.is_active) {
            if (get_table_record(table_name, index_record.record_offset, &table_definition, &record_lu) != NULL) {
                if (is_matching_filter(&record_lu, filter)) { //On met à jour le record avec les valeurs du ou des SET
                    for (int i=0; i<record_lu.fields_count; i++) { //On parcourt les colonnes et on modifie si elle est dans set_fields
                        for (int j=0; j<set_fields->fields_count; j++) {
                            if (strcmp(record_lu.fields[i].column_name, set_fields->fields[j].column_name) == 0) {
                                switch (record_lu.fields[i].field_type) {
                                    case TYPE_INTEGER:
                                        record_lu.fields[i].field_value.int_value = set_fields->fields[j].field_value.int_value;
                                        break;
                                    case TYPE_FLOAT:
                                        record_lu.fields[i].field_value.float_value = set_fields->fields[j].field_value.float_value;
                                        break;
                                    case TYPE_PRIMARY_KEY:
                                        record_lu.fields[i].field_value.primary_key_value = set_fields->fields[j].field_value.primary_key_value;
                                        update_key(table_name, record_lu.fields[i].field_value.primary_key_value);
                                        break;
                                    case TYPE_TEXT:
                                        strcpy(record_lu.fields[i].field_value.text_value, set_fields->fields[j].field_value.text_value);
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                    }

                    if (format_row(table_name, donnees, &table_definition, &record_lu) != NULL) {
                        fseek(data, index_record.record_offset, SEEK_SET); //On se positionne à l'offset défini dans index
                        fwrite(donnees, taille_buffer, 1, data);
                        nb_maj++;
                    }
                }
            }
        }
    }
    printf("Nb de ligne mise à jour : %d\n", nb_maj);
    fclose(data);
    fclose(index);
}
