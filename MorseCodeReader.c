#include <mega128.h>
#include <delay.h>
#include <string.h>
#include <stdio.h>

#define LCD_DATA		PORTA
// #define LCD_DATA		PORTC

#define LCD_RS		        PORTD.0  
#define LCD_RW_L		PORTD.1 
#define LCD_ENABLE		PORTD.2       
#define LCD_DELAY		1   //ms
#define LCD_DELAY2		20   //ms  

void LCD_INIT();       //TEXTLCD 초기화 과정, lcd내의 작은컨트롤러 초기화
void LCD_DISP_STRING(unsigned char *char_array, unsigned char *char_array2); //어떤스트링 값을 텍스트 LCD에 찍기* 함
void test_output();
void string_output_segment(char *);
char seg_pat[16] = {
 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07
 ,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

char test[4] = {0x78,0x79,0x6d,0x78};
char in [4] = {0x00,0x00,0x60,0x54};
char out[4] = {0x00,0x5C,0x1C,0x78};
char del[4] = {0x00,0x5E,0x79,0x38};

//unsigned char LCD_Line[33];
unsigned char LCD_Line1[17];
unsigned char LCD_Line2[17];

unsigned char led = 0xFE;
int pwm = 0x2000, cnt = 0;

void main(void){
	
	DDRA = 0xFF;
        DDRD = 0xFF; 
	DDRC = 0xFF; // led
	//DDRE = 0x00; // sw
	DDRB = 0xFF; //7-segment
	DDRF = 0xF0; //4~1 segment

        delay_ms(5);                    
        
        LCD_INIT();
	test_output();
	
}

void test_output(){
	int i;
          
	sprintf(LCD_Line1, "MorseCodeReader");
	sprintf(LCD_Line2, "test");
	LCD_DISP_STRING(LCD_Line1, LCD_Line2);
        
	
	TCCR1A = 0b00001011; //set oc1c
	TCCR1B = 0b00010100;
	TCCR1C = 0x0;
	TCNT1 = 0x0000;
	OCR1A = 0x4000;
	OCR1CH = (pwm & 0xFF00) >> 8;
	OCR1CL = pwm & 0x0FF;  
	                   
	TIMSK = 0x01;
	TCCR0 = 0x07;
	TCNT0 = 0x00;    
	
	SREG = 0x80;  	   
}               

void string_output_segment(char *string){
	int f, b=0;
	char fnum;

	fnum = 0b10000000;	

	for(f = 0; f < 4; f++){
		PORTF = fnum;
		PORTB = string[b];  
		delay_ms(5);
		fnum >>= 1;
		b++;       
		PORTB = 0x00;
	}
}
                   
interrupt [TIM0_OVF] void timer_comp0(void){ 
        cnt++;
        string_output_segment(test);           
        if(cnt > 780){//세 번 움직임.    
                 PORTC = 0xFF;
                 TIMSK = 0x00;
                 TCCR1A = 0x00; 
                 TCCR1B = 0x00;
                 sprintf(LCD_Line1, "");
        	 sprintf(LCD_Line2, "");
	         LCD_DISP_STRING(LCD_Line1, LCD_Line2);  
        }
        else if((cnt %32)== 31){
                 if(led == 0x7F){ 
                         PORTC = led;
                         led = 0xFE;
                 }    
                 else{
                         PORTC = led;  
                         led <<= 1;
                         led |= 0x01;                  
                 }
                 
        }        
}

void LCD_INIT() // 그대로 배껴서 쓰면 됨
{

            //int i;
 	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  LOW
        LCD_RS=0;       //    pokeb( LCD_RS, 0x00);    // INSTRUCTION
            delay_ms(LCD_DELAY);
	LCD_RW_L=0;     //    pokeb( LCD_RW_L, 0x00);    // WRITE

	    delay_ms(LCD_DELAY2);

	LCD_DATA= 0x38; //    pokeb(LCD_DATA, 0x38);   // SET FUNCTION   8bit Interface,2Line Display,5_7 DOT
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	      delay_ms(LCD_DELAY2);

 //	 pokeb( BUF4, 0x04);

	LCD_DATA= 0x0F; //    pokeb(LCD_DATA, 0x0F);   // DISPLAY   ON
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	      delay_ms(LCD_DELAY);

 //	 pokeb( BUF4, 0x05);

	LCD_DATA= 0x01; //    pokeb(LCD_DATA, 0x01);   // DISPLAY   CLEAR
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	      delay_ms(LCD_DELAY);


 //	 pokeb( BUF4, 0x06);

	LCD_DATA= 0x02; //    pokeb(LCD_DATA, 0x02);   // CURSOR   RETURN HOME
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	      delay_ms(LCD_DELAY);


 //	 pokeb( BUF4, 0x07);

	LCD_DATA= 0x06; //    pokeb(LCD_DATA, 0x06);   // ENTRY MODE CURSOR MOVE INCREASE,  SHIFT
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	      delay_ms(LCD_DELAY);


 //	 pokeb( BUF4, 0x08);

}


void LCD_DISP_STRING(unsigned char *char_array1, unsigned char *char_array2) // 
{
int i;
 unsigned char tmp_c_array[32];

        LCD_RS=0;       //    pokeb( LCD_RS, 0x00);    // INSTRUCTION
	LCD_RW_L=0;     //    pokeb( LCD_RW_L, 0x00);    // WRITE

 //	    	   delay_ms(LCD_DELAY2);
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);

//	    pokeb(LCD_DATA, 0x01);   // DISPLAY   CLEAR
//	    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
//	    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
//	    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
 //	      delay_ms(LCD_DELAY);


 //	 pokeb( BUF4, 0x06);

	LCD_DATA= 0x02;   // CURSOR   RETURN HOME
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	      delay_ms(2);


  	LCD_DATA= 0x80;   // SET DD RAM ADDRESS 첫번째줄  줄 첫번째 위치
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	      delay_ms(LCD_DELAY);



        LCD_RS=1;       //    pokeb( LCD_RS, 0x01);    // DATA
	LCD_RW_L=0;     //    pokeb( LCD_RW_L, 0x00);    // WRITE

	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);



  	   for  (i=0; i<= 0x1F; i++) {
//	   tmp_c_array[i]= ' ';
	   tmp_c_array[i]= 0x20;
   	   tmp_c_array[i]= ' ';

	    }    
	    for(i=0;char_array1[i]!=NULL;i++){
	    }
	    for(;i<16;i++){
	        char_array1[i] = ' ';
 	    }
            char_array1[i] = 0;

	   
	    strcpy( tmp_c_array,char_array1 );
 //          sprintf(tmp_c_array,"%s", char_array);

 //	    ti = strlen(char_array);  // 나머지 부분 공백으로 나오게하기
 //       for  (i=0; i< ti; i++)  tmp_c_array[i]= char_array [i]; // 나머지 부분 공백으로 나오게하기

        LCD_RS=1;       //    pokeb( LCD_RS, 0x01);    // DATA
	LCD_RW_L=0;     //    pokeb( LCD_RW_L, 0x00);    // WRITE
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);



	   for  (i=0; i<= 0x0F; i++) {
	LCD_DATA = tmp_c_array[i];         // EXT4
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);
	    }


	/////////////////////SET DD RAM ADDRESS /////////////////
        LCD_RS=0;       //    pokeb( LCD_RS, 0x00);    // INSTRUCTION
	LCD_RW_L=0;     //    pokeb( LCD_RW_L, 0x00);    // WRITE
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);


 	LCD_DATA= 0xC0;   // SET DD RAM ADDRESS 아랫 줄 첫번째 위치
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);

        LCD_RS=1;       //    pokeb( LCD_RS, 0x01);    // DATA
	LCD_RW_L=0;     //    pokeb( LCD_RW_L, 0x00);    // WRITE
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);

	 for  (i=0; i<= 0x1F; i++) {
//	   tmp_c_array[i]= ' ';
	   tmp_c_array[i]= 0x20;
   	   tmp_c_array[i]= ' ';

	    }
                   
	    for(i=0;char_array2[i] !=0;i++){
	    }
	    for(;i<=0x0F;i++){
	        char_array2[i] = ' ';
 	    }
            char_array2[i] = 0;
            
	    strcpy( tmp_c_array,char_array2 );

	    for  (i= 0 ; i<= 0x0F; i++) {
	LCD_DATA = tmp_c_array[i];         // EXT4
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);
	    }


}
