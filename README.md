repositorio: https://github.com/chalimbu/ciudad-inteligente-adaptativa


entrega 1: hacer uso de todos los sensores y actuadores
foto de la maqueta/sus sensores actuadores en marcacion-sensores.png

para el sensor de co2 se cambio el pin al 2, por que asi estaba conectado en la maqueta.
para correrlo es conectar la placa y correr el archivo en la ruta \parte1smartCity\parte1smartCity.ino
lo otro que tambien cambiamos es la variable CO2_THRESHOLD uqe nos permite probar la funcionalidad del sensor de C02 o no, debido a que los rangos de valores
estan muy junto y cualquier rango activa muy seguido el sensor(no permitiendo revisar las otras funcionalidades de la maqueta(.


entrega 2: agregarle comunicacion con una api de internet

referencia de proyecto articulo para la comunicacion serial sacada de https://github.com/mschoeffler/arduino-java-serial-communication (que tiene una licensia MIT)
para correr este proyecto con la parte 2 de comunicacion serial primero se debe correr el archivo en la ruta \parte2smartCity\parte2smartCity.ino en la placa
arduino luego entrar al proyecto que esta en la ruta \parte2smartCity\arduino-java-serial-communication\example01_print_text_on_LCD_module\java_send_text\src\de\mschoeffler\arduino\serialcomm\example01\Startup.java (este es un proyecto java,maven que se puede abrir por ejemplo con intellij) correr el main en este archivo

lo que hara esto es enviar por comunicacion serial un 0 o un 1 (o indicando que el clima esta despejado y 1 indicando cuando este lloviendo)
este valor lo saca de un api de clima que actualmente tiene las localizaciones de la universidad eafit, estas localizaciones se pueden cambiar en 
la linea 56 del archivo URL url = new URL("https://api.open-meteo.com/v1/forecast?latitude=6.199656063350807&longitude=-75.5792788322791&current_weather=true"); 

luego este valor se usa en el arduino para calcular cuanto tiempo duran los estados intermedios(semaforo en amarillo)