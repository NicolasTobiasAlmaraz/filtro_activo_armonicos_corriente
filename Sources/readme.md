# Filtro Activo de Armonicos de Corriente

## Autor
Nicolás Almaraz

## Descripción
Simulador de Filtro Activo de Armonicos de Corriente. Lee señal del sensor de corriente y saca vía DAC la señal que se debería inyectar en la línea para quitarle los armónicos.
Se tiene un Display LCD para visulizar medición de THD.

## Video de Funcionamiento:
<a href="https://www.youtube.com/watch?v=SaUtxi-bsFg&t=262s&ab_channel=NicolasAlmaraz">
  <img src="https://img.youtube.com/vi/SaUtxi-bsFg/0.jpg?timestamp=1" alt="Video de Funcionamiento" width="500"/>
</a>

## Documentación
[Documentación Doxygen](https://rawcdn.githack.com/NicolasTobiasAlmaraz/filtro_activo_armonicos_corriente/50b7d0af559790f539d4cfe7f90ac27d37ef2cf7/Doc%20Firmware/html/index.html)

## Directorios
```
/sources/Core/Src/
|-- main.c
|-- active_current_harmonics_filter/
|   |-- active_current_harmonics_filter.c
|   |-- active_current_harmonics_filter.h
|   |-- callback_functions_manager.c
|   |
|   |-- subsystems/
|   |   |-- push_button/
|   |   |   |-- push_button.c
|   |   |   |-- push_button.h
|   |   |
|   |   |-- cycle_detector/
|   |   |   |-- cycle_detector.c
|   |   |   |-- cycle_detector.h
|   |   |
|   |   |-- display/
|   |   |   |-- display.c
|   |   |   |-- display.h
|   |   |
|   |   |-- signal_analyzer/
|   |   |   |-- signal_analyzer.c
|   |   |   |-- signal_analyzer.h
|   |   |
|   |   |-- current_sensor/
|   |   |   |-- current_sensor.c
|   |   |   |-- current_sensor.h
|   |   |
|   |   |-- inject_simulator/
|   |   |   |-- inject_simulator.c 
|   |   |   |-- inject_simulator.h
|   |   |
|   |-- drivers/
|   |   |-- display_i2c/
|   |   |   |-- display_i2c.c
|   |   |   |-- display_i2c.h
|   |   |
|   |   | -- timer_driver/
|   |   |   |-- timer_driver.c
|   |   |   |-- timer_driver.h
|   |
|   |-- libraries/
|   |   |-- CMSIS_DSP/
```
## Diagrama Modularización
![image](https://github.com/user-attachments/assets/1cc70c7a-63f9-454d-b883-44be9710dacb)
