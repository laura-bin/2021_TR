#pragma once
/** *************************************************************************************
 * Lecteurs - Ecrivains
 * ====================
 * 
 * Statistics file
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

/**
 * Opens the stats file
 * 
 * @param readers_count
 * @param writers_count
 * @param max data value
 * 
 * @return either
 *      0 if the file has sucessfully been opened
 *     -1 if an error occured
 */
int open_stat_file(int readers_count, int writers_count, int max);

/**
 * Logs readers stats info
 * 
 * @param id: thread id
 */
void log_r_stat(unsigned id);

/**
 * Logs writers stats info
 * 
 * @param id: thread id
 */
void log_w_stat(unsigned id);

/**
 * Closes the stats file
 */
void close_stats_file();
