/* btoa: version 4.0
 * stream filter to change 8 bit bytes into printable ascii
 * computes the number of bytes, and three kinds of simple checksums
 * incoming bytes are collected into 32-bit words, then printed in base 85
 *  exp(85,5) > exp(2,32)
 * the ASCII characters used are between '!' and 'u'
 * 'z' encodes 32-bit zero; 'x' is used to mark the end of encoded data.
 *
 *  Paul Rutter		Joe Orost
 *  philabs!per		petsd!joe
 *
 *  WARNING: this version is not compatible with the original as sent out
 *  on the net.  The original encoded from ' ' to 't'; which cause problems
 *  with some mailers (stripping off trailing blanks).
 */

#ifdef _MSC_VER
#pragma warning (disable:4996)
#endif

#include <stdio.h>
#include <stdlib.h> // for exit(), ...
#include <string.h> // for strcmp(), ...

#ifdef _MSC_VER
#define reg int
#else
#define reg register
#endif

#define MAXPERLINE 78

long int Ceor = 0;
long int Csum = 0;
long int Crot = 0;

long int ccount = 0;
long int bcount = 0;
long int word;

#define EN(c)	(int) ((c) + '!')

// forward refs
void wordout(long int word);
void charout(reg c);

void encode(reg c) 
{
    Ceor ^= c;
    Csum += c;
    Csum += 1;
    if ((Crot & 0x80000000)) 
    {
        Crot <<= 1;
        Crot += 1;
    }
    else 
    {
        Crot <<= 1;
    }
    Crot += c;

    word <<= 8;
    word |= c;
    if (bcount == 3) 
    {
        wordout(word);
        bcount = 0;
    }
    else 
    {
        bcount += 1;
    }
}

void wordout(long int word) 
{
    if (word == 0) 
    {
        charout('z');
    }
    else
    {
        int tmp = 0;
        if(word < 0) 	/* Because some don't support unsigned long */
        {
            tmp = 32;
            word = word - (long)(85 * 85 * 85 * 85 * 32);
        }
        if(word < 0) 
        {
            tmp = 64;
            word = word - (long)(85 * 85 * 85 * 85 * 32);
        }
        charout(EN((word / (long)(85 * 85 * 85 * 85)) + tmp));
        word %= (long)(85 * 85 * 85 * 85);
        charout(EN(word / (85 * 85 * 85)));
        word %= (85 * 85 * 85);
        charout(EN(word / (85 * 85)));
        word %= (85 * 85);
        charout(EN(word / 85));
        word %= 85;
        charout(EN(word));
    }
}

void charout(reg c)
{
    putchar(c);
    ccount += 1;
    if (ccount == MAXPERLINE) 
    {
        putchar('\n');
        ccount = 0;
    }
}

void usage()
{
    printf("Run btoa without any command arguments\n");
    printf("Enter a string<enter>, then ^Z (EOF) to end\n");
    printf("Will output the encoded string, and end values to pass to 'atob'\n");
    printf("to get back the decoded string\n");

}

int main(int argc, char **argv) 
{
    reg c;
    long int n;

    if (argc != 1) 
    {
        usage();
        fprintf(stderr,"Error: should be no args to %s\n", argv[0]);
        exit(2);
    }
    printf("Enter a string<enter>, then ^Z (EOF) to end\n");
    n = 0;
    while ((c = getchar()) != EOF) {
        if (c < ' ') {
            continue;
        }
        encode(c);
        n += 1;
    }

    while (bcount != 0) {
        encode(0);
    }
    /* n is written twice as crude cross check*/
    printf("\nxbtoa End N %ld %lx E %lx S %lx R %lx\n", n, n, Ceor, Csum, Crot);
    exit(0);
}

/* eof */
