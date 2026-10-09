# USB4all u4a2: migración de C18 a XC8

Migración de la configuración activa de `USB4all.X` para **PIC18F4550**.

Compilación y enlace reales con XC8 4.00 Free; comprobaciones de
layout, HEX y registro de módulos ejecutadas.

## Alcance

Se migran los 23 `.c` de la configuración MPLAB activa y sus cabeceras, más
comprobaciones de layout y un vector en ensamblador. Los 20 módulos registrados
incluyen admin, pnp, port, ax, button, hackp, motors, butia, modActA/B/C, relay,
modSenA/B/C, grey, light, res, volt y distanc.

Los ejemplos y módulos opcionales que no pertenecen a esa configuración siguen
siendo C18; no se compilan mediante este Makefile. Tampoco se migra
`firmware/bootloader`: su contrato se conserva. Los proyectos `.mcp`, `.piklab`,
`USB4all.X` y `18f4550.lkr` se conservan como referencia histórica de C18.
**El punto de entrada de esta migración es `Makefile.xc8`, no el proyecto C18.**
No debe reutilizarse el `.lkr` de C18 en un enlace XC8.

## Compilar

Herramientas verificadas: MPLAB XC8 **4.00** Free para Linux y
Microchip **PIC18Fxxxx_DFP 1.7.171**. El compilador y el DFP no se redistribuyen.
Se requiere GNU Make y Python 3.9 o posterior. Para los tests de host, GCC/Clang.

```sh
cd firmware/u4a2
make -f Makefile.xc8 \
  XC8=/opt/microchip/xc8/v4.00/bin/xc8-cc \
  DFP=/opt/microchip/mplabx/v6.35/packs/Microchip/PIC18Fxxxx_DFP/1.7.171/xc8
```

`DFP` es la carpeta **xc8 dentro del pack**, no el archivo `.atpack` ni la raíz
del pack. Si `xc8-cc` está en PATH, se puede omitir `XC8`. La ruta de instalación
es configurable; no hay rutas de esta sesión dentro del Makefile.

El objetivo predeterminado compila y comprueba el HEX. Genera:

- `build/xc8/usb4all2.hex`: aplicación para el bootloader existente.
- `build/xc8/usb4all2.elf`, `.map`, `.sym` y ensamblador generado: inspección.

```sh
make -f Makefile.xc8 test XC8=/ruta/xc8-cc DFP=/ruta/del/pack/xc8
make -f Makefile.xc8 clean
```
## Decisiones de migración

1. **C y protocolo.** Se utiliza `<xc.h>` y se eliminan dependencias de las
   bibliotecas periféricas de C18 que no se usan. `rom` pasa a `const`; se eliminan
   `near`, `far` y los pragmas de secciones C18 de los fuentes activos. Los comandos
   tienen constantes con prefijo y campos `byte CMD`, evitando enumeraciones de
   ancho dependiente del compilador. Se eliminan alias anónimos duplicados y se
   fijan bitfields a `unsigned char`. Los valores de los comandos no cambian.
   `char` se compila explícitamente con signo, como en el proyecto original.
2. **Módulos.** `user/module_registry.h` enumera los descriptores enlazados. Ya no
   se recorren bloques de 12 bytes desde `0x33AB` ni se busca memoria borrada como
   terminador. Se usan punteros `const uTab *`, longitud acotada y NULL para un
   módulo inexistente. Las consultas de handlers ignoran entradas vacías.
   Los índices de tipo dependen del orden explícito de este registro: el host debe
   volver a enumerar los nombres, no conservar índices de un HEX C18 anterior.
   No existe compatibilidad binaria con módulos C18 compilados por separado:
   hay que recompilar la aplicación completa.
3. **Arranque e interrupciones.** `-mcodeoffset=0x8C0` encarga a XC8 el startup y
   el vector alto `0x8C8`. El vector bajo `0x8D8` contiene `RETFIE 0`; la aplicación
   desactiva prioridades (`IPEN=0`). El stub ensamblador evita reservar una tercera
   pila de software para un ISR bajo que no se utiliza. Se deshabilitan fuentes
   al arrancar y el dispatcher habilita interrupciones al registrar el primer
   listener. Se usa `-mstack=software` para las llamadas indirectas y reentrancia.
