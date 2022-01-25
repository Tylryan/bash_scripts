#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // EXIT_SUCCESS/EXIT_FAILURE
#include <sysexits.h>
#include <errno.h>
#include "./structures.c"

#define TRUE 1
#define FALSE 0

void print_help_message(char * PROGRAM_NAME);
int contains_a_dot(char * string, int str_len);
void base_elinks_url(UserEntry * user_search);
void add_command_ending(UserEntry * user_entry);
void print_help_message(char * program_name);
void args_to_url(UserEntry * user_entry, int arg_index);
int execute_or_print_error(char * command);
void add_elinks_config(UserEntry * user_entry);
void free_all_memory(UserEntry user_entry);
void run_elinks_no_args(UserEntry user_entry);
void run_as_complete_url(UserEntry user_entry, int start_of_args);
void run_as_search_query(UserEntry user_entry);

// ###################### FUNCTION INITIALIZATION ############################

// Does the string contain a dot?
int contains_a_dot(char * string, int str_len){
    for (int i = 0; i < str_len; i++) 
        if (string[i] == '.') return TRUE;
    return FALSE;
}

// Boy this was a pain to figure out.
// Must pass reference to change outside values, duh
void base_elinks_url(UserEntry * user_entry){
    strcpy(user_entry -> final_url, user_entry -> application_binary); 
    strcat(user_entry -> final_url, user_entry -> elinks_config_path);
}

void add_command_ending(UserEntry * user_entry){
    if (user_entry->use_gui == FALSE){
        strcat(user_entry -> final_url, " 2>/dev/null");
    }
    else {
        strcat(user_entry -> final_url, " 2>/dev/null &");
    }
}

void print_help_message(char * program_name){
        char help_command[30] = "bash bash/help_message.sh ";
        strcat(help_command, program_name);
        execute_or_print_error(help_command);
}

void args_to_url(UserEntry * user_entry, int arg_index){
    if (user_entry -> has_flag) arg_index = 2;
    strcat(user_entry -> search_terms, user_entry -> args[arg_index]);
    // Don't add "+" to the last argument
    if (arg_index != (user_entry -> arg_num -1)) 
        strcat(user_entry -> search_terms, "+");

}

int execute_or_print_error(char * command){
        int error;
        if ((error = system(command) != 0)) 
            strerror(errno);
        return 0;
}

void add_elinks_config(UserEntry * user_entry){
    int is_elinks = strcmp(user_entry -> application_binary, "elinks ");
    if (is_elinks == 0) 
        strcat(user_entry -> final_url, user_entry -> elinks_config_path);
}

void free_all_memory(UserEntry user_entry){
    // Free the memory of the entire struct
        free(user_entry.application_binary);
        free(user_entry.final_url);
}
void run_elinks_no_args(UserEntry user_entry){
        user_entry.final_url = realloc(user_entry.final_url, 25 * sizeof(char) );
        base_elinks_url(&user_entry);

        user_entry.final_url = realloc(user_entry.final_url, 10 * sizeof(char));
        add_command_ending(&user_entry);
        execute_or_print_error(user_entry.final_url);
}
void run_as_complete_url(UserEntry user_entry, int start_of_args){
        size_t url_len = 4 * strlen(user_entry.args[start_of_args]);
        if (strlen(user_entry.args[start_of_args]) > 150) {
            printf("Error: Url is too big\n");
            exit(EXIT_FAILURE);
        }
         strcat(user_entry.final_url,user_entry.application_binary);

         // Will only add config if application chosen is elinks */
         user_entry.final_url = realloc(user_entry.final_url, url_len * sizeof(char));
         add_elinks_config(&user_entry); 

         char * url=user_entry.args[start_of_args];
         // Max url length is 100 */
         user_entry.final_url = realloc(user_entry.final_url, 75 * sizeof(char)); 
         strcat(user_entry.final_url, url);

         user_entry.final_url = realloc(user_entry.final_url, 50 * sizeof(char));
         add_command_ending(&user_entry);

         execute_or_print_error(user_entry.final_url);


}
void run_as_search_query(UserEntry user_entry){
        // Turning args into "arg1+arg2+arg3"
        for (int i = 1; i < user_entry.arg_num; i++){
            args_to_url(&user_entry, i);
        }
        user_entry.final_url = realloc(user_entry.final_url, 250 * sizeof(char));
        strcat(user_entry.final_url, user_entry.application_binary);
        add_elinks_config(&user_entry);
        strcat(user_entry.final_url, user_entry.base_search);
        strcat(user_entry.final_url, user_entry.search_terms);
        strcat(user_entry.final_url, " 2>/dev/null");

        execute_or_print_error(user_entry.final_url);
}

