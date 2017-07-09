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
LIBS:ESP8266
LIBS:DIY-OLED
LIBS:TSWA
LIBS:Rotary_Encoder
LIBS:sf303gj26
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
L LM3671 U1
U 1 1 57A0D83A
P 3450 3600
F 0 "U1" H 3200 3825 60  0000 L CNN
F 1 "LM3671" H 3200 3350 60  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 3450 3250 60  0000 C CNN
F 3 "" H 3450 3250 60  0000 C CNN
	1    3450 3600
	1    0    0    -1  
$EndComp
$Comp
L Switch_SPDT_x2 SW1
U 1 1 57A0DADD
P 3550 2950
F 0 "SW1" H 3350 3100 50  0000 C CNN
F 1 "Switch_SPDT_x2" H 3300 2800 50  0000 C CNN
F 2 "CL-SA-12C:CL-SA-12C4" H 3850 3250 50  0000 C CNN
F 3 "" H 3550 2950 50  0000 C CNN
	1    3550 2950
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X02 P1
U 1 1 57A0DC23
P 2500 3650
F 0 "P1" H 2500 3800 50  0000 C CNN
F 1 "POWER" V 2600 3650 50  0000 C CNN
F 2 "PH_Connector:S2B-PH-K-S" H 3000 3450 50  0000 C CNN
F 3 "" H 2500 3650 50  0000 C CNN
	1    2500 3650
	-1   0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 57A0E1B1
P 4200 3500
F 0 "L1" V 4150 3500 50  0000 C CNN
F 1 "2.2uH" V 4300 3500 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 4400 3800 50  0000 C CNN
F 3 "" H 4200 3500 50  0000 C CNN
	1    4200 3500
	0    -1   -1   0   
$EndComp
$Comp
L C C2
U 1 1 57A0E396
P 4850 4000
F 0 "C2" H 4875 4100 50  0000 L CNN
F 1 "10uF" H 5000 4000 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 4500 3850 50  0000 C CNN
F 3 "" H 4850 4000 50  0000 C CNN
	1    4850 4000
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 57A0E3BE
P 2850 3400
F 0 "C1" H 2875 3500 50  0000 L CNN
F 1 "4.7uF" H 2600 3500 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 2350 3600 50  0000 C CNN
F 3 "" H 2850 3400 50  0000 C CNN
	1    2850 3400
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 584F7038
P 5450 4750
F 0 "P4" H 5450 4950 50  0000 C CNN
F 1 "SERIAL" V 5550 4750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.00mm" V 5700 3950 50  0000 C CNN
F 3 "" H 5450 4750 50  0000 C CNN
	1    5450 4750
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 58978884
P 6650 2800
F 0 "R2" V 6730 2800 50  0000 C CNN
F 1 "1k" V 6650 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5750 1950 50  0000 C CNN
F 3 "" H 6650 2800 50  0000 C CNN
	1    6650 2800
	-1   0    0    1   
$EndComp
NoConn ~ 8550 3650
$Comp
L R R1
U 1 1 589793EA
P 6300 2800
F 0 "R1" V 6380 2800 50  0000 C CNN
F 1 "10k" V 6300 2800 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5350 2200 50  0000 C CNN
F 3 "" H 6300 2800 50  0000 C CNN
	1    6300 2800
	1    0    0    -1  
$EndComp
$Comp
L DIY-OLED-0.96 U4
U 1 1 5898EB2B
P 6650 4950
F 0 "U4" V 6450 4850 60  0000 C CNN
F 1 "DIY-OLED-0.96" H 6650 5050 60  0000 C CNN
F 2 "DIY-OLED:DIY-OLED-0.96" H 6650 5300 60  0000 C CNN
F 3 "" H 6650 4950 60  0000 C CNN
	1    6650 4950
	-1   0    0    1   
$EndComp
NoConn ~ 7900 2550
NoConn ~ 7800 2550
NoConn ~ 7600 2550
NoConn ~ 7500 2550
NoConn ~ 7400 2550
$Comp
L ESP-12E U3
U 1 1 58977E50
P 7650 3450
F 0 "U3" H 7650 3350 50  0000 C CNN
F 1 "ESP-12E" H 7650 3550 50  0000 C CNN
F 2 "ESP8266:ESP-12" H 7650 3450 50  0001 C CNN
F 3 "" H 7650 3450 50  0001 C CNN
	1    7650 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	2700 3600 2700 2850
