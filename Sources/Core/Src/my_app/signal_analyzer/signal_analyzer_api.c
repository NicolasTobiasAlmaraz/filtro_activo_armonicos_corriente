/**
 * @file current_sensor.h
 * @author Nicolás Almaraz
 * @brief Procesamiento principal de señales para obtener THD e ic(t)
 */

//======================================
// Dependencias
//======================================
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

//======================================
// Declaración de Funciones Privadas
//======================================
/**
 * @brief Calculate one cycle average
 */
static cycle_t signal_analyzer_api_calculate_average_cycle(cycle_t *cycles, uint32_t len);

/**
 * @brief Calculate signal power
 */
static float signal_analyzer_api_calculate_sig_power(cycle_t cycle);

/**
 * @brief Calculate Fundamental Harmonic
 */
static sine_t signal_analyzer_api_calculate_fundamental(cycle_t cycle);

/**
 * @brief Calculate Fundamental Harmonic
 */
static float signal_analyzer_api_calculate_sine_power(sine_t sine);

/**
 * @brief Calculate THD
 */
static uint8_t signal_analyzer_api_calculate_thd(float fund_power, float harm_power);

/**
 * @brief Calculate signal to inject
 */
static cycle_t signal_analyzer_api_calculate_injection(sine_t fundamental, cycle_t ave_cycle, uint16_t zero_offset);

//======================================
// Implementación de Funciones Privadas
//======================================

cycle_t signal_analyzer_api_calculate_average_cycle(cycle_t *cycles, uint32_t len) {
	cycle_t cycle_prom;
	return cycle_prom;
}

float signal_analyzer_api_calculate_sig_power(cycle_t cycle) {
	return 0;
}

sine_t signal_analyzer_api_calculate_fundamental(cycle_t cycle) {
	sine_t fund;
	return fund;
}

float signal_analyzer_api_calculate_sine_power(sine_t sine) {
	return 0;
}

uint8_t signal_analyzer_api_calculate_thd(float fund_power, float harm_power) {
	return 0;
}

cycle_t signal_analyzer_api_calculate_injection(sine_t fundamental, cycle_t ave_cycle, uint16_t zero_offset) {
	cycle_t cycle;
	return cycle;
}


//======================================
// Implementación de Funciones Públicas
//======================================

void signal_analyzer_api_init() {

}

void signal_analyzer_api_start_new_analyze(cycle_t *cycles, uint32_t len, uint16_t zero_offset) {
	//todo hacerlo no bloqueante

	static float current_power;
	static sine_t fundamental;
	static float fundamental_power;

	// Average Signal
	g_ave_cycle = signal_analyzer_api_calculate_average_cycle(cycles, len);

	// Calculate Average Signal Power
	current_power = signal_analyzer_api_calculate_sig_power(g_ave_cycle);

	// Calculate Fundamental
	fundamental = signal_analyzer_api_calculate_fundamental(g_ave_cycle);

	// Calculate Fundamental Power
	fundamental_power = signal_analyzer_api_calculate_sine_power(fundamental);

	// Calculate THD
	g_thd = signal_analyzer_api_calculate_thd(fundamental_power, current_power);

	// Calculate Signal to Inject
	g_inject_cycle = signal_analyzer_api_calculate_injection(fundamental, g_ave_cycle, zero_offset);
}


uint8_t signal_analyzer_api_get_thd();

cycle_t signal_analyzer_api_get_inject_current();
