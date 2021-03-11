#pragma once
/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Data structure
 * Functions parameters structures
 * Data creation and deletion function prototypes
 * Read data function prototype
 * Write data function prototype
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

struct data {
    int counter;
    int max;
};

/* Read function parameters */
struct read_params {
    struct data *data;      // data to read
    int reader_id;          // calling thread id
};

struct write_params {
    struct data *data;      // data to read
    int increment_value;    // increment value
    int writer_id;          // calling thread id
};

/**
 * Initializes a data structure:
 * sets the counter to 0 and the max value
 * 
 * @return either
 *      the data structure created
 *      NULL if an error occured (errno is set)
 */
struct data *init_data(int max);

/**
 * Frees a data structure
 * 
 * @param data: data structure to free
 */
void free_data(struct data *data);

/**
 * Displays the data counter on the standard output
 * 
 * @return either
 *      0 if the counter is less than max
 *      1 if the counter is greater than or equal to the value
 */
int read_data(void *params);

/**
 * Increments the data counter with a given value
 * (limited to the max value)
 * 
 * @return either
 *      0 if the counter is less than max
 *      1 if the counter is greater than or equal to the value
 */
int write_data(void *params);