Wire Wire Line
	2700 2850 3250 2850
Wire Wire Line
	3850 2950 4000 2950
Wire Wire Line
	3000 3150 3000 3600
Wire Wire Line
	3000 3500 3050 3500
Connection ~ 3000 3500
Wire Wire Line
	2700 3700 3050 3700
Wire Wire Line
	2850 3550 2850 4050
Connection ~ 2850 3700
Wire Wire Line
	3850 3500 3900 3500
Wire Wire Line
	4000 2950 4000 3150
Wire Wire Line
	4000 3150 2850 3150
Wire Wire Line
	2850 3150 2850 3250
Connection ~ 3000 3150
Wire Wire Line
	4850 3400 4850 3850
Wire Wire Line
	3850 3700 4850 3700
Connection ~ 4850 3700
Wire Wire Line
	3000 3600 3050 3600
Wire Wire Line
	4500 3500 4850 3500
Connection ~ 4850 3500
Wire Wire Line
	5450 3750 6750 3750
Wire Wire Line
	8550 3750 9350 3750
Wire Wire Line
	5450 3750 5450 4550
Wire Wire Line
	5550 3650 6750 3650
Wire Wire Line
	5550 3650 5550 4550
Wire Wire Line
	6100 3350 6750 3350
Wire Wire Line
	6650 2950 6650 3350
Wire Wire Line
	6000 3050 6000 3150
Wire Wire Line
	6100 2750 6100 3350
Connection ~ 6650 3350
Wire Wire Line
	6000 3050 6750 3050
Wire Wire Line
	8650 3050 8550 3050
Wire Wire Line
	6450 3450 6750 3450
Wire Wire Line
	6450 3450 6450 4550
Wire Wire Line
	6650 4550 6650 4450
$Comp
L R R5
U 1 1 58D3649D
P 9050 2950
F 0 "R5" V 9130 2950 50  0000 C CNN
F 1 "10k" V 9050 2950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8900 2600 50  0000 C CNN
F 3 "" H 9050 2950 50  0000 C CNN
	1    9050 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	8550 3550 9050 3550
Wire Wire Line
	9050 3550 9050 3100
$Comp
L R R6
U 1 1 58D6E0DB
P 9350 2950
F 0 "R6" V 9430 2950 50  0000 C CNN
F 1 "10k" V 9350 2950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 9600 3150 50  0000 C CNN
F 3 "" H 9350 2950 50  0000 C CNN
	1    9350 2950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 3750 9350 3100
$Comp
L +3.3V #PWR01
U 1 1 58D6E218
P 4850 3400
F 0 "#PWR01" H 4850 3250 50  0001 C CNN
F 1 "+3.3V" H 4850 3540 50  0000 C CNN
F 2 "" H 4850 3400 50  0000 C CNN
F 3 "" H 4850 3400 50  0000 C CNN
	1    4850 3400
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 58D6E2FE
P 6300 2550
F 0 "#PWR02" H 6300 2400 50  0001 C CNN
F 1 "+3.3V" H 6300 2690 50  0000 C CNN
F 2 "" H 6300 2550 50  0000 C CNN
F 3 "" H 6300 2550 50  0000 C CNN
	1    6300 2550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR03
U 1 1 58D6E3A2
P 6750 4350
F 0 "#PWR03" H 6750 4200 50  0001 C CNN
F 1 "+3.3V" H 6750 4490 50  0000 C CNN
F 2 "" H 6750 4350 50  0000 C CNN
F 3 "" H 6750 4350 50  0000 C CNN
	1    6750 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 4350 6750 4550
$Comp
L +3.3V #PWR04
U 1 1 58D6E5CC
P 6650 2550
F 0 "#PWR04" H 6650 2400 50  0001 C CNN
F 1 "+3.3V" H 6650 2690 50  0000 C CNN
F 2 "" H 6650 2550 50  0000 C CNN
F 3 "" H 6650 2550 50  0000 C CNN
	1    6650 2550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 58D6E79B
P 9050 2700
F 0 "#PWR05" H 9050 2550 50  0001 C CNN
F 1 "+3.3V" H 9050 2840 50  0000 C CNN
F 2 "" H 9050 2700 50  0000 C CNN
F 3 "" H 9050 2700 50  0000 C CNN
	1    9050 2700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR06
