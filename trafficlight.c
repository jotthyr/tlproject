/*
 Plik trafficlight.c
*/
#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"

void UstawGlosnik(void);
static void lcd(unsigned long int a);

char str7[17]="--";

int main(void)
{
	/* Zmienna dla pêtli for w emergency mode */

	unsigned int i;
	unsigned int n;


	/* Napisy przechowujemy w tablicach */

	char str1[] = "CARS: RIDE";
	char str2[] = "PEDS: STOP";

	char str3[] = "CARS: STOP";
	char str4[] = "PEDS: WALK";

	char str5[] = "CARS: STOP: WAIT";
	char str6[] = "PEDS: STOP: WAIT";

	char str7[] = " Welcome ";
	char str8[] = "---------";
	char str9[] = "trafficlight";


	/* Funkcja inicjalizuje wyœwietlacz*/
	lcd_init();

	/* W³¹cza wyœwietlanie */
	LCD_DISPLAY(LCDDISPLAY);

	/* Czyci wyswietlacz */
	LCD_CLEAR;

	_delay_ms(500);

	/* Ustawienia glosnika */
	UstawGlosnik();

	DDRC  = 0xFC;
	DDRB  = 0x1F;
	PORTB = 0x11;
	PORTC = 0x03;

	LCD_LOCATE(0, 0);
	lcd_puts(str8);

	LCD_LOCATE(0, 1);
	lcd_puts(str9);
	_delay_ms(2500);

	LCD_CLEAR;

	LCD_LOCATE(0, 0);
	lcd_puts(str8);

	LCD_LOCATE(0, 1);
	lcd_puts(str7);

	_delay_ms(2500);

	LCD_CLEAR;


	while(1)
	{
	   /* --------LCD Stan Pocz¹tkowy-------*/
		LCD_LOCATE(0, 0);
		lcd_puts(str1);

		LCD_LOCATE(0, 1);
		lcd_puts(str2);

	   /* ---------Swiatla-----------------*/
		/* 5 diod  */

		PORTB = 0x11;
		PORTC = 0x03;

	   /* ---------Opoznienie-----------------*/
	        _delay_ms(300);


   /*-----------JESLI nacisniety przycisk trybu normalnego--------------*/

	  if(!(PINC & 0x01))
	    {
		 LCD_CLEAR;

	     /* --------LCD Stan Zmiany Œwiate³-------*/
		 LCD_LOCATE(0, 0);
		 lcd_puts(str3);

		 LCD_LOCATE(0, 1);
		 lcd_puts(str6);

		 PORTC = 0x03;

		 OCR1A = ICR1 - 2000;

		 _delay_ms(3000);
		 PORTB = 0x12;
		 _delay_ms(4000);
		 PORTB = 0x14;
		 _delay_ms(1000);
		 PORTB = 0x0C;

		/*ZIELONE PIESZEGO WLACZONE: odliczanie LCD, glosnik, miganie diody */
		/*---------------------------------------------------------------- */
	        for (n= 30;n >= 11; n--)
	         {

	        	LCD_CLEAR;

	        	LCD_LOCATE(0, 0);
	        	lcd_puts(str3);

	        	LCD_LOCATE(0, 1);
	        	lcd_puts(str4);
          
                       lcd(n);

                        PORTB = 0x2C;
	        	_delay_ms(500);

	        	PORTB = 0x0C;
	        	_delay_ms(500);

	        	

	         }

	        for (n= 10;n >= 1; n--)
	          {

	        	LCD_CLEAR;

	        	LCD_LOCATE(0, 0);
	        	lcd_puts(str3);

	        	LCD_LOCATE(0, 1);
	        	lcd_puts(str4);

	        	lcd(n);

	        	PORTB = 0x24;
	        	_delay_ms(250);

	        	PORTB = 0x04;
	        	_delay_ms(250);

	        	PORTB = 0x2C;
	        	_delay_ms(250);

	        	PORTB = 0x0C;
	            _delay_ms(250);


	         }


	        /*---------------------------------------------------------------- */


            /* Zmiana swiate³ po odliczaniu 30 sekund*/
	        LCD_CLEAR;

	        LCD_LOCATE(0, 0);
	        lcd_puts(str5);

	        LCD_LOCATE(0, 1);
	        lcd_puts(str2);

	        PORTB = 0x14;
	        _delay_ms(3000);

	        PORTB = 0x16;
	        _delay_ms(4000);
	        PORTB = 0x11;

	        LCD_CLEAR;

	        LCD_LOCATE(0, 0);
	        lcd_puts(str1);

	        LCD_LOCATE(0, 1);
	        lcd_puts(str2);

	        PORTC = 0x0C;



	     }


      if (!(PINC & 0x02))
	     {
		PORTC = 0x07;
		PORTB = 0x22;


		 LCD_CLEAR;

		 LCD_LOCATE(0, 0);
		 lcd_puts("!!! CAUTION !!!");

		 LCD_LOCATE(0, 1);
		 lcd_puts("TRAFIC LIGHT OFF");

		 _delay_ms(2000);

		while(1)
		     {

		      LCD_CLEAR;

		      LCD_LOCATE(0, 1);
		      lcd_puts("TRAFIC LIGHT OFF");

		      PORTB = 0x22;

		      for(i=100; i <= 4000; i++)
			     {
			     OCR1A = ICR1 - i;
			     _delay_us(400);
			     }

			 LCD_CLEAR;
			 LCD_LOCATE(0, 0);
			 lcd_puts("!!! CAUTION !!!");
			 LCD_LOCATE(0, 1);
			 lcd_puts("TRAFIC LIGHT OFF");

		         PORTB = 0x20;
		         _delay_ms(400);
		         PORTB = 0x22;

			 LCD_CLEAR;
			 LCD_LOCATE(0, 1);
			 lcd_puts("TRAFIC LIGHT OFF");


			 for(i=4000; i >= 100 ; i--)
		     	    {
		             OCR1A = ICR1 - i;
		     	     _delay_us(400);

			    }

			 PORTB = 0x20;

		 	 LCD_CLEAR;
			 LCD_LOCATE(0, 0);
		 	 lcd_puts("!!! CAUTION !!!");
			 LCD_LOCATE(0, 1);
			 lcd_puts("TRAFIC LIGHT OFF");

			 _delay_ms(400);


		     }
	       }
      _delay_ms(200);
	}
return 0;
}




