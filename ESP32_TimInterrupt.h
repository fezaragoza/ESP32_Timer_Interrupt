/* Owned by Fernando Zaragoza - September 2019 */
#ifndef _TIMERS_H
#define _TIMERS_H

/******************************************************************
 *                         I N C L U D E S                        *
 ******************************************************************/
#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <driver/ledc.h>
#include "Module.h"

/******************************************************************
 *                         D E F I N E S                          *
 ******************************************************************/
/* Fixed sampling times definitions */
#define MS1000			1000000
#define MS100			100000						// 100ms, value in us.
#define MS50			50000
#define MS20			20000
#define MS10			10000
/* Time conversion constants */
#define MS2MIN			0.00001666F
#define MSTIMESAMPLING (SAMPLINGTIME * 0.001F)
#define TIMECONVERSION (MSTIMESAMPLING * MS2MIN)
/* Timer config options for PWM generation */
#define ledPin			16					// 16 corresponds to GPIO16
#define frequen			5000				// 5kHz
#define ledChannel		0					// 0 to 15 (16 channels in total)
#define BIT_RESOL		8					// 1 to 16 bits - 8 means from 0 to 255
/* Modes, Channels and Timer number definitions */
#define LEDC_HS_MODE	LEDC_HIGH_SPEED_MODE
#define LEDC_TIMER0		LEDC_TIMER_0
#define LEDC_TIMER1		LEDC_TIMER_1
#define LEDC_TIMER2	    LEDC_TIMER_2
#define LEDC_8BITR		LEDC_TIMER_8_BIT
#define LEDC_FREQ		2000				// In Hz
#define LEDC_CH0		LEDC_CHANNEL_0
#define LEDC_CH1		LEDC_CHANNEL_1
#define LEDC_CH2		LEDC_CHANNEL_2
#define LEDC_CH3		LEDC_CHANNEL_3

/******************************************************************
 *                      T Y P E D E F S                           *
 ******************************************************************/

typedef struct s_flags {
	volatile unsigned int	counter;
	volatile bool			check_timer;
}flags_S;

typedef float  float32_t;
typedef double float64_t;

/******************************************************************
 *                        C L A S S E S                           *
 ******************************************************************/

class Timer
{
public:
	Timer();
	~Timer();
	volatile int serial;
	volatile int change;
	volatile int motor_control;
	void IRAM_ATTR handle(void);
	void pwmTimerConfig(ledc_mode_t speed_mode, ledc_timer_t timer_num, ledc_timer_bit_t bit_resolution, uint32_t freq_hz);
	void interruptInit(uint8_t timer_num, uint32_t sampling_time);
	void enable(void);
	void disable(void);
	//clkTimes_E checkClockActivation(void);
	void modulesInit(uint8_t count_modules, const moduleDesc_S modules[]);
	void runModules(void);
	bool checkTimer(void);
	portMUX_TYPE timerMux;
	flags_S		 flag;

private:
	ledc_timer_config_t _ledc_timer;
	hw_timer_t*			_timer;
	uint32_t			_sampling_time;
	const moduleDesc_S*	_modules;
	uint8_t				_count_modules;
	//clkActivation_S     _clk_active;
};

#endif

/**
* There are two options, leaving the private 
*/