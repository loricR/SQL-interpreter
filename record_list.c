//
// Created by flassabe on 30/11/2021.
//

#include "record_list.h"

#include <stdio.h>
#include <string.h>

void clear_list(record_list_t *record_list) {
    if (record_list != NULL) {
        record_list_node_t *tmp = record_list->head;
        while (tmp) {
            record_list->head = tmp->next;
            free(tmp);
            tmp = record_list->head;
        }
        record_list->head = record_list->tail = NULL;
    }
}

void add_record(record_list_t *record_list, table_record_t *record) {
    if (!record_list)
        return;

    record_list_node_t *new_node = malloc(sizeof(record_list_node_t));
    memcpy(&new_node->record, record, sizeof (table_record_t));
    new_node->next = NULL;

    if (record_list->head == NULL) {
        record_list->head = record_list->tail = new_node;
        new_node->previous = NULL;
    } else {
        record_list->tail->next = new_node;
        new_node->previous = record_list->tail;
        record_list->tail = new_node;
    }
}

/*!
 * @brief function field_record_length returns a field display length, i.e. the characters count it requires to be
 * displayed (equal to number of digits, signs, and '.' for numbers, number of characters (excluding the trailing '\0')
 * for strings)
 * @param field_record the field value whose display length must be computed
 * @return the display length of the field
 */
int field_record_length(field_record_t *field_record) {
    int length = 0;
    if (field_record->field_type == TYPE_UNKNOWN) { //Si type inconnu
        length = 0;
    }
    else if (field_record->field_type == TYPE_TEXT) { //Si type texte
        length = strlen(field_record->field_value.text_value);
    }
    else if (field_record->field_type == TYPE_INTEGER || field_record->field_type == TYPE_PRIMARY_KEY) { //Si type entier
        float temp;
        if (field_record->field_type == TYPE_INTEGER) {
            temp = field_record->field_value.int_value;
        }
        else {
            temp = field_record->field_value.primary_key_value;
        }
        
        if (temp == 0) {
            length = 1;
        }
        else if (temp > 0) {
            while (temp/10 >= 1) {
                length++;
                temp = temp/10;
            }
            length++;
        }
        else {
            length++; //Pour le signe -
            while (temp/10 <= (-1)) {
                length++;
                temp = temp/10;
            }
            length++;
        }
    }
    else if (field_record->field_type == TYPE_FLOAT) { //Si type float
        float temp;
        if (field_record->field_value.float_value <0) {
            length++; //Pour le signe -
            temp = -field_record->field_value.float_value;
        }
        //On compte les chiffres avant la virgule :
        while (temp/10 > 1) {
            length++;
            temp = temp/10;
        }
        length++;
        //On compte les chiffres après la virgule :
        if (temp != (int)temp) {
            length++; //Pour la virgule
        }
        while (temp != (int)temp) {
            length++;
            temp = temp*10;
        }
    }
    return length;
}

/*!
 * @brief function display_table_record_list displays a select query result. It consists of 4 steps:
 * - Step 1: go through the whole result and compute the maximum size for each field (use an array of MAX_FIELDS_COUNT
 * integers). Also update max lengths with column_names values.
 * - Step 2: display result header: make a line formatted as +----+----+---+------+ etc. with '+' for columns limits,
 * a number of '-' equal to the maximum size of current field + 2, and so on.
 * - Step 2 bis: display the columns names, right-aligned, separated by char '|' and with a one space left-padding.
 * - Step 2 tes: add the same line as in step 2.
 * - Step 3: for each record in the result, display its values like you did with header column names.
 * Step 2 and 3 require that you add extra space padding to the left of the values for those to be aligned.
 * - Step 4: add a line as in step 2.
 * @param record_list the record list to display
 *
 * For instance, a record list with two fields named 'id' and 'label' and two records (1, 'blah'), and (2, 'foo') will
 * display:
  +----+-------+
  | id | label |
  +----+-------+
  |  1 |  blah |
  |  2 |   foo |
  +----+-------+
 */
