#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  unsigned long int i;
  // checking there is a command line argument
  if (argc != 2) {
    printf("Usage: %s number\n", argv[0]);
    return 0;
  }
  // converting the command line argument to a unsigned long
  // and exit program if there is an error during the conversion.
  errno = 0;
  i = strtoul(argv[1], NULL, 0);
  if (errno != 0) {
    perror("Failed to convert number");
    return 0;
  }
  // write your solutions below this line
  // initialize array of 64 bits, x and j counters
  int binary[64] = {0};
  int x = 0;
  int j = 0;
  //repeatedly divide the decimal number by two and store the remained until we reach zero
  while (i > 0){
    binary[j] = i % 2;
    i = i/2;
    j++;
  }
  //initialize variable to keep track of whether or not we should include zeros, this will handle leading zeros
  int includeZeros = 0;
  //loop through array backwards to produce final binary number
  for (x = 63; x >= 0; x--){
    if (binary[x] == 1){
      includeZeros = 1;
    }
    if (x == 0 || includeZeros == 1 || binary[x] == 1){
      printf("%d", binary[x]);
    }
  }
  printf("\n");
  // write your solutions above this line
  return 0;
}
