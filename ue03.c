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

/*
int randoffset( void ) {
    static unsigned randInt = 1;

    if( randInt == 1 ) {                        
        randInt = (unsigned)rand();             //initialisiert randInt mit rand nummer
        randInt |= 1<<(sizeof(int)*8-1);        //bitweises oder mit... konstanter zahl die um 1 geshiftet wurde?!
    }

    randInt >>= 1;                              //rand nummer eins zuweisen und shiften
    return randInt & 1;                         //bitweises & return
}
*/

double randoffset(double prob)                  //wenn nicht fix: 
{
    static double randInt=1.0;
    randInt=(double)rand()/((double)RAND_MAX);
    if(randInt*prob>0.5)
        return 1;
    else return 0;
}

/** Gets the current unix time in seconds but with micro seconds precision */
double getTimeStamp(void) {                     //akzepier i dass des die zeit in sekunden zurück gibt.. weiß zwar ned wie aber ok
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
                    tmpwalk=0;        //coordinate of current iteration
    double          time;           //time for clocking the program
    double prob;
    
    /* argument handling */
    if(argc<3) {
        printf("Usage: %s repetitions boxes\n", argv[0]);
        return -1;
    }
    repetitions = (unsigned)atof(argv[1]); // atof to use shorthand such as 1e7
    boxcount    = (unsigned)atof(argv[2]);
    prob    = atof(argv[3]);
    /* allocate memory for boxes */
    boxes = (unsigned*) calloc(boxcount, sizeof(boxes));        //boxen für galton-brett 
    /* get the start time */
    time = getTimeStamp();                                      //start des "stoppen"
    /* seed random number generator */
    srand((int)time);                                           //initialisierung fürs rand

    /*################ ACTUAL SIMULATION ################*/
    for( i=0; i<repetitions; ++i ) {                            //also repetitions is praktisch die anzahl der zeilen von dem galton board?
        tmpwalk = 0;                                            //aktueller box index?
        for( j=0; j<boxcount-1; ++j )
            tmpwalk += randoffset(prob);                         //zu tmpwalk werden immer 0 und 1 addiert- je nachdem.. wozu?!
        ++boxes[tmpwalk];
    }                                                           //zusammenfassung: i versteh ned wozu die zeile mit tmpwalk+=randoffset() is.. =/

    
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
