# Filtro Activo de Armónicos de Corriente
## Alumno:
Nicolás Almaraz - nnico.almaraz@gmail.com
## Profesores:
- Ariel Lutenber
- Sergio Caprile
- Juan Manuel Reta
- Eduardo Filomena
## Fecha: 
18/08/2024

## Selección del proyecto a implementar

En este caso se analiza la posibilidad de que el "Trabajo Práctico Final del Curso de Sistemas Embebidos" resuelva el sistema embebido requerido para desarrollar el "Trabajo Práctico Integrador de Electrónica de Potencia" dictado en el curso de Electrónica de Potencia en la carrera de Ing. Electrónica en UTN FRBA.
Este último consiste en hacer un "Filtro Activo de Armónicos de Corriente". Ahora bien, dado que el tiempo disponible es limitado voy a analizar la posibilidad de resolver la problemática completa vs resolverla parcialmente:
* Proyecto 1: Filtro activo de armónicos de corriente (Resolución parcial: Fase 1/2)
* Proyecto 1: Filtro activo de armónicos de corriente (Resolución completa)

Los aspectos a analizar a la hora de elegir entre los dos proyectos son:
* Tiempo de implementación
* Disponibilidad de Hardware
* Gusto y conocimientos
* Utilidad del sistema

En la Tabla 1 se aprecia una comparativa de ambos proyectos:

| Proyecto      |                   | Tiempo de Implementación (Peso 10) | Gusto  y conocimientos (Peso 7) | Utilidad del Sistema (Peso 8)  | Disponibilidad de hardware (Peso 10) | Peso Final |
|---------------|-------------------|------------------------------------|---------------------------------|--------------------------------|--------------------------------------|------------|
| Proyecto 1    | Puntaje           |9                                   |8                                |8                               |8                                     |            |
| Proyecto 1    | Puntaje Ponderado |90                                  |56                               |64                              |80                                    |290         |
| Proyecto 2    | Puntaje           |6                                   |9                                |9                               |6                                     |            |
| Proyecto 2    | Puntaje Ponderado |60                                  |63                               |72                              |60                                    |255         |

_Tabla 1: Comparación de puntajes ponderados de los posibles_

En cuanto a tiempo de implementación gana ampliamente la resolución parcial.
Esto se debe principalmente a que el hardware correspondiente a la etapa de salida del proyecto completo aún está en desarrollo.
Entonces debido a esta limitación de hardware creo que lo ideal es trabajar sobre un proyecto parcial que será una base muy completa para futuro ser escalado.
Como conlcusión, el proyecto 1 es más conveniente de realizar como trabajo práctico final

## Descripción de ambos proyectos 
### Objetivo a largo plazo
El proyecto total consiste en un sistema que mide la forma de onda de corriente de una línea eléctrica a tensión reducida de 24VAC.
La idea de funcionamiento sería la siguiente:
- Se lee la fomra de onda de la corriente mediante un sensor de efecto Hall
- Se procesa la señal para determinar la cantidad de armónicos 
- Se inycta a la salida los armónicos en contrafase (a excepción de la fundamental)

De esta manera se consigue que la carga consuma la forma de onda natural para su funcionamiento pero la línea eléctrica consume una senoidal lo más puro posible

#### Ejemplo
Si tenemos un circuito como este de carga en la línea:
![alt text](<img/ej1.jpg>)

La manera de quitar los armónicos de la línea será inyectando armónicos de corriente en contrafase:
![alt text](<img/ej2.jpg>)

## Objetivo para el trabajo práctico del Curso de Sistemas Embebidos
La finalidad de este trabajo práctico final es asentar las bases para conseguir lo descrito en el título anterior.

Para ello la idea sería dejar listas las etapas de medición de entrada y procesamiento.
Dado que la etapa de potencia aún está en desarrollo.

Para ello voy a tener un diagrama en bloques como el siguiente para la conexión de hardware:
![alt text](<img/diag_bloq.jpg>)
 
Y para testear al correcto funcionamiento se plantea el siguiente diagrama:
![alt text](<img/test.jpg>)

### Elicitación de requisitos y casos de uso

#### Requerimientos

La Tabla 2 se presentan los requerimientos del proyecto.

| Grupo de Requerimiento  | Requerimiento  | Descripción                                                                                                                                     |
|-------------------------|----------------|-------------------------------------------------------------------------------------------------------------------------------------------------|
| Sensado de corriente    | 1.1            | El sistema deberá tener un filtro anialias a la entrada (w0=10kHz).                                                                             |
| Sensado de corriente    | 1.2            | El ADC será configurado por DMA en modo continuo con una frecuencia de muestreo de 10kHz                                                        |
| Aislación               | 1.3            | La lógica en todo momento debe estar aislada de la línea                                                                                        |
| Salida analógica        | 2.1            | La salida para mostrar la señal correctora será vía DAC                                                                                         |
| Procesamiento           | 3.1            | Se deberá calcular el primer armónico de corriente mediante FFT                                                                                 |
| Procesamiento           | 3.2            | Se debe conseguir la "forma de onda correctora" haciendo la resta entre la fundamental y la forma de onda de la línea                           |
| Procesamiento           | 3.3            | El sistema debe corregir en régimen permanente (no transitorios)                                                                                |
| Procesamiento           | 3.4            | La entrada y salida deben estar sincronizadas en fase mediante el detector de cruce por 0V                                                      |
| Display                 | 4.1            | En el display de caracteres LCD se mostrará THD y valor eficaz de la señal de entrada                                                           |
| Testeo                  | 5.1            | Para verificar el correcto funcionamiento se debe restar la señal de corriente de línea con la señal generada por el DAC y obtener una senoidal |

_Tabla 2: Requerimientos del sistema_

#### Casos de uso
* El sistema corregirá armónicos de corriente para una carga fija (Rectificador + RC), por lo que si el usuario cambia la carga, el sistema tendrá un tiempo de establecimiento considerable hasta que vuelva a sincronizarse

* El tiempo de establecimiento no es algo significante dado que el objetivo es corregir en régumen permanente

### Hardware
- Nucleo F429ZI
- Filtro RC antialias
- Filtro RC para suavizar salida del DAC
- Sensor de corriente por efecto Hall
- Detector de cruce por 0V optoacoplado

### Periféricos
- GPIO IN: Cruce por 0v
- I2C: Display
- ADC: Señal de corriente
- DAC: Señal de salida

#### Desarrollos a futuro
- El lazo de control para que quede un sistema realimentado
- Cambiar la salida DAC por una salida PWM que controle el driver de una salida puente transistorizada para la inyección de corriente
- Agregar un pulsador como habilitación para activar o desactivar la corrección
