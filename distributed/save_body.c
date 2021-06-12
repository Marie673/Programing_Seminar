#include <stdio.h>

#define BUFLEN 256

/*
   read clen bytes (read bytes before finding End of File if clen == -1)
   return 0 (clen > 0) or -1 (clen == -1)
 */

int save_body(FILE *fpr, FILE *fpw, int clen) {
    char buf[BUFLEN];
    int n, bufsize;

    while (clen == -1 || clen > 0) {
        if (clen == -1 || clen > BUFLEN)
            bufsize = BUFLEN;
        else
            bufsize = clen;
        n = fread(buf, 1, bufsize, fpr);
        if (n == 0)
            break;
        fwrite(buf, 1, n, fpw);
        if (clen > 0)
            clen -= n;
    }
    return clen;
}
