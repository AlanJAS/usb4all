# USB4all

USB4all es una placa para conectar sensores, actuadores y otros dispositivos electrónicos a una computadora mediante USB. Utiliza un **PIC18F4550** y organiza sus funcionalidades en módulos de firmware, con aplicaciones en el **Proyecto Butiá**.

El proyecto Butiá usa la placa USB4all (llamada USB4butia) con motores para crear una plataforma robótica educativa de bajo costo.

[Wiki Proyecto Butiá](https://www.fing.edu.uy/inco/proyectos/butia/mediawiki/index.php?title=Wiki_Buti%C3%A1)

Este repositorio reúne el firmware, el bootloader, diseños de placas, documentación técnica y una herramienta para grabar la aplicación por USB.

## Contenido

| Directorio | Contenido |
| --- | --- |
| [firmware/u4a2](firmware/u4a2/) | Firmware principal y módulos de usuario. |
| [firmware/bootloader](firmware/bootloader/) | Bootloader para instalar y actualizar la aplicación. |
| [fsusb-0.1.11-2](fsusb-0.1.11-2/) | Utilidad de programación USB incluida en el repositorio. |
| [pcb](pcb/) | Diseños de placas, sensores, actuadores y accesorios. |
| [doc](doc/) | Manuales, documentación del proyecto y del microcontrolador. |
| [usb4all_kernel_module](usb4all_kernel_module/) | Código histórico de un módulo para el kernel de Linux. |

## Ramas y herramientas

- **`master`**: desarrollo original, con proyectos para MPLAB C18.
- **[`xc8`](https://github.com/AlanJAS/usb4all/tree/xc8)**: migración a MPLAB XC8, con Makefiles independientes para la aplicación y el bootloader.

**Los comandos de compilación de este README corresponden a la rama `xc8`.** Los módulos opcionales y ejemplos históricos no están necesariamente migrados.

```sh
git clone --branch xc8 https://github.com/AlanJAS/usb4all.git
cd usb4all
```

Si ya tenés el repositorio:

```sh
git fetch origin
git switch xc8
```

## Compilar con XC8

### Requisitos

- GNU Make.
- MPLAB XC8 para PIC; los Makefiles están configurados para **XC8 4.00**.
- Pack **Microchip PIC18Fxxxx_DFP 1.7.171**.
- Python 3.9 o posterior para las comprobaciones de la aplicación.

El compilador y el pack de dispositivos se obtienen por separado:

- [MPLAB XC8](https://www.microchip.com/en-us/tools-resources/develop/mplab-xc-compilers/xc8)
- [Microchip Device Packs](https://packs.download.microchip.com/)

Los Makefiles utilizan estas rutas predeterminadas en Linux:

```text
XC8=/opt/microchip/xc8/v4.00/bin/xc8-cc
DFP=/opt/microchip/mplabx/v6.35/packs/Microchip/PIC18Fxxxx_DFP/1.7.171/xc8
```

`DFP` debe apuntar al subdirectorio `xc8` del pack.

### Aplicación

Desde la raíz del repositorio:

```sh
make -C firmware/u4a2
```

También podés ejecutar `make` directamente dentro de `firmware/u4a2`. El objetivo predeterminado compila y ejecuta las comprobaciones de la imagen.

Archivo para grabar por USB:

```text
firmware/u4a2/build/xc8/usb4all2.hex
```

Para repetir las comprobaciones o limpiar los resultados:

```sh
make -C firmware/u4a2 check
make -C firmware/u4a2 clean
```

### Bootloader

```sh
make -C firmware/bootloader
```

Archivo para grabar con un programador de hardware:

```text
firmware/bootloader/build/xc8/bootloader.hex
```

Para limpiar:

```sh
make -C firmware/bootloader clean
```

### Usar otras rutas

Las variables se pueden sobrescribir en la línea de comandos de cualquiera de los dos Makefiles:

```sh
make -C firmware/u4a2 \
  XC8=/otra/ruta/bin/xc8-cc \
  DFP=/otra/ruta/PIC18Fxxxx_DFP/1.7.171/xc8
```

Si el compilador está en el `PATH`, usá `XC8=xc8-cc`.

### Proyectos MPLAB

Se conservan proyectos de distintas etapas del desarrollo. Para reproducir la compilación descrita aquí, usá los Makefiles de `firmware/u4a2` y `firmware/bootloader`. Los proyectos y scripts de enlace de C18 no deben reutilizarse directamente para enlazar con XC8.

## Grabar la placa

### 1. Instalar el bootloader

Si el PIC no tiene el bootloader, necesitás un programador de hardware compatible, como PICkit.

1. Compilá el bootloader.
2. Conectá el programador según el pinout de tu placa.
3. Seleccioná el PIC18F4550 en el software del programador.
4. Grabá y verificá `firmware/bootloader/build/xc8/bootloader.hex`.

Luego podés cargar la aplicación por USB. **El HEX de la aplicación no incluye el bootloader**: en XC8 comienza en `0x08C0` y se genera sin bits de configuración.

### 2. Compilar fsusb

La utilidad incluida utiliza la API de **libusb 0.1** (`usb.h`, enlace con `-lusb`). Necesitás un compilador C, Make y las cabeceras y biblioteca compatibles con esa API; libusb 1.0 por sí sola no sustituye esta dependencia.

```sh
make -C fsusb-0.1.11-2
```

### 3. Entrar en modo bootloader

Con la placa conectada por USB:

1. Mantené presionado el botón de programación.
2. Pulsá y soltá el botón de reset.
3. Soltá el botón de programación.

Comprobá la identificación con:

```sh
lsusb
```

| Estado | VID:PID |
| --- | --- |
| Aplicación USB4all | `04d8:000c` |
| Bootloader | `04d8:000b` |

### 4. Grabar y ejecutar la aplicación

Desde la raíz del repositorio:

```sh
./fsusb-0.1.11-2/fsusb --program firmware/u4a2/build/xc8/usb4all2.hex
```

La opción `--program` graba y verifica. También podés verificar por separado:

```sh
./fsusb-0.1.11-2/fsusb --verify firmware/u4a2/build/xc8/usb4all2.hex
```

Para salir del bootloader y ejecutar la aplicación:

```sh
./fsusb-0.1.11-2/fsusb --reset
```

Comprobá que la placa vuelva a aparecer como `04d8:000c`.

Si el sistema deniega el acceso USB, revisá los permisos del dispositivo o las reglas udev de tu distribución. Para una operación puntual, podés ejecutar el comando de grabación con `sudo`.

## Arquitectura y módulos

El firmware base gestiona la comunicación USB, los módulos y los recursos compartidos. Cada módulo expone operaciones de inicialización, liberación y configuración, además de procesar sus comandos específicos.

En la rama `xc8`, los módulos activos se enumeran explícitamente en [module_registry.h](https://github.com/AlanJAS/usb4all/blob/xc8/firmware/u4a2/user/module_registry.h). Incluyen administración, detección de módulos, puertos, AX12, botones, motores y módulos de sensores y actuadores Butiá.

Para incorporar un módulo a esta compilación:

1. Implementá su descriptor y callbacks tomando como referencia un módulo activo de `firmware/u4a2/user/`.
2. Agregá sus fuentes y dependencias a `SOURCES` en el Makefile de la aplicación.
3. Declaralo y agregalo al final de `U4A_MODULES`, preservando el orden existente.
4. Compilá, ejecutá las comprobaciones y probá sus comandos en la placa.

Los índices dependen del orden del registro: el software de la computadora debe enumerar los módulos por nombre y no conservar índices de una imagen anterior. Los módulos compilados con C18 no son compatibles binariamente con XC8; hay que recompilar la aplicación.

## Comprobaciones y estado de la migración

La aplicación incluye comprobaciones de tamaños y disposición de estructuras, y un script que revisa el HEX y los símbolos generados: direcciones de la aplicación, vectores y ubicación de buffers USB, entre otros aspectos.

Estas comprobaciones no sustituyen las pruebas con la placa. La documentación de migración señala validaciones pendientes del intercambio USB, margen de pila y latencia bajo carga. Una compilación exitosa no demuestra por sí sola el funcionamiento completo en hardware.

El [documento de migración XC8](https://github.com/AlanJAS/usb4all/blob/xc8/firmware/u4a2/README-XC8.md) contiene detalles técnicos e historial de cambios. Algunas secciones conservan referencias a etapas anteriores, como `Makefile.xc8` o la ausencia de una migración del bootloader; para compilar, seguí los comandos anteriores y los Makefiles de la rama.

## Problemas frecuentes

| Problema | Qué revisar |
| --- | --- |
| No se encuentra `xc8-cc` | Sobrescribir `XC8` con la ruta de instalación. |
| Error al localizar el dispositivo o el pack | Revisar `DFP` y que termine en el subdirectorio `xc8`. |
| Falta `usb.h` o falla el enlace con `-lusb` | Instalar los archivos de desarrollo compatibles con libusb 0.1. |
| fsusb no detecta la placa para grabar | Comprobar el modo bootloader (`04d8:000b`), el cable y los permisos USB. |
| La placa no entra en modo bootloader | Comprobar que tenga instalado el bootloader y repetir la secuencia de botones. |
| La aplicación no arranca | Verificar la grabación, el bootloader, los bits de configuración y la compatibilidad del hardware. |

## Documentación y origen

- [USB4all: arquitectura y guía de uso](https://www.fing.edu.uy/inco/proyectos/butia/mediawiki/index.php?title=Usb4all)
- [Grabación del firmware](https://www.fing.edu.uy/inco/proyectos/butia/mediawiki/index.php?title=Usb4all#4._Grabando_el_Firmware)
- [Documentación incluida en el repositorio](doc/usb4all/)
- [Repositorio original en SourceForge](https://sourceforge.net/projects/usb4all/)

USB4all se originó como un trabajo de grado en Ingeniería en Computación de Aguirre, Fernández y Grossy. Su documentación y su integración con Butiá se publican en el sitio de la Facultad de Ingeniería de la Universidad de la República, Uruguay.

## Licencias

La documentación original presenta USB4all bajo **GNU GPL v2**. El repositorio también contiene componentes de terceros con avisos propios, incluidos fuentes de Microchip. Consultá las cabeceras y los archivos de licencia de cada componente antes de reutilizarlo o redistribuirlo.

La licencia de la utilidad fsusb está incluida en [fsusb-0.1.11-2/COPYING](fsusb-0.1.11-2/COPYING).

