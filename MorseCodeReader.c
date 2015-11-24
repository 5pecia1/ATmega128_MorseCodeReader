#include <mage128.h>
#include <delay.h>

void LCD_INIT();       //TEXTLCD �ʱ�ȭ ����, lcd���� ������Ʈ�ѷ� �ʱ�ȭ
void LCD_DISP_STRING(unsigned char *char_array, unsigned char * char_array2); //���Ʈ�� ���� �ؽ�Ʈ LCD�� ���* ��
void test_output();


//unsigned char LCD_Line[33];
unsigned char LCD_Line1[17];
unsigned char LCD_Line2[17];

char seg_pat[16] = {
 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07
 ,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
};

void main(void){
	
	DDRA = 0xFF;
        DDRD = 0xFF; 

	ADMUX = 0x10;      // ADC0 �ܱؼ� �Է� ����
        ADCSRA = 0x87;    // ADEN=1, 16MHz  256���� -> 125kHz
        delay_ms(5);                    
        
        LCD_INIT();
	test_output();
	While(1);
}

void test_output(){

	sprintf(LCD_Line1, "MorseCodeReader   ");
	spirntf(LCD_Line2, "made by sol, chul ");
	LCD_DISP_STRING(LCD_Line1, LCD_Line2);
	delay_ms(1000);
}

void LCD_INIT() // �״�� �貸�� ���� ��
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
int i,ti;
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


  	LCD_DATA= 0x80;   // SET DD RAM ADDRESS ù��°��  �� ù��° ��ġ
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

	    ti = strlen(char_array);
	    strcpy( tmp_c_array,char_array1 );
 //          sprintf(tmp_c_array,"%s", char_array);

 //	    ti = strlen(char_array);  // ������ �κ� �������� �������ϱ�
 //       for  (i=0; i< ti; i++)  tmp_c_array[i]= char_array [i]; // ������ �κ� �������� �������ϱ�

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


 	LCD_DATA= 0xC0;   // SET DD RAM ADDRESS �Ʒ� �� ù��° ��ġ
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

	    ti = strlen(char_array);
	    strcpy( tmp_c_array,char_array2 );

	    for  (i= 0x01 ; i<= 0x0F; i++) {
	LCD_DATA = tmp_c_array[i];         // EXT4
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	LCD_ENABLE=1;   //    pokeb( LCD_ENABLE, 0x01);  // E  HIGH PULSE
	LCD_ENABLE=0;   //    pokeb( LCD_ENABLE, 0x00);  // E  HIGH PULSE
	//    	   delay_ms(LCD_DELAY);
   		    	   delay_us(50);
	    }


}