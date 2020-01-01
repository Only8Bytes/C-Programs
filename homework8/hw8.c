//Author: Andrew Lazar
//Email: abl5458@psu.edu

#include <string.h>
#include <stdlib.h>
#include "mystring.h"
#include <errno.h>
int mystrcmp(const char *s1, const char *s2) {
  while (1){
    char c1 = *s1++;
    char c2 = *s2++;
    if (!c1 && !c2){return 0;};
    /*strings are equal if they both ended*/
    if (!c1){return -1;};
    /*if string one is shorter then it is less than string two*/
    if (!c2){return 1;};
    /*if string two is shorter then string one is greater*/
    if (c1 < c2){return -1;};
    /*if the nth character in string one is less than the nth character in string two, string two is greater*/
    if (c2 < c1){return 1;};
    /*vice versa*/
  }
}

char *mystrdup(const char *s) {
  int len = 0;
  int i;
  char* string;
  for (i = 0; s[i] != 0; i++){ /*calculate string length*/
    len += 1;
  }
  string = (char*) malloc(len + 1);
  if (string == NULL){
    errno = ENOMEM;
    return NULL;
  }
  for (i = 0; i < len; i++){
    /*place all characters of s in the newly allocated memory*/
    string[i] = s[i];
  }
  return string;
}
