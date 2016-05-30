// Cpu frequency for util/delay.h
#define F_CPU 1000000

// Half of delay in main execution loop, given in milliseconds
#define LOOP_DELAY 120

// Half length of indicator led on-off sequence. In other words, the time
// the indicator is lit and darkened. Given in units of LOOP_DELAY.
#define INDICATOR_HALF_PERIOD 4