/*--------------------------!!!FUNKCJE!!!------------------------------*/

static void lcd(unsigned long int a)
{

/* Funkcja aktualizuje zawartoœæ ekranu */

    signed char x;


/* Zamiana 32 bitowej liczby bez znaku na ci¹g znaków ASCII*/
  /*  x to liczba cyfr np 12 to x=2 lub 123 to x=3*/
    for(x=1; x>=0; a/=10 ,x--)
                str7[x] = a % 10 +'0';

/* Ustawia kursor w pierwszej kolumnie pierwszego wersza */
    LCD_LOCATE(12,0);

/* Wysy³a do wyœwietlacza ci¹g znaków z tablicy str1 */
    lcd_puts(str7);

    LCD_LOCATE(12,1);
    lcd_puts(str7);
}


void UstawGlosnik(void)
{
	DDRD |= 0xF0;
	TCCR1A |= 1<<WGM11 | 1<<COM1A1; // |(0<<COM0A0);
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	ICR1 = 5999;

}

/*---------------------------!!!FUNKCJE_LCD!!!-------------------------------*/

/* Zapis danej lub instrukcji */

void WriteToLCD (unsigned char v,unsigned char  rs)
{
    unsigned char bf;

    SET_OUT_LCD_D4;
    SET_OUT_LCD_D5;
    SET_OUT_LCD_D6;
    SET_OUT_LCD_D7;

    if(v&0x10) SET_LCD_D4; else CLR_LCD_D4;
    if(v&0x20) SET_LCD_D5; else CLR_LCD_D5;
    if(v&0x40) SET_LCD_D6; else CLR_LCD_D6;
    if(v&0x80) SET_LCD_D7; else CLR_LCD_D7;
 
    CLR_LCD_E;
    if(rs) SET_LCD_RS;else CLR_LCD_RS;
    CLR_LCD_RW;

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP; 
    CLR_LCD_E;
    LCD_NOP;
 
    if(v&0x01) SET_LCD_D4; else CLR_LCD_D4;
    if(v&0x02) SET_LCD_D5; else CLR_LCD_D5;
    if(v&0x04) SET_LCD_D6; else CLR_LCD_D6;
    if(v&0x08) SET_LCD_D7; else CLR_LCD_D7;
 
    LCD_NOP;
    SET_LCD_E;
    LCD_NOP; 
    CLR_LCD_E;
    LCD_NOP;
 
    SET_IN_LCD_D4;
    SET_IN_LCD_D5;
    SET_IN_LCD_D6;
    SET_IN_LCD_D7;

    CLR_LCD_RS;
    SET_LCD_RW;
    SET_LCD_D7;


/* Przyda³by siê pe³ny odczyt */
    do
    {
        LCD_NOP;
        SET_LCD_E;
        LCD_NOP;
        bf = IS_SET_LCD_D7;
        CLR_LCD_E;
        LCD_NOP;
        SET_LCD_E;
        LCD_NOP;
        LCD_NOP;
        CLR_LCD_E;
        
    }while( bf );
}


