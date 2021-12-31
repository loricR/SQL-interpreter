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
        //strcpy(buffer, "DROP DATABASE  , champ1 , champ2, 4, 5, 6 , 7, 8, 9, 10, 11, 12, 13 ,14, 15 ,16 FROM table;"); //Simulation d'une requete sql entrée par l'utilisateur
        //strcpy(buffer, "  (int1 int, text2 text, float3.1 float, chien3 chien, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float, float4 float ) "); //Simulation d'une requete sql entrée par l'utilisateur
        //strcpy(buffer, " michel = '2', ad = 5 azdazd "); //Simulation d'une requete sql entrée par l'utilisateur
        //strcpy(buffer, "SELECT     nom ,   prenom  FROM latable WHERE nom=5 AND prenom='4' AND feo=54; "); //Simulation d'une requete sql entrée par l'utilisateur
        //strcpy(buffer, "CREATE TABLE latable (    nom primary key,   prenom  float, dz int, nom float, dads int,  test zf) ;"); //Simulation d'une requete sql entrée par l'utilisateur
        //strcpy(buffer, "INSERT INTO table_name (field1, ..., fieldN) VALUES (value1, ..., valueN);"); //Simulation d'une requete sql entrée par l'utilisateur
        //strcpy(buffer, "UPDATE client SET rue = '49 Rue Ameline', ville = 'Saint-Eustache-la-Forêt', code_postal = '76210' WHERE id = 2 AND da = 5;"); //Simulation d'une requete sql entrée par l'utilisateur
        //strcpy(buffer, "DELETE FROM utilisateur WHERE id = 1 and azd = 4;");
        strcpy(buffer, "DROP table db_name; ");
        //strcpy(buffer, "michel = 2;"); //Simulation d'une requete sql entrée par l'utilisateur
        //printf("%s\n", get_sep_space(buffer));
        //printf("%s\n", get_sep_space_and_char(buffer, 'S'));
        //printf("%s\n", get_keyword(buffer, "SELECT"));
        //char p_champ[TEXT_LENGTH];
        //printf("%s : ", get_field_name(buffer, p_champ));
        
        //printf("%s\n", p_champ);
        //printf("%d", has_reached_sql_end(buffer));

        /*table_record_t liste_champs;
        liste_champs.fields_count = 0;
        char *sql = parse_fields_or_values_list(buffer, &liste_champs);
        if (sql == NULL) {
            printf("VIDE");
            return 0;
        }
        printf("%s\n", sql);
        printf("%d\n", liste_champs.fields_count);
        for (int i=0; i<liste_champs.fields_count; i++) {
            printf("champ : %s\n", liste_champs.fields[i].field_value.text_value);
        }*/

        /*table_definition_t liste_create;
        liste_create.fields_count = 0;
        char *sql = parse_create_fields_list(buffer, &liste_create);
        if (sql == NULL) {
            printf("VIDE");
            return 0;
        }
        printf("%s\n", sql);
        for (int i=0; i<liste_create.fields_count; i++) {
            printf("nom : %s - ", liste_create.definitions[i].column_name);
            printf("type : %d\n", liste_create.definitions[i].column_type);
        }*/

        /*field_record_t liste_equality;
        printf("%s\n", parse_equality(buffer, &liste_equality));
        printf("colonne : %s - ", liste_equality.column_name);
        printf("valeur : %s\n", liste_equality.field_value.text_value);*/

        /*table_record_t liste_champs;
        printf("%s\n", parse_set_clause(buffer, &liste_champs));
        for (int i=0; i<liste_champs.fields_count; i++) {
            printf("champ : %s - %s\n", liste_champs.fields[i].column_name, liste_champs.fields[i].field_value.text_value);
        }*/

        /*filter_t liste_where;
        printf("%s\n", parse_where_clause(buffer, &liste_where));
        for (int i=0; i<liste_where.values.fields_count; i++) {
            printf("champ : %s - %s - %d\n", liste_where.values.fields[i].column_name, liste_where.values.fields[i].field_value.text_value, liste_where.logic_operator);
        }*/

        /*query_result_t resultat;
        if (parse(buffer, &resultat) == NULL) {printf("retourne null\n");}
        printf("type : %d\n", resultat.query_type);
        printf("table : %s\n", resultat.query_content.select_query.table_name);
        for (int i=0; i<resultat.query_content.select_query.set_clause.fields_count; i++) {
            printf("champs : %d - %s\n", resultat.query_content.select_query.set_clause.fields_count, resultat.query_content.select_query.set_clause.fields[i].column_name);
        }
        for (int i=0; i<resultat.query_content.select_query.where_clause.values.fields_count; i++) {
            printf("champ where : %s = %s (%d)\n", resultat.query_content.select_query.where_clause.values.fields[i].column_name, resultat.query_content.select_query.where_clause.values.fields[i].field_value.text_value, resultat.query_content.select_query.where_clause.logic_operator);
        }*/

        /*query_result_t resultat;
        if (parse(buffer, &resultat) == NULL) {printf("retourne null\n");}
        printf("table : %s\n", resultat.query_content.create_query.table_name);
        for (int i=0; i<resultat.query_content.create_query.table_definition.fields_count; i++) {
            printf("champ : %s - type : %d\n", resultat.query_content.create_query.table_definition.definitions[i].column_name, resultat.query_content.create_query.table_definition.definitions[i].column_type);
        }*/

        /*query_result_t resultat;
        if (parse(buffer, &resultat) == NULL) {printf("retourne null\n");}
        printf("table : %s\n", resultat.query_content.insert_query.table_name);
        for (int i=0; i<resultat.query_content.insert_query.fields_names.fields_count; i++) {
            printf("champ : %s\n", resultat.query_content.insert_query.fields_names.fields[i].field_value.text_value);
        }
        for (int i=0; i<resultat.query_content.insert_query.fields_values.fields_count; i++) {
            printf("valeurs : %s\n", resultat.query_content.insert_query.fields_values.fields[i].field_value.text_value);
        }*/
        
        /*query_result_t resultat;
        if (parse(buffer, &resultat) == NULL) {printf("retourne null\n");}
        printf("table : %s\n", resultat.query_content.update_query.table_name);
        for (int i=0; i<resultat.query_content.update_query.set_clause.fields_count; i++) {
            printf("set : %s - %s\n", resultat.query_content.update_query.set_clause.fields[i].column_name, resultat.query_content.update_query.set_clause.fields[i].field_value.text_value);
        }
        for (int i=0; i<resultat.query_content.update_query.where_clause.values.fields_count; i++) {
            printf("where : %s - %s\n", resultat.query_content.update_query.where_clause.values.fields[i].column_name, resultat.query_content.update_query.where_clause.values.fields[i].field_value.text_value);
        }*/

        /*query_result_t resultat;
        if (parse(buffer, &resultat) == NULL) {printf("retourne null\n");}
        printf("table : %s\n", resultat.query_content.delete_query.table_name);
        for (int i=0; i<resultat.query_content.delete_query.where_clause.values.fields_count; i++) {
            printf("where : %s - %s\n", resultat.query_content.delete_query.where_clause.values.fields[i].column_name, resultat.query_content.delete_query.where_clause.values.fields[i].field_value.text_value);
        }*/

        /*query_result_t resultat;
        if (parse(buffer, &resultat) == NULL) {printf("retourne null\n");}
        printf("table : %s\n", resultat.query_content.table_name);*/

    } else {
        printf("Mode normal : \n");
        getchar();
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
                printf("Parse OK\n");
                if (!check_query(&query)) {
                    printf("La requete n'est pas conforme\n");
                } else {
                    expand(&query);
                    execute(&query);
                }
            } else {
                printf("Probleme construction de la requete : %s (vérifier les champs, le ; à la fin...)\n", buffer);
            }


            printf("Type  : %d = ", query.query_type);
            // TEST CREATE
            if (query.query_type == 1) {
                printf("CREATE\n");
                printf("Table : %s\n", query.query_content.create_query.table_name);
                for (int i=0; i<query.query_content.create_query.table_definition.fields_count; i++) {
                    printf("Champ : %s\n", query.query_content.create_query.table_definition.definitions[i].column_name);
                }
            // TEST INSERT
            } else if (query.query_type == 6) {
                printf("INSERT\n");
                printf("Table : %s\n", query.query_content.insert_query.table_name);
                for (int i=0; i<query.query_content.insert_query.fields_names.fields_count; i++) {
                    printf("Type: %d - champ : %s - valeur : ",
                            query.query_content.insert_query.fields_names.fields[i].field_type,
                            query.query_content.insert_query.fields_names.fields[i].column_name);
                    if (query.query_content.insert_query.fields_names.fields[i].field_type == TYPE_INTEGER) {
                        printf("%lld\n", query.query_content.insert_query.fields_names.fields[i].field_value.int_value);
                    } else if (query.query_content.insert_query.fields_names.fields[i].field_type == TYPE_PRIMARY_KEY) {
                        printf("%lld\n", query.query_content.insert_query.fields_names.fields[i].field_value.primary_key_value);
                    } else if (query.query_content.insert_query.fields_names.fields[i].field_type == TYPE_FLOAT) {
                        printf("%f\n", query.query_content.insert_query.fields_names.fields[i].field_value.float_value);
                    } else if (query.query_content.insert_query.fields_names.fields[i].field_type == TYPE_TEXT) {
                        printf("%s\n", query.query_content.insert_query.fields_names.fields[i].field_value.text_value);
                    }
                }
            }
            //for (int i=0; query.query_type )

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
