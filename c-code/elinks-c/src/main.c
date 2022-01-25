#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // EXIT_SUCCESS/EXIT_FAILURE
#include <sysexits.h>
#include <errno.h>

#include "./functions.c"

#define TRUE 1
#define FALSE 0

/* 
 * This code is probably pretty easy to break
*/

// Pretty much never allowed to use constants.
char YOUR_FAVORITE_BROWSER[] = "otter-browser ";
char ELINKS_CONFIG_PATH[] = "-config-dir ~/.config/elinks ";

// #######################  MAIN #############################################
int main(int argc, char *argv[]){

    char * PROGRAM_NAME = argv[0];

    UserEntry user_entry = {
        // application_binary
        malloc(25 * sizeof(char)), 
        // elinks_config_path
        ELINKS_CONFIG_PATH, 
        // SearchEngine
        duck, 
        // base_search
        "duckduckgo.com/?q=", 
        // search_terms
        malloc(100 * sizeof(char)), 
        // arg_num
        argc, 
        // args
        argv, 
        //  flag
        argv[1], 
        // has_flag
        0, 
        // use_gui
        0, 
        // final_url
        malloc(100 * sizeof(char))
    };
    // Default Elinks
    strcpy(user_entry.application_binary,"elinks ");
    user_entry.search_engine = duck;
    /* user_entry.base_search = "duckduckgo.com/?q="; */

    int max_args = 20;
    if (user_entry.arg_num > max_args){
        printf("Error: Entry too big.\n");
        exit(EXIT_FAILURE);
    }

    if (argc == 1) {
        run_elinks_no_args(user_entry);
        /* free_all_memory(user_entry); */
        exit(EXIT_SUCCESS);
    }

    int needs_help = (
            (strcmp(user_entry.flag, "-h") == 0) || 
            (strcmp(user_entry.flag, "--help") == 0)
            );
    int wants_gui_browser = (
            (strcmp(user_entry.flag, "-o") == 0) || 
            (strcmp(user_entry.flag, "--otter") == 0)
            );

    if (needs_help){
        // TODO Stack smashing here
        print_help_message(PROGRAM_NAME);
        exit(EXIT_SUCCESS);

    } 
    if (wants_gui_browser){
        // TODO Free this
        user_entry.application_binary = realloc(user_entry.application_binary, 25 * sizeof(char));
        strcpy(user_entry.application_binary, YOUR_FAVORITE_BROWSER);
        user_entry.has_flag = 1;
        user_entry.use_gui = 1;
    }


    // If there is a "." in arg1, it's probably a url
    int start_of_args = 1;
    if ( user_entry.has_flag ) start_of_args = 2;
    int is_complete_url = contains_a_dot(argv[start_of_args], strlen(argv[start_of_args]));
    // Should be len of string + 1;
    
    if (is_complete_url){
        run_as_complete_url(user_entry, start_of_args);
        /* free_all_memory(user_entry); */

        exit(EXIT_SUCCESS);
    }

    else {
        run_as_search_query(user_entry);
        /* free_all_memory(user_entry); */
        exit(EXIT_SUCCESS);
        }

    }
