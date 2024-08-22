# Definición de requisitos y casos de uso del Trabajo Final
## Alumno:
Nicolás Almaraz - nnico.almaraz@gmail.com

## Profesores:
- Ariel Lutenberg
- Sergio Caprile
- Juan Manuel Reta
- Eduardo Filomena

## Fecha: 
18/08/2024

## Selección del proyecto a implementar

En este caso se analiza la posibilidad de que el "Trabajo Práctico Final del Curso de Sistemas Embebidos" resuelva el sistema embebido requerido para desarrollar el "Trabajo Práctico Integrador de Electrónica de Potencia" dictado en el curso de Electrónica de Potencia en la carrera de Ing. Electrónica en UTN FRBA.

Este último consiste en un "Filtro Activo de Armónicos de Corriente". Ahora bien, dado que el tiempo disponible es limitado se analizará la posibilidad de resolver la problemática completa vs resolverla parcialmente:
* Proyecto 1: Filtro activo de armónicos de corriente (Solución parcial)
* Proyecto 2: Filtro activo de armónicos de corriente (Solución completa)

Los criterios a considerar para la comparativa de los dos proyectos son:
* Tiempo de implementación
* Disponibilidad de Hardware
* Gusto y conocimientos
* Utilidad del sistema

### Comparativa entre ambos proyectos
| Proyecto      |                   | Tiempo de Implementación (Peso 10) | Gusto  y conocimientos (Peso 7) | Utilidad del Sistema (Peso 8)  | Disponibilidad de hardware (Peso 10) | Peso Final |
|---------------|-------------------|------------------------------------|---------------------------------|--------------------------------|--------------------------------------|------------|
| Proyecto 1    | Puntaje           |9                                   |8                                |8                               |8                                     |            |
| Proyecto 1    | Puntaje Ponderado |90                                  |56                               |64                              |80                                    |290         |
| Proyecto 2    | Puntaje           |6                                   |9                                |9                               |6                                     |            |
| Proyecto 2    | Puntaje Ponderado |60                                  |63                               |72                              |60                                    |255         |

_Tabla 1: Comparación de ambos proyectos_

En cuanto a tiempo de implementación gana ampliamente la resolución parcial.
Esto se debe principalmente a que el hardware correspondiente a la etapa de salida del proyecto completo aún está en desarrollo.
Entonces debido a esta limitación de hardware creo que lo ideal es trabajar sobre un proyecto parcial que será una base muy completa para futuro ser escalado.
En resumen, el proyecto más conveniente de realizar es el "Filtro Activo de Armónicos de Corriente (Solución parcial)" como trabajo práctico final

## Descripción del Trabajo Práctico Final de Electrónica de Potencia
Para comenzar, antes de describir el proyecto a realizar en el "TP3 del Curso de Sistemas Embebidos", se dará una introducción al proyecto al que apunto a largo plazo. Por lo tanto, es importante tener en cuenta estos aspectos a la hora de plantear requerimientos y casos de uso para la solución parcial desarrollada en el Curso de Sistemas Embebidos.

El proyecto de Electrónica de Potencia consiste en un filtro activo de armónicos de corriente <sup>[1]</sup>. Es decir, es un sistema que se encarga de medir la forma de onda de corriente de una línea eléctrica (En nuestro caso a tensión reducida de 24 VCA) y eventualmente si corresponde corrige la distorsión armónica<sup>[2]</sup>. En otras palabras, busca que la corriente consumida por la línea sea senoidal pura. La metodología para corregir la distorsión armónica es consumir en contrafase los armónicos indeseados. Cabe destacar que la carga va a seguir consumiendo una corriente distorsionada. Lo que estamos solucionando con el filtro es que la corriente de la línea sea senoidal pura.

#### Caso de uso y marco teórico
En la *Imagen 1* se puede observar un ejemplo de carga que no es SLIT (Sistema Lineal e Invariante en el Tiempo). En este caso el rectificador es una carga que es alimentada con una tensión senoidal pero la corriente que consume no lo es (sistema no lineal).

