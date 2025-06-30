#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include "adc.h"
//#include "eeprom.h"
#include "uart.h"
#include "i2c.h"
#include "ds1307.h"
#include "external_EEPROM.h"
#include <xc.h>

//Variable Declarations
unsigned char time[9];
unsigned char EV[9][3] = {"ON", "GN", "G1", "G2", "G3", "G4", "G5", "GR", "C_"};
unsigned int speed = 0;
unsigned int index = 0;
unsigned char key;
unsigned int address = 0;
unsigned int add = 0;
unsigned int add1 = 0; 
unsigned int add2 = 12 ;
unsigned int event = 0;
unsigned int event_count = 0;
unsigned char array[10][15];
unsigned char option[4][16] = {"View_log       ", "Clear_log      ", "Download_log   ", "Set_time       "};
unsigned int count = 0;
unsigned int star = 0;
unsigned int flag = 0;
unsigned int delay = 0 ;


unsigned int hour_flag = 1 ;
unsigned int min_flag = 0 ;
unsigned int sec_flag = 0 ;


void check_matrix_keypad(void) {
    static unsigned long int i;
    key = read_switches(STATE_CHANGE);

}
//Function to read from EEPROM
void event_reader(void) {
    add=0;
    int j ;
    for (int i = 0; i < event_count; i++) 
    {
        for(j = 0; j < 14; j++)
        {
            if(j == 8)
            {
                array[i][j] = ' ' ;
            }
            else if(j == 11)
            {
                array[i][j] = ' ' ;
            }
            else
            {
                array[i][j] = read_external_EEPROM(add++);
            }
        }
        array[i][j]='\0';
    }

}
//Function to write into EEPROM
void event_store(void) {
    event_count++;
    if(event_count > 10)
    {
        event_count = 10 ;
        add1 = 0 ;
        add2 = 12 ;
        for(int i = 0 ; i < 9 ; i++)
        {
            for(int j = 0 ; j < 12 ; j++)
            {
                write_external_EEPROM(add1++, read_external_EEPROM(add2++));

            }
        }
        address = 108;
    }
    for (int i = 0; i < 8; i++) {
            write_external_EEPROM(address++, time[i]);
        }
        for (int i = 0; i < 2; i++) {
            write_external_EEPROM(address++, EV[index][i]);
        }
        write_external_EEPROM(address++, ('0' + (speed / 10)));
        write_external_EEPROM(address++, ('0' + (speed % 10)));

}

void view_log(void) {
    check_matrix_keypad(); 
  
    if(event_count == 0)
    {
        clcd_print("No Events", LINE1(0));
        for (unsigned long int i = 500000; i--;);
        CLEAR_DISP_SCREEN ;
        state = e_main_menu;
    }
    else
    {
        if (flag == 0) {
            event_reader();
            flag = 1;
        }
        clcd_print("# Time     EV SP", LINE1(0));
        clcd_putch('0' + event, LINE2(0));
        clcd_print(array[event], LINE2(2));
        if(key == MK_SW2 && event < event_count - 1)
        {
           event++; 
        }
        else if(key == MK_SW1 && event > 0)
        {
            event-- ;
        }
    }
    if(key == MK_SW12)
    {
        CLEAR_DISP_SCREEN ;
        state = e_main_menu;
      //  flag = 0 ;
        event = 0 ;
        add = 0;
    }
}

void clear_log(void) 
{
    event_count = 0 ;
    add = 0 ;
    address = 0;
    clcd_print("Logs are Cleared", LINE1(0));
    
    for (unsigned long int i = 500000; i--;);
    CLEAR_DISP_SCREEN ;
    state = e_main_menu;
}

void download_log(void) 
{
    //add = 0 ;
  
    char ch = '0' ;
    if(event_count == 0)
    {
        puts("No Events\n\r");
    }
    else
    {
        event_reader();
        
        puts("# Time     EV SP\n\r");
        for(int i = 0 ; i < event_count ; i++)
        {
            
            putch(ch);
            putch(' ');
            puts(array[i]);
            puts("\n\r");
            ch++ ;
        }
    }
    count = 0 ;
    event = 0 ;
    state = e_main_menu ; 
}

