/**
 * @file current_sensor.h
 * @author Nicolás Almaraz
 * @brief Procesamiento principal de señales para obtener THD e ic(t)
 */

//======================================
// Dependencias
//======================================
#include <stdio.h>
#include <stdlib.h>
#include <signal_analyzer/CMSIS_DSP/Include/arm_math.h>
#include "signal_analyzer_api.h"
#include "current_sensor/current_sensor_api.h"

//======================================
// Defines Privados
//======================================

//======================================
// Estructuras y Tipos de Datos Privados
//======================================
typedef struct {
	float amplitude;
	float frequency;
	float phase;
}sine_t;


//======================================
// Variables Privadas
//======================================
static cycle_t g_ave_cycle;
static cycle_t g_inject_cycle;
static uint8_t g_thd;
static uint32_t g_fs;

//======================================
// Declaración de Funciones Privadas
//======================================
/**
 * @brief Calculate one cycle average
 * @param cycles Sampled cycles
 * @retval average cycle
 *
 * This function takes 50 cycles sampled by the sensor current and reduces all these samples to an average cycle.
 * This cycle represents one period of the periodic signal.
 *
 * -------- Input: --------
 * Cycle 1:
 *
 *     * --> Glitch
 *    **
 *   *  *  *
 *       **
 *
 * Cycle 2:
 *
 *    **
 *   *  *  *
 *       **
 *        * --> Glitch
 * .....
 *
 * Cycle CYCLES:
 *
 *                * --> Glitch
 *    **    **    **    **
 *   *  *  *  *  *  *  *  *
 *       **    **    **
 *
 * -------- Output: --------
 *
 * Average Cycle:
 * (Only one cycle)
 *    **
 *   *  *  *
 *       **
 */
static cycle_t signal_analyzer_api_calculate_average_cycle(cycle_t *cycles, uint32_t num_cycles);

/**
 * @brief Calculate signal power
 * @param cycle One cycle of the signal
 * @retval power
 *
 * Energy = Σ {first_sample, last_sample} {sample**2}
 * Power = Energy / samples
 */
static float signal_analyzer_api_calculate_sig_power(cycle_t cycle);

/**
 * @brief Calculate Fundamental Harmonic
 * @param cycle This is a cycles of the signal
 * @param zero_offset This is the offset
 * @retval Returns the characteristic values of the fundamental harmonic (amplitude, frequency and phase)
 */
static sine_t signal_analyzer_api_calculate_fundamental(cycle_t cycle, uint16_t zero_offset);

/**
 * @brief Calculate the power of a sine
 *
 * power = amplitude **2 / 2
 *
 * @param sine
 * @retval power of the sine
 */
static float signal_analyzer_api_calculate_sine_power(sine_t sine);

/**
 * @brief Calculate THD
 * @param fund_power Power of the fundamental
 * @param harm_power Power of the harmonics
 *
 * THD% = 100% * power_harmonics / power_fundamental
 */
static uint8_t signal_analyzer_api_calculate_thd(float fund_power, float harm_power);

/**
 * @brief Calculate signal to inject
 * @param fundamental The wish signal of current in the line
 * @param ave_cycle One cycle of the current waveform
 * @retval The signal to inject each "start of cycle" interrupt
 */
static cycle_t signal_analyzer_api_calculate_injection(sine_t fundamental, cycle_t ave_cycle);

/**
 * @brief Calculate the minimal length of the cycles
 * @param cycles An array with different length cycles
 * @param num_cycles quantity of cycles
 * @retval The minimal cycle duration
 */
static uint32_t signal_analyzer_api_min_len(cycle_t *cycles, uint32_t num_cycles);

//======================================
// Implementación de Funciones Privadas
//======================================

cycle_t signal_analyzer_api_calculate_average_cycle(cycle_t *cycles, uint32_t num_cycles) {
	cycle_t cycle_prom;
	uint32_t cycle_i;
	uint32_t sample_i;
	uint32_t sum_sample=0;

	//Checks the minimun len of the samples
	uint32_t min_len = signal_analyzer_api_min_len(cycles, num_cycles);
	cycle_prom.len = min_len;

	//Align cycles and average the columns
	for( sample_i=0 ; sample_i<min_len ; sample_i++) {
		sum_sample = 0;
		for( cycle_i=0 ; cycle_i<num_cycles ; cycle_i++) {
			sum_sample += cycles[cycle_i].cycle[sample_i];
		}
		uint16_t prom = (uint16_t)(sum_sample / num_cycles);
		cycle_prom.cycle[sample_i] = prom;
	}
	return cycle_prom;
}

