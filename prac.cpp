#include<iostream>
#include<ctime>
#include<cstdlib>
using namespace std;

int main()
{
  char arr[2][10];
  char alpha = 'a';
  int i, j, k;
  string str = "";  
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
}
