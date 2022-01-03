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


size_t strlen_utf8(char *s)
{
    size_t len = 0;
    for (; *s; ++s) if ((*s & 0xC0) != 0x80) ++len;
    return len;
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
        //length = strlen(field_record->field_value.text_value);
        length = strlen_utf8(field_record->field_value.text_value);
    } else {
        char buff[50];
        if (field_record->field_type == TYPE_INTEGER) {
            length = sprintf(buff, "%lld", field_record->field_value.int_value);
        } else if (field_record->field_type == TYPE_PRIMARY_KEY){
            length = sprintf(buff, "%lld", field_record->field_value.primary_key_value);
        } else if (field_record->field_type == TYPE_FLOAT) {
            length = sprintf(buff, "%g", field_record->field_value.float_value);
        }
    }

    /*else if (field_record->field_type == TYPE_INTEGER || field_record->field_type == TYPE_PRIMARY_KEY) { //Si type entier
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
        

    }*/
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
    if (record_list->head != NULL) { //si le dernier pointeur est nul c'est qu'il n'y a pas de données
        record_list_node_t *suivant = record_list->head;

        int width_col[MAX_FIELDS_COUNT];
        int nb_max_col = suivant->record.fields_count;
        int max = 0;

        //on commence par lister la largeur min des en-tete de colonne
        for (int i=0; i<nb_max_col; i++) {
            width_col[i] = strlen(suivant->record.fields[i].column_name);
        }

        //on parcourt la liste des données pour calculer la largeur min de chaque colonne
        while (suivant != NULL) {
            for (int i=0; i< nb_max_col; i++) {
                max = field_record_length(&suivant->record.fields[i]);
                if (width_col[i] < max) {
                    width_col[i] = max;
                }
            }
            suivant = suivant->next;
        }
        suivant = record_list->head; // on se repositionne au début de la liste pour la suite

        //calcul de la taille des buffers
        int max_buffer = 0;
        for (int i=0; i<nb_max_col; i++) {
            max_buffer += 3; //un trait au début de la colonne + 1 espace avant et après
            max_buffer += width_col[i];
        }
        max_buffer += 2; //le dernier trait a droite et un caractère fin de chaine;

        char *buffer_ligne = malloc(max_buffer * sizeof(char)); //buffer pour afficher les lignes de séparation

        buffer_ligne[0]='+';
        for (int i=1; i<(max_buffer-1); i++) {
            buffer_ligne[i]='-';
        }
        for (int i=0, pos=0; i<nb_max_col; i++) {
            pos +=3; //2 espace et le séparateur de colonne
            pos += width_col[i];
            buffer_ligne[pos]='+';
        }
        buffer_ligne[max_buffer-1]='\0';

        printf("%s\n", buffer_ligne);

        // affichage des entete, on position le nopm du champ au centre de la colonne
        for (int i=0; i<nb_max_col; i++) {
            printf("| ");
            int longueur = width_col[i]-strlen(suivant->record.fields[i].column_name);
            for (int j=0; j<((longueur / 2) + (longueur % 2)); j++) { // nombre entier, donc si le nombre est impaire il manquerait un caractère, dans ce cas on l'ajoute
                printf(" ");
            }
            printf("%s ", suivant->record.fields[i].column_name); // il y a toujours un espace en plus par rapport à la taille max du texte, pour ne pas etre collé au |
            for (int j=0; j<(longueur / 2); j++) { //nombre entier, donc si le nombre est impaire il manquerait un caractère
                printf(" ");
            }
        }
        printf("|\n");

        printf("%s\n", buffer_ligne);

        while (suivant != NULL) {
            for (int i=0; i< suivant->record.fields_count; i++) {
                printf("| ");
                for (int j=0; j<(width_col[i]-field_record_length(&suivant->record.fields[i])); j++) {
                    printf(" ");
                }
                switch (suivant->record.fields[i].field_type)
                {
                case TYPE_INTEGER:
                    printf("%lld ", suivant->record.fields[i].field_value.int_value);
                    break;
                case TYPE_FLOAT:
                    printf("%g ", suivant->record.fields[i].field_value.float_value);
                    break;
                case TYPE_PRIMARY_KEY:
                    printf("%lld ", suivant->record.fields[i].field_value.primary_key_value);
                    break;
                case TYPE_TEXT:
                    printf("%s ", suivant->record.fields[i].field_value.text_value);
                    break;                
                default:
                    break;
                }
            }
            printf("|\n");
            suivant = suivant->next;
        }

        printf("%s\n", buffer_ligne);

        free(buffer_ligne);
    }
}
