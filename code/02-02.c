// C-Programm mit Funktion

int foo(int a, int b) { return a * b; }

int main(void) {
  int a = 0;
  int b = 0;

  a = 2;
  b = 3;

  a = foo(a, b);

  return a;
}
