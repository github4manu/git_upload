
//Function: This procedure applies to 433mHz module launch
//Platform:STC89C58RD+
//Time:August 30,2012
#include <reg51.h>   
#include <intrins.h>   
   
typedef unsigned char uchar;   
typedef unsigned int uint;   
   
sbit out = P1^0; //Transmitted signal pin    
       
 
void mDelay(uchar ms) // 1ms  
{                          
    uchar i;   
    while(ms--)   
    {   
        for(i = 0; i<123; i++);   
    }   
}   
   
 
void Delay(uchar in_data)  //1us   
{   
    uchar i;   
    for(i=in_data;i>0;i--);   
}     
void Pulse(uchar in_data)   
{   
    uchar i,j;   
    out=0;   
    for(i=in_data;i>0;i--)   //Generates a pulse ,Half cycle! ! ! !
    {      
        out=~out;   
        for(j=0;j<6;j++);   
    }   
}   
 
  
void outbit(bit i_bit)  //one bit output
{   
   
    if(i_bit)   
    {   
        Pulse(24);  //send '1' 
        out=0;   
        Delay(40);   
    }   
    else   
    {   
        Pulse(12);  //send '0' 
        out=0;   
        Delay(80);   
    }   
       
}   
   
void send(uchar in_data)   //send a data with 8 bits
{   
    uchar i;   
    bit DI;   
      
    Pulse(40);//start   
    out=0;          
    Delay(120);    //433mHz transmitter is activated
    for(i=0;i<8;i++)//send data   
    {   
         DI=(bit)(in_data&0x80);   
         outbit(DI);   
         in_data<<=1;   
    }   
}   
main()     
{   
    uchar i,j;   
    while(1)   
    {   
        out=0;   
        for(i=100;i<250;i++)   
        {      
            mDelay(250);   
            mDelay(100);   
			mDelay(250); 
			mDelay(250); 

            for(j=0;j<10;j++)   
                send(0);   
            send(i);   
        }   
    }      
}  