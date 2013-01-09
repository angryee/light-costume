/*
 * costume_2012.c
 *
 * Created: 11/10/2012 4:05:40 PM
 *  Author: Stephen Friederichs
 *
 * The goal of this project is to create a shirt bearing the image of a stoplight which has
 * electronics attached to it such that the stop lights mimic the pattern of common US 
 * streetlights:
 *
 *  The red light will start off being on.  If the user walks forward, the green light will turn on.  If the user stops,
 *  the yellow light turns on for three seconds, then the red light turns on.  The cycle repeats from this point
 *  if the user walks forward again.
 
 *		time	light	state	comment
 *	 --------------------------------------------
 *		0-		Red		On		Good so far
 *		0-		Yellow	Off
 *		0-		Green	Off
 *	---------------------------------------------
 *	*User walks forward at t=0*	
 *	 --------------------------------------------
 *		0+		Green	On		This is right
 *		0+		Red		On		But this one...	
 *		0+		Yellow	Off
 *	---------------------------------------------
 *	*User stops at t=1*			
 *	 --------------------------------------------
 *		1+		Red		On		Uhhh
 *		1+		Yellow	On		That's just not right at all
 *		1+		Green	On		None of them turned off
 *	
 * Or maybe this one:
 *
 *		time	light	state	comment
 *	 --------------------------------------------
 *		0-		Green	Off
 *		0-		Red		On		Good so far
 *		0-		Yellow	Off
 *	---------------------------------------------
 *	*User walks forward at t=0*
 *	 --------------------------------------------
 *		0+		Green	On		This is right
 *		0+		Red		Off		
 *		0+		Yellow	Off
 *	 --------------------------------------------
 *		1+		Green	Off	Hey, you're supposed to stay on!
 *		1+		Red		Off		
 *		1+		Yellow	Off		

 *		
 * Some might say I'm intentionally confusing the rules.  I am.  I'm great at it.  I could do it all night. 
 * I pity my wife ("Honey, you said I had to walk the dog, you DIDN'T say to walk her OUTSIDE!  You can't be angry
 * because I'm technically right!")
 *
 * Everyone who teaches you programming will repeatedly tell you that computers don't do what you want - only what 
 * you tell them to do.  If you can't express exactly what you want to happen then guess what?  Something else will
 * happen.  And let's be clear - this isn't a failing of computer but instead a failing of spoken and written language.
 * Human language is necessarily shorthand for something else.  The words we use have meanings and it's assumed the 
 * other party knows what you're talking about.  Computers don't know what you're talking about.  Unless you can
 * express what you want your program to do unambiguously and completely you won't be able to program it.  You'll 
 * end up programming something else - something you didn't want.  
 *
 * You could try rewriting the sentence - adding subordinate clauses and qualifiers and adjectives and conditionals 
 * (oh my!) 
 * to make sure that you express exactly what it is you think that the program should do.  But why bother?  
 * Words will always fail you.  Always.  It's what I've learned working as a test engineer.  
 * 
 * Test engineers read sentences like the one I wrote all day.  Many of those sentences are much worse and more 
 * ambiguous than what I wrote and the people who write them are totally in love with their profound verbiage
 * and will resist any efforts to clarify or elucidate their intentions.  As far as they are concerned, their
 * word is law.
 *  
 * I am not in love with those words I wrote.  I am not in love with words at all.  I prefer diagrams, tables, 
 * equations and pseudocode. 
 * 
  
 *
 *  There is only one light on at a time.  The red light starts off being on.  If the user walks forward, the green
 *  light turns on.  If the user stops, the yellow light comes on for three seconds, then the red light turns on until
 *  the user walks forward again. 
 *
 *  1) The initial light configuration will be:
 *     * Red: On
 *     * Yellow: Off
 *     * Green: Off
 *  
 *  If we're going to be using Special Terms like On and Held then we need to be consistent.  If you read the above
 *  requirement with the understanding that a light can be On or Held then Red will initially be On (not Held). 
 *  This will make triggering requirement 2 impossible because it only triggers when the Red Light is Held.  If it
 *  is initially On there's no requirement to transition it out of On and into Held so the sequence can start.
 *  So we change it to be consistent with the new scheme:
 *
 *  1) The initial light configuration will be:
 *     * Red: Held
 *     * Yellow: Off
 *     * Green: Off
 *
 *  2) While the red light is on, if the person wearing the shirt walks forward, the green light will turn on 
 *     and all other lights will turn off
 *
 *  Hey, On is capitalized a lot, isn't it? that Means Something, doesn't it?  And what about the Red Light, 
 *  Yellow Light and Green Light?  They're important too!  Let's use capitalization to our advantage.
 *  I'm going to rewrite the requirement like this:
 
 *  2) While the red light is On, if the person wearing the shirt walks forward, the green light will turn On
 *     and all other lights will turn Off
 *
 *  So now lights can be Off, On or Held.  We've decided that the Red Light can't be interrupted when it's On, only 
 *  when it's Held.  So, we have to modify this requirement to say that the sequence can only start when the Red Light
 *  is Held, not On:
 *
 *  2) While the Red Light is Held, if the person wearing the shirt walks forward, the green 
 *
 *  3) While the green light is On, if the person wearing the shirt comes to a stop, the green light will turn off,
 *     the yellow light will turn on for three seconds, then the red light will turn on.
 *
 *  4) The red light must be on for at least a second whenever it turns on.
 * 
 *
 *  3) While the green light is on, if the person wearing the shirt comes to a stop the lights will 
 *     follow this sequence:
 *
 *		time	light	state	comment
 *		0-		Green	On		Requirement specifies initial condition of lights
 *		0-		Yellow	Off
 *		0-		Red		Off
 *	---------------------------
 *		0+		Green	Off		As soon as possible (t=0+), the yellow light should come on
 *		0+		Yellow	On
 *		0+		Red		Off
 *	---------------------------
 *		3+		Green	Off		The yellow light persists for 3s only, then red comes on
 *		3+		Yellow	Off
 *		3+		Red		On
 *	---------------------------
 *		4+		Green	Off		The red light remains on for at least a second before it can be interrupted
 *		4+		Yellow	Off
 *		4+		Red		On
 * 
 *
 *
 *  3) While the green light is on, if the person wearing the shirt comes to a stop the lights will
 *     follow this sequence:
 *
 *		time	light	state	comment
 *		0-		Green	On		Requirement specifies initial condition of lights
 *		0-		Yellow	Off
 *		0-		Red		Off
 *	---------------------------
 *		0+		Green	Off		As soon as possible (t=0+), the yellow light should come on
 *		0+		Yellow	On
 *		0+		Red		Off
 *	---------------------------
 *		3+		Green	Off		The yellow light persists for 3s only, then red comes on
 *		3+		Yellow	Off
 *		3+		Red		On
 *	---------------------------
 *		4+		Green	Off		The red light remains on for at least a second before it can be interrupted
 *		4+		Yellow	Off
 *		4+		Red		Held
 * 
 * Here's some situations that will arise from these requirements:
 * Light is green, person is walking forward but stops, causing yellow light for three seconds, then red light for 
 * at least one.  Before light is red, person has come to a complete stop.  While Red Light is On, person starts
 * walking.  The Red Light will stay on in Held because the person started walking when it was On, not Held.  
 * Requirement 2 coversThere's no requirement about what happens when walking occurs and the light is On.
 */ 

