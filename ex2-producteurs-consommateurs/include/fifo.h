#pragma once
/** *************************************************************************************
 * Exercice 2 : Producteurs - Consommateurs
 * ========================================
 *
 * FIFO structure:
 *  - push a new element
 *  - pop an element
 * 
 * TR 2021 - Laura Binacchi
 ***************************************************************************************/


struct fifo {
    char text[255];
    struct fifo *input;     // input index: next input element
    struct fifo *output;    // output index: next output element
};

/**
 * Pushes a new element into the FIFO
 */
void push(char element);

/**
 * Pops the first element from the FIFO
 */
char pop(void);