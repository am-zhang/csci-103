#include <iostream>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include "bmplib.h"

using namespace std;

void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);
void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB], 
	      int N, double kernel[][11]);
void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB]);
void gaussian(double k[][11], int N, double sigma);
void gaussian_filter(unsigned char output [][SIZE][3], 
                     unsigned char input[][SIZE][3], int N, double sigma);
void unsharp(unsigned char output[][SIZE][3], unsigned char input[][SIZE][3], 
             int N, double sigma, double alpha);

int main(int argc, char* argv[])
{
   //First check argc
  if(argc < 3)
    {
      //we need at least ./filter <input file> <filter name> to continue
      cout << "usage: ./filter <input file> <filter name> <filter parameters>";
      cout << " <output file name>" << endl;
      return -1;
    }
   //then check to see if we can open the input file
   unsigned char input[SIZE][SIZE][RGB];
   unsigned char output[SIZE][SIZE][RGB];
   char* outfile;
   int N;
   double sigma, alpha;
   //double kernel[11][11];

   // read file contents into input array
   int status = readRGBBMP(argv[1], input); 
   if(status != 0)
   {
      cout << "unable to open " << argv[1] << " for input." << endl;
      return -1;
   }
   //Input file is good, now look at next argument
   if( strcmp("sobel", argv[2]) == 0)
   {
     sobel(output, input);
     outfile = argv[3];
   }
   else if( strcmp("blur", argv[2]) == 0)
   {
     if(argc < 6)
       {
	 cout << "not enough arguments for blur." << endl;
	 return -1;
       }
     N = atoi(argv[3]);
     sigma = atof(argv[4]);
     outfile = argv[5];
     gaussian_filter(output, input, N, sigma);
   }
   else if( strcmp("unsharp", argv[2]) == 0)
   {
     if(argc < 7)
       {
	 cout << "not enough arguments for unsharp." << endl;
	 return -1;
       }
     N = atoi(argv[3]);
     sigma = atof(argv[4]);
     alpha = atof(argv[5]);
     outfile = argv[6];
     unsharp(output, input, N, sigma, alpha);

   }
   else if( strcmp("dummy", argv[2]) == 0)
   {
     //do dummy
     dummy(output, input);
     outfile = argv[3];
   }
   else
   {
      cout << "unknown filter type." << endl;
      return -1;
   }

   if(writeRGBBMP(outfile, output) != 0)
   {
      cout << "error writing file " << argv[3] << endl;
   }

}   

// Creates an identity kernel (dummy kernel) that will simply
// copy input to output image and applies it via convolve()
void dummy(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB])
{
   double k[11][11];
   for (int i = 0; i < 3; i++)
   {
      for(int j = 0; j < 3; j++)
      {
         k[i][j] = 0;
      }
   }
   k[1][1] = 1;
   convolve(out, in, 3, k);
}


// Convolves an input image with an NxN kernel to produce the output kernel
void convolve(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB], 
	      int N, double kernel[][11])
{
 
   int padded[SIZE+10][SIZE+10][RGB];  // Use for input image with appropriate 
                                       // padding
   int temp[SIZE][SIZE][RGB];          // Use for the unclamped output pixel 
                                       // values then copy from temp to out, 
                                       // applying clamping 
   //first set all of padded to 0 (black)
   for (int i=0; i<SIZE+10; i++)
   {
       for (int j=0; j<SIZE+10; j++)
       {
           for (int k=0; k<RGB; k++)
           {
               padded[i][j][k] = 0;
           }
       }
   }

   //now copy input into padding to appropriate locations
   for (int i=0; i<SIZE; i++)
   {
       for (int j=0; j<SIZE; j++)
       {
           for (int k=0; k<RGB; k++)
           {
               padded[N/2+i][N/2+j][k] = in[i][j][k];
           }
       }
   }

   //initialize temp pixels to 0 (black)
   for (int i=0; i<SIZE; i++)
   {
       for (int j=0; j<SIZE; j++)
       {
           for (int k=0; k<RGB; k++)
           {
               temp[i][j][k] = 0;
           }
       }
   }

  //now perform convolve (using convolution equation on each pixel of the 
  // actual image) placing the results in temp (i.e. unclamped result)
  for (int y=0; y<SIZE; y++) 
   {
       for (int x=0; x<SIZE; x++)
       {
           for (int k=0; k<RGB; k++)
           {
               for (int i=0; i<N; i++) 
               {
                   for (int j=0; j<N; j++) 
                   {
                       temp[x][y][k] += 
                           (int)((padded[x+i][y+j][k])*(kernel[i][j]));  
                   }
               }
           }
       }
   }
  
   //now clamp and copy to output
   for (int i=0; i<SIZE+10; i++) // first clamps
   {
       for (int j=0; j<SIZE+10; j++)
       {
           for (int k=0; k<RGB; k++)
           {
               if (temp[i][j][k] > 255)
               {
                   temp[i][j][k] = 255;
               }
               else if (temp[i][j][k] < 0)
               {
                   temp[i][j][k] = 0;
               } 
           }
       }
   }
   for (int i=0; i<SIZE; i++) // then clamps to output
   {
       for (int j=0; j<SIZE; j++)
       {
           for (int k=0; k<3; k++)
           {
               out[i][j][k] = (unsigned char)temp[i][j][k];
           }
       }
   }
}