//Always define this before you include delay.h
#define F_CPU 8000000
	
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define TRUE 0xFF
#define FALSE 0x00

#define BOOL(x) ((x == FALSE)?FALSE:TRUE)
#define NOT(x) ((x==FALSE)?TRUE:FALSE)
#define AND(x,y) ((FALSE == (x&y))?FALSE:TRUE)
#define OR(x,y) ((FALSE == (x|y))?FALSE:TRUE)

#define SET(x,y) x |= (1 << y)
#define CLEAR(x,y) x &= ~(1<< y)
#define READ(x,y) ((FALSE == ((x & (1<<y))>> y))?FALSE:TRUE)
#define TOGGLE(x,y) (x ^= (1<<y))

enum ADC_Channels
{
	ADC_ACCEL_X = 0,	/*X-Axis*/
	ADC_ACCEL_Y,		/*Y-Axis*/
	ADC_ACCEL_Z,		/*Z-Axis*/
	ADC_ACCEL_0GD		/*Zero G detect - OMG I'm falling!*/
};

enum 
{
	EVENT_500MS,
	EVENT_DECIMATE,
	EVENT_PUSHBUTTON,
	EVENT_READ_ADC_Y,
	EVENT_FORWARD_MOTION,
	EVENT_HALT
};

enum acceleration_state_machine
{
	IDLE,					//Just chillin
	BEGIN_FORWARD_ACCEL,	
	END_FORWARD_ACCEL,
	FORWARD_MOTION,
	BEGIN_REVERSE_ACCEL,
	END_REVERSE_ACCEL	
};

