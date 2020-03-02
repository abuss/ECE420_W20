#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct data {
  int x, y;
  char* name;
  double sum;
};

int main() {
  struct data a;
  unsigned char* b;
  void* c;
  struct data d;

  a.x = 4;
  a.y = 9;
  a.sum = a.x + a.y;
  printf("x:%d y:%d sum:%f\n",a.x, a.y, a.sum);

  int a_size = sizeof(a);
  printf("Size of a: %d\n",a_size);

  b = (unsigned char*)&a;

  for(int i=0; i<a_size; ++i)
    printf("%x ",b[i]);
  printf("\n");


  c = (void*)malloc(a_size);
  memcpy(c, b, a_size);


  struct data* e = (struct data*)c;
  printf("x:%d y:%d sum:%f\n",e->x, e->y, e->sum);

  struct data f;
  memcpy(&f, c, a_size);
  printf("x:%d y:%d sum:%f\n",f.x, f.y, f.sum);

  return 0;
}