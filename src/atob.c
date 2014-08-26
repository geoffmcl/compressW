/* atob: version 4.0
 * stream filter to change printable ascii from "btoa" back into 8 bit bytes
 * if bad chars, or Csums do not match: exit(1) [and NO output]
 *
 *  Paul Rutter		Joe Orost
 *  philabs!per		petsd!joe
 */

#ifdef _MSC_VER
#pragma warning (disable:4996)
#endif
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h> // for exit(), ...
#include <string.h> // for strcmp(), ...
#ifdef _MSC_VER
#include <process.h>
#define getpid _getpid
#else
#include <unistd.h> // unlink(), getpid()...
#endif
#define reg int

#define ADD_FILE_DELETE /* remove to keep 'temporary' file */

#define streq(s0, s1)  strcmp(s0, s1) == 0

#define times85(x)	((((((x<<2)+x)<<2)+x)<<2)+x)

long int Ceor = 0;
long int Csum = 0;
long int Crot = 0;
long int word = 0;
long int bcount = 0;

#ifdef _MSC_VER
#define TMP_FORM "tempatob.%x"
//#define WRITE_MODE "wb"
#define WRITE_MODE "w+"
#else
#define TMP_FORM "/tmp/atob.%x"
#define WRITE_MODE "w+"
#endif

static FILE *tmp_file = 0;
static char tmp_name[264] = {0};
static int out_count = 0;
static char out_buffer[264];

void byteout(reg c);

void close_file()
{
  if (tmp_file)
      fclose(tmp_file);
  tmp_file = 0;
#ifdef ADD_FILE_DELETE
  if (tmp_name[0])
      unlink(tmp_name);	/* Make file disappear now it is closed */
#endif

}

void fatal( const char *msg ) {
  fprintf(stderr, "fatal: %s\n", msg);
  close_file();
  exit(1);
}

#define DE(c) ((c) - '!')

void decode(reg c) 
{
    if (c == 'z') 
    {
        if (bcount != 0) 
        {
            fatal("got 'z' when bcount NOT zero");
        }
        else
        {
            byteout(0);
            byteout(0);
            byteout(0);
            byteout(0);
        }
    }
    else if ((c >= '!') && (c < ('!' + 85))) 
    {
        if (bcount == 0) 
        {
            word = DE(c);
            ++bcount;
        }
        else if (bcount < 4) 
        {
            word = times85(word);
            word += DE(c);
            ++bcount;
        }
        else 
        {
            word = times85(word) + DE(c);
            byteout((int)((word >> 24) & 255));
            byteout((int)((word >> 16) & 255));
            byteout((int)((word >> 8) & 255));
            byteout((int)(word & 255));
            word = 0;
            bcount = 0;
        }
    }
    else
    {
        fatal("character not in the decode range");
    }
}

void byteout(reg c) 
{
    reg res;
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
    res = putc(c, tmp_file);
    if (res == EOF) {
        fprintf(stderr,"Error: unable to write to file %s\n", tmp_name);
        close_file();
        exit(1);
    }
    out_buffer[out_count++] = (char)c;
    out_buffer[out_count] = 0;
}

void usage()
{
    sprintf(tmp_name, TMP_FORM, getpid());
    printf("Run atob without any command arguments\n");
    printf("Will create a temporary file like '%s'\n", tmp_name);
    printf("Will await a string 'xbtoa Begin<enter> to start the process...\n");
    printf("Will expect a seq like '5sdq,77Km?8P2WL', as output from btoa\n");
    printf("Then a seq 'xbtoa End N 12 c E 1 S 35d R 42012', again as output by btoa\n");
    printf("If the sequences match, atob will output the original string enetered in btoa\n");
    printf("Those 2 seq will be output by atob is 'ABCDEFGHIJKL' is entered.\n");
}

int main(int argc, char **argv) 
{
    reg c;
    long int i;
    char buf[100];
    long int n1, n2, oeor, osum, orot;

    if (argc != 1) {
        usage();
        fprintf(stderr,"Error: Should be no args to %s\n", argv[0]);
        exit(2);
    }
    sprintf(tmp_name, TMP_FORM, getpid());
    tmp_file = fopen(tmp_name, WRITE_MODE);
    if (tmp_file == NULL) {
        fprintf(stderr,"Unable to create file '%s'!\n", tmp_name);
        return 1;
    }
#ifdef _MSC_VER /* can NOT delete an open file! */
    fprintf(stderr,"Created output file '%s'\n", tmp_name);
    fprintf(stderr,"1: Enter 'xbtoa Begin<enter> to start process...\n");
#else
    unlink(tmp_name);	/* Make file disappear */
#endif
    /* search for header line */
    for (;;) 
    {
        if (fgets(buf, sizeof(buf), stdin) == NULL) 
        {
            fatal("fgets() returned NULL");
        }
        if (streq(buf, "xbtoa Begin\n")) 
        {
            break;
        }
    }

#ifdef _MSC_VER /* can NOT delete an open file! */
    fprintf(stderr,"2: Enter a sequence output by btoa, like '5sdq,77Km?8P2WL'<enter>\n"
        "   Then the xbtoa output like 'xbtoa End N 12 c E 1 S 35d R 42012<enter>,\n"
        "   plus last char 'x'<enter>\n");
    fprintf(stderr,"The above two sequences should produce 'ABCDEFGHIJKL'\n");
#endif
    while ((c = getchar()) != EOF) 
    {
        if (c == '\n')
        {
            continue;
        } 
        else if (c == 'x') 
        {
            break;
        }
        else
        {
            decode(c);
        }
    }

    if(scanf("btoa End N %ld %lx E %lx S %lx R %lx\n",
         &n1, &n2, &oeor, &osum, &orot) != 5) 
    {
        fatal("bad format or Csum to atob - expected 'btoa End N ...' as output by btoa!");
    }
    if ((n1 != n2) || (oeor != Ceor) || (osum != Csum) || (orot != Crot)) 
    {
        fatal("one or more values not equal to expected!");
    }
    else
    {
        fprintf(stderr,"Written %d chars to %s\n", out_count, tmp_name);
        /*copy OK tmp file to stdout*/;
        c = fseek(tmp_file, 0L, SEEK_SET);
        if (c) 
        {
            fprintf(stderr,"Seek on '%s' FAILED!\n", tmp_name);
            close_file();
            exit(1);
        }
        // fprintf(stderr,"Buffered '%s]\n", out_buffer);
        fprintf(stderr,"Extract sequence of %ld chars from file...\n", n1);
        for (i = 0; i < n1; i++)
        {
            c = getc(tmp_file);
            if (c == EOF) {
                fprintf(stderr,"getc returned EOF!\n");
                break;
            } 
            else 
            {
                putchar(c);
            }
        }
        fprintf(stderr,"\n");
    }

#ifdef _MSC_VER /* can NOT delete an open file! */
    close_file();
#endif

    exit(0);
}

/* eof */
