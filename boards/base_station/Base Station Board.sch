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
LIBS:Coordinator Board-cache
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
L CONN_02X20 P3
U 1 1 586759B9
P 3950 3850
F 0 "P3" H 3950 4900 50  0000 C CNN
F 1 "Raspberry Pi" V 3950 3850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x20_Pitch2.54mm" H 3950 2900 50  0000 C CNN
F 3 "" H 3950 2900 50  0000 C CNN
	1    3950 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2900 3700 2900
Wire Wire Line
	3600 2700 3600 2900
Wire Wire Line
	3100 3500 3100 3700
Wire Wire Line
	4200 3100 4400 3100
Wire Wire Line
	4400 3100 4400 5000
$Comp
L R R1
U 1 1 58675C54
P 6050 3000
F 0 "R1" V 6130 3000 50  0000 C CNN
F 1 "1k" V 6050 3000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6550 3150 50  0000 C CNN
F 3 "" H 6050 3000 50  0000 C CNN
	1    6050 3000
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 58675C93
P 6050 3700
F 0 "R2" V 6130 3700 50  0000 C CNN
F 1 "1k" V 6050 3700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6800 3800 50  0000 C CNN
F 3 "" H 6050 3700 50  0000 C CNN
	1    6050 3700
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 58675CC0
P 6050 4400
F 0 "R3" V 6130 4400 50  0000 C CNN
F 1 "1k" V 6050 4400 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6550 4400 50  0000 C CNN
F 3 "" H 6050 4400 50  0000 C CNN
	1    6050 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5700 2850 6250 2850
Wire Wire Line
	5700 3550 6350 3550
Wire Wire Line
	5700 4250 6450 4250
Wire Wire Line
	6250 4550 6050 4550
Connection ~ 6250 4550
Wire Wire Line
	6250 3850 6050 3850
$Comp
L CONN_01X04 P1
U 1 1 58675F6F
P 2400 3350
F 0 "P1" H 2400 3600 50  0000 C CNN
F 1 "LCD" V 2500 3350 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" V 2750 2600 50  0000 C CNN
F 3 "" H 2400 3350 50  0000 C CNN
	1    2400 3350
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 3500 2600 3500
Wire Wire Line
	2700 3400 2700 3900
Wire Wire Line
	2700 3400 2600 3400
Wire Wire Line
	3700 3100 2800 3100
Wire Wire Line
	2800 3100 2800 3300
Wire Wire Line
	2800 3300 2600 3300
Wire Wire Line
	3700 3000 2700 3000
Wire Wire Line
	2700 3000 2700 3200
Wire Wire Line
	2700 3200 2600 3200
$Comp
L CONN_01X03 P2
U 1 1 58758969
P 2400 4000
F 0 "P2" H 2400 4200 50  0000 C CNN
F 1 "Serial" V 2500 4000 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03_Pitch2.54mm" V 2600 4600 50  0000 C CNN
F 3 "" H 2400 4000 50  0000 C CNN
	1    2400 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	4200 3200 4500 3200
Wire Wire Line
	4500 3200 4500 5100
Wire Wire Line
	4500 5100 2800 5100
Wire Wire Line
	2800 5100 2800 4100
Wire Wire Line
	2800 4100 2600 4100
Wire Wire Line
	4200 3300 4600 3300
Wire Wire Line
	4600 3300 4600 5200
Wire Wire Line
	4600 5200 2900 5200
Wire Wire Line
	2900 5200 2900 4000
Wire Wire Line
	2900 4000 2600 4000
Wire Wire Line
	2600 3900 3100 3900
Connection ~ 2700 3900
Wire Wire Line
	3500 2600 3500 3600
Wire Wire Line
	3500 2600 6250 2600
Wire Wire Line
	6250 2600 6250 2850
Connection ~ 6050 2850
Wire Wire Line
	3700 3400 3400 3400
Wire Wire Line
	3400 3400 3400 2500
Wire Wire Line
	3400 2500 6350 2500
Wire Wire Line
	6350 2500 6350 3550
Connection ~ 6050 3550
Wire Wire Line
	3700 3500 3300 3500
Wire Wire Line
	3300 3500 3300 2400
Wire Wire Line
	3300 2400 6450 2400
Wire Wire Line
	6450 2400 6450 4250
Connection ~ 6050 4250
$Comp
L SPST SW1
U 1 1 587594B8
P 5200 2850
F 0 "SW1" H 5200 2950 50  0000 C CNN
F 1 "SPST" H 5200 2750 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH_6mm" H 5200 3550 50  0000 C CNN
F 3 "" H 5200 2850 50  0000 C CNN
	1    5200 2850
	1    0    0    -1  
$EndComp
$Comp
L SPST SW2
U 1 1 587595F9
P 5200 3550
F 0 "SW2" H 5200 3650 50  0000 C CNN
F 1 "SPST" H 5200 3450 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH_6mm" H 5200 4800 50  0000 C CNN
F 3 "" H 5200 3550 50  0000 C CNN
	1    5200 3550
	1    0    0    -1  
$EndComp
$Comp
L SPST SW3
U 1 1 58759632
P 5200 4250
F 0 "SW3" H 5200 4350 50  0000 C CNN
F 1 "SPST" H 5200 4150 50  0000 C CNN
F 2 "Buttons_Switches_ThroughHole:SW_PUSH_6mm" H 5300 2950 50  0000 C CNN
F 3 "" H 5200 4250 50  0000 C CNN
	1    5200 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 2700 4700 2700
Wire Wire Line
	4700 2700 4700 4250
Connection ~ 4700 2850
Connection ~ 4700 3550
Wire Wire Line
	3500 3600 3700 3600
Wire Wire Line
	3100 3700 3700 3700
Wire Wire Line
	6050 3150 6550 3150
Wire Wire Line
	6550 3150 6550 5300
Wire Wire Line
	6550 5300 3600 5300
Wire Wire Line
	3600 5300 3600 4100
Wire Wire Line
	3100 4100 3700 4100
Wire Wire Line
	6250 3850 6250 5000
Wire Wire Line
	6250 5000 4400 5000
Wire Wire Line
	3100 3900 3100 4100
Connection ~ 3600 4100
$EndSCHEMATC
