/*
 * @file main.c
 *
 * @brief Main source code for the ADC program.
 *
 * This file contains the main entry point and function definitions for the ADC program.
 *
 * It interfaces with the following:
 *  - EduBase Board Potentiometer (PE2)
 *  - EduBase Board Analog Light Sensor (PE1)
 *
 * @author
 */


#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"
#include "EduBase_LCD.h"
#include "ADC.h"
#include "GPIO.h"


static double adc_buffer[8];

int Analog_Voltage_to_Digital(double voltage, double in_min, double in_max, int out_min, int out_max);


int main(void)
{
//	// procedure //
//		SysTick_Delay_Init();

//	EduBase_LCD_Init();

//	ADC_Init();

//	EduBase_LCD_Enable_Display();
//	EduBase_LCD_Clear_Display();

//	EduBase_LCD_Set_Cursor(0, 0);
//	EduBase_LCD_Display_String("Potentiometer");
//	
//	while(1)
//	{		
//		ADC_Sample(adc_buffer);

//		EduBase_LCD_Set_Cursor(0, 1);
//		EduBase_LCD_Display_Double(adc_buffer[0]);

//		SysTick_Delay1ms(1000);
//	}

		// ************* //

//// task 1 //
//  SysTick_Delay_Init();

//	EduBase_LCD_Init();

//	ADC_Init();

//	EduBase_LCD_Enable_Display();
//	EduBase_LCD_Clear_Display();

//	EduBase_LCD_Set_Cursor(0, 0);
//	EduBase_LCD_Display_String("Light Sensor");

//	while(1)
//	{
//		ADC_Sample(adc_buffer);

//		EduBase_LCD_Set_Cursor(0, 1);
//		EduBase_LCD_Display_Double(adc_buffer[1]);

//		SysTick_Delay1ms(1000);
//	}

		// ********** //
		
		// task 2 //
		
		
 int digital_value = 0;

    // Initialize SysTick timer
    SysTick_Delay_Init();

    // Initialize the LCD on the EduBase board
    EduBase_LCD_Init();

    // Initialize the ADC module
    ADC_Init();

    // Initialize the LEDs on the EduBase board
    EduBase_LEDs_Init();

    // Enable and clear the LCD display
    EduBase_LCD_Enable_Display();
    EduBase_LCD_Clear_Display();

    // Display title on the first row of the LCD
    EduBase_LCD_Set_Cursor(0, 0);
    EduBase_LCD_Display_String("Light Sensor");

    while(1)
    {
        // Sample the ADC channels and store values into adc_buffer
        ADC_Sample(adc_buffer);

        // Convert the measured analog light sensor voltage to a digital value
        digital_value = Analog_Voltage_to_Digital(adc_buffer[1], 0.0, 3.3, 0, 255);

        // Display the measured light sensor voltage on the second row
        EduBase_LCD_Set_Cursor(0, 1);
        EduBase_LCD_Display_Double(adc_buffer[1]);

        // Display the digital value on the second row starting at column 13
        EduBase_LCD_Set_Cursor(13, 1);
        EduBase_LCD_Display_Integer(digital_value);

        // Turn on all EduBase LEDs if digital value is greater than or equal to 200
        if(digital_value >= 200)
        {
            EduBase_LEDs_Output(0x0F);
        }

        // Turn off all EduBase LEDs if digital value is less than 200
        else
        {
            EduBase_LEDs_Output(0x00);
        }

        SysTick_Delay1ms(1000);
    }
}

int Analog_Voltage_to_Digital(double voltage, double in_min, double in_max, int out_min, int out_max)
{
    int digital_value;

    if(voltage <= in_min)
    {
        return out_min;
    }
    else if(voltage >= in_max)
    {
        return out_max;
    }

    digital_value = (int)(((voltage - in_min) * (out_max - out_min)) /
                    (in_max - in_min) + out_min);

    return digital_value;
		
}

