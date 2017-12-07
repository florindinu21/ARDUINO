//am mapat matricea cu
//0 unde e loc liber
//1 unde e obstacol
//2 unde urmeaza sa se construiasca obstacol
//3 nava inamica
//4 proiectil de la nava inamica
//5 nava noastra
#include <LedControl.h>
#include <LiquidCrystal.h>
const int rs = 1, en = 2, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
int Ox=0;           //joystick
int Oy=1;           //joystick
int pushs=0;       
int back_light =0;   //pinul A->LED ,conectat la DIG0
bool check_push_write=0; 
bool end_of_the_game=0;
int select;
int pull=8;          //joystick
int x;
int y;
int row_=0;          //pentru a umbla pe matrice
int col_=3;
byte mat[8][8];
int number_of_ships=0;
int ship_shoot_row=0;
int ship_shoot_col=0;
int score=0;
long obs_interval_generate=3300;
long space_ship_interval_generate=6200;
long ship_shoot_interval_generate =2000;
long obs_interval_fall=1700;
long start_time=0;
const long difficulty1_interval=60000;
const long difficulty2_interval=100000;
const long difficulty3_interval=140000;
unsigned long prev_millis_generate = 0;
unsigned long prev_millis_fall = 0;
unsigned long prev_millis_space_ship = 0;
unsigned long prev_millis_ship_shoot = 0;
 unsigned long current_millis_generate=0;
 unsigned long current_millis_fall=0;
unsigned long current_millis_space_ship_generate=0;
unsigned long current_millis_ship_shoot_generate=0;
unsigned long current_difficulty=0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LedControl lc=LedControl(12,11,10,1);
void init_matrix()
{
  for(int i=0;i<=7;i++)
   for(int j=0;j<=7;j++)
     mat[i][j]=0;
}
  
  void init_my_space_ship()
  { 
    lc.setLed(0,row_,col_,true); //mijlocul
    lc.setLed(0,row_,col_+1,true);
    lc.setLed(0,row_,col_-1,true);
    lc.setLed(0,row_+1,col_,true);
    mat[row_][col_]=5;             //pe matrice e 5 unde e nava noastra
    mat[row_][col_+1]=5;
    mat[row_][col_-1]=5;
    mat[row_+1][col_]=5;
  }

  void write_push(bool option)
  {
    lc.setLed(0,7,1,option);
    lc.setLed(0,6,1,option);
    lc.setLed(0,5,1,option);
    lc.setLed(0,4,1,option);
    lc.setLed(0,6,2,option);
    lc.setLed(0,6,3,option);
    lc.setLed(0,7,3,option);
    lc.setLed(0,7,5,option);
    lc.setLed(0,6,5,option);
    lc.setLed(0,5,5,option);
    lc.setLed(0,5,6,option);
    lc.setLed(0,5,7,option);     //sa scrie sau sa stearga "PUSH" de pe matrice
    lc.setLed(0,6,7,option);
    lc.setLed(0,7,7,option);
    lc.setLed(0,3,3,option);
    lc.setLed(0,3,2,option);
    lc.setLed(0,2,2,option);
    lc.setLed(0,1,3,option);
    lc.setLed(0,0,2,option);
    lc.setLed(0,0,3,option);
    lc.setLed(0,3,5,option);
    lc.setLed(0,2,5,option);
    lc.setLed(0,1,5,option);
    lc.setLed(0,0,5,option);
    lc.setLed(0,2,6,option);
    lc.setLed(0,3,7,option);
    lc.setLed(0,2,7,option);
    lc.setLed(0,1,7,option);
    lc.setLed(0,0,7,option);
    
    }
  
void move_right(int c,int r,int col_before=100)
{
  if((mat[r][c+1]==0 ||mat[r][c+1]==5) && mat[r+1][c]==0) //daca nu e obstacol
   { 
      lc.setLed(0,r,c,true);
      lc.setLed(0,r,c+1,true);
      lc.setLed(0,r,c-1,true);
      lc.setLed(0,r+1,c,true);
      lc.setLed(0,r,c-2,false); 
      lc.setLed(0,r+1,c-1,false);
      mat[r][c]=5;
      mat[r][c+1]=5;
      mat[r][c-1]=5;
      mat[r+1][c]=5;
      if(c-2>=0)
        mat[r][c-2]=0;
      mat[r+1][c-1]=0;
      if(col_before==0)           //daca a plecat din colt stanga
      { 
        lc.setLed(0,r+2,col_before,false);    //sterge ceva din fosta configurare
        mat[r+2][col_before]=0; 
       }
  }
  else end_game();

}
void move_right_and_shoot(int col,int row)
{
  if(col==1)     //daca a plecat din colt stanga
    {
      lc.setLed(0,row+2,0,false);  //sterge din fosta configuratie
      mat[row+2][0]=0;
     }
  move_right(col,row);
  shoot(col,row);
  }
 
