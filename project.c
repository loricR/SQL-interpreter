#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "utils.h"
#include "database.h"
#include "sql.h"
#include "table.h"
#include "check.h"
#include "query_exec.h"
#include "expand.h"

#define SQL_COMMAND_MAX_SIZE 1500

int main(int argc, char *argv[]) {

    // Here: check parameters with getopt
    int opt = 0;
    char param;
    while ((opt = getopt(argc, argv, "d:l:")) != -1){
    	switch (opt) {
    		case 'd' :
    			printf("Vous avez passe le parametre d\n");
                param = 'd';
    			break;
    		case 'l' :
    			printf("Vous avez passe le parametre l\n");
                param = 'l';
    			break;
    	}
    }

    if (directory_exists("db")) {
        chdir("db"); //On se déplace dans le répertoire "db"
    }

    char buffer[SQL_COMMAND_MAX_SIZE];
    do {
        printf("> ");
        fflush(stdin);
        if (fgets(buffer, SQL_COMMAND_MAX_SIZE, stdin) == NULL)
            continue;
        buffer[strlen(buffer)-1] = '\0';
        if (strcmp(buffer, "exit") == 0)
            break;
        // Here: parse SQL, check query, execute query
        query_result_t query;
        query_result_t *query_result;
        query_result = parse(buffer, &query);
        if (query_result != NULL) {
            if (!check_query(&query)) {
                printf("La requete n'est pas conforme\n");
            } else {
                expand(&query);
                execute(&query);
            }
        } else {
            printf("Probleme construction de la requete : %s (vérifier les champs, le ; à la fin...)\n", buffer);
        }
    } while (true);

    return 0;
}
