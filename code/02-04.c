// Test fuer Zufallsvariable
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  int random_nr = 0;

  srand((unsigned)time(NULL));

  random_nr = rand();
  if (random_nr < RAND_MAX / 2)
    printf("Die Zufallszahl liegt in der unteren Hälfte! \n");
  else
    printf("Die Zufallszahl liegt in der oberen Hälfte! \n");

  return 0;
}