void move_left(int c,int r,int col_before=100)
{
  if( (mat[r][c-1]==0 ||mat[r][c-1]==5) &&mat[r+1][c]==0) //daca nu e obstacol
  {
      lc.setLed(0,r,c,true);
      lc.setLed(0,r,c+1,true);
      lc.setLed(0,r,c-1,true);
      lc.setLed(0,r+1,c,true);
      lc.setLed(0,r,c+2,false); 
      lc.setLed(0,r+1,c+1,false);
      mat[r][c]=5;
      mat[r][c+1]=5;
      mat[r][c-1]=5;
      mat[r+1][c]=5;
      if(c+2<=7)
         mat[r][c+2]=0;
      mat[r+1][c+1]=0;
      if(col_before==7) //daca a plecat din coltul din  dreapta
        {
           lc.setLed(0,r+2,col_before,false);   //sterge ceva din fosta configuratie
           mat[r+2][col_before]=0; 
        } 
  }
  else end_game();
  
}
 void move_left_and_shoot(int col,int row)
{
  if(col==6)    //daca a plecat din coltul din dreapta
  {
    lc.setLed(0,row+2,7,false);   //sterge din fosta configuratie
    mat[row+2][7]=0;
  }
  move_left(col,row);
  shoot(col,row);
}
void right_corner(int c,int r)
{ 
  if(mat[r+2][c]==0 &&mat[r+1][c]==0) //daca nu e obstacol
  {
      lc.setLed(0,r+2,c,true);
      lc.setLed(0,r+1,c,true);
      lc.setLed(0,r,c,false);
      lc.setLed(0,r+1,c-1,false);
      mat[r+2][c]=5;
      mat[r+1][c]=5;
      mat[r][c]=0;
      mat[r+1][c-1]=0;
  }
  else
  end_game();
}
void left_corner(int c,int r)
{  
  if(mat[r+2][c]==0 && mat[r+1][c]==0) //daca nu e obstacol
   {
        lc.setLed(0,r+2,c,true);
        lc.setLed(0,r+1,c,true);
        lc.setLed(0,r,c,false);
        lc.setLed(0,r+1,c+1,false);
        mat[r+2][c]=5;
        mat[r+1][c]=5;
        mat[r][c]=0;
        mat[r+1][c+1]=0;
   }
   else end_game();



}
bool check(int row,int col) //se va verifica la fiecare loop daca intre timp nava a fost lovita 
{ 
  //verifica toate partile navei in functie de pozitia in care se afla
  if(col==0)
  { 
    if(mat[row+1][col]!=5 || mat[row+2][col]!=5 || mat[row][col+1]!=5 || mat[row][col+2]!=5)
       return 0;
       return 1;
  }
  if(col==7)
    {
    if(mat[row+1][col]!=5 || mat[row+2][col]!=5 || mat[row][col-1]!=5 || mat[row][col-2]!=5)
        return 0;
        return 1;
    }
   if(mat[row][col]!=5 ||mat[row][col-1]!=5 || mat[row][col+1]!=5 ||mat[row+1][col]!=5)
         return 0;
         return 1;
 }

 void destroy(int i1,int j1,int i2,int j2,int i3,int j3,int i4,int j4)
 {
      mat[i1][j1]=0;
      mat[i2][j2]=0;
      mat[i3][j3]=0;
      mat[i4][j4]=0;
      lc.setLed(0,i1,j1,false);
      lc.setLed(0,i2,j2,false);
      lc.setLed(0,i3,j3,false);
      lc.setLed(0,i4,j4,false);
  }

  void shoot(int c,int r)
  { 
  int i;
 
   if(mat[r+2][c]==0)  //daca nu se afla nimic
   {
      lc.setLed(0,r+2,c,true);
      delay(30);
   }
   else if(mat[r+2][c]==4)  //daca este proiectil de la nava inamica il distruge
    {
         lc.setLed(0,r+2,c,false);
         mat[r+2][c]=0;             
         ship_shoot_row=0;
         ship_shoot_col=0;
         return;
    }
   else return;
   i=r+3;
   while(i<=7)
   { 
       lc.setLed(0,i-1,c,false); //stinge pe precedentul
       if(mat[i][c]==1)    //daca se afla obstacol nu face nimic
       break;
       else if(mat[i][c]==4)    //daca e proiectil de la nava inamica il distruge
       {
          lc.setLed(0,i,c,false);
          mat[i][c]=0;
          ship_shoot_row=0;
          ship_shoot_col=0;
          break;
          
        }
      else if (mat[i][c]==3)    //daca e nava inamica,cauta pozitia ei si o distruge
      {
        if(mat[i+1][c]==3 && mat[i+1][c+1]==3 &&mat[i+1][c-1]==3)
         {
            destroy(i,c,i+1,c,i+1,c+1,i+1,c-1);
            number_of_ships--;
            score++;
            return;
         }
         else if(mat[i][c-1]==3 &&mat[i][c-2]==3 && mat[i-1][c-1]==3)
         {
             destroy(i,c,i,c-1,i,c-2,i-1,c-1);
             number_of_ships--;
             score++;
             return;
          }
          else if(mat[i][c+1]==3 &&mat[i][c+2]==3 && mat[i-1][c+1]==3)
          {
            destroy(i,c,i,c+1,i,c+2,i-1,c+1);
            number_of_ships--;
            score++;
            return;
           
           }
       }
     else    //daca nu e nimic in drum,se aprinde ledul
       {
           lc.setLed(0,i,c,true);
           delay(30);
           i++;
        }
    
   }
   lc.setLed(0,i-1,c,false); 
   
 }


 void diagonal_1_shoot()
{
   int col=6;
   int row=1;
   if(mat[row][col]==1)    //daca e obstacol il distruge
   {
      destroy(row,col-1,row,col,row+1,col,row+1,col-1);
      lc.setLed(0,row-1,col+1,false);
      score++;
      return;
   }
  else if(mat[row][col]==4)   //daca e proiectil de la nava inamica il distruge
  {
      lc.setLed(0,row,col,false);
      mat[row][col]=0;
      ship_shoot_row=0;
      ship_shoot_col=0;
      lc.setLed(0,row-1,col+1,false);
      return;
   }
    lc.setLed(0,row,col,true);   //daca nu e nimic,se aprinde primul led
    delay(30);
    while( (col--)!=0 && (row++)!=7)
     {
      {
       if(mat[row][col]==1)   //daca gaseste obstacol ii cauta pozitia si il distruge
                              //e posibilitatea ca sa fie 2 obstacole unul langa altul asa ca am pus conditii
                              //sa nu se stearga cumva mijlocul dintre ele (mijlocul sa nu fie vazut ca un obstacol) si sa nu lase bucati incorecte
         {  
           if(mat[row][col+1]==1 &&mat[row+1][col]==1 &&mat[row+1][col+1]==1)
             { 
                  if( mat[row-1][col]==1 &&mat[row-1][col+1]==1)
                  {
                    destroy(row,col+1,row,col,row-1,col,row-1,col+1);
                    lc.setLed(0,row-1,col+1,false);
                    score++;
                    return;
                    
                   }
                   else if(mat[row][col-1]==1 && mat[row+1][col-1]==1 && mat[row][col-2]!=1 &&mat[row+1][col-2]!=1)
                   {
                      destroy(row,col-1,row,col,row+1,col,row+1,col-1);
                     lc.setLed(0,row-1,col+1,false);
                     score++;
                     return;
                     
                   }
                    else 
                   {
                    destroy(row,col+1,row,col,row+1,col,row+1,col+1);
                    lc.setLed(0,row-1,col+1,false);
                    score++;
                    return; 
                    
                    }
                }
    
    
    
            else if(mat[row][col-1]==1 &&mat[row+1][col-1]==1 &&mat[row+1][col]==1)
            {
              if(mat[row+2][col]==1 && mat[row+2][col-1]==1 && mat[row-1][col]==1 &&mat[row-1][col-1]==1)
                {  
                  destroy(row,col-1,row,col,row-1,col,row-1,col-1); 
                  lc.setLed(0,row-1,col+1,false);
                  score++;
                  return;
                  
                }
              else if (mat[row][col+1]==1 &&mat[row+1][col+1]==1)
              { 
                destroy(row,col+1,row,col,row+1,col,row+1,col+1);
                lc.setLed(0,row-1,col+1,false);
                score++;
                return;
                
              }
             else
              {
                destroy(row,col-1,row,col,row+1,col,row+1,col-1);
                lc.setLed(0,row-1,col+1,false);
                score++;
                return;
                
               }
            }
      
     
            else if(mat[row-1][col]==1 &&mat[row-1][col-1]==1 &&mat[row][col-1]==1)
            { 
                if(mat[row+1][col]==1 &&mat[row+1][col-1]==1)
                {
                 
                  destroy(row,col-1,row+1,col,row+1,col-1,row,col);
                  lc.setLed(0,row-1,col+1,false);
                  score++;
                  return;
                    
                }
                
                else
                { 
                  destroy(row,col-1,row-1,col,row-1,col-1,row,col);
                  lc.setLed(0,row-1,col+1,false);
                  score++;
                  return;
                  
                 }
             }
        }
   else if(mat[row][col]==4)  //daca e proiectil de la nava inamica il distruge
   {
        lc.setLed(0,row,col,false);
        mat[row][col]=0;
        ship_shoot_row=0;
        ship_shoot_col=0;
        lc.setLed(0,row-1,col+1,false);
        return;
        
    }
    else if(mat[row][col]==3)  //daca e nava inamica o distruge
    {
        destroy(row,col,row+1,col,row+1,col+1,row+1,col-1);
        number_of_ships--;
        lc.setLed(0,row-1,col+1,false);
        score++;
        return;
        
     }
    else  //daca nu e nimic,se aprinde ledul si se stinge cel din urma
    {
        lc.setLed(0,row,col,true);
        lc.setLed(0,row-1,col+1,false);
        delay(30);
     }
      
  }
  
 }
 
 lc.setLed(0,7,0,false);
}
 void diagonal_2_shoot()
 {
  int i=1;
  if(mat[i][i]==1)  //daca e obstacol il elimina
   {
      destroy(i,i+1,i+1,i,i+1,i+1,i,i);
      score++;
      return;
   }
   
   else if(mat[i][i]==4)   //daca e proiectil inamic il distruge
   {
       lc.setLed(0,i,i,false);
       mat[i][i]=0;
       ship_shoot_row=0;
       ship_shoot_col=0;
       return;
    }
    
  lc.setLed(0,i,i,true);
  delay(30);
  for(i=2;i<=7;i++)
  {
    { 
      if(mat[i][i]==1)        //daca gaseste obstacol ii cauta pozitia si il distruge
                              //e posibilitatea ca sa fie 2 obstacole unul langa altul asa ca am pus conditii
                              //sa nu se stearga cumva mijlocul dintre ele (mijlocul sa nu fie vazut ca un obstacol) si sa nu lase bucati incorecte
       {
        if(mat[i][i+1]==1 &&mat[i+1][i+1]==1 &&mat[i+1][i]==1 )
        {
          if(mat[i][i+2]==1 && mat[i+1][i+2]==1 && mat[i][i-1]!=0 && mat[i+1][i-1]!=0)
        
            { 
              destroy(i,i-1,i,i,i+1,i-1,i+1,i);
              lc.setLed(0,i-1,i-1,false);
              score++;
              return;
              
            }
          else if(mat[i-1][i]==1 && mat[i-1][i+1]==1 && mat[i-2][i]!=1 && mat[i-2][i+1]!=1)
            {
               destroy(i,i+1,i,i,i-1,i+1,i-1,i);
               lc.setLed(0,i-1,i-1,false);
               score++;
               return;
               
             }
          else 
            {
              destroy(i,i+1,i,i,i+1,i,i+1,i+1);
              lc.setLed(0,i-1,i-1,false);
              score++;
              return;
              
             }
        }
       else if(mat[i][i+1]==1 &&mat[i-1][i]==1 &&mat[i-1][i+1]==1)
          { 
            if(mat[i+1][i]==1 && mat[i+1][i+1]==1)   
              { 
                destroy(i,i+1,i,i,i+1,i,i+1,i+1);
                lc.setLed(0,i-1,i-1,false);
                score++;
                return;
                
               }
            else
              { 
                destroy(i,i+1,i,i,i-1,i,i-1,i+1);
                lc.setLed(0,i-1,i-1,false);
                score++;
                return;
                
               }
          }
        
      else if(mat[i][i-1]==1 &&mat[i+1][i]==1 &&mat[i+1][i-1]==1)
         { 
            if(mat[i][i+1]==1 && mat[i+1][i+1]==1 )
          
              {
                destroy(i,i+1,i,i,i+1,i,i+1,i+1);
                lc.setLed(0,i-1,i-1,false);
                score++;
                return;
              }
           else 
              {
                destroy(i,i-1,i,i,i+1,i,i+1,i-1); 
                lc.setLed(0,i-1,i-1,false);
                score++;
                return;
              }
           
        }
     }
  else if(mat[i][i]==4)  //daca e proiectil inamic il distruge
     {
        lc.setLed(0,i,i,false);
        mat[i][i]=0;
        ship_shoot_row=0;
        ship_shoot_col=0;
        lc.setLed(0,i-1,i-1,false);
        return;
        
      }
      else if(mat[i][i]==3)   //daca e nava inamica o distruge
      {
        destroy(i,i,i+1,i,i+1,i+1,i+1,i-1);
        lc.setLed(0,i-1,i-1,false);
        number_of_ships--;
        score++;
        return;
        
       }
    else    //daca nu e nimic,aprinde led-ul si il stinge pe cel din urma
      {
        lc.setLed(0,i,i,true);
        lc.setLed(0,i-1,i-1,false);
        delay(30);
     }
      
   }
   
  }
  
  lc.setLed(0,i-1,i-1,false);
    
 
 }
 void generate_space_ship()
 {
    int i;
    for( i=1;i<=6;i++)  //se verifica daca exista loc disponibil pentru generat
      {  
        if(mat[7][i]==0 && mat[7][i-1]==0 &&mat[7][i+1]==0 && mat[6][i]==0)
        break;
      }
    if(i<7)  //daca da,o genereaza si o mapeaza pe matrice cu 3
      {
       int random_number=random(1,7);
       while(mat[7][random_number]==1 ||mat[7][random_number+1]==1 ||mat[7][random_number-1]==1 || mat[6][random_number]==1 )
         random_number=random(1,7);
       lc.setLed(0,7,random_number,true);
       lc.setLed(0,7,random_number+1,true);
       lc.setLed(0,7,random_number-1,true);
       lc.setLed(0,6,random_number,true);
       mat[7][random_number]=3;
       mat[7][random_number+1]= 3;
       mat[7][random_number-1]=3;
       mat[6][random_number]=3;
       number_of_ships++;
       }
 }
 void space_ship_shoot()
 { 
  if(ship_shoot_row==0 && ship_shoot_col==0) //daca nu exista alt proiectil lansat,lanseaza unul
 
    {
      for(int i=1;i<=6;i++)
       if(mat[7][i]==3 && mat[7][i-1]==3 && mat[7][i+1]==3 && mat[6][i]==3 && mat[5][i]==0) //se cauta nava pentru a incepe sa cada din varful ei
        {
           ship_shoot_row=5;
           ship_shoot_col=i;
           lc.setLed(0,5,i,true);
           mat[5][i]=4;
           return;
      
        }
     
    }
  else if (mat[ship_shoot_row-1][ship_shoot_col]!=1 && ship_shoot_row>0)  //daca nu e nimic in cale,avanseaza proiectilul
     {
        ship_shoot_row--;
        lc.setLed(0,ship_shoot_row,ship_shoot_col,true);
        mat[ship_shoot_row][ship_shoot_col]=4;              
        lc.setLed(0,ship_shoot_row+1,ship_shoot_col,false);
        mat[ship_shoot_row+1][ship_shoot_col]=0;
      
     }
    else    //daca a intalnit ceva,se reseteaza
     {
        lc.setLed(0,ship_shoot_row,ship_shoot_col,false);
        mat[ship_shoot_row][ship_shoot_col]=0;
        ship_shoot_row=0;
        ship_shoot_col=0;
    }
 
 }
 
