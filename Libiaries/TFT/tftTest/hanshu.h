int spd=60;
int l1=A1;
int l2=A2;
Servo ss;

void  left(int x)
{ 
       
       SetMotor(1,-spd,0);
       SetMotor(6,spd,0);  
       delay(x);
       SetMotor(1,0,1);
       SetMotor(6,0,1);  
     
}

void  right(int y)
{ 
  
       SetMotor(1,spd,0);
       SetMotor(6,-spd,0);  
       delay(y);
       SetMotor(1,0,1);
       SetMotor(6,0,1);  
}
void line(int z)
{ 
  
       SetMotor(1,spd,0);
       SetMotor(6,spd,0);  
       delay(z);
       SetMotor(1,0,1);
       SetMotor(6,0,1);  
}

void  back(int q)
{ 
  
       SetMotor(1,-spd,0);
       SetMotor(6,-spd,0);  
       delay(q);
       SetMotor(1,0,1);
       SetMotor(6,0,1);  

}
void  duoji1()
{
    ss.write(0);
    delay(500); 
 }
void  duoji2()
{
    ss.write(180);
    delay(500); 
 }
 void xun()
{
while(1)
    {
      if(analogRead(l1)>600&&analogRead(l2)>600)
     {
       SetMotor(1,spd,0);
       SetMotor(6,spd,0);  
     }
     else
     {
       if(analogRead(l1)<600)
       {
         SetMotor(1,30,0);
         SetMotor(6,spd,0);  
       }
       else
       {
         SetMotor(6,30,0);
         SetMotor(1,spd,0);  
       }
       
    }
    if(analogRead(l1)<600&&analogRead(l2)<600)
      {break;}
    }
       SetMotor(1,0,1);
       SetMotor(6,0,1); 
 }
void xian()
{
    while(analogRead(l1)>600&&analogRead(l2)>600)
    {
       SetMotor(1,spd,0);
       SetMotor(6,spd,0);      
    }
    SetMotor(1,0,1);
    SetMotor(6,0,1); 
    if(analogRead(l1)<600)
    {
       while(analogRead(l2)>600)
       {
          SetMotor(6,30,0);  
       }
       SetMotor(6,0,1); 
    }
    else
    {
      while(analogRead(l1)>600)
     {
        SetMotor(1,30,0);   
     }
     SetMotor(1,0,1); 
    }
}


