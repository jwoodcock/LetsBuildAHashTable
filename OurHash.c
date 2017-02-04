/*
 * Let's Build a Hash Table in C
 * Tuturial/Presentation prepared by Jacques Woodcock
 * Given to PyTennessee Feb 4th
 *
 * @jwoodcock on https://github.com/jwoodcock/LetsBuildAHashTable
 */


/*
 * First we need to include some standard libraries that we'll be relying on
 * Remember, C as a language has little to no flare built in. So if you want
 * nice utilities to help you code, you typically bring them in via the compiler
 * or from an external file
 */

#include <stdio.h> // cause we'll want data coming in and out of the app
#include <string.h> // we'll be manipulating strings
#include <math.h> // and we'll be doing some math

/*
 * Next we define the actual data model for our hash table, the most important data type
 * we'll use as it's the actual representation of the data we'll be storing.
 */

struct our_hash {
    char fname[20];
    char lname[20];
    char email[20];
    int in_use;
};

/*
 * DEFINITIONS
 * In C we have to predefine all our methods and their types
 */
// we'll define 4 hash algorithms for testing
unsigned long       hash(char const *value);
unsigned long       hash2(char const *value);
unsigned long        hash3(char const *value);
unsigned long       worst_hash(char const *value);
// our CRUD for the table
unsigned int        add_entry(char const *fname, char const *lname, char const *value);
void                remove_entry();
void                update_entry(char const *fname, char const *lname, char const *email, char const *old_email);
struct our_hash*    look_up(char const *email);
unsigned long       hash_key(char const *key); // rename to something more obvious
void                list_values();
// Utility for swapping hash to use
unsigned int        set_hash(unsigned int hash);
unsigned int        get_info_and_action(char const *action);
void                handle_look_up();

/**
 * Colors!!!!
 */
#define KNRM  "\x1B[0m"
#define KRED   "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"

// Set Globals
struct our_hash hash_table[91]; // the hash table we are going to use
const int table_size = 91; // the max size the table will be
unsigned int HASH = 0; // which hash to use set at run time
unsigned int EXIT = 0; // watch to see if the app is ready to exit


/*
 * Our main controller
 */
int main()
{
    char action[20];
    char input[20];

    // set the hash we're going to use
    set_hash(0);

    while(EXIT == 0) {
        printf("%s\n", KGRN);
        printf("What would you like to do? \n");
        printf("Add Entry (add), Update Entry (update), Remove Entry (remove), Look Up (look), List Data (list), Exit App (exit) \n");
        printf("%s\n", KNRM);
        scanf("%s", action);

        if (strcmp(action, "add") == 0) {
            get_info_and_action("add");
        } else if (strcmp(action, "update") == 0) {
            get_info_and_action("update");
        } else if (strcmp(action, "remove") == 0) {
            remove_entry();
        } else if (strcmp(action, "look") == 0) {
            handle_look_up();
        } else if (strcmp(action, "list") == 0) {
            list_values();
        } else if (strcmp(action, "exit") == 0) {
            EXIT = 1;
        }
    }

    return 0;
}

unsigned int get_info_and_action(char const *action)
{
    char first_name[20];
    char last_name[20];
    char email[30];
    char old_email[30];

    printf("%s\n", KGRN);
    printf("Enter a first name. \n");
    printf("%s\n", KNRM);
    scanf("%s", first_name);

    printf("%s\n", KGRN);
    printf("Enter a last name. \n");
    printf("%s\n", KNRM);
    scanf("%s", last_name);

    printf("%s\n", KGRN);
    printf("Enter a email address. \n");
    printf("%s\n", KNRM);
    scanf("%s", email);

    printf("%s\n", KGRN);
    if (strcmp(action, "add") == 0) {
        add_entry(first_name, last_name, email);
    } else if (strcmp(action, "update") == 0) {
        printf("%s\n", KGRN);
        printf("Since we're updating, give me the original email too");
        printf("%s\n", KNRM);
        scanf("%s", old_email);
        update_entry(first_name, last_name, email, old_email);
    }
    printf("%s\n", KNRM);

    return 0;
}

void handle_look_up()
{
    char email[30];
    printf("%s\n", KGRN);
    printf("Enter an email to search for. \n");
    printf("%s\n", KNRM);
    scanf("%s", email);
    struct our_hash* entry = look_up(email);

    if (strcmp(entry->fname, "") != 0) {
        printf("  First Name: %s\n", entry->fname);
        printf("  Last Name: %s\n", entry->lname);
        printf("  Email: %s\n\n", entry->email);
    } else {
        printf("%s\n", KRED);
        printf("No entry with that email found. \n");
        printf("%s\n", KNRM);
    }
}

/*****************************************
 *              Hash Methods
 *****************************************/

/**
 * This method does a hash look up for a povided email and returns
 * a pointer to a match if one is found or NULL if one is not found.
 */
struct our_hash* look_up(char const *email)
{
    unsigned int hash_value = hash_key(email);
    unsigned int offset = hash_value % table_size;
    unsigned int step = ((hash_value / table_size) % table_size);

    step |= 1;

    for (int i = 0; i < table_size; i++) {
        struct our_hash* entry = &hash_table[offset];
        if (!entry->in_use) {
            return entry;
        }
        if (strcmp(entry->email, email) == 0) {
            return entry;
        }

        offset = (offset + step) % table_size;  // no match found yet. move on.
    }

    return NULL; // oh no, the hash table is full
}