typedef uint8_t boolean;

//System ticks - always important
uint32_t ticks = 0;
uint32_t events = 0x0000;

//Filter coefficients
//#define B0 .037786f
//#define B1 .037786f
//#define A1 -.92443f

//Fixed-point coefficients
#define LPF_B0 0x000009AC	
#define LPF_B1 0x000009AC
#define LPF_A1 0x0000ECA7

#define NOMINAL_Y_ACCEL 0x00580000	//Split the difference between 0x59 and 0x58

int32_t adc_lp_filter(uint8_t adc_sample)
{
	static uint8_t last_sample=(NOMINAL_Y_ACCEL>>16);	//Idle
	static int32_t last_result=NOMINAL_Y_ACCEL;
	int32_t result;
	uint32_t temp,temp2;
	int64_t temp3;

	//Floating point warning!  Don't do this:
	//result = B0*adc_sample + B1*last_sample - A1*last_result
	//the problem is adding two small #'s: it can lead to incorrect results
	//The solution in this case is to add the two numbers then multiply them
	//This works since B0 and B1 are the same #
	temp = (int32_t)adc_sample*LPF_B0;
	temp2 = (int32_t)last_sample*LPF_B1;
	temp3 = ((int64_t)last_result*LPF_A1)>>16;
	
	result = temp + temp2 + (int32_t)temp3;
	
	last_result = result;
	last_sample = adc_sample;
	
	return result; 
}

#define HPF_B0 0x000322A9	//3.1354
#define HPF_B1 0x000322A9	//3.1354
#define HPF_A1 0x0000FEFF	//.99608

int32_t adc_hp_filter(int32_t adc_sample)
{
	static int32_t last_sample=NOMINAL_Y_ACCEL;
	static int32_t last_result = 0x00000000;
	int32_t result;
	int32_t temp, temp2, temp3;

	temp = (int32_t)((((int64_t)last_result*HPF_A1))>>16);
	temp2 = (int32_t)((((int64_t)last_sample*HPF_B1))>>16);
	temp3 = (int32_t)((((int64_t)adc_sample*HPF_B0))>>16);
	
	result = temp-temp2+temp3;
	last_result = result;
	last_sample = adc_sample;
	return result;
}

