EESchema Schematic File Version 2
LIBS:diode
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
LIBS:nebrius-microcontrollers
LIBS:nebrius-regulators
LIBS:nebrius-lcd
LIBS:Endpoint Board-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Switch_SPDT_x2 SW1
U 1 1 57A0DADD
P 6250 3100
F 0 "SW1" H 6050 3250 50  0000 C CNN
F 1 "Switch_SPDT_x2" H 6000 2950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03" H 6550 3400 50  0000 C CNN
F 3 "" H 6250 3100 50  0000 C CNN
	1    6250 3100
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X02 P1
U 1 1 57A0DC23
P 5000 3800
F 0 "P1" H 5000 3950 50  0000 C CNN
F 1 "POWER" V 5100 3800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.00mm" H 5500 3600 50  0000 C CNN
F 3 "" H 5000 3800 50  0000 C CNN
	1    5000 3800
	-1   0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 57A0E1B1
P 6900 3650
F 0 "L1" V 6850 3650 50  0000 C CNN
F 1 "2.2uH" V 7000 3650 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 7100 3950 50  0000 C CNN
F 3 "" H 6900 3650 50  0000 C CNN
	1    6900 3650
	0    -1   -1   0   
$EndComp
$Comp
L C C2
U 1 1 57A0E396
P 7550 4150
F 0 "C2" H 7575 4250 50  0000 L CNN
F 1 "10uF" H 7700 4150 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7588 4000 50  0000 C CNN
F 3 "" H 7550 4150 50  0000 C CNN
	1    7550 4150
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 57A0E3BE
P 5350 3550
F 0 "C1" H 5375 3650 50  0000 L CNN
F 1 "4.7uF" H 5100 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4850 3750 50  0000 C CNN
F 3 "" H 5350 3550 50  0000 C CNN
	1    5350 3550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 584F7038
P 9350 3750
F 0 "P4" H 9350 3950 50  0000 C CNN
F 1 "SERIAL" V 9450 3750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.00mm" H 9650 3500 50  0000 C CNN
F 3 "" H 9350 3750 50  0000 C CNN
	1    9350 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5200 3750 5200 3000
Wire Wire Line
	5200 3000 5950 3000
Wire Wire Line
	6550 3100 6700 3100
Wire Wire Line
	5500 3300 5500 4300
Wire Wire Line
	5500 4300 6400 4300
Wire Wire Line
	5350 3850 5200 3850
Wire Wire Line
	5350 3700 5350 4600
Wire Wire Line
	8950 4600 5350 4600
Connection ~ 5350 3850
Wire Wire Line
	6550 3650 6600 3650
Wire Wire Line
	6700 3300 6700 3100
Wire Wire Line
	5350 3300 6700 3300
Wire Wire Line
	5350 3300 5350 3400
Connection ~ 5500 3300
Wire Wire Line
	7550 3650 7550 4000
Wire Wire Line
	6550 3850 7650 3850
Connection ~ 7550 3850
Wire Wire Line
	7550 4600 7550 4300
Wire Wire Line
	7200 3650 7650 3650
Wire Wire Line
	8650 3550 8950 3550
Wire Wire Line
	8950 3550 8950 4600
Connection ~ 7550 4600
Wire Wire Line
	8750 4400 8750 3750
Wire Wire Line
	8750 3750 8650 3750
Wire Wire Line
	6200 4400 8750 4400
Wire Wire Line
	6200 4400 6200 4700
Wire Wire Line
	8650 3650 8850 3650
Wire Wire Line
	8850 3650 8850 4500
Wire Wire Line
	8850 4500 6300 4500
Wire Wire Line
	6300 4500 6300 4700
Connection ~ 7550 3650
Wire Wire Line
	8650 3850 9150 3850
Wire Wire Line
	7650 3550 7550 3550
Wire Wire Line
	7550 3550 7550 3400
Wire Wire Line
	7550 3400 9050 3400
Wire Wire Line
	9050 3400 9050 3750
Wire Wire Line
	9050 3750 9150 3750
Wire Wire Line
	9150 3650 8950 3650
Connection ~ 8950 3650
Wire Wire Line
	7650 3750 7350 3750
Wire Wire Line
	7350 3750 7350 3000
Wire Wire Line
	7250 3000 7250 3650
Connection ~ 7250 3650
Wire Wire Line
	6400 4300 6400 4700
Wire Wire Line
	6100 4700 6100 4600
Connection ~ 6100 4600
$Comp
L CONN_01X02 P5
U 1 1 588596B2
P 7300 2800
F 0 "P5" H 7300 2950 50  0000 C CNN
F 1 "RUN" V 7400 2800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.00mm" V 7550 2600 50  0000 C CNN
F 3 "" H 7300 2800 50  0000 C CNN
	1    7300 2800
	0    -1   -1   0   
$EndComp
$Comp
L CONN_01X02 P6
U 1 1 58859834
P 6850 4900
F 0 "P6" H 6850 5050 50  0000 C CNN
F 1 "PGRM" V 6950 4900 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.00mm" V 7150 5750 50  0000 C CNN
F 3 "" H 6850 4900 50  0000 C CNN
	1    6850 4900
	0    1    1    0   
$EndComp
Wire Wire Line
	6800 4700 6800 4600
Connection ~ 6800 4600
Wire Wire Line
	6900 4700 6900 4400
Connection ~ 6900 4400
$Comp
L LM3671 U1
U 1 1 5AB1453B
P 6100 3750
F 0 "U1" H 6100 4050 60  0000 C CNN
F 1 "LM3671" H 6100 3450 60  0000 C CNN
F 2 "" H 6100 3750 60  0001 C CNN
F 3 "" H 6100 3750 60  0001 C CNN
	1    6100 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3650 5500 3650
Connection ~ 5500 3650
Wire Wire Line
	5650 3850 5500 3850
Connection ~ 5500 3850
Wire Wire Line
	5650 3750 5350 3750
Connection ~ 5350 3750
$Comp
L ESP-01 U2
U 1 1 5AB148BB
P 8150 3700
F 0 "U2" H 8150 4050 60  0000 C CNN
F 1 "ESP-01" H 8150 3350 60  0000 C CNN
F 2 "" H 8150 3700 60  0001 C CNN
F 3 "" H 8150 3700 60  0001 C CNN
	1    8150 3700
	1    0    0    -1  
$EndComp
NoConn ~ 5950 3200
$Comp
L DotStar U3
U 1 1 5AB151BC
P 6250 5050
F 0 "U3" H 6600 5100 60  0000 C CNN
F 1 "DotStar" H 6250 5200 60  0000 C CNN
F 2 "" H 6250 5050 60  0001 C CNN
F 3 "" H 6250 5050 60  0001 C CNN
	1    6250 5050
	-1   0    0    1   
$EndComp
$EndSCHEMATC