/**
 * @file ADC.c
 *
 * @brief Source code for the ADC driver.
 *
 * This file contains the function definitions for the ADC driver.
 *
 * ADC Module 0 is used to sample the potentiometer and the analog
 * light sensor that are connected on the EduBase board. ADC Module 0
 * is configured to sample the potentiometer first and the light sensor after.
 *
 * After the light sensor is sampled, an interrupt signal is set to 
 * indicate that the sampling sequence has ended. After the conversion
 * results have been read from the corresponding FIFO, the interrupt is cleared.
 *
 * The following pins are used:
 *  - Potentiometer   <-->  Tiva LaunchPad Pin PE2 (Channel 1)
 *  - Light Sensor    <-->  Tiva LaunchPad Pin PE1 (Channel 2)
 *
 * @author
 *
 */

#include "ADC.h"

void ADC_Init(void)
{
	// Enable the clock to ADC0
	SYSCTL->RCGCADC |= 0x01;

	// Enable the clock to Port E
	SYSCTL->RCGCGPIO |= 0x10;

	// Configure PE2 and PE1 as analog inputs
	GPIOE->AFSEL |= 0x06;
	GPIOE->DEN &= ~0x06;
	GPIOE->AMSEL |= 0x06;

	// Disable Sample Sequencer 0 before configuration
	ADC0->ACTSS &= ~0x01;

	// Use software trigger for Sample Sequencer 0
	ADC0->EMUX &= ~0x000F;

	// Sample PE2 first, then PE1
	// PE2 = Channel 1
	// PE1 = Channel 2
	ADC0->SSMUX0 = 0x21;

	// First sample: no interrupt
	// Second sample: end of sequence and interrupt flag
	ADC0->SSCTL0 = 0x60;

	// Enable Sample Sequencer 0
	ADC0->ACTSS |= 0x01;
}

void ADC_Sample(double analog_value_buffer[])
{
	uint32_t adc_result;

	// Start sampling on Sample Sequencer 0
	ADC0->PSSI = 0x01;

	// Wait until conversion sequence is complete
	while((ADC0->RIS & 0x01) == 0);

	// Read first sample: potentiometer
	adc_result = ADC0->SSFIFO0 & 0xFFF;
	analog_value_buffer[0] = (adc_result * 3.3) / 4095.0;

	// Read second sample: light sensor
	adc_result = ADC0->SSFIFO0 & 0xFFF;
	analog_value_buffer[1] = (adc_result * 3.3) / 4095.0;

	// Clear completion flag
	ADC0->ISC = 0x01;
}