void sobel(unsigned char out[][SIZE][RGB], unsigned char in[][SIZE][RGB])
{
   double k[11][11];
   double s_h1[3][3] = { {-1, 0, 1}, 
                         {-2, 0, 2}, 
                         {-1, 0, 1} };
   double s_h2[3][3] = { {1, 0, -1}, 
                         {2, 0, -2}, 
                         {1, 0, -1} };
   
   unsigned char h1_soble[SIZE][SIZE][RGB]; //hold intemediate images
   unsigned char h2_soble[SIZE][SIZE][RGB]; 

   for (int i=0; i < 11; i++)
   {
      for(int j=0; j < 11; j++)
      {
         k[i][j] = 0;
      }
   }

   // Copy in 1st 3x3 horizontal sobel kernel (i.e. copy s_h1 into k)
   for (int i = 0; i < 3; i++)
   {
      for(int j=0; j < 3; j++)
      {
         k[i][j] = s_h1[i][j];
      }
   }

   // Call convolve to apply horizontal sobel kernel with result in h1_soble
   convolve(h1_soble, in, 3, k);

   // Copy in 2nd 3x3 horizontal sobel kernel (i.e. copy s_h2 into k)
   for (int i = 0; i < 3; i++)
   {
      for(int j=0; j < 3; j++)
      {
         k[i][j] = s_h2[i][j];
      }
   }

   // Call convolve to apply horizontal sobel kernel with result in h2_soble
   convolve(h2_soble, in, 3, k);

   // Add the two results (applying clamping) to produce the final output 
   for (int i=0; i<SIZE; i++)
   {
       for (int j=0; j<SIZE; j++)
       {
           for (int c=0; c<3; c++)
           {
               if (h1_soble[i][j][c] > 255)
               {
                   h1_soble[i][j][c] = 255;
               }
               else if (h1_soble[i][j][c] < 0)
               {
                   h1_soble[i][j][c] = 0;
               } 
               if (h2_soble[i][j][c] > 255)
               {
                   h2_soble[i][j][c] = 255;
               }
               else if (h2_soble[i][j][c] < 0)
               {
                   h2_soble[i][j][c] = 0;
               } 
           }
       }
   }
   
   for (int i=0; i<SIZE; i++) // adds h1 and h2
   {
       for (int j=0; j<SIZE; j++)
       {
           for (int c=0; c<3; c++)
           {
               out[i][j][c] = 
                   (unsigned char)(h1_soble[i][j][c] + h2_soble[i][j][c]);
           }
       }
   }
  


}

void gaussian(double k[][11], int N, double sigma)
{
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            k[i][j] = exp(-( ((i-(N/2))*(i-(N/2))/(2*sigma*sigma)) 
                      + ((j-(N/2))*(j-(N/2))/(2*sigma*sigma)) )); 
            // calculates given formula
        }
    }
    k[N/2][N/2] = 1;
    
    double total = 0; // for normalizing
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            total += k[i][j];
        }
    }
    for (int i=0; i<N; i++)
    {
        for (int j=0; j<N; j++)
        {
            k[i][j] = k[i][j]/total;
        }
    }
    
    for (int i=0; i<N; i++) // prints out the normalized values of the kernel
    {
        for (int j=0; j<N; j++)
        {
            cout << k[i][j] << " ";
        }
        cout << endl;
    }
}

void gaussian_filter(unsigned char output [][SIZE][3], 
                     unsigned char input[][SIZE][3], int N, double sigma)
{
    double temp[11][11];
    for (int i=0; i<11; i++)
    {
        for (int j=0; j<11; j++)
        {
            temp[i][j] = 0;
        }
    }
    gaussian(temp, N, sigma);
    convolve(output, input, N, temp);
}

void unsharp(unsigned char output[][SIZE][3], 
             unsigned char input[][SIZE][3], int N, double sigma, double alpha)
{
    double blur[SIZE][SIZE][RGB];
    gaussian_filter(output, input, N, sigma);
    for (int i=0; i<SIZE; i++) 
    // calculates detail map by subtracting blur from original input
    {
       for (int j=0; j<SIZE; j++)
       {
           for (int k=0; k<3; k++)
           {
               blur[i][j][k] = input[i][j][k] - output[i][j][k];
           }
       }
    }  
   
    for (int i=0; i<SIZE; i++) // then clamps and sums sharpened image
    {
       for (int j=0; j<SIZE; j++)
       {
           for (int k=0; k<3; k++)
           {
               if (((double)input[i][j][k] + alpha*blur[i][j][k]) > 255)
               {
                   output[i][j][k] = 255;
               }
               else if (((double)input[i][j][k] + alpha*blur[i][j][k]) < 0)
               {
                   output[i][j][k] = 0;
               } 
               else
               {
                   output[i][j][k] = 
                       (unsigned char)((double)input[i][j][k] 
                        + alpha*blur[i][j][k]);
               }
           }
       }
    }
}