Este fenómeno trae múltiples desventajas técnicas desde el punto de vista de la instalación eléctrica. En primer lugar, genera ruido eléctrico que puede interferir en otras cargas de la misma línea. Por otro lado, el transformador encargado de suministrar energía a la red está preparado para trabajar linealmente en la frecuencia adecuada (50 Hz en Argentina) y no en todo el espectro. Por último, tener cargas no lineales implica componentes de potencia significativa en armónicos, esto se resume en que tengamos alta potencia de deformación (D) y en consecuencia bajo factor de potencia (FP). 

En resumen, por todas estas razones, las cargas no lineales deterioran el transformador del proveedor de energía (por ejemplo, EDESUR) y contaminan el suministro a los demás usuarios (por ejemplo, los vecinos de mí cuadra). Debido a esto es que el proveedor de energía eléctrica cobra multas por tener bajo FP. Cabe destacar que todo esto es muy relevante a nivel industrial y no es tan crítico a nivel doméstico.

![alt text](<img/ej1.jpg>)

*Imagen 1: Carga no lineal*

#### Funcionamiento

![alt text](<img/ej2.jpg>)

*Imagen 2: Funcionamiento conceptual*

Para solucionar todos estos problemas detallados en la sección anterior se utilizan filtros activos de armónicos de corriente. En la *Imagen 2* se puede ver el principio de funcionamiento adoptado para implementar el proyecto (existen otros esquemas más eficientes pero más complejos).

*1 - Medición de corriente*

Se mide la corriente de línea. En nuestro caso utilizamos un sensor de efecto Hall<sup>[3]</sup> ACS712<sup>[4]</sup>. Entonces de esta manera conseguimos una medición de corriente aislando la lógica de la línea.

*2 - Procesamiento*

Basándonos en la medición anterior podemos identificar a la componente fundamental.
Cabe destacar que en equipos profesionales, lo correcto sería utilizar un DSP más poderoso debido a que debería corregir transitorios. Sin embargo, en nuestro caso solamente nos vamos a limitar a corregir en régimen permanente. En consecuencia, tanto el procesamiento como las mediciones pueden demorarse. Entonces, esto nos habilita a usar algoritmos lentos, por ejemplo, una FFT<sup>[5]</sup>.

Una vez que tenemos la fundamental podemos aplicar la ley de Kirchhoff para determinar la corriente a inyectar:

I(t) = IL(t) + IC(t)

Donde:
- I: Corriente de línea - Medida con el sensor
- IL: Corriente de carga
- IC: Corriente correctora - Corriente a inyectar

Por lo tanto, si queremos que la corriente I(t) sea senoidal pura:

I(t) = fundamental(t)

Quedando entonces:

IC(t) = fundamental(t) - IL(t)

*3 - Se consume corriente de corrección*

Ahora que ya se conoce la corriente necesaria, el microcontrolador debe generar las señales necesarias para controlar a la etapa de salida, generando así la corriente IC(t). En este punto es muy importante conocer la función transferencia que vincula la tensión generada por el microcontrolador con la corriente consumida en la línea.

En la *Imagen 2* se muestra únicamente el espectro en módulo de las corrientes y no hay que perder de vista que también está el espectro de fase. Es por eso que matemáticamente es incorrecto restar "armónico por armónico" como se hace en el gráfico (no existen módulos negativos). Sin embargo, me tomé la licencia de representarlo de esa manera a modo didáctico. Ahora bien, lo correcto en realidad es sumarle a los armónicos indeseados, una corriente de igual módulo y desfasado 180°.

Esto último que se menciona es de suma importancia porque es el efecto similar a lo que ocurre cuando la potencia reactiva de un capacitor suple al consumo de potencia reactiva de un inductor (en el "triángulo de potencias" decimos que se anulan porque la energía de ambos componentes está desfasada 180°). Entonces, en nuestro caso utilizaremos el mismo principio físico para eliminar armónicos.

*4 - Sincronización*

