#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<fstream>

using namespace std;

string * dictionary;
int dictionarysize = 369646;
const int row = 2;
const int col = 15;
char boardalpha[row][col];
 void boardAlphabet()
{   
    
    
    for(int i = 0; i< 2; i++)
      {
      for(int j = 0; j< 15; j++ )
        {
        
        boardalpha[i][j] = 'a' + j ;
        }
      }
}

int main()
{
  srand(time(0));
  boardAlphabet();



   int i, j, k ;
   string str1 = "" , str2 = "", str3 = "" , str4 = "" , str5 = "";
  
      for(i = 0; i < 1; i++)
          {
          for(j = 0 ; j < 15 ; j++)
              {
              str1 += boardalpha[i][j]; 
              if(j>0)
                str2 += boardalpha[i][j-1];
              if(j>1)
                str3 += boardalpha[i][j-2];
              if(j>2)
                str4 += boardalpha[i][j-3];
              if(j>3)
                str5 += boardalpha[i][j-4];
              
                
    
              }
          }
  cout<<boardalpha[0][5];
  cout<<boardalpha[1][6];
  cout<<boardalpha[0][3];
  cout<<boardalpha[0][0]<<endl;
  cout<<str1<<endl;
  cout<<str2<<endl;
  cout<<str3<<endl;
  cout<<str4<<endl;
  cout<<str5<<endl;
  
  cout<<sizeof(str1)<<endl;
  cout<<sizeof(str2)<<endl;
  cout<<CountAlphabets(str3)<<endl;
  return 0;
}
