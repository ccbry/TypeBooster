#include<stdio.h>
#include<math.h>
#include<happy.h>

int main(void){
   printf("Before enter:\n");
   int a;
   scanf("%d", &a);
   printf("%d", a);

   return 0;
}
_Bool isPrime(int totest){
   if(totest == 1){
      return 0;
   }
   for (int i = 2; i <= sqrt(totest); i ++){
      if(totest % i == 0){
         return 0;
      }
   }

   return 1;
}