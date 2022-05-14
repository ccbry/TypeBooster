#include <math.h>
_Bool isPrime(int totest)
{
   if (totest == 1)
   {
      return 0;
   }
   for (int i = 2; i <= sqrt(totest); i++)
   {
      if (totest % i == 0)
      {
         return 0;
      }
   }

   return 1;
}