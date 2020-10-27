/*******************************************************************************
 * CS 103 Twenty-One (Blackjack) PA
 * Name: Amanda Zhang
 * USC email: amzhang@usc.edu
 ******************************************************************************/

#include <iostream>
#include <cstdlib>

using namespace std;

/* Prototypes */
void shuffle(int cards[]);
void printCard(int id);
int cardValue(int id);
void printHand(int hand[], int numCards);
int getBestScore(int hand[], int numCards);

const int NUM_CARDS = 52;

/**
 * Global arrays to be used as look-up tables.
 */
const char suit[4] = {'H','S','D','C'};
const char* type[13] = 
  {"2","3","4","5","6","7",
   "8","9","10","J","Q","K","A"};
const int value[13] = {2,3,4,5,6,7,8,9,10,10,10,10,11};

/**
 * Should permute the deck of cards, effectively shuffling it.
 */
void shuffle(int cards[])
{
    for (int i=0; i<52; i++)
    {
        cards[i] = i;
    }
    for (int i=51; i>0; i--)
    {
        int temp = 0;
        int j = rand() % (i + 1);
        temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

/**
 * Prints the card in a "pretty" format:   "type-suit"
 *  Examples:  K-C  (King of clubs), 2-H (2 of hearts)
 *  Valid Types are: 2,3,4,5,6,7,8,9,10,J,Q,K,A
 *  Valid Suits are: H, D, C, S
 */
void printCard(int id)
{
    if (id <= 12)
    {
        cout << type[id] << '-' << suit[0];
    }
    else if (id <= 25)
    {
        cout << type[id-13] << '-' << suit[1];
    }
    else if (id <= 38)
    {
        cout << type[id-26] << '-' << suit[2];
    }
    else
    {
        cout << type[id-39] << '-' << suit[3];
    }
}

/**
 * Returns the numeric value of the card.
 *  Should return 11 for an ACE and can then
 *  be adjusted externally based on the sum of the score.
 */
int cardValue(int id)
{
    if (id <= 12)
    {
        return value[id];
    }
    else if (id <= 25)
    {
        return value[id-13];
    }
    else if (id <= 38)
    {
        return value[id-26];
    }
    else
    {
        return value[id-39];
    }
}

/**
 * Should print out each card in the hand separated by a space and
 * then print a newline at the end.  
 * Should use printCard() to print out each card.
 */
void printHand(int hand[], int numCards)
{
    for (int i=0; i<numCards; i++)
    {
        printCard(hand[i]);
        cout << ' ';
    }
    cout << endl;
}

/**
 * Should return the total score of the provided hand.  
 * ACES should be treated as 11s to make the highest possible hand
 *  and only being reduced to 1s as needed to avoid busting.
 */
int getBestScore(int hand[], int numCards)
{
    int sum = 0;
    int aceCount = 0;
    for (int i=0; i<numCards; i++)
    {
        if (cardValue(hand[i]) == 11) 
        {
            aceCount++;
        }
        sum += cardValue(hand[i]);
    }
    if (sum <= 21)
    {
        return sum;
    }
    else 
    {
        for (int i=1; i<=aceCount; i++)
        {
            if (sum - (10*i) <= 21)
            {
                return sum-(10*i);
            }
            else if (sum - (10*aceCount) > 21)
            {
                return sum - (10*aceCount);
            }
        }
    }
    return sum;
}

/**
 * Main program logic for the game of 21
 */
int main(int argc, char* argv[])
{
  if(argc < 2){
    cout << "Error - Please provide the seed value." << endl;
    return 1;
  }
  int seed = atoi(argv[1]);
  srand(seed);

  int cards[52];
  int dhand[9];
  int phand[9];
 
  shuffle(cards);

  char option = 'y';
  // keeps the game going until player quits
  while (option == 'y') 
  {
      // deals the initial hands of cards
      for (int i=0; i<4; i++) 
      {
          if (i%2 == 0)
          {
              phand[i/2] = cards[i];
          }
          else
          {
              dhand[i/2] = cards[i];
          }
      }
      int cardCount = 4;
      // prints initial hands
      cout << "Dealer: ? "; 
      printCard(dhand[1]);
      cout << endl;
      cout << "Player: ";
      printHand(phand,2);
      
      int cardsP = 2;
      int cardsD = 2;

      // checks initial player hand if player immediately has 21
      if (getBestScore(phand,2) == 21 && getBestScore(dhand,2) != 21) 
      {
          cout << "Dealer: ";
          while (getBestScore(dhand,cardsD) < 17)
          {
              dhand[cardsD] = cards[cardCount];
              cardCount++;
              cardsD++;
          }     
          printHand(dhand,cardsD);
          // dealer busts
          if (getBestScore(dhand,cardsD) > 21) 
          {
              cout << "Dealer busts" << endl;
              cout << "Win " << getBestScore(phand,2) << ' ';
              cout << getBestScore(dhand,cardsD) << endl;
          }
          // player wins
          else if (getBestScore(dhand,cardsD) < getBestScore(phand,cardsP)) 
          {
              cout << "Win " << getBestScore(phand,2) << ' ';
              cout << getBestScore(dhand,cardsD) << endl;
          }
      }

      // if both player and dealer tie for 21 on the first hand
      else if (getBestScore(phand,2) == 21 && getBestScore(dhand,2) == 21) 
      {
          cout << "Dealer ";
          printHand(dhand,2);
          cout << "Tie " << getBestScore(phand,2) << ' ';
          cout << getBestScore(dhand,2) << endl;
      }
      else
      {
          char hit = 'h';
          cout << "Type 'h' to hit and 's' to stay: " << endl;
          cin >> hit;
          // gives player hand when deciding to hit
          while (hit == 'h') 
          {
              cout << "Player: ";
              phand[cardsP] = cards[cardCount];
              cardCount++;
              cardsP++;
              printHand(phand,cardsP);
              // player busts
              if (getBestScore(phand,cardsP) > 21) 
              {
                  cout << "Player busts" << endl;
                  cout << "Lose " << getBestScore(phand,cardsP) << ' ';
                  cout << getBestScore(dhand,cardsD) << endl;
                  break;
              }
              // player gets perfect hand
              else if (getBestScore(phand,cardsP) == 21) 
              {
                  cout << "Dealer: ";
                  while (getBestScore(dhand,cardsD) < 17)
                  {
                      dhand[cardsD] = cards[cardCount];
                      cardCount++;
                      cardsD++;
                  }
                  printHand(dhand,cardsD);
                  // ties for 21
                  if (getBestScore(dhand,cardsD) == 21) 
                  {
                      cout << "Tie " << getBestScore(phand,cardsP) << ' ';
                      cout << getBestScore(dhand,cardsD) << endl;
                  }
                  // or else player wins
                  else 
                  {
                      cout << "Win " << getBestScore(phand,cardsP) << ' ';
                      cout << getBestScore(dhand,cardsD) << endl; 
                  }
                  break;
              }
              cout << "Type 'h' to hit and 's' to stay: " << endl;
              cin >> hit;
             
          }

          // gives dealer's hand when deciding to stay
          while (hit == 's') 
          {
              cout << "Dealer: ";
              while (getBestScore(dhand,cardsD) < 17)
              {
                  dhand[cardsD] = cards[cardCount];
                  cardCount++;
                  cardsD++;
              }
              printHand(dhand,cardsD);

              // dealer busts
              if (getBestScore(dhand,cardsD) > 21) 
              {
                  cout << "Dealer busts" << endl;
                  cout << "Win " << getBestScore(phand,cardsP) << ' ';
                  cout << getBestScore(dhand,cardsD) << endl;
                  break;
              }

              // dealer loses
              else if (getBestScore(dhand,cardsD) > getBestScore(phand,cardsP)) 
              {
                  cout << "Lose " << getBestScore(phand,cardsP) << ' ';
                  cout << getBestScore(dhand,cardsD) << endl;
                  break;
              }

              // dealer ties
              else if (getBestScore(dhand,cardsD) == getBestScore(phand,cardsP)) 
              {
                  cout << "Tie " << getBestScore(phand,cardsP) << ' ';
                  cout << getBestScore(dhand,cardsD) << endl;
                  break;
              }
              
              // player wins
              else
              {
                  cout << "Win " << getBestScore(phand,cardsP) << ' ';
                  cout << getBestScore(dhand,cardsD) << endl;
                  break;
              }

          }
          // causes program to quit if any other letter is entered
          if (hit != 'h' && hit != 's') 
          {
              return 0;
          }
      }
      // reshuffles cards for next round
      shuffle(cards); 
      cout << endl << "Play again? [y/n]" << endl;
      cin >> option;
  }

  return 0;
}