Este código es para un despertador utilizando un Arduino y una pantalla LCD con controlador I2C, junto con un módulo de tiempo real (RTC) y un zumbador.

El código comienza incluyendo las librerías necesarias para el RTC y la pantalla LCD, además de definir las entradas y salidas necesarias para el zumbador y los botones.

A continuación, se establecen algunas variables que se utilizarán posteriormente para almacenar la hora de la alarma, si está activada y el estado de los botones. Luego, en la función de configuración, se inicializan los pines y se borra la pantalla.

En el loop principal, se actualiza la hora del RTC y se verifica si la pantalla debe borrarse. Luego, se verifica si se debe activar o desactivar la alarma según el botón principal. A continuación, se muestra la fecha y hora actual en la pantalla LCD, junto con la hora de la alarma si está activada.

Se pueden ajustar la hora y los minutos de la alarma mediante el botón izquierdo, y se puede apagar la pantalla con el botón derecho. Si se presiona el botón principal y la hora actual coincide con la hora de la alarma, se activa el zumbador y se muestra la fecha y hora en la pantalla.

La función buzz() se encarga de generar el sonido de la alarma y printAlarmTime() imprime la hora de la alarma en la pantalla. La función CheckAlarm() se utiliza para verificar si se debe activar la alarma y, en caso afirmativo, activar el zumbador y mostrar la hora en la pantalla.

Por último, el código incluye un pequeño truco para apagar la pantalla temporalmente y volver a encenderla después de un cierto tiempo.