int16_t compute_velocity(/*This is already expressed in a 32-bit fixed-point integer*/int32_t filtered_y_accel)	
{
	static int32_t current_velocity= 0x00000000;
	static int32_t last_accel = 0x00000000;
	
	current_velocity += ((last_accel+filtered_y_accel)>>1);
	last_accel = filtered_y_accel;

	return (current_velocity>>16);	//Return only integer portion
}
int main(void)
{
	boolean a;
	boolean timer_overflow;
	uint8_t i;
	uint8_t adc_y_axis;
	uint8_t accel_state = IDLE;
	int32_t filtered_y_axis = 0;
	uint8_t uart_tx = 0;
	int16_t velocity = 0x0000;
	int16_t last_velocity = 0x0000;
	//Init
	
	//System Clock Options
	//Desired system clock is:
	// * From an internal source - this is mandatory as I didn't put a crystal on the board
	// * The higher clock frequency, the better, however
	// * We need to generate specific timing: multiples of 1s.  
	//   The clock frequency may need to be altered to assist this requirement
	// * Also, eventually we might have to worry about power usage in which case 
	//   the lower the frequency, the better
	//
	//As is typical with low-level requirements, this is partially arbitrary and partially practical and entirely
	//contradictory.
	//Also, it is very likely to change.  That's the problem with low-level requirements: you have to change
	//them a lot to get the system to actually work.
	
	//ATMega328 Datasheet Pg. 27 Section 9.2 - Clock sources are selectable by Flash Fuse bits 
	//ATMega328 Datasheet Pg. 27 Section 9.2.1 - Default clock source is an internal RC oscillator divided by 
	//8 to give a system clock of 1MHz
	
	//Flash Fuse Bits scare me - they are the means to destroy your uC. Look up sob stories online about senior 
	//projects ruined the day before 
	//Like a gun, Flash Fuse Bits are 'safe' when handled properly.  Guns are fun, right?  But look at it this way:
	//Accidental gun injury is a game you can't lose if you don't play and can only win if you never forget you're
	//handling a dangerous weapon.  Given my poor attention span and concern for my family I choose not to play this
	//game often.  By the same logic, I stay farther away from Flash Fuse Bits because they can do worse than 
	//kill you : they can annoy you and force you to pay for overnight shipping.  If there's a fate worse than death
	//it involves ruined electronics and overnight shipping. And perhaps bears.
	
	//Knowing how dangerously annoying Fuse Bits can be, I'll stick with the clock frequency of 1MHz.  My
	//only question is whether the peripherals (timers, ADCs, etc) will be driven by that clock or a different one.
	//As far as I can tell, yes.  ClkAsy might be driven by an external oscillator separate from the system clock,
	//but as far as I can tell the rest should all be driven synchronously to the system clock with and there
	//aren't any other prescalers mentioned except in the sections specifically for the timers.  I think I can 
	//assume that the clock going to the timer is 1MHz if I use ClkIO as the source.
	//This is the main problem with trying to use datasheets as the ultimate reference.  There's plenty of information,
	//but the organization leaves something to be desired sometimes.  You never know when something absurdly important
	//to you working with a timer is going to be stuck in the section about power saving modes or brown out detection.
	
	//Sadly I can't avoid changing the fuse bits.  I intend to use the UART and there are few good options for
	//UART baud rate when the oscillator is 1MHz.  To remedy this I've disabled the CKDIV8 fuse so that the
	//oscillator frequency is 8MHz again
	

		 
	//I/O Time
	
	//Here's my I/O list:
	//
	//	Port	Pin		I/O		Function			Notes
	//-------------------------------------------------------------------------------
	//	C		4		I		Tactile pushbutton	Needs internal pullup
	//	D		7		O		Heartbeat LED
	//	B		1		O		Green Light PWM		Controlled via timer, not GPIO
	//	D		5		O		Red Light PWM		Controlled via timer, not GPIO
	//	D		6		O		Yellow Light PWM	Controlled via timer, not GPIO
	
	//Due to the way the AVR is designed, pins can be configured as General Purpose I/O pins (GPIO) (Section 14.2) or 
	//as their Alternate Port Functions (Section 14.3). Since we'll be using three of these pins as PWM outputs
	//we don't need to configure them as GPIO.  When we configure the timer to generate PWM that overrides any GPIO
	//configuration for the pins.  We can ignore those three for now.
	
	//PC4 - Generic Switch Input
	
	//ATMega328 Datasheet Table 14-1 Pg 78
	//Configuring pin PC4 for input, pullup
	CLEAR(DDRC,4);	//Direction: Input
	SET(PORTC,4);	//Pullup: Enabled
	
	//ATMega328 Datasheet Table 14-1 Pg 78
	//Heartbeat LED - PD7
	//Set as Output High (initially)
	SET(DDRD,7);	//Direction: output
	CLEAR(PORTD,7);	//State: Lo

	PRR = 0x00;	//DIsable all power reduction
	
	//ATMega328 Datasheet Section 16.11.1 pg 132 - TCCR1A
	//No waveform generation is required on this timer, so set all 
	//ports to normal operation
	
	TCCR1A = 0x00;
    
	//ATMega328 Datasheet Section 16.11.3 pg 135 - TCCR1C
	//This register is used for output compare.  Again, we're not doing that
	//So this can be all 0's
	
	TCCR1C = 0x00;
	
	//TCCR1B
	//Note: I've disabled the CKDIV8 fuse so that the Fosc and clock source is 8MHz
	//ATMega328 Datasheet Section 16.11.2 pg 134 - TCCR1A
	//No input capture used - bits 7:6 are 0
	//No waveform generation used - bits 4:3 are 0
	//Clock source select is bits 2:0
	//Timer is either stopped or clocked from either ClkIO or an external source
	//ClkIO is source, 1MHz clock
	//There are prescaler options of 1,8,64,256,1024
	//We need to generate a time of 3s
	//We have 16 bits to work with as well, so a lot of different ways we can do this 
	
	//As per ATMega328 Datasheet Section 16.9.1 page 123, setting the timer to Normal mode causes the counter
	//to count up until it reaches 0xFFFF at which point it will overrun and start back at 0.  To configure this
	//timer/counter to produce a reliable period we need to start counting at a value that causes it to reach
	//0xFFFF in a known period of time.  THe first question is what period of time do we want that to be?
	
	//First question is what is the default? 
	//The default behavior is a prescaler of 1, clock source of 8MHz and a period of 0xFFFF counts or 65566 counts
	//That gives us a period of .125e-6*65536 = ~.008192s = 8.192ms
	
	//A prescaler of 256 gives us 256 times that, roughly 2.1s
	//The clock source is 8000000/256.  This means 1 tick is .125e-6*256 = 32us
	//500ms / .032ms /tick = 1953.125 ticks
	//We can only count an integer number of ticks so we either have to count up to 1953 ticks or 1954 ticks
	//1953 is closest to 1953.125 so we'll use that
	//However, this isn't the value that we will put into the count registers.  We need to count 1953 counts and 
	//be at 0xFFFF for this scheme to work.  So, we need to populate the count register with a value that is 1953
	//less than 0xFFFF.  Some quick math gives us 65535-1953 = 63852 = 0xF85E 
	//Why do I use hex when I could use decimal?  I don't know.  I'm old school.
	
	//Name this something that makes sense when you write it in code
	#define PERIOD_500MS 0xC2F6
	#define PERIOD_10MS  0xF3C7
	#define PERIOD_100MS 0xF3CA
	#define PERIOD_250MS 0xE17B
	#define PERIOD_800HZ 0xFFD9
	
	#define PERIOD_HB_LED 400
	#define PERIOD_DECIMATE 40
	
	#define TIMER1_PERIOD PERIOD_800HZ
	
	//See?  It's obvious what's going on here
	TCNT1 = TIMER1_PERIOD;
	
	//Now to set the interrupt masks
	//I want no interrupts - we'll poll the overflow flag to determine when an overflow has occurred
	//ATMega328 Datasheet Section 16.11.8 Pg 136 - TIMSK1
	//No interrupts, all 0's
	
	TIMSK1 = 0x00;
	
	//Now the only thing left to do is turn the timer/counter on.  But don't do it yet!  That's always the last
	//thing you do before you start the main loop.  You don't want to miss an overflow situation while you're 
	//initializing other parts of the system.

	//Setting up Timer 0 for PWM mode
	//As per ATMega Datasheet Section 15.9.1 Page 105 Paragraph 1: the DDRn bits for the output compare lines
	//must be set to output for any of this to work
	//Output compare A - Port D pin 6
	//This is 
	SET(DDRD,6);	//Direction: output

	//Output compare B - Port D pin 5
	//SET(DDRD,5);
		
	//Fast PWM mode for Timer 0 
	//Count increases from 0x00 to 0xFF
	//It is SET at 0x00 and is CLEARED when it matches the output compare 
	//Period is set only according to the source

	TCNT0 = 0x00;
	//ATMega328 Datasheet Section 15.9.1 Pg 105 - TCCR0A 
	TCCR0A =	(0x02<<6) |	//Noninverting PWM output A: Set output A at BOTTOM, CLEAR at compare match
				(0x02<<4) |	//Noninverting PWM output B
				(0x03);	//Fast PWM mode 3: RUn from 0x00 to 0xFF
				
	//ATMega328 Datasheet Section 15.9.2 Pg 108 - TCCR0B
	TIMSK0 =	0X00;		//No interupts needed
	
	#define GREEN_PWM 0x80
	#define YELLOW_PWM 0x80
	#define RED_PWM 0x80
	
	OCR0A = YELLOW_PWM;
	OCR0B = 0x01;		//NOTE!: even at OCR0B at 0x00 it still turns on!  Turn the output off if you don't want 
						//lights to turn on.
	
	TCCR0B =	(0x00<<7) |	//Force output compare A - not needed because of PWM
				(0x00<<6) |	//Force output compare B - not needed because of PWM mode
				(0x00<<3) |	//WGM most-significant bit - should be set to 0 for this mode
				(0x01);		//Clock-select = 4 - 8MHz/256 = 31KHz period. But for now set it to 0 so it doesn't run
				


	
	//Now it's time for the ADC
	//I just go through the datasheet registers and configure them as I see fit.
	
	//ATMega328 - Section 24.9.1 Pg 24 - ADMUX Register
	//AVCC - 5V
	//AREF - 3.3V
	//Reference (Bits 7:6) - AVCC - this is only temporary.  I need a jumper to short the 3.3V line to the external reference
	
	/*ADC result - left-adjusted (Bit 5).  The ADC result is 10-bits wide.  This is difficult to work with - 
	most people only want to work with 8 bits and in practice the least-significant bit is so noisy
	that it doesn't matter. Losing another bit of resolution is an acceptable sacrifice to make the
	ADC easier to work with.  The AVR allows the ADC result to be left-adjusted so that the most
	significant 8 bits are stored in one register.  Thus, if you left-adjust the result you only
	need to read one register to get the result*/
	
	/*ADC Channel - I only care about one - the Y axis on the accelerometer which is channel 1.*/
	
	ADMUX =		(0x01 << 6) /*Reference - AVCC - 5V. */ 
			|	(0x01 << 5) /* Left-adjust ADC result - refer to Section 24.9.3.2 pg 256*/
			|	(ADC_ACCEL_Y); /*Channel set to Y-Axis output on accelerometer*/
	
	//ATMega328 Datasheet - Section 24.9.2 - ADCSRA - ADC Status and Control Register
	//ADCEN - Bit 7 - Enable ADC - Obviously set this to 1
	//ADCSC - Bit 6 - Start Converstion - Not yet: 0
	//ADATE - Bit 5 - Auto-trigger ADC - I'll be manually triggering the ADC, so 0
	//ADCIF - Bit 4 - ADC Interrupt Flag - Set when conversion completes.  Ignore.
	//ADCIE - Bit 3 - ADC Interrupt Enable - Everything will be polled for this, so 0
	//ADPS - Bits 2:0 - ADC Prescaler
	/*ATMega328 Section 24.4 Pg245 discusses what the prescaler should be set to:
	
	By default, the successive approximation circuitry requires an input clock frequency between 50kHz and 200kHz to
	get maximum resolution.
	
	The ClkIO is 1MHz and the prescaler options are 2,4,8,16,32,64 and 128. 1MHz/8 = ~125KHz, so that seems good.
	That value is 3
	*/
	
	ADCSRA =	(0x01 << 7)	//Enable ADC
			  |	(0x03);		//Set prescaler to 1/8 ClkIO - 125KHz
			  
	//ATMega328 Datasheet Section 24.9.5 Pg 257 - DIDR0
	//This register allows digital input buffers on ADC pins to be disabled.  This saves power, so I'll do it
	//I have four channels: 0-3 which equates to 0x0F
	
	DIDR0 = 0x0F;	//Turn off digital filtering on ADC channels 0-3
	
	
	//Configure UART for 115200 8N1 Tx Communication
	//We want to transmit accelerometer information for debug purposes
	
	//Step 1 - Baud rate
	//ATMega328 Datasheet Section 20.10 - Table 20-6 pg 192
	//Baud rate settings for fosc of 8MHZ
	//Choosing baud rate of 38.4K for minimum error
	//U2Xn = 0
	//UBRRn = 12
	
	UBRR0 = 12;
	
 	//UCSR0A - UART 0 Control and Status Register A
	//ATMega328 Datasheet Section 20.11.2 pg 194
	//Bits 7:2 - Status bits
	//Bit 1 - Double UART transmission speed - Yes : 1
	//Bit 0 - Multi-Processor Communication Mode - No:0
	
	UCSR0A = 0x00;
	
	//UCSR0B - UART 0 Control and Status Register B
	//ATMega328 Datasheet Section 20.11.3 pg 
	//Bit 7 - Rx Complete Interrupt Enable - 0
	//Bit 6 - Tx Complete Interrupt Enable - 0
	//Bit 5 - USART Data Register Empty interrupt enable - 0
	//Bit 4 - Receiver Enable - Set to 0
	//Bit 3 - Transmitter Enable - Set to 1
	//Bit 2 - Character Size Bit 2 - Set to 0 for 8 bits
	//Bit 1 - 9th receive bit - Ignore
	//Bit 0 - 9th transmit bit - Ignore
	
	UCSR0B = 0x00 | (1 << 3);
	
	//UCSR0C - UART 0 Control and Status Register C
	//ATMega328 Datasheet Section 20.11.4 - Pg 196
	//BIts 7:6 - Set to asynchronous (clockless) mode: 00
	//Bits 5:4 - Parity setting - None : 00
	//Bit 3 - Stop select - 1 : 0
	//Bit 2:1 - Character size - 8 : 11
	//Bit 0 - Clock polarity: Don't care : 0
	
	UCSR0C = 0x03 << 1;
	
	//Send a known pattern to verify the UART works
	UDR0 = 0xA5;
	
	//Wait until transmit is complete
	while(FALSE == READ(UCSR0A,6));
	
	UDR0 = 0x5A;
	
	while(FALSE == READ(UCSR0A,6));
	
	UDR0 = 0xA5;
	
	//Wait until transmit is complete
	while(FALSE == READ(UCSR0A,6));
	
	
	//Flash the LED for a second to show that initialization has successfully occurred
	//For some reason this does not last 1s at all
	SET(PORTD,7);
	_delay_ms(1000);
	CLEAR(PORTD,7);
	
	//ADC all done! Now I can initiate a conversion by setting bit 6 (ADCSC) of ADCSRA to 1 and then polling 
	//bit 3 (ADCIF) until it turns to 1
	
	//Here we can turn the timer on
	//ATMega328 Datasheet Section 16.11.2 Pg 135 - TCCR1B
	//No Waveform generation: bits 4:3 = 0
	//No input capture: bits 7:6 = 0
	//Clock select: ClkIO/256 - bits 2:0 = 100b = 0x04
	
	TCCR1B = 0x04;	//This starts the counter/timer
	
	
	while(1)
    {

		//Determine what events need to be handled this frame
		
		//Handle system timer
		timer_overflow = READ(TIFR1,0);
			
		if(TRUE == timer_overflow)
		{
			SET(TIFR1,0);		//ATMega328 datasheet Section 16.11.9 pg137 - This clears the overflow flag

			TCNT1 = TIMER1_PERIOD;	//Reset the counter period
			ticks++;
			
			//Signal ADC Read - this is done every 1.25ms
			SET(events,EVENT_READ_ADC_Y);

			switch(ticks)
			{
				case 0:
				case 40:
				case 80:
				case 120:
				case 160:
				case 200:
				case 240:
				case 280:
				case 320:
				case 360:
				case 400:
					SET(events,EVENT_DECIMATE);
					break;
				default:
					break;
			}
			
			if(PERIOD_HB_LED == ticks)
			{
				ticks = 0;
				//Signal that this event occurred in this frame
				SET(events,EVENT_500MS);
			}
		}			
		
		//Check pushbutton
		if(TRUE == NOT(READ(PINC,4)))
		{
			SET(events,EVENT_PUSHBUTTON);
		}

/*
		//Handle 500ms tick
		if(TRUE == READ(events,EVENT_500MS))
		{
			TOGGLE(PORTD,7);
		}
*/
		//Handle reading ADC - Y axis
		if(TRUE == READ(events,EVENT_READ_ADC_Y))
		{

			SET(ADCSRA,6);	//Start conversion
			
			//Wait until conversion finishes - this should never be more than
			//25* (8000000/8)^-1 seconds, which is about 25us
			//Typical measured is ~14.5us
			while(FALSE == READ(ADCSRA,4));	
	
			SET(ADCSRA,4);	//Clear the interrupt flag by setting it to 1

			adc_y_axis = ADCH;	//Transfer ADC result to y-axis variable
			
		
			//filtered_y_axis accel in 32-bit fixed-point format
			filtered_y_axis = adc_lp_filter(adc_y_axis);
			filtered_y_axis = adc_hp_filter(filtered_y_axis);

			
		}	

		if(TRUE ==	READ(events,EVENT_DECIMATE))
		{
			velocity = compute_velocity(filtered_y_axis);


				UDR0 = (uint8_t)(filtered_y_axis>>24);
				while(FALSE == READ(UCSR0A,6));
				UDR0 = (uint8_t)(filtered_y_axis>>16);
				while(FALSE == READ(UCSR0A,6));

/*
			if(filtered_y_axis < -1*0x00060000)
			{
				UDR0 = (uint8_t)(filtered_y_axis>>24);
				while(FALSE == READ(UCSR0A,6));
				UDR0 = (uint8_t)(filtered_y_axis>>16);
				while(FALSE == READ(UCSR0A,6));
			}
*/

		}	
		
		if(TRUE == READ(events,EVENT_FORWARD_MOTION))
		{
			SET(PORTD,7);			
		}

		if(TRUE == READ(events,EVENT_HALT))
		{
			//Turn yellow light on for 3s, then 
			CLEAR(PORTD,7);
		}		
		//When the switch is pressed, people tend to think of this as 'true'
		//However, the voltage at the pin will be pulled down to 0, which is logical 'false'
		//Therefore, we NOT the value read from bit 4 of the PINB register
		
		//Haha, too bad this doesn't work because my footprints for the tactile switches weres wrong
		//So the switches are always pressed.
/*		
        pushbutton = NOT(READ(PINC,4));
		
		//TRUE meaning switch is depressed
		if(TRUE == pushbutton)
		{
			SET(PORTD,7);	//Turn on LED (port sources current)
		}
		else
		{
			CLEAR(PORTD,7);
		}		
*/
		//TOGGLE(PORTD,7);
		//PIND |= (1<<7);	

		
		//Clear all events in this frame
		events = 0x0000;
    }
}