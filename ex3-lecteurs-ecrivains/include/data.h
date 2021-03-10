#pragma once
/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Data structure:
 *  - creation and deletion
 *  - read data
 *  - write data
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

struct data {
    int counter;
    int max;
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
 * @param data: data to display
 * @param reader_id: calling thread id
 */
void read_data(struct data *data, int reader_id);

/**
 * Increments the data counter with a given value
 * (limited to the max value)
 * 
 * @param data: data structure to increment
 * @param value: increment value
 * @param writer_id: calling thread id
 */
void write_data(struct data *data, int value, int writer_id);
