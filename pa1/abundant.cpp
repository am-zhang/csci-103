/********************************************************
 * CS 103 PA 1: Abundant numbers
 * Name: Amanda Zhang
 * USC email: amzhang@usc.edu
 * Description:
 *   Displays the top 3 most "abundant" (see writeup)
 *   numbers in the input sequence.  The input sequence
 *   is assumed to be valid natural numbers separated
 *   by spaces and ending with 0.
 ********************************************************/

#include <iostream>
using namespace std;

int main()
{
  // n1 is the number with the highest abundance, a1
  // n2 is the number with the 2nd-highest abundance, a2
  // n3 is the number with the 3rd-highest abundance, a3
  unsigned int n1 = 0, n2 = 0, n3 = 0;
  int a1 = 0, a2 = 0, a3 = 0;

  // num_abundant counts how many numbers in the input
  // sequence are abundant
  int num_abundant = 0;

  cout << "Enter a sequence of natural numbers, separated by spaces, ";
  cout << "and ending with 0."<< endl;

  int input = 1;
  while (input != 0) // ensures number is not 0 in which the program stops
  {
      int asum = 0;
      bool abundant = false;
      cin >> input;
      for (int i=1; i<input; i++) // checks divisors
      {
          if ((input % i) == 0)
          {
              asum += i;
          }
      }
      
      if (asum > input) // checks if the number is abundant based on sum of divisors
      {
          num_abundant++;
          abundant = true;
      }
      
      if (abundant && (asum > a1)) // compares abundance to current top 3 and assigns the input to n1/n2/n3 
      {
          n3 = n2;
          n2 = n1;
          a3 = a2;
          a2 = a1;

          n1 = input;
          a1 = asum;
      }
      else if (abundant && (asum > a2)) // rearranges the "leaderboard"
      {
          n3 = n2;
          a3 = a2;
          
          n2 = input;
          a2 = asum;
      }
      else if (abundant && (asum > a3))
      {
          n3 = input;
          a3 = asum;
      }
  }

  cout << "Abundant number count: " << num_abundant << endl;
  cout << "Top 3 most abundant numbers: " << endl;
  cout << n1 << " : " << a1 << endl;
  cout << n2 << " : " << a2 << endl;
  cout << n3 << " : " << a3 << endl;


  return 0;

}
