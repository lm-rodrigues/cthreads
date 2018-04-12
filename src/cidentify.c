#include "../include/cthread.h"

#define GROUP_NAMES_SIZE 92

int cidentify (char *name, int size) {
  char group_names[] = "Arthur L. Fuchs, 00261577 \nGabriel Martins, 00242288 \nLeonardo Marques Rodrigues, 00213751\n";
  int i = 0;

  if ( size < GROUP_NAMES_SIZE ) {
    return -1;
  }
  else {
    while (i < size && group_names[i] != '\0') {
      name[i] = group_names[i];
      i++;
    }

    name[i] = '\0';
    return 0;
  }
}
