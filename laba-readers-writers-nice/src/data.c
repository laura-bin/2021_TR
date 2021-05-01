/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
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
#include <unistd.h>

#include "data.h"
#include "statistics.h"

struct data *init_data(int max) {
    struct data *data = malloc(sizeof(struct data));
    if (data == NULL) {
        return NULL;
    }
    memset(data, 0, sizeof(struct data));
    data->max = max;
    return data;
}

void free_data(struct data *data) {
    free(data);
}

int read_data(void *params) {
    struct read_params *p = (struct read_params *)params;
    int sleep_time = rand() % 2000 + 1;
    
    usleep(sleep_time);

    printf("r %3d < %3d\n", p->reader_id, p->data->counter);
    log_r_stat(p->reader_id);
    return p->data->counter < p->data->max ? 0 : 1;
}

int write_data(void *params) {
    struct write_params *p = (struct write_params *)params;
    int sleep_time = rand() % 2000 + 1;
    
    usleep(sleep_time);
    
    p->data->counter++; //p->increment_value;
    if (p->data->counter > p->data->max) {
        p->data->counter = p->data->max;
    }
    printf("w %3d > %3d (+%d)\n", p->writer_id,
                p->data->counter, p->increment_value);
    log_w_stat(p->writer_id);
    return p->data->counter < p->data->max ? 0 : 1;
}