4. **USB.** Los BDT son objetos `volatile` absolutos de cuatro bytes, en
   `0x400/404/408/40C`. Su dirección de buffer es un **entero de 16 bits**, no un
   puntero cuyo tamaño pueda optimizar XC8. SETUP está en `0x480`, el buffer de
   control en `0x488`, OUT en `0x4C8` e IN en `0x508`. El layout se comprueba al
   compilar. Los descriptores y el VID/PID **04d8:000c** se conservan. Las tablas y
   descriptores constantes ya no requieren las posiciones de ROM del linker C18.
5. **EEPROM y reinicio.** La escritura del indicador de bootloader usa registros
   del PIC, protege la secuencia de desbloqueo, restaura GIE y espera a que termine
   la escritura antes de poder reiniciar. La desconexión USB usa un retardo explícito
   de 100 ms. Se conserva el reloj de CPU **20 MHz**, `FOSC=HS`; USB usa su PLL de
   48 MHz. Cambiarlo a una CPU de 48 MHz alteraría la UART AX12 y las temporizaciones.

## Comprobaciones y límites

`xc8/layout_checks.c` verifica con el **compilador PIC** los tamaños de tipos,
BDT, descriptores, SETUP y paquetes, y los offsets de campos del protocolo. No se
usan los tamaños de estructuras del PC para validar el ABI del PIC.

`xc8/check_image.py` verifica checksum Intel HEX, rango exclusivo
`0x08C0..0x7FFF`, destinos de vectores, símbolos de RAM USB, contenido de los
principales descriptores y que las pilas reservadas no se solapen con los buffers.
Los tests negativos comprueban que se rechacen imágenes con regiones protegidas,
vectores, descriptores o símbolos alterados. El test C de host ejecuta el loader
real con descriptores de prueba: listado, búsquedas, callbacks y entradas inválidas.

La compilación verificada ocupa **23 697 bytes de programa (77,6 % del espacio
posterior al bootloader)** y **538 bytes de datos**. Además reserva **696 bytes de
pila de software**, repartidos en 348 para main y 348 para la interrupción alta.
El «100 %» de pila del resumen del compilador indica memoria reservada, **no una
medición del máximo uso durante la ejecución**.

XC8 emite el warning **1393**, porque no puede acotar la profundidad de la pila
hardware en el grafo de llamadas indirectas, y el aviso **2223** por el contexto
amplio del ISR. No se silencian. No se ha demostrado un límite de pila en ejecución;
es necesario medir margen de pila y latencia bajo carga antes de uso operativo.
Quedan también advertencias heredadas de conversiones numéricas, código no usado
y casts de buffers USB. El registro de compilación acompaña la entrega.

Para incorporar un módulo opcional: migrar sus dependencias C18, prefijar comandos,
conservar layouts de paquetes, añadir su `.c` a `SOURCES`, su descriptor al final
 de `U4A_MODULES` y ampliar las pruebas. No basta con añadir un pragma `romdata`.

## Referencias

