# Programación de microcontroladores

Hola. Este es el repositorio de prácticas de la asignatura PdM del bimestre 1-2024

## Práctica 1

Esta práctica fue adaptada para utilizar una plaxa nucleo-746ZGTX por la disponibilidad del momento (físicamente es igual que la 429ZI solo que es cortexM7), así que para utilizar en una plaza 429ZI, es necesario solo copiar y reemplazar la función ```int main()``` en main.c

## Práctica 2

En main.h Se encuentran las funciones necesarias para el retardo no bloqueante, y en main.c se encuentra la lógica asociada a estas funciones y el uso de los leds. El punto 3 se encuentra en bloque de comentario

## Práctica 3

Se Genera archivo header y soource de la practica 2 para generar como API el uso de los retardos no bloqueantes. ```(\Driver\API\Inc\API_Delay.h y \Driver\API\Src\API_Delay.c )```  

## Práctica 4

#### Punto 1:
Se implementa FEM sin modularizar con el encendido del led 3 cada que ha pasado el tiempo de antirrebote.


#### Punto2 2:
Implementación de máquina de estados finitos de un antirebote por software. En la ruta ```(\Driver\API\)``` se encuentra el header y source del API_Debounce. Los led 1 y 2 van a parpadear de 100 a 500 ms que se calculan aleatoriamente, mientras el led 3 indicará si se ha presionado el boton de la STM.

### Estados de API_Debpunce

![Estados del API debounce](PRACTICA%204/FSM.png)


