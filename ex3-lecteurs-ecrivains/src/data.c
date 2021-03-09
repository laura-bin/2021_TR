/** *************************************************************************************
 * Exercice 3 : Lecteurs - Ecrivains
 * =================================
 * 
 * Data structure functions implementation:
 *  - creation and deletion
 *  - read data
 *  - write data
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

struct data *init_data(void) {
    struct data *data = malloc(sizeof(struct data));
    if (data == NULL) {
        return NULL;
    }
    memset(data, 0, sizeof(struct data));
    return data;
}

void free_data(struct data *data) {
    free(data);
}

void read_data(struct data *data, int reader_id) {
    printf("reader %d reads %3d\n", reader_id, data->counter);
}

void write_data(struct data *data, int value, int writer_id) {
    data->counter += value;
    printf("writer %d writes %3d TOTAL> %3d\n", writer_id, value, data->counter);
}
