/* Owned by Fernando Zaragoza - October 2019 */
#include "ESP32_TimInterrupt.h"
#include "Module.h"
#include <FunctionalInterrupt.h>

//void (*timer_handler)(void) = NULL;
/*
static const clkActivation_S availableCLK[NUM_CLK] = {
	{ true , false, false, false, false },
	{ true , true , false, false, false },
	{ true , true , true , false, false },
	{ true , true , true , true , false },
	{ false, false, false, false, true  }
};
*/

Timer::Timer()
{
	_ledc_timer;
	_timer = NULL;
	timerMux = portMUX_INITIALIZER_UNLOCKED;
}

Timer::~Timer()
{
	timerAlarmDisable(_timer);
	timerDetachInterrupt(_timer);
}

void Timer::pwmTimerConfig(ledc_mode_t speed_mode, ledc_timer_t timer_num, ledc_timer_bit_t bit_resolution, uint32_t freq_hz)
{
	_ledc_timer.speed_mode		= speed_mode;
	_ledc_timer.timer_num		= timer_num;
	_ledc_timer.duty_resolution = bit_resolution;
	_ledc_timer.freq_hz			= freq_hz;
	ledc_timer_config(&_ledc_timer);
}

void Timer::interruptInit(uint8_t timer_num, uint32_t sampling_time)
{
	_sampling_time = sampling_time;
	_timer			= timerBegin(timer_num, 80, true);
	timerFuncAttachInterrupt(_timer, std::bind(&Timer::handle, this), true);
	timerAlarmWrite(_timer, sampling_time, true);
	timerAlarmEnable(_timer);
	//checkClockActivation();
}

void Timer::enable(void) {
	flag.counter = 0;
	timerAlarmEnable(_timer);
}

void Timer::disable(void) {
	timerAlarmDisable(_timer);
}

bool Timer::checkTimer(void)
{
	if (flag.check_timer == 1)
	{
		portENTER_CRITICAL(&timerMux);
		flag.check_timer = false;
		portEXIT_CRITICAL(&timerMux);
		flag.counter++;
		runModules();
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
clkTimes_E Timer::checkClockActivation(void)
{
	/* Sampling time in us (miceoseconds) //
	switch (_sampling_time)
	{
	case 10000:

		break;
	case 20000:

		break;
	case 50000:

		break;
	case 100000:

		break;
	case 1000000:

		break;
	default:

		break;
	}

}
*/
void Timer::modulesInit(uint8_t count_modules, const moduleDesc_S modules[])
{
	_modules = modules;						///< @comment "The arg: modules[] is a pointer if only declared without * operator in the method arg, and without passing as reference in the .ino file."
	_count_modules = count_modules;
# if SERIAL_DEBUG
	Serial.print("There are: ");
	Serial.print(_count_modules);
	Serial.println(" modules to be run.");
# endif
}

void Timer::runModules(void)
{
# if SERIAL_DEBUG
	Serial.print("Running: ");
	Serial.print(_count_modules);
	Serial.println(" modules.");
# endif
	for (uint8_t i = 0; i < _count_modules; i++)
	{
		if ((_modules[i].periodicX1_CLK) != NULL )	///< @comment "Can use with pointer (*) or without." "Use nullptr or NULL."
		{
			((_modules[i]).periodicX1_CLK)();
		}
	}

}

void IRAM_ATTR Timer::handle(void)
{
	portENTER_CRITICAL_ISR(&timerMux);
	flag.check_timer = true;
	portEXIT_CRITICAL_ISR(&timerMux);
}
