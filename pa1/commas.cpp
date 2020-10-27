/********************************************************
 * CS 103 PA 1: Commas
 * Name: Amanda Zhang
 * USC email: amzhang@usc.edu
 * Description:
 *  Displays a 64-bit integer (long long) with
 *   commas for thousands, millions, billions, etc.
 ********************************************************/

#include <iostream>
#include <cmath>
using namespace std;

int main()
{
  long long n;
  cout << "Enter an integer:" << endl;

  cin >> n;
  if (n < 0) // checks if input is negative
  {
      cout << '-';
      n = -n;
  }
    
  int digits = 0;
 
  for (long long i=n; i>0; i/=10) // checks how many digits n has
  {
      digits++;
  }

  int firstBlock = digits % 3; // how to isolate the first few (1, 2, or 3) digits, or the first "block"
  if (firstBlock == 0) 
  {
      firstBlock += 3;
  }
  if (n<1000) // checks that the output is greater than 1000 and therefore requires commas
  {
      cout << n << endl;
      return 0;
  }

  digits -= firstBlock; // keeps track of remaining digits
  cout << n/(long long)pow(10,digits); // outputs the first block
  n %= (long long)pow(10,digits); // removes the first block
  digits -= 3;

  while (digits>=0)
  {
      
      if (n/(long long)pow(10,digits) < 1000) // checks whether the block needs padding zeroes
      {
          cout << ',';
          if (n/(long long)pow(10,digits) < 10) // pads zeroes
          {
              cout << 0 << 0;
          }
          else if (n/(long long)pow(10,digits) < 100)
          {
              cout << 0;
          }
          cout << n/(long long)pow(10,digits);
      }
      
      n %= (long long)pow(10,digits); // removes the first three digits
      digits -= 3;

  }
  cout << endl;
 
  return 0;

}
