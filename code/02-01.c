// C-Programm mit while-Schleife

int main(void) {
  int a = 0;
  int b = 0;

  a = 2;
  b = 3;

  while (b > 0) {
    a += a;
    b--;
  }

  return a;
}