#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"
#include "database.h"
#include "sql.h"
#include "table.h"
#include "check.h"

#define SQL_COMMAND_MAX_SIZE 1500

int main(int argc, char *argv[]) {

    // Here: check parameters with getopt
    int opt = 0;
    char param;
    char argument[1000];
    while ((opt = getopt(argc, argv, "d:l:")) != -1){
    	switch (opt) {
    		case 'd' :
    			printf("Vous avez passe le parametre d\n");
                param = 'd';
    			//argument = optarg;
    			break;
    		case 'l' :
    			printf("Vous avez passe le parametre l\n");
                param = 'l';
    			//argument = optarg;
    			break;
    	}
    }


    char buffer[SQL_COMMAND_MAX_SIZE];

    char test;
    printf("Test ? (o/n) ");
    test = getchar();
    if (test == 'o') {
        printf("Mode test : \n");
        //strcpy(buffer, "SELECT  , champ1 , champ2, 4, 5, 6 , 7, 8, 9, 10, 11, 12, 13 ,14, 15 ,16 FROM table;"); //Simulation d'une requete sql entrée par l'utilisateur
        strcpy(buffer, "  (int1 int, text2 text, float3.1 float, chien3 chien, float4 float ) "); //Simulation d'une requete sql entrée par l'utilisateur
        //printf("%s\n", get_sep_space(buffer));
        //printf("%s\n", get_sep_space_and_char(buffer, 'S'));
        //printf("%s\n", get_keyword(buffer, "SELECT"));
        //char p_champ[TEXT_LENGTH];
        //printf("%s : ", get_field_name(buffer, p_champ));
        
        //printf("%s\n", p_champ);
        //printf("%d", has_reached_sql_end(buffer));

        /*table_record_t liste_champs;
        liste_champs.fields_count = 0;
        printf("%s\n", parse_fields_or_values_list(buffer, &liste_champs));
        printf("%d\n", liste_champs.fields_count);
        for (int i=0; i<liste_champs.fields_count; i++) {
            printf("champ : %s\n", liste_champs.fields[i].field_value.text_value);
        }*/

        table_definition_t liste_create;
        liste_create.fields_count = 0;
        printf("%s\n", parse_create_fields_list(buffer, &liste_create));
        for (int i=0; i<liste_create.fields_count; i++) {
            printf("nom : %s - ", liste_create.definitions[i].column_name);
            printf("type : %d\n", liste_create.definitions[i].column_type);
        }
    } else {
        printf("Mode normal : \n");
        do {
            printf("> ");
            fflush(stdin);
            if (fgets(buffer, SQL_COMMAND_MAX_SIZE, stdin) == NULL)
                continue;
            buffer[strlen(buffer)-1] = '\0';
            if (strcmp(buffer, "exit") == 0)
                break;
            // Here: parse SQL, check query, execute query
        } while (true);
    }

    /*do {
        printf("> ");
        fflush(stdin);
        if (fgets(buffer, SQL_COMMAND_MAX_SIZE, stdin) == NULL)
            continue;
        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer, "exit") == 0)
            break;
        // Here: parse SQL, check query, execute query
    } while (true);*/

    return 0;
}