/**
 * This method removes an entry from the hash table based on the
 * provided email if there is a match.
 */
void remove_entry()
{
    char email[30];
    printf("%s\n", KGRN);
    printf("Enter email of record to remove. \n");
    printf("%s\n", KNRM);
    scanf("%s", email);

    struct our_hash* entry = look_up(email);
    if (entry->in_use && strcmp(entry->email, email) == 0) {
        printf("%s\n", KRED);
        printf("We have removed the entry for %s\n", entry->email);
        printf("%s\n", KNRM);
        entry->in_use = 0;
        strcpy(entry->fname, "");
        strcpy(entry->lname, "");
        strcpy(entry->email, "");
    }
}

/**
 * This method updates an entry in the hash table with the newly
 * provided data if at match is found.
 */
void update_entry(char const *fname, char const *lname, char const *email, char const *old_email)
{
    struct our_hash* entry = look_up(old_email);
    if (entry->in_use && strcmp(entry->email, old_email) == 0) {

            printf("%s\n", KYEL);
            printf("We will now update %s data to  %s %s %s.\n", old_email, fname, lname, email);
            printf("%s\n", KNRM);

        strcpy(entry->fname, fname);
        strcpy(entry->lname, lname);
        strcpy(entry->email, email);
    }
}

/**
 * This method add an entry in the first available empty table slot.
 */
unsigned int add_entry(char const *fname, char const *lname, char const *email)
{
    unsigned int hash_value = hash_key(email);
    printf("hash %i", hash_value);
    unsigned int offset = hash_value % table_size;
    unsigned int step = ((hash_value / table_size) % table_size);

    step |= 1;

    for (int i = 0; i < table_size; i++) {
        struct our_hash* entry = &hash_table[offset];

        if (!entry->in_use) {

            printf("%s\n", KYEL);
            printf("We will now add %s %s %s to our hash table at position %i.\n", fname, lname, email, offset);
            printf("%s\n", KNRM);
            entry->in_use = 1;
            strcpy(entry->fname, fname);
            strcpy(entry->lname, lname);
            strcpy(entry->email, email);
            return offset;
        } else if (entry->in_use && strcmp(entry->email, email) == 0) {
            printf("%s\n", KRED);
            printf("Collision!!!");
            printf("%s\n", KNRM);
            
            return 0;
            // let's talk about collision handling
            // there's 2 main ways to handle collision
            // a) Open Addressing
            // this is where we find our index, it's occupid and so we develop a
            // prob to find the next index. when we do this, we'll need to add the same
            // math logic to our look up method so both the add and look up flow in
            // same way
            //
            // Some prob types are
            //
            // Linear Probing
            // Quadratic Probing
            // Double Hashing
            
            // b) Separate Chaining
            // we identify there's a collision and create a linked list of values
            // that all fell into the collision index. We then find the next open
            // slot in the linked list and insert our data. again we'll want to
            // add this same logic to look up so we can find the right data
            //
            // Some types of chaining are
            //
            // chaining with linked list
            // chaining with list head cells
            // chaining with other structures 
            //
        }

        offset = (offset + step) % table_size;  // no match found yet. move on.
    }

    return 0; // no room left
}

/**
 * This method lists all stored values
 */
void list_values()
{
    printf("---------------------------------\n");
    printf("Listing contents of table\n\n");
    for (int i = 0; i < table_size; i++) {
        struct our_hash* entry = &hash_table[i];

        if (entry->in_use) {
            printf("%s\n", KYEL);
            printf("Entry offset: %i\n", i);
            printf("%s\n", KNRM);
            printf("  First Name: %s\n", entry->fname);
            printf("  Last Name: %s\n", entry->lname);
            printf("  Email: %s\n\n", entry->email);
        }
    }
    printf("---------------------------------\n");
}
/**
 * This method gets a hash valued based on the provided key using the
 * configured hashing algorithm.
 */
unsigned long hash_key(char const *key)
{
    if (HASH == 0) {
        return hash(key) % table_size;
    }
    if (HASH == 1) {
        return hash2(key) % table_size;
    }
    if (HASH == 2) {
        return hash3(key) % table_size;
    }
    if (HASH == 3) {
        return worst_hash(key) % table_size;
    }
    return 0;
}

/**
 * Method for configuring the table's hash method.
 */
unsigned int set_hash(unsigned int hash)
{
    if (hash > 3) {
        return 0;
    }

    HASH = hash;
    return 1;
}

/**
 * -------------------------------------------
 * Hashes - The different hashes you can use
 * -------------------------------------------
 */

/**
 * Hash 1 which uses bitshifting
 */
unsigned long hash(char const *value)
{
    const int ret_size = 32;
    size_t ret = 0x555555;
    const int per_char = 7;

    while (*value) { 
        ret ^= *value++;
        ret = ((ret << per_char) | (ret >> (ret_size - per_char)));
    }
   return ret;
}

/**
 * Hash 2 which uses bitshifting
 */
unsigned long hash2(char const *value)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *value++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * Hash 3 simple hashing based on the length of the provided key
 * Would not use in production.
 */
unsigned long hash3(char const *value)
{
    unsigned int h = 0;
    for (size_t i = 0; i < strlen(value); i++) {
        h += value[i];
    }
    return h;
}

/**
 * Hash 4 should never be used unless you are testing the collision handling
 * of the hash table.
 */
unsigned long worst_hash(char const *value)
{ 
    return 4; 
}