U 1 1 58D6E7CF
P 9350 2700
F 0 "#PWR06" H 9350 2550 50  0001 C CNN
F 1 "+3.3V" H 9350 2840 50  0000 C CNN
F 2 "" H 9350 2700 50  0000 C CNN
F 3 "" H 9350 2700 50  0000 C CNN
	1    9350 2700
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 58D6E803
P 8650 2700
F 0 "#PWR07" H 8650 2550 50  0001 C CNN
F 1 "+3.3V" H 8650 2840 50  0000 C CNN
F 2 "" H 8650 2700 50  0000 C CNN
F 3 "" H 8650 2700 50  0000 C CNN
	1    8650 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 2700 8650 3050
Wire Wire Line
	9350 2700 9350 2800
Wire Wire Line
	9050 2700 9050 2800
Wire Wire Line
	6300 2550 6300 2650
Wire Wire Line
	6300 2950 6300 3250
Wire Wire Line
	6300 3250 6750 3250
Wire Wire Line
	6750 3550 6550 3550
Wire Wire Line
	6550 3550 6550 4550
Wire Wire Line
	6750 3150 6200 3150
$Comp
L GND #PWR08
U 1 1 58D6ED20
P 6200 4200
F 0 "#PWR08" H 6200 3950 50  0001 C CNN
F 1 "GND" H 6200 4050 50  0000 C CNN
F 2 "" H 6200 4200 50  0000 C CNN
F 3 "" H 6200 4200 50  0000 C CNN
	1    6200 4200
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 58D6EE96
P 6200 3950
F 0 "R4" V 6280 3950 50  0000 C CNN
F 1 "10k" V 6200 3950 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 6000 3400 50  0000 C CNN
F 3 "" H 6200 3950 50  0000 C CNN
	1    6200 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 3150 6200 3800
Wire Wire Line
	6200 4100 6200 4200
$Comp
L GND #PWR09
U 1 1 58D6F166
P 2850 4050
F 0 "#PWR09" H 2850 3800 50  0001 C CNN
F 1 "GND" H 2850 3900 50  0000 C CNN
F 2 "" H 2850 4050 50  0000 C CNN
F 3 "" H 2850 4050 50  0000 C CNN
	1    2850 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 58D6F261
P 4850 4250
F 0 "#PWR010" H 4850 4000 50  0001 C CNN
F 1 "GND" H 4850 4100 50  0000 C CNN
F 2 "" H 4850 4250 50  0000 C CNN
F 3 "" H 4850 4250 50  0000 C CNN
	1    4850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4850 4150 4850 4250
$Comp
L GND #PWR011
U 1 1 58D6F42E
P 6000 3150
F 0 "#PWR011" H 6000 2900 50  0001 C CNN
F 1 "GND" H 6000 3000 50  0000 C CNN
F 2 "" H 6000 3150 50  0000 C CNN
F 3 "" H 6000 3150 50  0000 C CNN
	1    6000 3150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 58D6F4C6
P 5200 4550
F 0 "#PWR012" H 5200 4300 50  0001 C CNN
F 1 "GND" H 5200 4400 50  0000 C CNN
F 2 "" H 5200 4550 50  0000 C CNN
F 3 "" H 5200 4550 50  0000 C CNN
	1    5200 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 4550 5200 4550
$Comp
L GND #PWR013
U 1 1 58D6F646
P 6900 4550
F 0 "#PWR013" H 6900 4300 50  0001 C CNN
F 1 "GND" H 6900 4400 50  0000 C CNN
F 2 "" H 6900 4550 50  0000 C CNN
F 3 "" H 6900 4550 50  0000 C CNN
	1    6900 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 4450 6900 4450
Wire Wire Line
	6900 4450 6900 4550
$Comp
L SF303GJ26 U5
U 1 1 59534F8F
P 9650 4850
F 0 "U5" H 9500 4900 60  0000 C CNN
F 1 "SF303GJ26" H 9500 4650 60  0000 C CNN
F 2 "SF303GJ26:SF303GJ26" H 9450 4500 60  0000 C CNN
F 3 "" H 9650 4850 60  0000 C CNN
	1    9650 4850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5953540A
P 10100 5000
F 0 "#PWR014" H 10100 4750 50  0001 C CNN
F 1 "GND" H 10100 4850 50  0000 C CNN
F 2 "" H 10100 5000 50  0000 C CNN
F 3 "" H 10100 5000 50  0000 C CNN
	1    10100 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 4800 10100 4800