void display_table_record_list(record_list_t *record_list) {
    //record_list_node_t *tmp = record_list->head;
    // Affichage du result header
    do {
        //tmp = record_list->head;
        int size = field_record_length(record_list->head->record.fields);
        

        //Affichage : 
        printf("+");

        for (int i=0; i<(size+2); i++) {
            printf("-");
        }
        //record_list->head = record_list->head->next;
        //free(tmp);
        //tmp = record_list->head;
        
    } while (record_list != NULL);
    //free(tmp);
    printf('+\n'); //+ de fin de passage de ligne














    /*// Affichage des noms de colonnes 
    do {
        tmp = record_list->head;
        char *name;

        int size;
        char *buffer = (char *) malloc(sizeof(char) * TEXT_LENGTH); 

        // Calcul de la taille maximale
        size = strlen(get_field_name(tmp->record, buffer));
        if (strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)) > size) {
            size = strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)); // lltoa convertit un long long en chaîne de caractère
        } else if (strlen(tmp->record.fields->field_value.text_value) > size) {
            size = strlen(tmp->record.fields->field_value.text_value);
        } else if (strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)) > size) {
            size = strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)); // ulltoa convertit un unsigned long long en chaîne de caractère
        } else if (strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)) > size) {
            size = strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)); // Convertit un double en chaîne de caractère
        }
        free(buffer);

        // Mise a jour du nom en fonction de la taille
        if (size > field_record_length(size)) {
            char *buffer = (char *)mallc(sizeof(char)* size);
            buffer = "";
            do {
                strcat(buffer, " ");
                size--;

            } while (size-field_record_length(record_list) != 0);
            strcat(buffer, get_field_name(tmp->record, name));
            name = buffer; 
            free (buffer);
        }

        printf("| ");
        printf("%s ", name);

        record_list->head = tmp->next;
        free(tmp);
        tmp = record_list->head;
Type lu: 4, champ lu: prenom
    } while (record_list != NULL);
    printf(" |"); // Barre de fin et passage de ligne


    // On re-affiche le result header
    do {
        tmp = record_list->head;
        int size;
        char *buffer = (char *) malloc(sizeof(char) *
Type lu: 4, champ lu: prenom TEXT_LENGTH);
 
        // Calcul de la taille maximale
        size = strlen(get_field_name(tmp->record, buffer));
        if (strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)) > size) {
            size = strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)); // lltoa convertit un long long en chaîne de caractère
        } else if (strlen(tmp->record.fields->field_value.text_value) > size) {
            size = strlen(tmp->record.fields->field_value.text_value);
        } else if (strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)) > size) {
            size = strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)); // ulltoa convertit un unsigned long long en chaîne de caractère
        } else if (strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)) > size) {
            size = strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)); // Convertit un double en chaîne de caractère
        }
        free(buffer);

        // Affichage : 
        printf("+");

        for (int i=0; i<size+2; i++) {
            printf("-");
        }
        record_list->head = tmp->next;
        free(tmp);
        tmp = record_list->head;
    } while (record_list != NULL);
    free(tmp);
    printf('+\n'); //+ de fin de passage de ligne


    // Affichage des valeurs 
    do {
        tmp = record_list->head;
        int size;
        char *buffer = (char *) malloc(sizeof(char) * TEXT_LENGTH);

        // Calcul de la taille maximale
        size = strlen(get_field_name(tmp->record, buffer));
        if (strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)) > size) {
            size = strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)); // lltoa convertit un long long en chaîne de caractère
        } else if (strlen(tmp->record.fields->field_value.text_value) > size) {
            size = strlen(tmp->record.fields->field_value.text_value);
        } else if (strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)) > size) {
            size = strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)); // ulltoa convertit un unsigned long long en chaîne de caractère
        } else if (strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)) > size) {
            size = strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)); // Convertit un double en chaîne de caractère
        }
        free(buffer);

        record_list_node_t *temp = record_list->head;
        int chaine_size = 0;
        for (temp; record_list != NULL; record_list->head = temp->next) {
            chaine_size++;
        }
        free(temp);

        chaine_size = TEXT_LENGTH*chaine_size + chaine_size*2 + (chaine_size-2)*3; // On obtient l'affichage d'une ligne contenant le nombre maximum de carcatère possible
        char *chaine = (char *) malloc(sizeof(char)* chaine_size);
        char *name = (char *) malloc(sizeof(char)* TEXT_LENGTH);
        chaine ="| ";
        char space = " | "; // Espace entre les valeurs d'une même ligne


        do {
            tmp = record_list->head;
            int size;
            char *buffer = (char *) malloc(sizeof(char) * TEXT_LENGTH);

            // Calcul de la taille maximale
            size = strlen(get_field_name(tmp->record, buffer));
            if (strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)) > size) {
                size = strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)); // lltoa convertit un long long en chaîne de caractère
            } else if (strlen(tmp->record.fields->field_value.text_value) > size) {
                size = strlen(tmp->record.fields->field_value.text_value);
            } else if (strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)) > size) {
                size = strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)); // ulltoa convertit un unsigned long long en chaîne de caractère
            } else if (strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)) > size) {
                size = strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)); // Convertit un double en chaîne de caractère
            }
            free(buffer);
            name = tmp->record.fields->field_value.text_value;


            // Mise a jour de la valeur en fonction de la taille
            if (size > strlen(name)) {
                char *buffer = (char *)malloc(sizeof(char)* size);
                buffer = "";
                do {
                    strcat(buffer, " ");
                    size--;

                } while (size-strlen(name) != 0);
                strcat(buffer, get_field_name(tmp->record, name));
                name = buffer; 
                free (buffer);
            }
        
        } while (record_list != NULL);


        record_list->head = tmp->next;
        free(tmp);
        tmp = record_list->head;
    } while (record_list != NULL);


    // Dernier affichage du result header
    do {
        tmp = record_list->head;
        int size;
        char *buffer = (char *) malloc(sizeof(char) * TEXT_LENGTH); 

        // Calcul de la taille maximale
        size = strlen(get_field_name(tmp->record, buffer));
        if (strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)) > size) {
            size = strlen(lltoa(tmp->record.fields->field_value.int_value, buffer, 10)); // lltoa convertit un long long en chaîne de caractère
        } else if (strlen(tmp->record.fields->field_value.text_value) > size) {
            size = strlen(tmp->record.fields->field_value.text_value);
        } else if (strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)) > size) {
            size = strlen(ulltoa(tmp->record.fields->field_value.primary_key_value, buffer, 10)); // ulltoa convertit un unsigned long long en chaîne de caractère
        } else if (strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)) > size) {
            size = strlen(sprintf(buffer, "%lf", tmp->record.fields->field_value.float_value)); // Convertit un double en chaîne de caractère
        }
        free(buffer);

        //Affichage : 
        printf("+");

        for (int i=0; i<size+2; i++) {  
            printf("-");
        }
        record_list->head = tmp->next;
        free(tmp);
        tmp = record_list->head;
    } while (record_list != NULL);
    free(tmp);
    printf('+\n'); //+ de fin de passage de ligne*/
}