void generate_obs()
{
   int i;
   for(i=0;i<=6;i++)  //se verifica daca exista loc disponibil pentru generare proiectil
    if(mat[6][i]==0 && mat[6][i+1]==0 && mat[7][i]==0 &&mat[7][i+1]==0)   //sa verifice daca exista spatiu disponibil
     break;
   if(i!=7) //daca da,se mapeaza pe matrice cu 1
   {
      int random_number=random(7);
      while(mat[7][random_number]!=0 ||mat[7][random_number+1]!=0 ||mat[6][random_number]!=0 || mat[6][random_number+1]!=0  ) 
      random_number=random(7);
      lc.setLed(0,7,random_number,true);
      lc.setLed(0,7,random_number+1,true);
      lc.setLed(0,6,random_number,true);
      lc.setLed(0,6,random_number+1,true);
      mat[7][random_number]=1;
      mat[6][random_number]= 1;
      mat[7][random_number+1]=1;
      mat[6][random_number+1]=1;
      
   }
  
 }
 void fall()
 {
  int i,j;
  for(i=7;i>=0;i--)
   { 
    for(j=0;j<=7;j++)
      if(mat[i][j]==1&& mat[i][j+1]==1 && mat[i-1][j]==1&& mat[i-1][j+1]==1 &&mat[i-2][j]!=1 &&mat[i-2][j]!=4 && mat[i-2][j+1]!=1 &&mat[i-2][j+1]!=4 && i>1  )
       {  //daca a gasit proiectil (4 patratele de 1 alaturate),iar pe urmatoarea linie este gol,avanseaza
          //cu 2 e notata partea care urmeaza a fi aprinsa
        lc.setLed(0,i,j,false);
        lc.setLed(0,i,j+1,false);
        lc.setLed(0,i-2,j,true);
        lc.setLed(0,i-2,j+1,true);
        mat[i][j]=0;
        mat[i][j+1]=0;
        mat[i-2][j]=2;
        mat[i-2][j+1]=2;
       }
     else if(mat[i][j]==2)
        mat[i][j]=1;
   
     else if(mat[i][j]==1&& mat[i][j+1]==1 && mat[i-1][j]==1&& mat[i-1][j+1]==1 && i==1)
      { //daca e obstacol pe ultima linie,sterge tot obstacolul
        mat[i][j]=0;
        mat[i][j+1]=0;
        mat[i-1][j]=0;
        mat[i-1][j+1]=0;
        lc.setLed(0,i,j,false);
        lc.setLed(0,i,j+1,false);
        lc.setLed(0,i-1,j,false);
        lc.setLed(0,i-1,j+1,false);
        
       }
       
    }
           
  }
  void end_game()
  { 
      pushs=0;
      end_of_the_game=1;
      check_push_write=1;
      bool ok=0;
      lc.shutdown(0,false);// turn off power saving, enables display
      lc.setIntensity(0,8);// sets brightness (0~15 possible values)
      lc.clearDisplay(0);// clear screen
      if(ok==0)
       {
         for (int row=0; row<8; row++)
           {
             for (int col=0; col<8; col++)
               {
                lc.setLed(0,col,row,true); // turns on LED at col, row
                delay(25);
                }
            }
 
          for (int row=0; row<8; row++)
          {
            for (int col=0; col<8; col++)
               {
                lc.setLed(0,col,row,false); // turns off LED at col, row
                delay(25);
               }
           }
            ok++;
          }   
      write_the_score();

  
  }
  void clear_all()
  {
      lc.shutdown(0,false);// turn off power saving, enables display
      lc.setIntensity(0,8);// sets brightness (0~15 possible values)
      lc.clearDisplay(0);// clear 
      digitalWrite(back_light, LOW);
      lcd.clear();
  }
  void difficulty3() 
  {  
     obs_interval_generate=1400;
     obs_interval_fall=800;
     space_ship_interval_generate=4200;
     ship_shoot_interval_generate =1000;
   }
  
   void difficulty2()  
  {
      obs_interval_generate=1800;
      obs_interval_fall=1100;
      space_ship_interval_generate=4500;
      ship_shoot_interval_generate =1200;
  
  }
   void difficulty1()   
  { 
      obs_interval_generate=2500;
      space_ship_interval_generate=5000;
      obs_interval_fall=1300;
      
  }

 
  void initiate_game()
  {
      write_push(false);
      check_push_write=0;
      init_my_space_ship();
      start_time=millis();
      digitalWrite(back_light, HIGH);
  }

  void write_the_score()
  {
   
    lcd.print("Score: ");
    lcd.print(score);
  }


