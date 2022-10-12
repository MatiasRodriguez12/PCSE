# PCSE
Workspace para Protocolos de Comunicación en Sistemas Embebidos - CESE - FIUBA

Alumno: Rodriguez, Matías Nahuel

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

Este repositorio contiene un driver para el ADC ADS1115, diseñado como trabajo práctico final de la asignatura Protocolos de Comunicación en Sistemas Embebidos, perteneciente a la Carrera de Especialización en Sistemas Embebidos - FIUBA.
  
Dentro de la carpeta "ADS1115" se encuentran las subcarpetas "src" e "inc". Estas contienen los archivos fuentes y encabezados del driver.

Los archivos ADS1115.c y ADS1115.h contienen funciones genéricas. Estas funciones no dependen del hardware, lo que las vuelven portables.

Los archivos ADS1115_port.c y ADS1115_port.h contienen funciones específicas para la placa STM32 NUCLEO F429ZI.

Si se desea trasladar este driver a otra placa que no sea la nombrada anteriormente, las funciones definidas en ADS1115_port.c y ADS1115_port.h deben ser modificadas y adaptadas a esta nueva placa.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

Para lograr la comunicación entre placa y sensor se utiliza el protocolo I2C.

Se ofrecen dos tipos de controladores:

1) De tipo Polled Driver: Se espera hasta la habilitación del puerto READY para obtener los datos del registro de conversión.

2) Utilizando un retardo bloqueante fijo (en ms): En estas condiciones no es requerida la habilitación del puerto READY.
   Esta ultima opción posibilita al ADC a trabajar en modo comparador.
   
A su vez, las funciones se pueden dividir en tres:

1) Funciones de iniciación: Se encargan de configurar un canal de conversión, seteando los parámetros segun una configuración por defecto.

2) Funciones de conversión: Se encargan de iniciar la conversión y leer el valor almacenado en el registro de conversión.
 
3) Funciones de actualización: Se encargan de actualizar los parámetros del canal de conversión.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

El driver configura un canal de conversión utilizando estructuras. Estas estructuras contienen los valores de todos los parámetros del canal.

Dicha estructura se define de la siguiente manera:

typedef struct{

   uint8_t channel
   
   uint8_t pga;
   
   uint8_t operationMode;
   
   uint8_t dataRate;
   
   uint8_t comparadorMode;
   
   uint8_t comparadorPolarity;
   
   uint8_t latchingComparador;
   
   uint8_t stateComparator;
   
   uint16_t countConversion;

} signalADS1115;

Donde:

channel: parámetro de multiplexor. Define si se va a utilizar un solo canal en modo individual, o dos canales simultaneos trabajando en modo diferencial.

pga: ganancia del amplificador. Configura el valor de fondo de escala para la conversión.

operationMode: indica si se va a usar el canal en modo single-shot o continuous.

dataRate: indica la frecuencia de muestreo.

comparatorMode: indica el modo de comparador que se va a utilizar.

comparatorPolarity: indica la polaridad que va a poseer el puerto ALERT/READY cuando se encuentre activo.

latchingComparador: indica el instante en que se produce el latch del comparador.

stateComparador: habilita/deshabilita el uso del puerto ALERT/READY. Si se desea habilitar, se debe indicar el número de conversiones a ejecutarse antes de su activación.

countConversion: espacio destinado a almacenar el valor leído del registro de conversión.

-----------------------------------------------------------------------------------------------------------------------------------------------------------------------

Para utilizar las funciones diseñadas en este driver, se debe pasar como parámetro de entrada de función el puntero a la estructura que contiene la configuración del canal.

En la carpeta "Ejemplo ADS1115" se muestra un código ejemplo en donde se emplea el driver en modo Polled Driver.

Este ejemplo emplea a los canales A0 y A1 trabajando en modo individual, y los canalos A2 y A3 trabajando en modo diferencial entre ellos, todo de manera simultanea.
