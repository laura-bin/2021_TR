/** *************************************************************************************
 * Exercice 2 : Producteurs - Consommateurs
 * ========================================
 *
 * Reading thread: reads a text file and stores in in the buf1 FIFO
 * Uppercase thread: reads the buf1 FIFO, and turn each character into uppercase
 *      before filling the buf2 FIFO
 * Display thread: reads the buf2 FIFO and display each character on the standard output
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/

/**
 * Reading thread
 * reads a text file and stores in in the buf1 FIFO
 * 
 * @param params
 */
void *read(void *params);

/**
 * Uppercase thread
 * reads the buf1 FIFO, and turn each character into uppercase
 * before filling the buf2 FIFO
 * 
 * @param params
 */
void *uppercase(void *params);

/**
 * Display thread
 * rreads the buf2 FIFO and display each character
 * on the standard output
 * 
 * @param params
 */
void *display(void *params);


int main (int argc, char *argv[]) {
    return 0;
}