Wire Wire Line
	10100 4800 10100 5000
Wire Wire Line
	6650 2550 6650 2650
NoConn ~ 7700 2550
Wire Wire Line
	10100 3250 10100 4700
Wire Wire Line
	10100 3250 8550 3250
Wire Wire Line
	8900 4900 9000 4900
Wire Wire Line
	8900 3350 8900 4900
Wire Wire Line
	8900 3350 8550 3350
Wire Wire Line
	8800 4700 9000 4700
Wire Wire Line
	8800 3450 8550 3450
$Comp
L Switch_DPST SW2
U 1 1 59611BF3
P 5900 2450
F 0 "SW2" V 6050 2450 50  0000 C CNN
F 1 "Switch_DPST" V 5950 1850 50  0000 C CNN
F 2 "KMR6:KMR641NG" V 6400 2450 50  0000 C CNN
F 3 "" H 5900 2450 50  0000 C CNN
	1    5900 2450
	0    1    -1   0   
$EndComp
$Comp
L GND #PWR015
U 1 1 59615099
P 6300 2150
F 0 "#PWR015" H 6300 1900 50  0001 C CNN
F 1 "GND" H 6300 2000 50  0000 C CNN
F 2 "" H 6300 2150 50  0000 C CNN
F 3 "" H 6300 2150 50  0000 C CNN
	1    6300 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2050 6100 2150
Wire Wire Line
	6100 2050 6300 2050
Wire Wire Line
	6300 2050 6300 2150
NoConn ~ 5700 2150
NoConn ~ 5700 2750
$Comp
L R_PACK4 RP1
U 1 1 59615F7B
P 9650 4200
F 0 "RP1" H 9650 4650 50  0000 C CNN
F 1 "10k" H 9650 4150 50  0000 C CNN
F 2 "R_Array:R_Array_Concave_4x0805" H 10300 3400 50  0000 C CNN
F 3 "" H 9650 4200 50  0000 C CNN
	1    9650 4200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	8800 3450 8800 4700
Wire Wire Line
	10100 4700 10000 4700
Wire Wire Line
	10200 3150 8550 3150
$Comp
L +3.3V #PWR016
U 1 1 59616333
P 9600 3800
F 0 "#PWR016" H 9600 3650 50  0001 C CNN
F 1 "+3.3V" H 9600 3940 50  0000 C CNN
F 2 "" H 9600 3800 50  0000 C CNN
F 3 "" H 9600 3800 50  0000 C CNN
	1    9600 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 3800 9600 4000
Wire Wire Line
	9300 3900 9600 3900
Wire Wire Line
	9500 3900 9500 4000
Connection ~ 9600 3900
Wire Wire Line
	9400 3900 9400 4000
Connection ~ 9500 3900
Wire Wire Line
	9300 3900 9300 4000
Connection ~ 9400 3900
Wire Wire Line
	9300 4400 9300 4450
Wire Wire Line
	9400 4400 9400 4550
Connection ~ 8800 4450
Wire Wire Line
	9500 4400 9500 4550
Connection ~ 10200 4450
Wire Wire Line
	9600 4400 9600 4450
Connection ~ 10100 4550
Connection ~ 8900 4550
Wire Wire Line
	9600 4450 10400 4450
Wire Wire Line
	9500 4550 10100 4550
Wire Wire Line
	9400 4550 8900 4550
Wire Wire Line
	9300 4450 8800 4450
$Comp
L R R7
U 1 1 5961DC36
P 10200 4750
F 0 "R7" V 10280 4750 50  0000 C CNN
F 1 "0" V 10200 4750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 10130 4750 50  0001 C CNN
F 3 "" H 10200 4750 50  0001 C CNN
	1    10200 4750
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 5961DC89
P 10400 4750
F 0 "R8" V 10480 4750 50  0000 C CNN
F 1 "0" V 10400 4750 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 10330 4750 50  0001 C CNN
F 3 "" H 10400 4750 50  0001 C CNN
	1    10400 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10200 3150 10200 4600
Wire Wire Line
	10400 4450 10400 4600
Wire Wire Line
	10000 4900 10200 4900
Wire Wire Line
	9000 4800 8800 4800
Wire Wire Line
	8800 4800 8800 5300
Wire Wire Line
	8800 5300 10400 5300
Wire Wire Line
	10400 5300 10400 4900
$EndSCHEMATC
