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
#define PWM                     0x2000
#define MORSE_LENGTH                36
#define TEST_COUNT              1//780
#define LINE_LENGTH             17        

void LCD_INIT();       //TEXTLCD 초기화 과정, lcd내의 작은컨트롤러 초기화
void LCD_DISP_STRING(unsigned char *char_array, unsigned char *char_array2); //어떤스트링 값을 텍스트 LCD에 찍기* 함
void test_output();                  
void start_morse();
void string_output_segment(char *);
void display_word(int);

char seg_pat[16] = {
 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07
 ,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71
 };        
 
unsigned long int morse[MORSE_LENGTH]= {//
 0b10111,  0b111010101, 0b11101011101, //abc
 0b1110101, 0b1, 0b101011101,          //def
 0b111011101, 1010101, 0b101,          //ghi
 0b1011101110111, 0b111010111, 0b101110101,//jkl 
 0b1110111, 0b11101, 0b11101110111,        //mno
 0b10111011101, 0b1110111010111, 0b1011101,//pqr
 0b10101, 0b111, 0b1010111,                //stu
 0b101010111, 0b101110111, 0b11101010111,  // vwx
 0b1110101110111, 0b11101110101,           //yz
 0b10111011101110111, 0b101011101110111, 0b1010101110111,//123
 0b10101010111, 0b101010101, 0b11101010101,              //456
 0b1110111010101, 0b111011101110101, 0b11101110111011101,//789
 0b1110111011101110111                                   //0
};
unsigned char morse_change[MORSE_LENGTH] = {
 'A','B','C',
 'D','E','F',
 'G','H','I',
 'J','K','L',
 'M','N','O',
 'P','Q','R',
 'S','T','U',
 'V','W','X',
 'Y','Z',
 '1','2','3',
 '4','5','6',
 '7','8','9',
 '0' 
};

char test[4] = {0x78,0x79,0x6d,0x78};
char in [4] = {0x00,0x00,0x30,0x54};
char out[4] = {0x00,0x5C,0x1C,0x78};
char del[4] = {0x00,0x5E,0x79,0x38};
char max[4] = {0x00, 0b00010101,0b01110111,0b01110110};
char pulseA = 0b00000011;      
char pulseB = 0x0;

unsigned char LCD_Line1[LINE_LENGTH];
unsigned char LCD_Line2[LINE_LENGTH];

unsigned char led = 0xFE;
unsigned int cnt = 0, step = 0, click = 0, click_cnt = 0;
unsigned int current_length = 0, current_output = 0; 
unsigned long int   notend=0, out_word=0; 


void main(void){
	int i;
	DDRA = 0xFF;
        DDRD = 0xFF; 
	DDRC = 0xFF; // led
	//DDRE = 0x00; // sw
	DDRB = 0xFF; //7-segment
	DDRF = 0xF0; //4~1 segment

        delay_ms(5);                    
        LCD_INIT();
	test_output();          
	while(step == 0);
        start_morse();
        
	 while(1);
       
	
}
           
void start_morse(){
        //imter0 7segment, timer1 cnt++
        TIMSK = 0b00000110; //timer0 cmp on, timer1 ovf on
        TCCR0 = 0b00001111;//timer 0
        OCR0 = 255;
        TCNT0 = 0x00;
        
        TCCR1A = 0b00001000 | pulseA; //timer1, set oc1c
	TCCR1B = 0b00010100 | pulseB;
	TCCR1C = 0x0;
	OCR1A = 0x4000;
	OCR1CH = (PWM & 0xFF00) >> 8;
  	OCR1CL = PWM & 0x0FF;
  	
  	EIMSK = 0b01110000;//sw interrupt
        EICRB = 0b00101001; 

	SREG = 0x80; 
        
}
void test_output(){
	int i;
          
	sprintf(LCD_Line1, "MorseCodeReader");
	sprintf(LCD_Line2, "test");
	LCD_DISP_STRING(LCD_Line1, LCD_Line2);
        
	
	TCCR1A = 0b00001000 | pulseA; //set oc1c
	TCCR1B = 0b00010100 | pulseB;
	TCCR1C = 0x0;
	OCR1A = 0x4000;
	OCR1CH = (PWM & 0xFF00) >> 8;
	OCR1CL = PWM & 0x0FF;  
	                   
	TIMSK = 0x01;
	TCCR0 = 0x07;
	TCNT0 = 0x00;    
	
	SREG = 0x80;  	       
}               
  
