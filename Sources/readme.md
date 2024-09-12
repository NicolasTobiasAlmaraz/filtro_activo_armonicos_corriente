# TP3: Trabajo Práctico Final - Curso de Sistemas Embebidos

## Alumno
Nicolás Almaraz - nnico.almaraz@gmail.com

# Estructura de Archivos Fuente
La aplicación que desarrollé se encuentra en la estructura de directorios detallada más abajo.

Por otro lado, es importante verificar las configuraciones de sistema (periféricos, clocks, DMA, etc) hechas en el archivo ".ioc".

Cabe destacar que el resto de archivos fuente son generados por STM32 Cube IDE correspondientes a drivers y bibliotecas de ARM.

Finalmente, otra aclaración importante es que para la correcta compilación del proyecto fue necesario indicar al compilador la inclusión de la biblioteca [CMSIS-DSP](https://github.com/ARM-software/CMSIS-DSP), ya que es una biblioteca estática.

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

# Video del funcionamiento:

[![Video de TP3](https://img.youtube.com/vi/SaUtxi-bsFg/0.jpg)](https://www.youtube.com/watch?v=SaUtxi-bsFg)
