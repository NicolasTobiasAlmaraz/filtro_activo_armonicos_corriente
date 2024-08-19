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
* Proyecto 1: Filtro activo de armónicos de corriente  - Fase 1/2
* Proyecto 2: Filtro activo de armónicos de corriente (Resolución completa)

A continuación analizaré la viabilidad de estas dos opciones más adelante analizaremos los detalles de funcionamiento de las dos opciones.

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

Como conlcusión, el proyecto más conveniente de realizar es el "Filtro Activo de Armónicos de Corriente - Fase 1/2" como trabajo práctico final

## Descripción de ambos proyectos 
### Objetivo para el proyecto completo
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
 
Y para testear al correcto funcionamiento se plantea el siguiente diagrama.

La idea sería que teniendo la forma de onda de la línea y le sumamos la señal generada por el controlador obtenemos como resultado la senoidal fundamental

![alt text](<img/test.jpg>)

### Hardware
- Nucleo F429ZI
- Filtro RC antialias
- Filtro RC para suavizar salida del DAC
- Sensor de corriente por efecto Hall
- Detector de ciclos de la línea optoacoplado (3.3V semiciclo (+) y 0v semiciclo (-)) 

### Double buffering en la entrada y la salida
Tanto la entrada como la salida tendrán un esquema de ping pong buffering, es decir, existirán los punteros
- ptr_DMA_in
- ptr_procesador_in
- ptr_procesador_out
- ptr_DMA_out

La idea es que simultáneamente:
- El ADC cargue ptr_DMA_in
- El procesador procese los datos ptr_procesador_in
- El procesador escriba los datos de salida en ptr_procesador_out
- El DAC saque los datos de ptr_DMA_out

Idealmente:
- el procesador debe terminar antes que los DMA de entrada y salida
- los DMA de entrada y salida deben terminar simultáneamente

Una vez que terminaron los DMA, se hace un trueque de buffers:
- ptr_procesador --> Pasa a ser el del DMA
- ptr_DMA        --> Pasa a ser el del procesador

Se repite el ciclo.

Trabajar con DMA hará que el procesador no esté demasiado cargado.

Más información: https://es.wikipedia.org/wiki/Buffer_m%C3%BAltiple

### Elicitación de requisitos y casos de uso

#### Requerimientos

La Tabla 2 se presentan los requerimientos del proyecto.

| Grupo de Requerimiento  | Requerimiento  | Descripción                                                                                                                                     |
|-------------------------|----------------|-------------------------------------------------------------------------------------------------------------------------------------------------|
| Aislación               | 1.0            | La lógica en todo momento debe estar aislada de la línea de 24VAC                                                                               |
| Deteccion de ciclos     | 2.0            | El sistema se sincroniza con la linea mediante un detector de ciclos                                                                            |
| Sensado de corriente    | 3.1            | El sistema deberá tener un filtro anialias a la entrada (f0 = 5kHz).                                                                            |
| Sensado de corriente    | 3.2            | El ADC tomará muestras de corriente de línea con fs=20kHz (Ts=50 us) por una ventana de 50 ciclos                                               |
| Salida analógica        | 4.1            | La salida para mostrar la señal correctora será vía DAC                                                                                         |
| Sistema de buffers      | 5.1            | Se tendrá un esquema double-buffering tanto para tomar muestras como para sacarlas                                                              |
| Procesamiento           | 6.1            | Se deberá calcular la componente fundamental de corriente                                                                                       |
| Procesamiento           | 6.2            | Se debe conseguir la "forma de onda correctora" haciendo la resta entre la fundamental y la señal muestrada                                     |
| Procesamiento           | 6.3            | El sistema debe corregir en régimen permanente (No importa si el procesamiento / muestreo es lento)                                             |
| Display                 | 6.1            | En el display de caracteres LCD se mostrará THD de la señal de entrada                                                                          |
| Testeo                  | 7.1            | Para dar por válido el funcionamiento se debe cumplir que señal_50Hz(t) = muestreo(t) - señal_correctora(t). Para validar esto se usa la operación math del osciloscopio|

_Tabla 2: Requerimientos del sistema_

#### Casos de uso

##### Procesamiento
_Disparador_
Llegaron 50 ciclos (idealmente 1seg con F=50Hz)

_Precondicion_
Se haya previamente iniciado el timer de sincronizacion: timer_sinc_start()

_Flujo basico_
- Switch buffers
- tiempo_muestreo = timer_sinc_start()    (idealmente 1seg)
- muestras = tiempo_muestreo * fs         (idealmente 20 mil con fs=20kHz)
- Calculo FFT de la señal muestreada a la entrada
- Identifico modulo y fase de la fundamental
- Identifico modulo de las armonicas
- Calculo THD
- Completo el vector uint32_t fundamental[muestras]; con la señal fundamental repetira múltiples veces
- Calculo sig_correctora(t) = sig_linea(t) - fundamental(t)
- actualizo display

_Flujo alternativo_
Me quedo esperando

##### Display
_Disparador_
Pidieron actualizar display

_Precondicion_
Hayan inicializado el display

_Flujo basico_
Actualiza valor de THD mostrado

_Flujo alternativo_
No cambia lo mostrado en pantalla

### Periféricos Utilizados
- GPIO IN: Detector de ciclos
- I2C: Display
- ADC: Señal de corriente
- DAC: Señal de salida

#### Desarrollos a futuro
- El lazo de control para que quede un sistema realimentado
- Cambiar la salida DAC por una salida PWM que controle el driver de una salida puente transistorizada para la inyección de corriente
- Agregar un pulsador como habilitación para activar o desactivar la corrección