void set_time(void) 
{
    check_matrix_keypad();
    static unsigned int flag1 = 0 ,hour = 0, min = 0 ,sec = 0 ;
    clcd_print("HH:MM:SS", LINE1(2));
    if(flag1 == 0)
    {
        hour = ((time[0] - 48) * 10) + (time[1] - 48);
        min = ((time[3] - 48) * 10) + (time[4] - 48) ;
        sec = (10 * (time[6] - 48))+ (time[7]-48) ;
        flag1 = 1;
    }
    
    if(hour_flag == 1)
    {
        if(key == MK_SW1)
        {
            hour++ ;
            if(hour == 24)
            {
                hour = 0;
            } 
        }
        if(delay++ < 300)
        {
            clcd_putch('0' + (hour/10) , LINE2(2));
            clcd_putch('0' + (hour%10) , LINE2(3));
            clcd_putch(':' , LINE2(4));
            clcd_putch('0' + (min/10) , LINE2(5));
            clcd_putch('0' + (min%10) , LINE2(6));
            clcd_putch(':' , LINE2(7));
            clcd_putch('0' + (sec/10) , LINE2(8));
            clcd_putch('0' + (sec%10) , LINE2(9));
        }
        else if(delay < 600)
        {
            clcd_putch(0xFF, LINE2(2));
            clcd_putch(0xFF, LINE2(3));
            
        }
        else
        {
            delay = 0 ;
        }
        
        if(key == MK_SW2)
        {
            min_flag = 1 ;
            hour_flag = 0 ;
            sec_flag = 0 ;
        }
    }
    else if(min_flag == 1)
    {
        if(key == MK_SW1)
        {
            min++ ;
            if(min == 60)
            {
                min = 0;
            } 
        }
        if(delay++ < 300)
        {
            clcd_putch('0' + (hour/10) , LINE2(2));
            clcd_putch('0' + (hour%10) , LINE2(3));
            clcd_putch(':' , LINE2(4));
            clcd_putch('0' + (min/10) , LINE2(5));
            clcd_putch('0' + (min%10) , LINE2(6));
            clcd_putch(':' , LINE2(7));
            clcd_putch('0' + (sec/10) , LINE2(8));
            clcd_putch('0' + (sec%10) , LINE2(9));
        }
        else if(delay < 600)
        {
            clcd_putch(0xFF, LINE2(5));
            clcd_putch(0xFF, LINE2(6));
        }
        else
        {
            delay = 0 ;
        }
        
        if(key == MK_SW2)
        {
            min_flag = 0 ;
            hour_flag = 0 ;
            sec_flag = 1 ;
        }
    }
    else if(sec_flag == 1)
    {
        if(key == MK_SW1)
        {
            sec++ ;
            if(sec == 60)
            {
                sec = 0;
            } 
        }
        if(delay++ < 300)
        {
            clcd_putch('0' + (hour/10) , LINE2(2));
            clcd_putch('0' + (hour%10) , LINE2(3));
            clcd_putch(':' , LINE2(4));
            clcd_putch('0' + (min/10) , LINE2(5));
            clcd_putch('0' + (min%10) , LINE2(6));
            clcd_putch(':' , LINE2(7));
            clcd_putch('0' + (sec/10) , LINE2(8));
            clcd_putch('0' + (sec%10) , LINE2(9));
        }
        else if(delay < 600)
        {
            clcd_putch(0xFF, LINE2(8));
            clcd_putch(0xFF, LINE2(9));
        }
        else
        {
            delay = 0 ;
        }
        
        if(key == MK_SW2)
        {
            min_flag = 0 ;
            hour_flag = 1 ;
            sec_flag = 0 ;
        }
    }
    if(key == MK_SW12)
    {
        state = e_dashboard ;
        CLEAR_DISP_SCREEN;
        return ;
    }
    else if(key == MK_SW11)
    {
        write_ds1307(HOUR_ADDR,(((hour/10)<<4)|(hour%10) & 0x07f));
        write_ds1307(MIN_ADDR,((min/10)<<4)|(min%10));
        write_ds1307(SEC_ADDR,((sec/10)<<4)|(sec%10));
        CLEAR_DISP_SCREEN;
        state = e_dashboard ;
    }
   
}

void display_main_menu(void) {
    check_matrix_keypad();

    clcd_print(option[count], LINE1(2));
    clcd_print(option[count + 1], LINE2(2));

    if (key == MK_SW1) {
        if (star == 1) {
            star = 1;
        } else if (count > 0) {
            count--;
        }
        star = 0;
    }
    else if (key == MK_SW2) {
        if (star == 0) {
            star = 0;
        } else if (count < 2) {
            count++;
        }
        star = 1;
    }
    if (star == 1) {
        clcd_putch('*', LINE2(0));
        clcd_putch(' ', LINE1(0));
    } else {
        clcd_putch('*', LINE1(0));
        clcd_putch(' ', LINE2(0));
    }
    if (key == MK_SW11) {
        if (count == 0 && star == 0) {
            state = e_view_log;
            CLEAR_DISP_SCREEN;
        } else if (count == 0 && star == 1 || count == 1 && star == 0) {
            state = e_clear_log;
            CLEAR_DISP_SCREEN;
        } else if (count == 1 && star == 1 || count == 2 && star == 0) {
            state = e_download_log;
            CLEAR_DISP_SCREEN;
        } else if (count == 2 && star == 1) {
            state = e_set_time;
            CLEAR_DISP_SCREEN;
        }
    }
    if(key == MK_SW12)
    {
        state = e_dashboard;
        CLEAR_DISP_SCREEN;
        add = 0 ;
        address = 0 ;
        event_count = 0 ;
    }

}

void view_dashboard(void) {
    static int i = 0 ;
    clcd_print("TIME      EV SP ", LINE1(0));
    check_matrix_keypad();
    speed = read_adc(CHANNEL4) / 10.33;
    if(i == 0)
    {
        i = 1;
        event_store();
    }
    if (key == MK_SW1) 
    {
        index = 8;
        event_store();
        //event_reader();
    }
    else if (key == MK_SW2) 
    {
        if (index == 8) {
            index = 1;
        }
        else if (index < 7) {
            index++;
        }
        event_store();
        //event_reader();
    }
    else if (key == MK_SW3) 
    {
         if (index == 8) {
            index = 1;
        }
         else if (index > 1) {
            index--;
        }
        event_store();
        //event_reader();
    }
    clcd_print(time, LINE2(0));
    clcd_print(EV[index], LINE2(10));
    clcd_putch('0' + (speed / 10), LINE2(13));
    clcd_putch('0' + (speed % 10), LINE2(14));
    if (key == MK_SW11) {
        state = e_main_menu;
        CLEAR_DISP_SCREEN;
    }
}