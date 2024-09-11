# TP3: Trabajo Práctico Final - Curso de Sistemas Embebidos

## Alumno
Nicolás Almaraz - nnico.almaraz@gmail.com

# Estructura de Archivos Fuente

Mí aplicación se en los siguientes códigos fuente:

```
/sources/Core/Src/
|
|-- main.c
|-- my_app/
|   |
|   |-- app_inject_simulator/
|   |   |-- app_inject_simulator.c
|   |   |-- app_inject_simulator.h
|   |
|   |-- app_processing/
|   |   |-- app_processing.c
|   |   |-- app_processing.h
|   |   |
|   |   |-- current_sensor_api/
|   |   |   |-- current_sensor_api.c
|   |   |   |-- current_sensor_api.h
|   |   |
|   |   |-- display_api/
|   |   |   |-- display_api.c
|   |   |   |-- display_api.h
|   |   |   |-- display_driver/
|   |   |       |-- display_driver.c
|   |   |       |-- display_driver.h
|   |   |
|   |   |-- push_button_driver/
|   |   |   |-- push_button_driver.c
|   |   |   |-- push_button_driver.h
|   |   |
|   |   |-- signal_analyzer_api/
|   |   |   |-- signal_analyzer_api.c
|   |   |   |-- signal_analyzer_api.h
|   |   |   |-- CMSIS_DSP/
|   |
|   |-- common_apis/
|   |   |-- callback_functions_manager.c
|   |
|   |   |-- timer_api/
|   |   |   |-- timer_api.c
|   |   |   |-- timer_api.h
|   |
|   |   |-- cycle_detector/
|   |   |   |-- cycle_detector.c
|   |   |   |-- cycle_detector.h

```

El resto de archivos fuente son generados por STM32 Cube IDE

Por otro lado, es importante verificar las configuraciones de sistema hechas en el IOC

# Video del funcionamiento:
[Video de Nicolás Almaraz](https://www.youtube.com/watch?v=SaUtxi-bsFg&ab_channel=NicolasAlmaraz)
