#include <stdio.h>
#include <happy.h>
int main(void)
{
   printf("Before enter:\n");
   int a;
   scanf("%d", &a);
   if (isPrime(a))
   {
      printf("%d", a);
   }
   return 0;
}