- [Repositorio base](https://github.com/AlanJAS/usb4all/tree/2f7fcce129c57ebdb55fdf889e35fee13326dd4f/firmware/u4a2)
- [Arquitectura y grabación USB4all](https://www.fing.edu.uy/inco/proyectos/butia/mediawiki/index.php?title=Usb4all)
- [Microchip: code offset con XC8](https://developerhelp.microchip.com/xwiki/bin/view/software-tools/compilers/xc8/code-offset-pic/)
- [Microchip: opción DFP](https://onlinedocs.microchip.com/oxy/GUID-BB433107-FD4E-4D28-BB58-9D4A58955B1A-en-US-9/GUID-508506FB-BEFB-4101-BA76-26A921891125.html)
- [Compilador XC8](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/xc8)
- [Device Family Packs](https://packs.download.microchip.com/)

## Actualización: velocidades y tiempos PWM (XC8 4.00)

Tras el parche de conversiones explícitas, el segundo parche valida los comandos
`SET_VEL_MTR` y `SET_VEL_2MTR` antes de modificar el estado. Las velocidades deben
estar entre 0 y 1023, las direcciones entre 0 y 1, y el identificador del comando
individual debe ser 0 (izquierdo) o 1 (derecho). Se requieren al menos 5 bytes para
el comando individual y 7 para el doble, incluyendo el byte de comando.
El comando doble se valida completo antes de actualizar cualquiera de los motores.

Una solicitud inválida o incompleta se ignora: no cambia el estado y no se envía
el eco de éxito. El cliente puede observar un timeout. No se introduce un código
NACK nuevo. El valor 0xFFFF sigue siendo un indicador interno; no es una velocidad
admitida por USB. El formato, la respuesta y las inversiones de dirección de los
comandos válidos permanecen iguales.

Los tiempos PWM son `word` y se calculan como `speed * TIME_C / 1023`, con
multiplicación de 32 bits y truncamiento a ticks enteros. Para velocidades válidas,
el resultado pertenece a 0..TIME_C. Las llamadas AX12 comprueban el rango antes
de convertir la velocidad a `int`; no se cambia la API general de `endlessTurn`.


## Validación de recepción USB (parche 0003)

El despachador descarta transferencias OUT que no contengan los tres bytes de
cabecera y al menos un byte de comando, o que superen los 64 bytes del endpoint.
Antes de invocar el callback verifica que el handler esté dentro de la tabla,
esté abierto y tenga una función registrada. Siempre rearma OUT tras procesar
o descartar una transferencia; no rearma si el periférico aún es su propietario.
Se utiliza la longitud recibida del USB, sin interpretar de otra manera los
campos reservados/de longitud de la cabecera. Se admiten paquetes con relleno.
Una transferencia completa de 64 bytes entrega los 61 bytes de datos al módulo
(antes el despachador la recortaba a 63 bytes totales).

Los receptores pnp, port, button, hackpoints, butia, modAct y modSen comprueban
que exista el comando antes de leerlo. modAct exige dos bytes para TURN;
hackpoints exige dos o tres según el comando y limita el pin a 0..7. Los comandos
rechazados no modifican salidas ni envían una respuesta de éxito. No se añade
un código de error al protocolo; el cliente puede agotar su tiempo de espera.


## Buffers de trabajo USB (parche 0004)

Los módulos activos reciben y preparan respuestas en dos buffers de RAM de
61 bytes. El despachador copia OUT mediante accesos volatile; USBGenWrite2 copia
la respuesta hacia IN mediante accesos volatile y entrega la propiedad al USB
solo al finalizar. No se eliminan calificadores mediante casts.

Si IN tiene una respuesta pendiente, USBGenRead2 conserva OUT y pospone la
invocación del módulo hasta que IN esté libre. Por tanto, una solicitud nueva
puede esperar a que el host lea la respuesta anterior; no se ejecutan sus efectos
mientras espera. Se preservan el formato de cabecera y los 61 bytes de carga útil.
Solo se admite el endpoint 1, que es el único configurado por USBInitEPs.

getSharedBuffer sigue devolviendo byte*, ahora a RAM de trabajo compartida.
Su uso es síncrono desde main: preparar y enviar la respuesta dentro del callback
Received. No es una cola ni admite productores desde ISR; los módulos opcionales
que transmitan de forma asíncrona necesitan un diseño específico antes de activarse.
Los dos buffers suman 122 bytes de RAM. XC8 4.00 compila y check_image valida la
imagen; las comprobaciones locales con USB simulado verifican espera, copias y
que preparar una respuesta no altere una transmisión pendiente. Falta comprobar
el intercambio USB en la placa. El análisis de pila/latencia queda pendiente.