uint32_t signal_analyzer_api_min_len(cycle_t *cycles, uint32_t num_cycles) {
	uint32_t i;
	uint32_t min_len = cycles[0].len;
	for( i=1 ; i<num_cycles ; i++ ) {
		uint32_t len = cycles[i].len;
		if( len < min_len )
			min_len = len;
	}
	return min_len;
}


float signal_analyzer_api_calculate_sig_power(cycle_t cycle) {
	uint32_t energy = 0;
	for(uint32_t i=0 ; i<cycle.len ; i++) {
		energy += (cycle.cycle[i] * cycle.cycle[i]);
	}

	float power = (float) energy / cycle.len;
	return power;
}

sine_t signal_analyzer_api_calculate_fundamental(cycle_t cycle, uint16_t zero_offset) {
	float32_t *ptr_fft;
	float32_t *ptr_in;
	float32_t *magnitude;
	uint32_t len = cycle.len;

	//Assign memory for ptr in and ptr out
	ptr_in = (float32_t*) calloc (len, sizeof(uint16_t));
	ptr_fft = (float32_t*) calloc (len, sizeof(uint16_t));
	magnitude = (float32_t*) calloc (len/2, sizeof(uint16_t));

	//Filter Continuous Component of the signal
	for(uint32_t i=0; i<len ; i++) {
		ptr_in[i] = cycle.cycle[i] - zero_offset;
	}

	// --- Calculate the FFT ---

	//Init handler
	arm_rfft_fast_instance_f32 fft_handler;
	arm_rfft_fast_init_f32(&fft_handler, len);

	//Calculate FFT
	arm_rfft_fast_f32(&fft_handler, ptr_in, ptr_fft, 0);
    //Discard the negative components
	arm_cmplx_mag_f32(ptr_fft, magnitude, len/2);

	// Calculate the fundamental harmonic
	uint32_t fund_i;
	float32_t amplitude;
	arm_max_f32(magnitude, len/2, &amplitude, &fund_i);
	float32_t fund_freq = (g_fs*fund_i)/len;

	//Fill the return structure
	sine_t fundamental;
	fundamental.amplitude = amplitude;
	fundamental.frequency = fund_freq;
	fundamental.phase = 0;	//Assuming the correction wants cos(phi) = 1

	return fundamental;
}

float signal_analyzer_api_calculate_sine_power(sine_t sine) {
	return 0;
}

uint8_t signal_analyzer_api_calculate_thd(float fund_power, float harm_power) {
	return 0;
}

cycle_t signal_analyzer_api_calculate_injection(sine_t fundamental, cycle_t ave_cycle) {
	cycle_t cycle;
	return cycle;
}


//======================================
// Implementación de Funciones Públicas
//======================================

void signal_analyzer_api_init() {

}

void signal_analyzer_api_start_new_analyze(cycle_t *cycles, uint32_t len, uint16_t zero_offset) {
	static float current_power;
	static sine_t fundamental;
	static float fundamental_power;

	// Average Signal
	g_ave_cycle = signal_analyzer_api_calculate_average_cycle(cycles, len);

	// Calculate Average Signal Power
	current_power = signal_analyzer_api_calculate_sig_power(g_ave_cycle);

	// Calculate Fundamental
	fundamental = signal_analyzer_api_calculate_fundamental(g_ave_cycle, zero_offset);

	// Calculate Fundamental Power
	fundamental_power = signal_analyzer_api_calculate_sine_power(fundamental);

	// Calculate THD
	g_thd = signal_analyzer_api_calculate_thd(fundamental_power, current_power);

	// Calculate Signal to Inject
	g_inject_cycle = signal_analyzer_api_calculate_injection(fundamental, g_ave_cycle);
}


uint8_t signal_analyzer_api_get_thd() {
	return g_thd;
}

cycle_t signal_analyzer_api_get_inject_current() {
	return g_inject_cycle;
}

void signal_analyzer_api_set_fs(uint32_t fs) {
	g_fs = fs;
}
