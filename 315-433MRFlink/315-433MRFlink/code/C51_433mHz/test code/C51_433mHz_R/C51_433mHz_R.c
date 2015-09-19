
//Function: This procedure applies to 433mHz modules of receiving
//Platform:STC89C58RD+
//Time:August 30,2012
#include <reg51.h>   
#include <intrins.h>   
   
typedef unsigned char uchar;   
typedef unsigned int uint;   
sbit rs = P2^7;     //LCD1602 pins   
sbit rw = P2^6;   
sbit ep = P2^5;
   
sbit input = P1^0;  //Received signal pin

uint xT=12345;   
uchar k; 

uchar nlcd[]={'0','1','2','3','4','5','6','7','8','9'};     
uchar dis1[] = "welcome to us    ";   
uchar dis2[] = "???              ";   

void delay(uchar ms)   
{                           // 1ms
    uchar i;   
    while(ms--)   
    {   
        for(i = 0; i<123; i++);   
    }   
}   
   
void uDelay()  //3us 
{   
    _nop_();   
    _nop_();   
    _nop_();   
}   
   
bit lcd_bz()   
{                           // Test LCD busy status  
    bit result;   
    rs = 0;   
    rw = 1;   
    ep = 1;   
    uDelay();   
    result = (bit)(P0 & 0x80);   
    ep = 0;   
    return result;     
}   
   
lcd_wcmd(uchar cmd)   
{                           // Write instruction to LCD
    while(lcd_bz());   
    rs = 0;   
    rw = 0;   
    ep = 0;   
    uDelay();      
    P0 = cmd;   
    uDelay();   
    ep = 1;   
    uDelay();   
    ep = 0;        
}   
   
lcd_pos(uchar pos)   
{                           //Setting the display position 
    lcd_wcmd(pos | 0x80);   
}   
   
lcd_wdat(uchar dat)    
{                           //Write characters to the LCD display data
    while(lcd_bz());   
    rs = 1;   
    rw = 0;   
    ep = 0;   
    P0 = dat;   
    uDelay();   
    ep = 1;   
    uDelay();   
    ep = 0;    
}   
   
lcd_init()   
{                           //LCD initialization
    lcd_wcmd(0x38);         //   
    delay(1);   
    lcd_wcmd(0x0c);         //   
    delay(1);   
    lcd_wcmd(0x06);         //   
    delay(1);   
    lcd_wcmd(0x01);         //
    delay(1);   
}   
   
Number(uint x)  //display data
{   
    uchar i=0;   
    uchar temp[6];   
    if(x>0)   
    {   while(x)           
        {   
            temp[i]=x%10;   
            x=x/10;   
            i++;   
        }   
        lcd_pos(0x40);   
        while(i--)   
        {   
            lcd_wdat(nlcd[temp[i]]);   
        }   
        lcd_wdat(' ');   
        uDelay();   
        lcd_wdat(' ');   
        uDelay();   
        lcd_wdat(' ');   
    }   
}   
   
void Delay_uS(uchar in_data)   //1us
{   
    uchar i;   
    for(i=in_data;i>0;i--);   
}   

uchar processLOW(void)  //Low pulse length = (i * 10 + 27) us  
{   
     uchar i;   
     while(input); //Waiting for the arrival of the low-level 
     Delay_uS(2);  //2us 
     while(input);  // Waiting for the arrival of the low-level
     while(!input)   
     {     
        i++;   
        if(i == 106) return 0; //If the pulse is too long, indicating that the interfering signal pulse is generated.  
        _nop_();  _nop_();   
     }   
     return i;   
}   
  
uchar process(void)  //The length of the high pulse = (i * 10 + 130) us  
{   
     uchar i;   
     while(!input);   
     Delay_uS(15);   
     if(!input) return 0;    
     i=0;   
     while(input)   
     {     
        i++;   
        if(i == 87) return 0;    
        _nop_();  _nop_();   
     }   
     return i;   
}   
   
 
uchar receive(void)//recive data   
{   
    uchar in_data=0;   
    uchar j,i;   
    j=processLOW();   
    if(j>72)//Judged low level is valid 
    {      
        for(i=0;i<8;i++)   
        {   
            j=process();   
            in_data<<=1;   
            if(j>53)     //Measuring the level of "1", then add 1. 
                in_data=in_data|1;   
            else if(j>19 && j<46)//Low effective length of between 20-45.
                in_data=in_data|0;   
            else   
                return 0;   
         }   
     }   
     else   
        return 0;    
     return(in_data);   
}   
   
main()   
{   
    uchar i;   
    lcd_init();             // initialization LCD1602              
    delay(10);   
    lcd_pos(0); // Set the display position of the character 0 of the first row of the first  
    while(dis1[i] != '\0')   
    {                       //   
        lcd_wdat(dis1[i]);   
        i++;   
    }      
    while(1)   
    {            
        input=1;   
        if(input)   
        {             
            xT=receive();   
            Number(xT);    
        }     
    }   
   
}    
