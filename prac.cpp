#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;

int main()
{
  char arr[2][10];
  int arr2[4][5] = {{1,2,3,4,5},
                    {6,7,8,9,10},
                    {0,9,8,7, 6},
                    {5,4,3,2, 1}};
  char alpha = 'a';
  int i, j, k;
  string str = "";  
  string str2 = "";
  string str3 = "";
    for(i = 0; i< 2; i++)
      {
      for(j = 0; j< 10; j++)
        {
        arr[i][j] = alpha;
        alpha++;
        cout<<arr[i][j];
        }
        cout<<endl;
      }
      
      for(i = 0; i< 1 ; i++)
        {
        for(j = 0 ; j < 10 ; j++)
          {
          str += arr[i][j]; 
          if(j == 5)
            str3 += arr[i][j];
          }
          
        }
        
        if(str == "abcdefghij")
          cout<<"Its working!!";
          
        for(i = 0 ; i < 4 ; i++)
          {
          for(j=0; j< 5; j++)
            {
            if(j == 2)
              {
              str2 += arr2[i][j];
              cout<<arr2[i][j];
              }
              
            }
          
          }
          cout<<str2<<endl;
          cout<<str3<<endl;
}