void setup() 
{
  init_matrix();
  pinMode(9, OUTPUT);
  analogWrite(9,100);
  lcd.begin(16,2);
  lc.shutdown(0,false);// turn off power saving, enables display
  lc.setIntensity(0,8);// sets brightness (0~15 possible values)
  lc.clearDisplay(0);// clear
  pinMode(Ox,INPUT);
  pinMode(Oy,INPUT);
  digitalWrite(pull,HIGH);
  randomSeed(analogRead(13));
  pinMode(back_light, OUTPUT);

}


void loop() 
{
  select=digitalRead(pull);
  if(select==LOW) //daca se apasa
  {
      pushs++;
      check_push_write=1;
  }
  if (check_push_write==0 &&pushs==0) //daca inca nu s-a apasat pe JoyStick scrie "PUSH" pe matrice
      write_push(true);
  else if(pushs==1 && check_push_write==1 &&end_of_the_game==0) //daca s-a apasat pe buton si nu s-a terminat jocul se initializeaza
      initiate_game();
  
  if(pushs>=1 &&end_of_the_game==0)
  {  
     current_millis_generate=millis();    //generare obstacol
     current_millis_fall=millis();        //cadere obstacol
     current_millis_space_ship_generate=millis();   //generare nava inamica
     current_millis_ship_shoot_generate=millis();   //avansare proiectil
     current_difficulty=millis();         //dificultatea
   
      if(current_difficulty-start_time>=difficulty3_interval)

       difficulty3();
     else if(current_difficulty-start_time>=difficulty2_interval)
     
       difficulty2();
       
     else if(current_difficulty-start_time>=difficulty1_interval)
     
       difficulty1();
 
     if(current_millis_generate-prev_millis_generate>=obs_interval_generate) 
     {
        prev_millis_generate= current_millis_generate;
        generate_obs();
     }
     if(current_millis_fall-prev_millis_fall>=obs_interval_fall)
     {
        prev_millis_fall=current_millis_fall;
        fall();
     }
  
     if(current_millis_space_ship_generate-prev_millis_space_ship>=space_ship_interval_generate && number_of_ships==0)
     {
        prev_millis_space_ship=current_millis_space_ship_generate;
        generate_space_ship();
   
     }
    if(current_millis_ship_shoot_generate-prev_millis_ship_shoot>=ship_shoot_interval_generate)
     {
        prev_millis_ship_shoot=current_millis_ship_shoot_generate;
        space_ship_shoot();
        
     }
    if(check(row_,col_)==1 ) //daca inca nu a fost lovita nava,se continua jocul
     {
        x=analogRead(Ox);
        y=analogRead(Oy);
        int col_before=col_;
        if(x>700 && col_-1>-1)  //se modifica col_ in functie de cum miscam joystickul sau ramane la fel daca nu miscam
           col_--;
        else if(x<300 && col_+1<8)
           col_++;
        else col_=col_before;
        
        if(y<300 &&col_==col_before && col_!=7 &&col_!=0)
           shoot(col_,row_);
       else if(y<300 &&col_>col_before &&col_!=7)
           {
  
            move_right_and_shoot(col_,row_);
            
           }

        else if(y<300 &&col_<col_before &&col_!=0)
           {  
            
            move_left_and_shoot(col_,row_);
            
           }

        else if(col_==7 && y<300)
           {
            if(col_>col_before)
                right_corner(col_,row_);
            diagonal_1_shoot();
           }
        else if(col_==0 && y<300)
           {
             if(col_<col_before)
                left_corner(col_,row_);
             diagonal_2_shoot();

            }
 
        else if(col_+1==8)
           {  
            if(col_!=col_before) 
               right_corner(col_,row_);
 
           }
        else if (col_-1 ==-1)
           {
             if(col_!=col_before) 
                left_corner(col_,row_);
           }
        else if(col_>col_before)
           {
             move_right(col_,row_,col_before);

           }
        else if(col_<col_before)
           {
              move_left(col_,row_,col_before);
              
           }
  
       delay(150);
     }
   else end_game();
 }
 else if (end_of_the_game==1 && pushs>=1)
  {
    clear_all();
    
  }
}

  



