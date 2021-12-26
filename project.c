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
        strcpy(buffer, "SELECT champ1, champ2 FROM table"); //Simulation d'une requete sql entrée par l'utilisateur
        printf("%s\n", get_sep_space(buffer));
        //printf("%s\n", get_sep_space_and_char(buffer, ','));
        printf("%s\n", get_keyword(buffer, "SELECT"));
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