En el punto anterior es muy importante que la corriente hay que "empezar a consumirla" en el momento adecuado, es decir, la inyección de corriente debe estar sincronizada con la tensión de línea. De lo contrario, el resultado no será el esperado. 

En nuestro caso la metodología de sincronización la hacemos con un detector de ciclos. Este circuito se encarga de detectar ciclos. Entrega 3.3 V cuando la tensión de línea está en el semi-ciclo positivo y 0 V cuando.

La lógica consiste en samplear una cantidad entera de ciclos (delimitados por el circuito anterior). Cuando se calcula la componente fundamental por fourier se obtiene una función como la siguiente:

fundamental(t) = A0 * cos(w0 * t + θ)

El ángulo "θ" nos dice que existe un desfasaje entre la detección del inicio del ciclo y la ubicación de la fundamental. Por ende, cuando se hace la operación:

IC(t) = fundamental(t) - I(t)

Se obtiene directamente el vector a inyectar en la línea referido al inicio del ciclo.

Un dato interesante es que cambiando el valor de θ, forzándolo a gusto podemos cambiar la naturaleza con la que la línea ve al sistema (capacitivo, resistivo o inductivo). Es decir, además de conseguir THD = 0 % podríamos conseguir que FP = 1. La gran consideración que hay que tener es que si hacemos esto, la etapa de potencia debe suministrar una cantidad de energía considerable.

## Descripción del Trabajo Práctico Final del Curso de Sistemas Embebidos
La finalidad de este trabajo práctico final es asentar las bases para conseguir lo descrito en la sección anterior, "Descripción del Trabajo Final de Electrónica de Potencia".

Para ello la propuesta es desarrollar las etapas de medición y procesamiento. La etapa de potencia aún está en desarrollo y por eso no se incluye en el presente trabajo práctico. 

Sin embargo, el objetivo será entonces, medir la señal de corriente de línea, calcular la corriente IC(t) y mostrar dicho cálculo en la salida DAC. Por otro lado, también se debe procesar la señal de corriente de línea y calcular el THD para mostrarlo en el display.

En la *Imagen 3* podemos ver de manera simplificada las señales de entrada y salida del sistema:

![alt text](<img/diag_bloq.jpg>)

*Imagen 3: Diagrama de señales de entrada y salida*

Entradas:
- Circuito detector de ciclos
- Medición de corriente de línea

Salidas:
- Display LCD
- Forma de onda de la corriente que se debería inyectar en la línea para eliminar los armónicos indeseados

### Testeo
Para verificar el correcto funcionamiento propongo hacer el ensayo que se muestra en la *Imagen 4*:

![alt text](<img/test.jpg>)

La manera de testear al sistema es midiendo las formas de onda de entrada y salida con un osciloscopio:
- CH1: Corriente de linea: I(t)
- CH2: Corriente a inyectar: IC(t)

NOTA: Ambos canales del osciloscopio mide señales de tensión cuya forma de onda es proporcional a la corriente.

De esta manera si se utiliza la función "math" del osciloscopio debería obtenerse una corriente senoidal:

fundamental(t) = I(t) + IC(t)

### Hardware

#### Diagrama general
En la *Imagen 5* se muestra el circuito esquemático a utilizar:

![alt text](<img/circuito.png>)

*Imagen 5: Diagrama en bloques general*

#### Circuito de acondicionamiento ACS712
En la *Imagen 6* se ve el diagrama en bloques del sensor ACS712. Como se observa, la entrada de corriente se mide con un shunt y está aislada de la salida.

![alt text](<img/sens_corr_datasheet.png>)

*Imagen 6: Imagen del sensor y diagrama en bloques del circuito integrado*

Un punto a tener en cuenta es que la señal viene acondicionada entre 0 y 5 V. Por eso hay que re-acondicionarlo para ajustarle el rango dinámico a 0 - 3.3 V.

Por otro lado, hay que añadirle un filtro para evitar aliasing al momento de muestrear en el microcontrolador. En este caso la frecuencia de corte es de 5 kHz, es decir, vamos a leer hasta el armónico 100.

![alt text](<img/acond_sens_corr.png>)

