//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include "util.h"

using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

string * dictionary;
int dictionarysize = 369646;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 930, height = 660;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;

//----self declared----//
const int max_rows = 9;
const int max_col = 15;
char boardalpha[max_rows][max_col];  
char shoot;
float x_cord, y_cord;
float v = 1.0;
float center_pos_x = width/2.0;
float center_pos_y = 10.0;
float gradient_x = 0.0;
float gradient_y = 0.0;
float ball_x = center_pos_x;
float ball_y = center_pos_y;

string word = "aah";

int pile_height[max_rows]= {0};
int t = 120;
string str1 = "";  //---string for storing alphabets of first two rows---//
string str2 = "";
string str3 = "";
string str4 = "";
string str5 = "";       
string str6 = "";
string str7 = "";
string str8 = "";
string str9 = "";
string str10 = "";
string str11 = "";
string str12 = "";
string str13 = "";
string str14 = "";
string str15 = "";

string  str2_1 = "";
string  str2_2 = "";
string  str2_3 = "";
string  str2_4 = "";
string  str2_5 = "";
string  str2_6 = "";
string  str2_7 = "";
string  str2_8 = "";
string  str2_9 = "";
string  str2_10 = "";
string  str2_11 = "";
string  str2_12 = "";
string  str2_13 = "";

string  str3_1 = "";
string  str3_2 = "";
string  str3_3 = "";
string  str3_4 = "";
string  str3_5 = "";
string  str3_6 = "";
string  str3_7 = "";
string  str3_8 = "";
string  str3_9 = "";
string  str3_10 = "";
string  str3_11 = "";
string  str3_12 = "";
string  str3_13 = "";

const int nalphabets = 26;

enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};

GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...


//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bi-linear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bi-linear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));

		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels

	* cheight= height of displayed cookiei pixels.
	* */
{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}

void boardAlphabet()  //---Function for assigning random numbers to first two rows---//
{   
    
    
    for(int i = 0; i< 2; i++)
      {
      for(int j = 0; j< 15; j++ )
        {
        
        boardalpha[i][j] = rand()%26 ;
        }
      }
}

