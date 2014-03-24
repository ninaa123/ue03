/*encoding: UTF-8, break: linux, indent: spaces, tabsize: 4, language: C99/eng*/
/**
 * \file galton.c
 *
 * Exercise Sheet #3
 * Tutorial "Statisik und Datenanalyse",
 * SS2014 University of Innsbruck
 *
 * This simulates a galton board. Boxes are numerated 0 to argv[2].
 * Output is corresponding number of items in each box after simulation.
 *
 * Compile with:
 * gcc -Wall -W -pedantic -ansi -std=c99 -O3 -o "galton" "galton.c" -lm
 *
 * \date       20. Mar 2014
 * \author     Jan Bischko
 * \copyright  Copyright © 2014, Leopold Franzens Universität Innsbruck
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
/** Returns +1 or 0 at random with probabitlity 0.5 each. */
int randoffset( void ) {
    static unsigned randInt = 1;

    if( randInt == 1 ) {
        randInt = (unsigned)rand();
        randInt |= 1<<(sizeof(int)*8-1);
    }

    randInt >>= 1;
    return randInt & 1;
}

/** Gets the current unix time in seconds but with micro seconds precision */
double getTimeStamp(void) {
    struct timeval timestr;
    void *Tzp = 0;
    gettimeofday(&timestr, Tzp);
    return (double)timestr.tv_sec+1.0E-06*(double)timestr.tv_usec;
}

////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] ) {
    /*################ PREPERATIONS (boring) ################*/
    /* declarations */
    unsigned int    boxcount,       //how many boxes there are
                    repetitions,    //how many experiments to be made
                    *boxes,         //contents of each box
                    i, j,           //iterators
                    tmpwalk;        //coordinate of current iteration
    double          time;           //time for clocking the program
    /* argument handling */
    if( argc < 3 ) {
        printf("Usage: %s repetitions boxes\n", argv[0]);
        return -1;
    }
    repetitions = (unsigned)atof(argv[1]); // atof to use shorthand such as 1e7
    boxcount    = (unsigned)atof(argv[2]);
    /* allocate memory for boxes */
    boxes = (unsigned*) calloc(boxcount, sizeof(boxes));
    /* get the start time */
    time = getTimeStamp();
    /* seed random number generator */
    srand((int)time);

    /*################ ACTUAL SIMULATION ################*/
    for( i=0; i<repetitions; ++i ) {
        tmpwalk = 0;
        for( j=0; j<boxcount-1; ++j )
            tmpwalk += randoffset();
        ++boxes[tmpwalk];
    }

    /*#################### CLEAN UP #####################*/
    time = getTimeStamp()-time;
    printf(
        "runtime %g\n"
        "iterations %g\n"
        "boxes %g\n"
        "\n",
        time, (double)i, (double)boxcount
    );
    /* output and exiting */
    for( i=0; i<(unsigned)argc; ++i)
        if( argv[i][0] == 'n' ) goto skipoutput;
    for( i=0; i<boxcount; ++i)
        printf("%d\t%d\n",i,boxes[i]);

    skipoutput:
        free(boxes);
        return 0;
}
