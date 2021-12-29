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
            while (temp/10 > 1) {
                length++;
                temp = temp/10;
            }
            length++;
        }
        else {
            length++; //Pour le signe -
            while (temp/10 < (-1)) {
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
}
