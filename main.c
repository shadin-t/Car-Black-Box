/*---------Documentation-------
 * Project :  CAR BLACK BOX
 * Author  : Mohammad Shadin
 * Date    : 21-02-2025
 *
 * Created on 3 February, 2025, 12:48 PM
 */

#include "black_box.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "main.h"
#include "adc.h"
#include "uart.h"
#include "i2c.h"
#include "ds1307.h"
#include <xc.h>

State_t state;
unsigned char clock_reg[3];

extern unsigned char time[9];

void init_config()
{
    
    state = e_dashboard;
    init_matrix_keypad();
	init_clcd();
    init_adc();
    init_uart();
    init_i2c();
    init_ds1307();
}
static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	
    time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
    time[1] = '0' + (clock_reg[0] & 0x0F);
	time[2] = ':';
	time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	time[4] = '0' + (clock_reg[1] & 0x0F);
	time[5] = ':';
	time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	time[7] = '0' + (clock_reg[2] & 0x0F);
	time[8] = '\0';
}

void main(void) 
{
    init_config();

    while(1)
    {
        get_time();
        // Detect key press
        
        switch (state)
        {
            case e_dashboard:
                // Display dashboard
                view_dashboard();
                break;
            
            case e_main_menu:
                // Display dashboard
                display_main_menu();
                break;
            
            case e_view_log:
                // Display dashboard
                view_log();
                break;
                 
            case e_download_log:
                download_log();
                break;
                
            case e_clear_log:
                clear_log();
                break;
                     
            case e_set_time:
                set_time();
                break;
                
        }
        
    }
    
}