/*--------------------------------------------------------*/
/* Funkcja odczytuje adres i flage zajetosci */

unsigned char ReadAddressLCD ( void)
{
    unsigned char g = 0 ;

    CLR_LCD_RS;
    SET_LCD_RW; 

    SET_IN_LCD_D4;
    SET_IN_LCD_D5;
    SET_IN_LCD_D6;
    SET_IN_LCD_D7;

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP;

    if(IS_SET_LCD_D4) g+=16;
    if(IS_SET_LCD_D4) g+=32;
    if(IS_SET_LCD_D4) g+=64;
    if(IS_SET_LCD_D4) g+=128;
 
    CLR_LCD_E;
    LCD_NOP;
    SET_LCD_E;  
    LCD_NOP;
  
    if(IS_SET_LCD_D4) g+=8;
    if(IS_SET_LCD_D4) g+=4;
    if(IS_SET_LCD_D4) g+=2;
    if(IS_SET_LCD_D4) g+=1;
  
    CLR_LCD_E; 

    return  g ;
}


/*---------------------------------------------------------*/
/* Inicjalizacja wyœwietlacza */

void lcd_init(void)
{
    _delay_ms(31);    
   
    SET_OUT_LCD_RS;
    SET_OUT_LCD_RW;
    SET_OUT_LCD_E;
    SET_OUT_LCD_D4;
    SET_OUT_LCD_D5;
    SET_OUT_LCD_D6;
    SET_OUT_LCD_D7;

    CLR_LCD_E;
    CLR_LCD_RS;
    CLR_LCD_RW;
    SET_LCD_D4;
    SET_LCD_D5;
    CLR_LCD_D6;
    CLR_LCD_D7;        
  
    LCD_NOP;
    SET_LCD_E;
    LCD_NOP; 
    CLR_LCD_E;
    LCD_NOP;
    _delay_ms(10);

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP; 
    CLR_LCD_E;
    LCD_NOP;
    _delay_ms(2);

    LCD_NOP;
    SET_LCD_E;
    LCD_NOP; 
    CLR_LCD_E;
    LCD_NOP;
    _delay_ms(2);

    CLR_LCD_D4;
    LCD_NOP;
    SET_LCD_E;
    LCD_NOP; 
    CLR_LCD_E;
    LCD_NOP;
    _delay_us(80);

    WriteToLCD (0x28 , LCDCOMMAND) ;
    LCD_DISPLAY(0) ;
    LCD_CLEAR ;
    LCD_ENTRY_MODE(LCDINCREMENT) ;
}


/*--------------------------------------------------------*/
/* Wyswietla tekst na aktualnej pozycji kursora */

void lcd_puts(char *str)
{
    unsigned char i =0;

    while( str[i])
    LCD_WRITE_DATA(str[i++]);
}