void display_word(int num){      
        LCD_Line1[current_length++] = morse_change[num];
        LCD_Line1[current_length] = 0;        
           
        LCD_DISP_STRING(LCD_Line1, LCD_Line2);       
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

interrupt [EXT_INT4] void external_int4(void){//input 
         //make MAX i<LINE_LENGTH && 
        if(current_length != LINE_LENGTH- 1 && step == 1){ 
                if(click ==0){//push
                        click=1; 
                        click_cnt=0;
                        cnt=0;       
                }
                else{//up             
                        click++;
                        cnt = 0;
                }                                 
        }                                            
}                       

interrupt [EXT_INT5] void external_int5(void){//del
        if(step == 1)
                step = 2;       
}    
interrupt [EXT_INT6] void external_int6(void){//output
        step = 4; 
        cnt = 0;
}
  
interrupt  [TIM1_OVF] void timer_int(void){
        
        
        if(click % 2){//누르고 있을 동안
                click_cnt++;
        } 
        else{      //안누르는 동안
                cnt++;
        }
        
}  

interrupt  [TIM0_COMP] void timer_comp0(void){
        int i, check; 
        
        if(current_length == LINE_LENGTH - 1){
               string_output_segment(max);           
               cnt++; 
        }
        
        else if(step == 1){
                string_output_segment(in);

                if(click == 0 && cnt > 3){//word to word 
                        if(cnt >=7 && notend == 0 && current_length != 0 && LCD_Line1[current_length-1]!=' '){
                                LCD_Line1[current_length++] = ' ';
                                LCD_Line1[current_length] = 0;
                                LCD_DISP_STRING(LCD_Line1, LCD_Line2);
                        }
                        if(notend){ 
                                for(i = 0; i< MORSE_LENGTH; i++){
                                        if(notend == morse[i]){ 
                                                 notend = 0;   
                                                 click =0;
                                                 cnt=0;
                                                 display_word(i);
                                                 break;
                                        }
                                }  
                        }                
                }                                      
                else{//spell to spell
                      if(click_cnt >= 3 && click == 2){//long line 
                             notend <<= 4;
                             notend |= 0b111;
                                     cnt = 0;
                             click = 0;
                      }                            
                      else if(click == 2){//short line 
                             notend <<= 2;
                             notend |= 0b1;
                             click = 0;
                      }
                }
        	 sprintf(LCD_Line2, "c:%d,nt:%x,cc:%d",cnt, notend,click_cnt);
	         LCD_DISP_STRING(LCD_Line1, LCD_Line2); 
        }
        else if(step == 2){//del memory | word
                if(notend){
                        notend =0;
                }                 
                else if(current_length > 0){ 
                       LCD_Line1[--current_length] = 0;        
                       LCD_DISP_STRING(LCD_Line1, LCD_Line2);       
                }        
                step =3;
                cnt =0;
                click = 0;
        }              
        else if(step == 3){//display del for segment
                if(cnt > 32+16){
                        step = 1;
                        cnt = 0;
                }               
                string_output_segment(del);           
                cnt++;

        }
        else if(step == 4){//output line  
                string_output_segment(out);   
                
                if(current_output < current_length || out_word != 0){//라인이 비어 있지 않으면 
                        if(out_word == 0){
                                out_word = LCD_Line1[current_output];
                                if((char)out_word == ' '){//space면 한번 쉼
                                        out_word = 0; 
                                        LCD_Line1[current_output++] = ' ';
                                        LCD_DISP_STRING(LCD_Line1, LCD_Line2); 
                                        delay_ms(500);
                                        return;
                                }
                                LCD_Line1[current_output++] = ' '; 
                                LCD_DISP_STRING(LCD_Line1, LCD_Line2);
                                       
                                
                                if(out_word >= 'A'){//알파벳이면
                                        out_word = morse[out_word-'A'];
                                }
                                else{                              
                                        out_word = morse[out_word-'0'+26];
                                }     
                                
                                while((out_word & 0x80000000) != 0x80000000){ 
                                        out_word =out_word << 1;             
                                }
                                led = 0xFF;
                                step=5;  
                        }        
                        else{                                            
                                if((out_word & 0x40000000) == 0x40000000){//long word
                                        out_word = out_word << 4;
                                        led = 0x00;
                                }                            
                                else{//short word
                                        out_word = out_word << 2;
                                        led = 0xFE;
                                } 

                                step = 5;         
                        }                             

                }
                else{     
                        PORTC = 0xFF;
                        step = 1;
                        cnt =0;
                        current_output = 0;
                        current_length = 0;
                }
        }
        else if(step == 5){
                if(cnt > 32+16){
                        step = 4;
                        cnt = 0;
                }
                else if(cnt > 32){
                        led=0xFF;
                }
                
                PORTC = led;               
                string_output_segment(out);           
                cnt++;
        }
        
}   
                
interrupt [TIM0_OVF] void timer_ovf0(void){//first test code 
        cnt++;
        string_output_segment(test);           
        if(cnt > TEST_COUNT){//세 번 움직임.    
                 PORTC = 0xFF;
                 TIMSK = 0x00;
                 TCCR1A = 0x00; 
                 TCCR1B = 0x00;
                 sprintf(LCD_Line1, "%s", "");
        	 sprintf(LCD_Line2, "%s", "");
                LCD_Line1[0] = LCD_Line2[0] = 0;
	         LCD_DISP_STRING(LCD_Line1, LCD_Line2);  
	         step++;
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