/*
* Main Canvas drawing function.
* */
void shooterAlphabet()         //---function to display single alphabet placed on board---//
{
  shoot = rand() % 26;

}
void fillingArray()  //---Function for filing array -1 ---//
{
  int i, j;
    
    for(i = 2; i< 9; i++)
      {
      for(j = 0; j< 15 ; j++)
        {
        boardalpha[i][j] = -1;
        }
      }

}
void bursting()
{
  int i, j, k, a , l = 14;

    for(i = 0 ; i < 1 ; i++)
      
      {
      
  int l1 = 14;
  int l2 = 13;
  int l3 = 12;
  int l4 = 11;
  int l5 = 10;
  int l6 = 9;
  int l7 = 15;
  int l8 = 15;
  int l9 = 15;
  int l10 = 15;
  int l11 = 15;
  int l12 = 15;
        for(j = 0; j < 13 ; j++)
            
            
            {
            
              for(k = j ; k < 15 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"1)"<<str1<<endl;
                  str1 = "";
                  for(l = 0 ; l < 369646 ; l++)
                    {
                    if(str1 == dictionary[l])
                      {
                      boardalpha[i][k] == -1;
                      }
                    }
              
              if(j>0)
              {
              
              for(k = 1; k < l1 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"2)"<<str1<<endl;
                  str1 = "";
                  l1--;
                }  
              
              if(j>1)
                {
                
              for(k = 2; k < l2  ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"3)"<<str1<<endl;
                  str1 = "";
                  l2--;
                }
                
             if(j>2)
                {   
                
              for(k = 3; k < l3; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"4)"<<str1<<endl;
                  str1 = "";
                  l3--;
                }
                
            if(j>3)
                {
                
              for(k = 4; k < l4 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"5)"<<str1<<endl;
                  str1 = "";
                  l4--;
                 } 
              if(j>4)
                {
                 
              
              for(k = 5; k < l5 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"6)"<<str1<<endl;
                  str1 = "";
                  l5--;
                }
             if(j>5)
                {
                      
              for(k = 6; k < l6 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"7)"<<str1<<endl;
                  str1 = "";
                  l6--;
                }
           
            if(j>6)
              {
              for(k = 7; k < l7 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"8)"<<str1<<endl;
                  str1 = "";
                  l7--;
                }
             
             if(j>6)
              {
              for(k = 8; k < l8 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"9)"<<str1<<endl;
                  str1 = "";
                  l8--;
                }
                
            if(j>9)
              {
              for(k = 9; k < l9 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"10)"<<str1<<endl;
                  str1 = "";
                  l9--;
              }
             
             if(j>6)
              {
              for(k = 10; k < l10 ; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"11)"<<str1<<endl;
                  str1 = "";
                  l10--;
                }
             
             if(j>6)
              {
              for(k = 11; k < l11; k++)
                  
                  {
                  str1 += 'a' + boardalpha[i][k];
                  
                  }
                  cout<<"12)"<<str1<<endl;
                  str1 = "";
                  l11--;
              }
                  
                  
              
            }
      
      }


}
void burstingRows()
{
  int i, j, k , l;
  
  
    /*  for(i = 0; i < 1; i++)
          {
          for(j = 0 ; j < 15 ; j++)
              {                                                                          
              str1 += 'a' + boardalpha[i][j];
                for(k = 0; k < 369646; k++)
                  {
                  if(str1 == dictionary[k])
                    {
                    cout<<"Word found"<<endl;
                    
                    cout<<str1<<endl<<j<<endl;
                    for(l = j-j ; l < j+1 ; l++)
                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str1 = "";
                      score += j;
                      }

                    }
                  }
                
              if(j<14) 

                {
                str2 += 'a' + boardalpha[i][j+1];

                 for(k = 0; k < 369646; k++)
                  {
                  if(str2 == dictionary[k])

                      {
                      cout<<"Word found"<<endl<<j<<endl;
                      
                      cout<<str2<<endl;
                      for(l = j-j + 1 ; l < j+2 ; l++)
                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str2 = "";
                      score += j;
                      }

                      }

                  }
                }
              if(j<13) 
                {
                str3 += 'a' + boardalpha[i][j+2];

                for(k = 0; k < 369646; k++)
                  {
                  if(str3 == dictionary[k])
                      {

                      cout<<"Word found"<<endl<<j<<endl;
                      
                      cout<<str3<<endl;
                      for(l = j -j + 2; l < j+3; l++)

                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str3 = "";

                      score += j;
                      }
                      }
                  }

                }
              if(j<12)
                {

                str4 += 'a' + boardalpha[i][j+3];
                for(k = 0; k < 369646; k++)
                  {
                  if(str4 == dictionary[k])

                    {
                    cout<<"Word found"<<endl<<j<<endl;
                    

                    cout<<str4<<endl;
                    for(l = j - j + 3 ; l < j+4; l++)

                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;

                      str4 = "";
                      score += j;

                      }
                    }
                  }  

                }
                  

              if(j<11) 
                {

                str5 += 'a' + boardalpha[i][j+4];
                for(k = 0; k < 369646; k++)

                  {

                  if(str5 == dictionary[k])
                      {

                      cout<<"Word found"<<endl<<j<<endl;
                      

                      cout<<str5<<endl;

                      for(l = j - j + 4 ; l < j+5; l++)

                      {
                      boardalpha[i][l] = -1;

                      cout<<endl<<"L = "<<l<<endl;

                      str5 = "";

                      score += j;

                      }

                      }

                  }

                }

              if(j<10) 

                {

                str6 += 'a' + boardalpha[i][j+5];
                for(k = 0; k < 369646; k++)
                  {
                  if(str6 == dictionary[k])
                     {

                     cout<<"Word found"<<endl<<j<<endl;
                      
                      cout<<str6<<endl;
                      for(l = j - j + 5 ; l < j +6; l++)

                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str6 = "";
                      score += j;
                      }
                      }
                  } 

                }
              if(j<9)

                {
                str7 += 'a' + boardalpha[i][j+6];
                for(k = 0; k < 369646; k++)

                  {
                  if(str7 == dictionary[k])
                      {
                      cout<<"Word found"<<endl<<j<<endl;
                      
                      cout<<str7<<endl;
                      for(l = j - j + 6 ; l < j+7; l++)

                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str7 = "";
                      score += j;
                      }
                      }

                  }  
                }
              if(j<8)
                {
                str8 += 'a' + boardalpha[i][j+7];
                for(k = 0; k < 369646; k++)
                  {

                  if(str8 == dictionary[k])
                      {
                      cout<<"Word found"<<endl<<j<<endl;
                      
                      cout<<str8<<endl;
                      for(l = j - j + 7 ; l < j+8 ; l++)

                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str8 = "";
                      score += j;

                      }
                      }
                  }   
                }
              if(j<7) 
                {

                str9 += 'a' + boardalpha[i][j+8]; 
                for(k = 0; k < 369646; k++)
                  {
                  if(str9 == dictionary[k])

                    
                    {
                      cout<<"Word found"<<endl<<j<<endl;
                      
                      cout<<str9<<endl;

                      for(l = j - j + 8 ; l < j+9; l++)
                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;

                      str9 = "";
                      score += j;
                      }
                    }

                  } 
                }
              if(j<6) 
                {

                str10 += 'a' + boardalpha[i][j+9];
                for(k = 0; k < 369646; k++)
                  {
                  if(str10 == dictionary[k])

                      {
                      cout<<"Word found"<<endl<<j<<endl;
                      

                      cout<<str10<<endl;
                      for(l = j - j + 9 ; l < j+10; l++)
                      {

                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str10 = "";

                      score += j;
                      }

                      }
                  }  
                }

              if(j<5)
                {

                str11 += 'a' + boardalpha[i][j+10];
                 for(k = 0; k < 369646; k++)
                  {

                  if(str11 == dictionary[k])
                      {

                      cout<<"Word found"<<endl<<j<<endl;

                    
                      cout<<str11<<endl;

                      for(l = j - j + 10 ; l < j+11 ; l++)
                      {

                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;

                      str11 = "";

                      score += j;

                      }
                      }

                  } 

                }

              if(j<4)

                {

                str12 += 'a' + boardalpha[i][j+11];

                 for(k = 0; k < 369646; k++)

                  {

                  if(str12 == dictionary[k])
                      {

                      cout<<"Word found"<<endl<<j<<endl;
                      

                      cout<<str12<<endl;
                      for(l = j - j + 11 ; l < j+12 ; l++)

                      {
                      boardalpha[i][l] = -1;
                      cout<<endl<<"L = "<<l<<endl;
                      str12 = "";
                      score += j;
                      }

                      }
                  }  
                }
              if(j<3)
                {
                str13 += 'a' + boardalpha[i][j+12];

                  for(k = 0; k < 369646; k++)
                  {
                  if(str13 == dictionary[k])
                      {
                      cout<<"Word found"<<endl<<j<<endl;

                      
                      cout<<str13<<endl;
                      for(l = j - j + 12 ; l < j+13; l++)
                      {
                      boardalpha[i][l] = -1;

                      cout<<endl<<"L = "<<l<<endl;
                      str13 = "";
                      score = score + j;
                      }
                      }

                  }            
                }
                
            

              
                 
              
          cout<<str1<<endl;

          cout<<str2<<endl;
          cout<<str3<<endl;
          cout<<str4<<endl;
          cout<<str5<<endl;

          cout<<str6<<endl;
          cout<<str7<<endl;
          cout<<str8<<endl;

          cout<<str9<<endl;
          cout<<str10<<endl;
          cout<<str11<<endl;
          cout<<str12<<endl;

          cout<<str13<<endl;
          
        cout<<endl<<endl;

              }
          }
     

        cout<<score;
      


    score = score/2;*/
}


void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	glClearColor(1.0/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Color
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

	//write your drawing commands here or call your drawing functions...
        
        
                 for(int row = 0; row<2; row++)                     //---loop for displaying the first two rows of the board---//
                {
                   for(int col= 0; col < 15 ; col++)
                      {
                      int x = col * 60 + 10 ;
                      int y = height - (row+1) * 60 -   30;
                      DrawAlphabet(static_cast<alphabets>(boardalpha[row][col]), x, y, awidth, aheight);
            
                       }
            
                  }

    
                       for(int row = 2; row< 9; row ++ )  //---loop for displaying the shooted alphabets on the screen---//
                      
                      {
                        
                        for(int col = 0; col < 15; col ++)
                            
                            {
                              
                              if(boardalpha[row][col] != -1)
                                  
                                  {
                                      int x = col * 60 + 10;
                                      int y = height - (row + 1)*60 - 30;
                                      DrawAlphabet((alphabets)(boardalpha[row][col]), x, y, awidth, aheight);
                                  
                                  }
                            
                              }   
                            }
                            
                            
          
        DrawAlphabet((alphabets)shoot, ball_x , ball_y, awidth, aheight);
        
            
        
	DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[BLUE_VIOLET]);
	DrawString(width / 2 - 30, height - 25, width, height,
		"Time Left:" + Num2Str(120) + " secs", colors[RED]);
	DrawString(20, 10, width, height,
		"Play like a champ!!", colors[BLUE]);
	
        DrawString(width / 2 - 30, height - 25, width, height,
		" ", colors[RED]);
		
		
		
		
	// #----------------- Write your code till here ----------------------------#
	//DO NOT MODIFY THESE LINES
	DrawShooter((width / 2) - 35, 0, bwidth, bheight);
	glutSwapBuffers();
	//DO NOT MODIFY THESE LINES..
}

/* Function sets canvas size (drawing area) in pixels...

*  that is what dimensions (x and y) your game will have

*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)

* */
void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); //--- setting canvas size ---//
	
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arrow)

* is pressed from the keyboard

*

* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...

*

* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the

* program coordinates of mouse pointer when key was pressed.

*

* */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	*  this function*/
	/*
	glutPostRedisplay();

	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*

* You will have to add the necessary code here for finding the direction of shooting
*

* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*

* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swapping balls don't burst then re-swap the balls

}

/*This function is called (automatically) whenever your mouse button is clicked within inside the game window

*

* You will have to add the necessary code here for shooting, etc.

*

* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),

* x & y that tells the coordinate of current position of move mouse

*

* */

void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{
                      
                      if(gradient_x == 0)
                        {
                              if(gradient_y == 0)
                                {                                         //---Calculating the gradients---//
                                    gradient_x = (x - center_pos_x) / 60.0;                               gradient_y = (height - 10 - y)/60.0;
                                
                                }
                        }
                        
  
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{

	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)

* is pressed from the keyboard

* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.

* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*

* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).

* You can use this function to animate objects and control the

* speed of different moving objects by varying the constant FPS.

*

* */
void Timer(int m) {
        
        ball_x = ball_x + gradient_x * v;
        ball_y = ball_y + gradient_y * v;
        
        if (ball_x <= 0 || ball_x >= width - awidth)    //---bouncing condition---// 
          {
              gradient_x = -gradient_x; 
          }
          
         
         int col = round(ball_x/60) ; //---calculating the column where the ball is reached---//
         
        if (ball_y >= height - 140 - pile_height[(int)ball_x/60]*60 - 70)  //---Stopping the ball after reaching the first two rows and initially stored ball--//
          {
              
              int row = pile_height[col] + 2;
              
              boardalpha[row][col] = shoot;
              pile_height[col]++;
              
              
              ball_x = center_pos_x;       //---resetting the positions---//
              ball_y = center_pos_y;
              gradient_x = 0;
              gradient_y = 0;
              shoot = static_cast<alphabets>(rand() % 26); //---generating new random alphabet---//
              
          }

        
	glutPostRedisplay();
	glutTimerFunc(100.0/FPS, Timer, 0);
}

/*
* our gateway main function

* */
int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...
        
        SetCanvasSize(930,660);// Canvas size
        srand(time(0));
        boardAlphabet();
        shooterAlphabet();
        fillingArray();
        
        //check();
        
 	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize];
	
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	for(int i=0; i < 5; ++i)
	//check(i);
		cout<< " word "<< i << " =" << dictionary[i] <<endl;
        burstingRows();
        bursting();
	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets



	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("Word Shooter"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);

	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
