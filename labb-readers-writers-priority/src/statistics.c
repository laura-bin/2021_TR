/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
 * 
 * Statistics logger
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/


#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

#include "statistics.h"

FILE *r_stat_file;      // readers statistics file
FILE *w_stat_file;      // writers statistics file
clock_t start;

int open_stat_file(int readers_count, int readers_niceness,
        int writers_count, int writers_niceness, int max) {

    char r_filename[128];
    char w_filename[128];

    // format the file name : n_readers niceness n_writers niceness
    sprintf(r_filename, "stats/r_%d_%d_%d_%d_%d.csv", readers_count, readers_niceness,
                writers_count, writers_niceness, max);
    sprintf(w_filename, "stats/w_%d_%d_%d_%d_%d.csv", readers_count, readers_niceness,
                writers_count, writers_niceness, max);

    // initialize the start value
    start = clock();

    // create the stat sub directory
    mkdir("stats", 0755);

    // open the stats files
    r_stat_file = fopen(r_filename, "a");
    w_stat_file = fopen(w_filename, "a");
    if (r_stat_file == NULL || w_stat_file == NULL) {
        return -1;
    }

    return 0;
}

void log_r_stat(unsigned id) {
    fprintf(r_stat_file, "%u;%ld\n", id, clock() - start);
    fflush(r_stat_file);
}

void log_w_stat(unsigned id) {
    fprintf(w_stat_file, "%u;%ld\n", id, clock() - start);
    fflush(w_stat_file);
}

void close_stats_file() {
    fclose(r_stat_file);
    fclose(w_stat_file);
}