*Imagen 7: Acondicionamiento sensor de corriente*

En la *Imagen 8* e *Imagen 9* se pueden ver las simulaciones en tiempo y en frecuencia para una señal de corriente de 5A.

![alt text](<img/acond_sens_corr_t.png>)

*Imagen 8: Respuesta temporal para entrada de 5A*

![alt text](<img/acond_sens_corr_f.png>)

*Imagen 9: Respuesta en frecuencia para entrada de 5A*

#### Circuito Detector de Ciclos
Este circuito entrega 3.3 V cuando llega cuando la tensión de línea está en el semi-ciclo positivo y 0 V cuando está en el negativo. En la *Imagen 10* se ve el circuito y en la *Imagen 11* las señales de entrada (línea) y salida (tensión lógica).
Cabe destacar que la lógica está aislada de la línea mediante un opto-acoplador.

![alt text](<img/detect_ciclos.png>)

*Imagen 10: Circuito de detección de ciclos*

![alt text](<img/detect_ciclos_t.png>)

*Imagen 11: Formas de onda de entrada (tensión de línea) y salida (tensión lógica)*

#### Circuito Display
Para el display se utiliza el esquema de la *Imagen 12*

![alt text](<img/display.png>)

*Imagen 12: Display*

#### Circuito DAC
Para el DAC simplemente voy a utilizar un circuito RC para suavizar las altas frecuencias del DAC (*Imagen 13*)

![alt text](<img/dac.png>)

*Imagen 13: DAC*

Cabe destacar que esta salida es provisoria. En el proyecto final la salida no será DAC sino que será PWM y controlarán a la etapa de potencia que se encarga de transformar la señal PWM a corriente consumida en la línea.

### Adquisición de datos
Para adquirir los datos de manera eficiente propongo la implementación de la técnica de "double buffering en la entrada y la salida"<sup>[6]</sup>.
Para el procesamiento en tiempo real es una buena estrategia para hacer uso eficiente del procesador. La idea es que el procesador no use demasiados recursos en samplear, queremos que se aproveche lo máximo posible para resolver las operaciones matemáticas requeridas en el procesamiento.

Entonces la técnica de double buffering (también llamado ping pong buffering) tiene un esquema de funcionamiento como el siguiente:

Existirán los punteros:
- ptr_DMA_in
- ptr_procesador_in
- ptr_procesador_out
- ptr_DMA_out

La idea es que simultáneamente:
- El ADC cargue ptr_DMA_in (haciendo uso del DMA)
- El procesador procese los datos ptr_procesador_in
- El procesador escriba los datos de salida en ptr_procesador_out
- El DAC saque los datos de ptr_DMA_out (haciendo uso del DMA)

Idealmente:
- el procesador debe terminar antes que los DMA de entrada y salida
- los DMA de entrada y salida deben terminar simultáneamente

De esta manera solamente se encarga de procesar los datos de ptr_procesador_in y los deposita en ptr_procesador_out
Una vez que terminó de procesar se queda esperando a que los DMA terminen de ingresar y sacar datos.

Una vez que terminaron los DMA, se hace un trueque de buffers (tanto en la entrada como en la salida):
- ptr_procesador --> Pasa a ser el del DMA
- ptr_DMA        --> Pasa a ser el del procesador

Este proceso se repite todo el tiempo. 

Trabajar con DMA hará que el procesador no esté demasiado cargado y se crea un esquema de "pipelines".

### Elicitación de requisitos y casos de uso

#### Requerimientos

La Tabla 2 se presentan los requerimientos del proyecto.

