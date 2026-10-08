EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:generico-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date "20 mar 2012"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 2900 1150 2    60   Input ~ 0
GND
NoConn ~ 2700 1950
NoConn ~ 2600 1950
NoConn ~ 2100 1950
NoConn ~ 2000 1950
Wire Wire Line
	3750 2350 3650 2350
Wire Wire Line
	3650 2350 3650 2400
Wire Wire Line
	3650 2400 3450 2400
Wire Wire Line
	2400 2150 2400 1950
Wire Wire Line
	2500 1950 2500 2650
Wire Wire Line
	2300 1950 2300 2650
Wire Wire Line
	2200 1950 2200 2350
Wire Wire Line
	3450 2250 3750 2250
Wire Wire Line
	3450 2100 3700 2100
Wire Wire Line
	3700 2100 3700 2150
Wire Wire Line
	3700 2150 3750 2150
Text GLabel 3450 2400 0    60   Input ~ 0
VDD
Text GLabel 3450 2100 0    60   Input ~ 0
GND
Text GLabel 3550 1300 0    60   Input ~ 0
idAN
Text GLabel 5700 1300 2    60   Input ~ 0
VDD
Text GLabel 2400 2150 3    60   Input ~ 0
GND
Text GLabel 2200 2350 3    60   Input ~ 0
VDD
Text GLabel 3450 2250 0    60   Input ~ 0
AN
$Comp
L CONN_3 K1
U 1 1 4F6888FE
P 4100 2250
F 0 "K1" V 4050 2250 50  0000 C CNN
F 1 "Conn_Sensor" V 4150 2250 40  0000 C CNN
F 2 "" H 4100 2250 60  0001 C CNN
F 3 "" H 4100 2250 60  0001 C CNN
	1    4100 2250
	1    0    0    -1  
$EndComp
Text GLabel 2500 2650 3    60   Input ~ 0
AN
Text GLabel 2300 2650 3    60   Input ~ 0
idAN
$Comp
L RJ45 J1
U 1 1 4F68816E
P 2350 1500
F 0 "J1" H 2550 2000 60  0000 C CNN
F 1 "RJ45" H 2200 2000 60  0000 C CNN
F 2 "" H 2350 1500 60  0001 C CNN
F 3 "" H 2350 1500 60  0001 C CNN
	1    2350 1500
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 54A31792
P 4050 1300
F 0 "R1" V 4130 1300 40  0000 C CNN
F 1 "R" V 4057 1301 40  0000 C CNN
F 2 "" V 3980 1300 30  0000 C CNN
F 3 "" H 4050 1300 30  0000 C CNN
	1    4050 1300
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 54A3179F
P 4050 1450
F 0 "R2" V 4130 1450 40  0000 C CNN
F 1 "R" V 4057 1451 40  0000 C CNN
F 2 "" V 3980 1450 30  0000 C CNN
F 3 "" H 4050 1450 30  0000 C CNN
	1    4050 1450
	0    -1   -1   0   
$EndComp
$Comp
L R R3
U 1 1 54A317A5
P 4050 1600
F 0 "R3" V 4130 1600 40  0000 C CNN
F 1 "R" V 4057 1601 40  0000 C CNN
F 2 "" V 3980 1600 30  0000 C CNN
F 3 "" H 4050 1600 30  0000 C CNN
	1    4050 1600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3800 1300 3550 1300
Wire Wire Line
	3700 1300 3700 1600
Wire Wire Line
	3700 1450 3800 1450
Connection ~ 3700 1300
Wire Wire Line
	3700 1600 3800 1600
Connection ~ 3700 1450
$Comp
L JUMPER_G P1
U 1 1 54B1217C
P 5050 1500
F 0 "P1" H 5050 1750 50  0000 C CNN
F 1 "JUMPER_G" V 5050 1550 40  0000 C CNN
F 2 "" H 5050 1500 60  0000 C CNN
F 3 "" H 5050 1500 60  0000 C CNN
	1    5050 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1300 4450 1300
Wire Wire Line
	4450 1300 4450 1350
Wire Wire Line
	4450 1350 4650 1350
Wire Wire Line
	4300 1450 4650 1450
Wire Wire Line
	4300 1600 4450 1600
Wire Wire Line
	4450 1600 4450 1550
Wire Wire Line
	4450 1550 4650 1550
Wire Wire Line
	5450 1550 5600 1550
Wire Wire Line
	5600 1550 5600 1300
Wire Wire Line
	5600 1300 5700 1300
Wire Wire Line
	5450 1450 5600 1450
Connection ~ 5600 1450
Wire Wire Line
	5450 1350 5600 1350
Connection ~ 5600 1350
$EndSCHEMATC