| Grupo de Requerimiento  | Requerimiento  | Descripción                                                                                                                                     |
|-------------------------|----------------|---------------------------------------------------------------------------------------------------------------------|
| Aislamiento             | 1.0            | La lógica en todo momento debe estar aislada de la línea de 24 VCA                                                  |
| Sincronización          | 2.0            | El sistema se sincroniza con la linea mediante un detector de ciclos                                                |
| Sensado de corriente    | 3.1            | El sistema deberá tener un filtro anti-alias a la entrada (f0 = 5 kHz).                                             |
| Sensado de corriente    | 3.2            | El ADC tomará muestras de corriente de línea con fs = 10 kHz (Ts = 50 us) por una ventana de 50 ciclos              |
| Salida analógica        | 4.1            | La salida para mostrar la señal correctora será vía DAC                                                             |
| Sistema de buffers      | 5.1            | Se tendrá un esquema double-buffering tanto para tomar muestras como para sacarlas                                  |
| Procesamiento           | 6.1            | Se deberá calcular la componente fundamental de corriente                                                           |
| Procesamiento           | 6.2            | Se debe conseguir la "forma de onda correctora" haciendo la resta entre la fundamental y la señal muestreada        |
| Procesamiento           | 6.3            | El sistema debe corregir en régimen permanente (No importa si el procesamiento / muestreo es lento)                 |
| Display                 | 6.1            | En el display de caracteres LCD se mostrará THD de la señal de entrada                                              |
| Testeo                  | 7.1            | Para verificar el funcionamiento usar el método de la sección "Testeo"                                              |

_Tabla 2: Requerimientos del sistema_

#### Casos de uso

##### Caso de uso Procesamiento
###### Disparador
Llegaron 50 ciclos // idealmente 1seg para freq de linea de 50 Hz

###### Precondición
Se haya previamente iniciado el timer de sincronización: timer_sinc_start()

###### Flujo basico
- Detengo DMA
- tiempo_muestreo = timer_sinc_stop()      //idealmente 1seg
- tiempo_muestreo = timer_sinc_start()     //Inicializo timer
- Switcheo buffers                         //Switcheo buffers del double buffering                  
- Inicializo DMA disparo único (ADC y DAC)
- muestras = tiempo_muestreo * fs // idealmente 20 mil con fs = 20 kHz
- Calculo FFT de la señal muestreada a la entrada
- Identifico modulo y fase de la fundamental
- Calculo THD
- Completo el vector uint32_t fundamental[muestras] con la señal fundamental (Repetida múltiples veces)
- Calculo sig_correctora(t) = sig_linea(t) - fundamental(t)
- actualizo display

###### Flujo alternativo
Me quedo esperando a que se termine la ventana de 50 ciclos

##### Caso de usoDisplay
###### Disparador
Llamen a la función updateDisplay()

###### Precondicion
Hayan inicializado el display

###### Flujo basico
Actualiza valor de THD mostrado

###### Flujo alternativo
No cambia lo mostrado en pantalla

### Periféricos Utilizados
- GPIO IN: Detector de ciclos
- I2C: Display
- ADC con DMA: Señal de corriente
- DAC con DMA: Señal de salida

### Desarrollos a futuro
- El lazo de control para que quede un sistema realimentado
- Cambiar la salida DAC por una salida PWM que controle el driver de una salida puente transistorizada para la inyección de corriente
- Agregar un pulsador como habilitación para activar o desactivar la corrección

## Referencias
[1] [Simulación de un filtro activo de armónicos de corriente con el software de simulación Plecs](https://www.plexim.com/sites/default/files/demo_models_categorized/plecs/single_phase_active_filter.pdf)

[2] [Tasa de Distorsión Armónica Total (THD)](https://es.wikipedia.org/wiki/Distorsi%C3%B3n_arm%C3%B3nica)

[3] [Efecto Hall](https://es.wikipedia.org/wiki/Efecto_Hall)

[4] [Sensor ACS712](https://www.allegromicro.com/en/products/sense/current-sensor-ics/zero-to-fifty-amp-integrated-conductor-sensor-ics/acs712)

[5] [FFT](https://es.wikipedia.org/wiki/Transformada_r%C3%A1pida_de_Fourier)

[6] [Double Buffering](https://es.wikipedia.org/wiki/Buffer_m%C3%BAltiple)

## Extra
Ejemplo de filtro activo industrial de la empresa española Circutor: [Link](https://circutor.com/articulos/funcionamiento-real-de-un-filtro-de-armonicos-activo/)
