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
LIBS:ESP8266
LIBS:nebrius-lcd
LIBS:nebrius-regulators
LIBS:nebrius-switch
LIBS:nebrius-conn
LIBS:nebrius-logic
LIBS:logic_programmable
LIBS:74xgxx
LIBS:onsemi
LIBS:Hub Board-cache
EELAYER 25 0
EELAYER END
$Descr USLetter 11000 8500
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
P 2500 4800
F 0 "U1" H 2300 5050 60  0000 L CNN
F 1 "LM3671" H 2250 4550 60  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5_HandSoldering" H 2000 3950 60  0000 C CNN
F 3 "" H 2500 4450 60  0000 C CNN
	1    2500 4800
	1    0    0    -1  
$EndComp
$Comp
L Switch_SPDT_x2 SW1
U 1 1 57A0DADD
P 2550 4150
F 0 "SW1" H 2350 4300 50  0000 C CNN
F 1 "Switch_SPDT_x2" H 2300 4000 50  0000 C CNN
F 2 "CL-SA-12C:CL-SA-12C4" H 2850 4450 50  0000 C CNN
F 3 "" H 2550 4150 50  0000 C CNN
	1    2550 4150
	-1   0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 57A0E1B1
P 3450 4700
F 0 "L1" V 3400 4700 50  0000 C CNN
F 1 "2.2uH" V 3550 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3700 5200 50  0000 C CNN
F 3 "" H 3450 4700 50  0000 C CNN
	1    3450 4700
	0    -1   -1   0   
$EndComp
$Comp
L C C2
U 1 1 57A0E396
P 3850 5200
F 0 "C2" H 3875 5300 50  0000 L CNN
F 1 "10uF" H 4000 5200 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3100 4500 50  0000 C CNN
F 3 "" H 3850 5200 50  0000 C CNN
	1    3850 5200
	1    0    0    -1  
$EndComp
$Comp
L C C1
U 1 1 57A0E3BE
P 1850 4600
F 0 "C1" H 1875 4700 50  0000 L CNN
F 1 "4.7uF" H 1600 4700 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1350 4800 50  0000 C CNN
F 3 "" H 1850 4600 50  0000 C CNN
	1    1850 4600
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 584F7038
P 4450 5950
F 0 "P4" H 4450 6150 50  0000 C CNN
F 1 "SERIAL" V 4550 5950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.00mm" V 4700 5150 50  0000 C CNN
F 3 "" H 4450 5950 50  0000 C CNN
	1    4450 5950
	0    1    1    0   
$EndComp
$Comp
L DIY-OLED-0.96 U4
U 1 1 5898EB2B
P 5750 6100
F 0 "U4" V 5550 6000 60  0000 C CNN
F 1 "DIY-OLED-0.96" H 5750 6200 60  0000 C CNN
F 2 "nebrius-lcd:DIY-OLED-0.96" H 6050 7650 60  0000 C CNN
F 3 "" H 5750 6100 60  0000 C CNN
	1    5750 6100
	-1   0    0    1   
$EndComp
$Comp
L ESP-12E U3
U 1 1 58977E50
P 6650 4650
F 0 "U3" H 6650 4550 50  0000 C CNN
F 1 "ESP-12E" H 6650 4750 50  0000 C CNN
F 2 "ESP8266:ESP-12E_SMD" H 6650 4650 50  0001 C CNN
F 3 "" H 6650 4650 50  0001 C CNN
	1    6650 4650
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR01
U 1 1 58D6F166
P 1850 5250
F 0 "#PWR01" H 1850 5000 50  0001 C CNN
F 1 "GND" H 1850 5100 50  0000 C CNN
F 2 "" H 1850 5250 50  0000 C CNN
F 3 "" H 1850 5250 50  0000 C CNN
	1    1850 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 58D6F261
P 3850 5450
F 0 "#PWR02" H 3850 5200 50  0001 C CNN
F 1 "GND" H 3850 5300 50  0000 C CNN
F 2 "" H 3850 5450 50  0000 C CNN
F 3 "" H 3850 5450 50  0000 C CNN
	1    3850 5450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 58D6F42E
P 5000 4650
F 0 "#PWR03" H 5000 4400 50  0001 C CNN
F 1 "GND" H 5000 4500 50  0000 C CNN
F 2 "" H 5000 4650 50  0000 C CNN
F 3 "" H 5000 4650 50  0000 C CNN
	1    5000 4650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 58D6F4C6
P 4200 5750
F 0 "#PWR04" H 4200 5500 50  0001 C CNN
F 1 "GND" H 4200 5600 50  0000 C CNN
F 2 "" H 4200 5750 50  0000 C CNN
F 3 "" H 4200 5750 50  0000 C CNN
	1    4200 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR05
U 1 1 58D6F646
P 6000 5700
F 0 "#PWR05" H 6000 5450 50  0001 C CNN
F 1 "GND" H 6000 5550 50  0000 C CNN
F 2 "" H 6000 5700 50  0000 C CNN
F 3 "" H 6000 5700 50  0000 C CNN
	1    6000 5700
	1    0    0    -1  
$EndComp
$Comp
L Switch_DPST SW2
U 1 1 59611BF3
P 4900 3650
F 0 "SW2" V 5050 3650 50  0000 C CNN
F 1 "Switch_DPST" V 4950 3050 50  0000 C CNN
F 2 "nebrius-switch:KMR641NG" V 4750 2800 50  0000 C CNN
F 3 "" H 4900 3650 50  0000 C CNN
	1    4900 3650
	0    1    -1   0   
$EndComp
$Comp
L GND #PWR06
U 1 1 59615099
P 4900 3100
F 0 "#PWR06" H 4900 2850 50  0001 C CNN
F 1 "GND" H 4900 2950 50  0000 C CNN
F 2 "" H 4900 3100 50  0000 C CNN
F 3 "" H 4900 3100 50  0000 C CNN
	1    4900 3100
	1    0    0    -1  
$EndComp
NoConn ~ 4700 3350
NoConn ~ 4700 3950
$Comp
L R_PACK4 RP1
U 1 1 59615F7B
P 8550 5750
F 0 "RP1" H 8550 6200 50  0000 C CNN
F 1 "10k" H 8550 5700 50  0000 C CNN
F 2 "nebrius-discrete:R_Array_Concave_4x2012.kicad_mod" V 7550 6050 50  0000 C CNN
F 3 "" H 8550 5750 50  0000 C CNN
	1    8550 5750
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR07
U 1 1 5A47E663
P 3000 3800
F 0 "#PWR07" H 3000 3650 50  0001 C CNN
F 1 "+5V" H 3000 3940 50  0000 C CNN
F 2 "" H 3000 3800 50  0001 C CNN
F 3 "" H 3000 3800 50  0001 C CNN
	1    3000 3800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR08
U 1 1 5A47E6D7
P 9600 1350
F 0 "#PWR08" H 9600 1200 50  0001 C CNN
F 1 "+5V" H 9600 1490 50  0000 C CNN
F 2 "" H 9600 1350 50  0001 C CNN
F 3 "" H 9600 1350 50  0001 C CNN
	1    9600 1350
	1    0    0    -1  
$EndComp
NoConn ~ 6900 3750
NoConn ~ 6800 3750
$Comp
L 10118194 U0
U 1 1 5A5AABAF
P 1000 4900
F 0 "U0" H 900 5400 60  0000 C CNN
F 1 "10118194" H 900 5200 60  0000 C CNN
F 2 "nebrius-conn:10118194" H 1000 4900 60  0001 C CNN
F 3 "" H 1000 4900 60  0001 C CNN
	1    1000 4900
	0    -1   -1   0   
$EndComp
NoConn ~ 1200 4900
NoConn ~ 1200 5000
NoConn ~ 1200 5100
NoConn ~ 2250 4050
$Comp
L GND #PWR09
U 1 1 5A6AE250
P 7100 2050
F 0 "#PWR09" H 7100 1800 50  0001 C CNN
F 1 "GND" H 7100 1900 50  0000 C CNN
F 2 "" H 7100 2050 50  0001 C CNN
F 3 "" H 7100 2050 50  0001 C CNN
	1    7100 2050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 5A6B6F9C
P 7100 2900
F 0 "#PWR010" H 7100 2650 50  0001 C CNN
F 1 "GND" H 7100 2750 50  0000 C CNN
F 2 "" H 7100 2900 50  0001 C CNN
F 3 "" H 7100 2900 50  0001 C CNN
	1    7100 2900
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 5A6B7018
P 5600 2900
F 0 "#PWR011" H 5600 2650 50  0001 C CNN
F 1 "GND" H 5600 2750 50  0000 C CNN
F 2 "" H 5600 2900 50  0001 C CNN
F 3 "" H 5600 2900 50  0001 C CNN
	1    5600 2900
	-1   0    0    -1  
$EndComp
$Comp
L +5V #PWR012
U 1 1 5A6B779E
P 7000 2500
F 0 "#PWR012" H 7000 2350 50  0001 C CNN
F 1 "+5V" H 7000 2640 50  0000 C CNN
F 2 "" H 7000 2500 50  0001 C CNN
F 3 "" H 7000 2500 50  0001 C CNN
	1    7000 2500
	-1   0    0    -1  
$EndComp
$Comp
L Fuse F0
U 1 1 5A6BD755
P 1450 5200
F 0 "F0" V 1530 5200 50  0000 C CNN
F 1 "Fuse" V 1375 5200 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1380 5200 50  0001 C CNN
F 3 "" H 1450 5200 50  0001 C CNN
	1    1450 5200
	0    1    1    0   
$EndComp
NoConn ~ 6700 3750
NoConn ~ 6600 3750
NoConn ~ 6500 3750
NoConn ~ 6400 3750
$Comp
L SF303GJ26 U5
U 1 1 59534F8F
P 8500 6450
F 0 "U5" H 8350 6500 60  0000 C CNN
F 1 "SF303GJ26" H 8350 6250 60  0000 C CNN
F 2 "nebrius-switch:SF303GJ26" H 8800 6000 60  0000 C CNN
F 3 "" H 8500 6450 60  0000 C CNN
	1    8500 6450
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x03 J5
U 1 1 5AB1617F
P 9600 4850
F 0 "J5" H 9600 5050 50  0000 C CNN
F 1 "Sensor" H 9600 4650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.00mm" H 9600 4850 50  0001 C CNN
F 3 "" H 9600 4850 50  0001 C CNN
	1    9600 4850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5AB174D5
P 9300 5050
F 0 "#PWR013" H 9300 4800 50  0001 C CNN
F 1 "GND" H 9300 4900 50  0000 C CNN
F 2 "" H 9300 5050 50  0001 C CNN
F 3 "" H 9300 5050 50  0001 C CNN
	1    9300 5050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5AB1859C
P 9250 6500
F 0 "#PWR014" H 9250 6250 50  0001 C CNN
F 1 "GND" H 9250 6350 50  0000 C CNN
F 2 "" H 9250 6500 50  0001 C CNN
F 3 "" H 9250 6500 50  0001 C CNN
	1    9250 6500
	1    0    0    -1  
$EndComp
$Comp
L 74AUP1G14 U2
U 1 1 5AB191E4
P 6700 6500
F 0 "U2" H 6600 6650 50  0000 C CNN
F 1 "74AUP1G14" H 6700 6350 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5_HandSoldering" H 6700 6500 50  0001 C CNN
F 3 "" H 6700 6500 50  0001 C CNN
	1    6700 6500
	-1   0    0    1   
$EndComp
$Comp
L VCC #PWR015
U 1 1 5AB1AF17
P 3850 4600
F 0 "#PWR015" H 3850 4450 50  0001 C CNN
F 1 "VCC" H 3850 4750 50  0000 C CNN
F 2 "" H 3850 4600 50  0001 C CNN
F 3 "" H 3850 4600 50  0001 C CNN
	1    3850 4600
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR016
U 1 1 5AB1B2B1
P 5850 5500
F 0 "#PWR016" H 5850 5350 50  0001 C CNN
F 1 "VCC" H 5850 5650 50  0000 C CNN
F 2 "" H 5850 5500 50  0001 C CNN
F 3 "" H 5850 5500 50  0001 C CNN
	1    5850 5500
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR017
U 1 1 5AB1B53C
P 8200 5350
F 0 "#PWR017" H 8200 5200 50  0001 C CNN
F 1 "VCC" H 8200 5500 50  0000 C CNN
F 2 "" H 8200 5350 50  0001 C CNN
F 3 "" H 8200 5350 50  0001 C CNN
	1    8200 5350
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR018
U 1 1 5AB1B5C4
P 9300 4650
F 0 "#PWR018" H 9300 4500 50  0001 C CNN
F 1 "VCC" H 9300 4800 50  0000 C CNN
F 2 "" H 9300 4650 50  0001 C CNN
F 3 "" H 9300 4650 50  0001 C CNN
	1    9300 4650
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR019
U 1 1 5AB1B881
P 7650 3900
F 0 "#PWR019" H 7650 3750 50  0001 C CNN
F 1 "VCC" H 7650 4050 50  0000 C CNN
F 2 "" H 7650 3900 50  0001 C CNN
F 3 "" H 7650 3900 50  0001 C CNN
	1    7650 3900
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR020
U 1 1 5AB1BAEF
P 5700 2500
F 0 "#PWR020" H 5700 2350 50  0001 C CNN
F 1 "VCC" H 5700 2650 50  0000 C CNN
F 2 "" H 5700 2500 50  0001 C CNN
F 3 "" H 5700 2500 50  0001 C CNN
	1    5700 2500
	1    0    0    -1  
$EndComp
$Comp
L R_PACK4 RP2
U 1 1 5AB1C1CF
P 6750 3250
F 0 "RP2" H 6750 3700 50  0000 C CNN
F 1 "10k" H 6750 3200 50  0000 C CNN
F 2 "nebrius-discrete:R_Array_Concave_4x2012.kicad_mod" V 6750 1950 50  0000 C CNN
F 3 "" H 6750 3250 50  0000 C CNN
	1    6750 3250
	0    -1   -1   0   
$EndComp
$Comp
L VCC #PWR021
U 1 1 5AB1C417
P 6050 3200
F 0 "#PWR021" H 6050 3050 50  0001 C CNN
F 1 "VCC" H 6050 3350 50  0000 C CNN
F 2 "" H 6050 3200 50  0001 C CNN
F 3 "" H 6050 3200 50  0001 C CNN
	1    6050 3200
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5AB1D5D2
P 5500 4000
F 0 "R1" V 5580 4000 50  0000 C CNN
F 1 "1k" V 5500 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 5430 4000 50  0001 C CNN
F 3 "" H 5500 4000 50  0001 C CNN
	1    5500 4000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR022
U 1 1 5AB1D659
P 5500 3750
F 0 "#PWR022" H 5500 3600 50  0001 C CNN
F 1 "VCC" H 5500 3900 50  0000 C CNN
F 2 "" H 5500 3750 50  0001 C CNN
F 3 "" H 5500 3750 50  0001 C CNN
	1    5500 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 4350 5350 4350
Wire Wire Line
	5650 3650 5650 4450
Wire Wire Line
	8850 6400 9250 6400
Wire Wire Line
	5550 4650 5550 5700
Wire Wire Line
	5750 4650 5550 4650
Wire Wire Line
	5650 4750 5650 5700
Wire Wire Line
	5750 4750 5650 4750
Wire Wire Line
	2000 4350 2000 4900
Wire Wire Line
	2000 4900 2050 4900
Wire Wire Line
	1600 4800 1600 4800
Connection ~ 1850 4800
Connection ~ 7850 2800
Wire Wire Line
	7200 2800 7200 1850
Connection ~ 8500 2800
Wire Wire Line
	7850 2800 7850 1850
Wire Wire Line
	8500 2800 8500 1850
Wire Wire Line
	9150 2800 9150 1850
Wire Wire Line
	7550 4850 9400 4850
Wire Wire Line
	1700 5200 1600 5200
Wire Wire Line
	1200 5200 1300 5200
Wire Wire Line
	7000 2500 7000 2600
Wire Wire Line
	1700 4250 1700 5200
Wire Wire Line
	2250 4250 1700 4250
Connection ~ 3000 4150
Wire Wire Line
	1200 4800 2050 4800
Wire Wire Line
	4900 3000 4900 3100
Wire Wire Line
	5100 3000 4900 3000
Wire Wire Line
	5100 3000 5100 3350
Wire Wire Line
	6000 5600 6000 5700
Wire Wire Line
	5750 5600 6000 5600
Wire Wire Line
	4350 5750 4200 5750
Wire Wire Line
	3850 5350 3850 5450
Wire Wire Line
	7650 3900 7650 4250
Wire Wire Line
	5850 5500 5850 5700
Wire Wire Line
	8150 3550 8150 4950
Wire Wire Line
	8050 3650 8050 4750
Wire Wire Line
	8050 4750 7550 4750
Wire Wire Line
	5750 5700 5750 5600
Wire Wire Line
	7650 4250 7550 4250
Wire Wire Line
	5000 4250 5750 4250
Connection ~ 5500 4550
Wire Wire Line
	5100 3950 5100 4550
Wire Wire Line
	5000 4250 5000 4650
Wire Wire Line
	5500 4550 5500 4150
Wire Wire Line
	4550 4850 4550 5750
Wire Wire Line
	4550 4850 5750 4850
Wire Wire Line
	4450 4950 4450 5750
Wire Wire Line
	8150 4950 7550 4950
Wire Wire Line
	4450 4950 5750 4950
Connection ~ 3850 4700
Wire Wire Line
	3750 4700 3850 4700
Connection ~ 3850 4900
Wire Wire Line
	3850 4900 2950 4900
Wire Wire Line
	3850 4600 3850 5050
Connection ~ 2000 4350
Wire Wire Line
	1850 4350 1850 4450
Wire Wire Line
	3000 4350 1850 4350
Wire Wire Line
	3000 3800 3000 4350
Wire Wire Line
	2950 4700 3150 4700
Wire Wire Line
	1850 4750 1850 5250
Connection ~ 2000 4700
Wire Wire Line
	2000 4700 2050 4700
Wire Wire Line
	2850 4150 3000 4150
Connection ~ 7200 2800
Connection ~ 5650 4450
Wire Wire Line
	8950 6300 8850 6300
Wire Wire Line
	8950 6150 8400 6150
Wire Wire Line
	8400 6150 8400 5950
Wire Wire Line
	9050 6500 8850 6500
Wire Wire Line
	9050 5050 9050 6500
Wire Wire Line
	9050 6050 8500 6050
Wire Wire Line
	8500 6050 8500 5950
Wire Wire Line
	7750 6300 7850 6300
Wire Wire Line
	7750 4550 7750 6300
Wire Wire Line
	7750 6150 8300 6150
Wire Wire Line
	8300 6150 8300 5950
Wire Wire Line
	7650 6050 8200 6050
Wire Wire Line
	8200 6050 8200 5950
Wire Wire Line
	8200 5450 8500 5450
Wire Wire Line
	8300 5450 8300 5550
Wire Wire Line
	8500 5450 8500 5550
Wire Wire Line
	8400 5450 8400 5550
Connection ~ 8300 5450
Connection ~ 8400 5450
Connection ~ 8200 5450
Wire Wire Line
	9400 4950 9300 4950
Wire Wire Line
	9300 4950 9300 5050
Wire Wire Line
	9400 4750 9300 4750
Wire Wire Line
	9300 4750 9300 4650
Connection ~ 7650 6050
Wire Wire Line
	7750 4550 7550 4550
Connection ~ 7750 6150
Wire Wire Line
	7850 5150 8950 5150
Connection ~ 8950 6150
Wire Wire Line
	7550 4350 7950 4350
Wire Wire Line
	7950 4350 7950 5050
Wire Wire Line
	7950 5050 9050 5050
Connection ~ 9050 6050
Wire Wire Line
	8200 5350 8200 5550
Wire Wire Line
	9250 6400 9250 6500
Wire Wire Line
	7250 6300 7250 6500
Connection ~ 7250 6500
Wire Wire Line
	7650 6400 7850 6400
Wire Wire Line
	7000 6500 7850 6500
Wire Wire Line
	7650 4450 7550 4450
Wire Wire Line
	7550 4650 7850 4650
Wire Wire Line
	7850 4650 7850 5150
Wire Wire Line
	7650 4450 7650 6400
Wire Wire Line
	6400 3050 6400 3000
Wire Wire Line
	6200 3000 6700 3000
Wire Wire Line
	6200 3000 6200 3300
Wire Wire Line
	6200 3300 6050 3300
Wire Wire Line
	6050 3300 6050 3200
Wire Wire Line
	6500 3000 6500 3050
Connection ~ 6400 3000
Wire Wire Line
	6600 3000 6600 3050
Connection ~ 6500 3000
Wire Wire Line
	6700 3000 6700 3050
Connection ~ 6600 3000
Wire Wire Line
	8050 3650 6600 3650
Wire Wire Line
	6600 3650 6600 3450
Wire Wire Line
	8150 3550 6700 3550
Wire Wire Line
	6700 3550 6700 3450
Wire Wire Line
	6500 3450 6500 3650
Wire Wire Line
	6500 3650 5650 3650
Wire Wire Line
	5500 3750 5500 3850
Wire Wire Line
	6400 3450 6400 3550
Wire Wire Line
	6400 3550 5250 3550
Wire Wire Line
	5250 3550 5250 6300
Wire Wire Line
	5250 6300 7250 6300
Wire Wire Line
	5350 4350 5350 6500
Wire Wire Line
	5350 6500 6450 6500
Wire Wire Line
	8950 5150 8950 6300
Wire Wire Line
	5700 2500 5700 2600
Wire Wire Line
	6900 2800 9150 2800
Wire Wire Line
	6900 2700 7100 2700
Wire Wire Line
	7000 2600 6900 2600
Wire Wire Line
	4600 4450 5750 4450
Wire Wire Line
	4600 4450 4600 2800
Wire Wire Line
	4600 2800 5800 2800
Wire Wire Line
	5800 2700 5600 2700
Wire Wire Line
	5700 2600 5800 2600
$Comp
L SN74LVC1T45 U7
U 1 1 5A6B663C
P 6350 2700
F 0 "U7" H 6350 2700 60  0000 C CNN
F 1 "SN74LVC1T45" H 6350 3000 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6_Handsoldering" H 6350 2700 60  0001 C CNN
F 3 "" H 6350 2700 60  0001 C CNN
	1    6350 2700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5100 4550 5750 4550
Wire Wire Line
	5600 2700 5600 2900
Wire Wire Line
	7100 2700 7100 2900
$Comp
L NeoPixel J1
U 1 1 5AF67332
P 7200 1650
F 0 "J1" V 6750 1650 60  0000 C CNN
F 1 "NeoPixel" V 6900 1650 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03_Pitch2.54mm" H 7200 1650 60  0001 C CNN
F 3 "" H 7200 1650 60  0001 C CNN
	1    7200 1650
	0    1    1    0   
$EndComp
$Comp
L NeoPixel J2
U 1 1 5AF673AE
P 7850 1650
F 0 "J2" V 7400 1650 60  0000 C CNN
F 1 "NeoPixel" V 7550 1650 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03_Pitch2.54mm" H 7850 1650 60  0001 C CNN
F 3 "" H 7850 1650 60  0001 C CNN
	1    7850 1650
	0    1    1    0   
$EndComp
$Comp
L NeoPixel J3
U 1 1 5AF67417
P 8500 1650
F 0 "J3" V 8050 1650 60  0000 C CNN
F 1 "NeoPixel" V 8200 1650 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03_Pitch2.54mm" H 8500 1650 60  0001 C CNN
F 3 "" H 8500 1650 60  0001 C CNN
	1    8500 1650
	0    1    1    0   
$EndComp
$Comp
L NeoPixel J4
U 1 1 5AF67484
P 9150 1650
F 0 "J4" V 8700 1650 60  0000 C CNN
F 1 "NeoPixel" V 8850 1650 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03_Pitch2.54mm" H 9150 1650 60  0001 C CNN
F 3 "" H 9150 1650 60  0001 C CNN
	1    9150 1650
	0    1    1    0   
$EndComp
Wire Wire Line
	7100 1850 7100 2050
Wire Wire Line
	7100 1950 9050 1950
Wire Wire Line
	7750 1950 7750 1850
Connection ~ 7100 1950
Wire Wire Line
	8400 1950 8400 1850
Connection ~ 7750 1950
Wire Wire Line
	9050 1950 9050 1850
Connection ~ 8400 1950
Wire Wire Line
	9600 2050 9600 1350
Wire Wire Line
	7300 2050 9600 2050
Wire Wire Line
	9250 2050 9250 1850
Wire Wire Line
	8600 2050 8600 1850
Connection ~ 9250 2050
Wire Wire Line
	7950 2050 7950 1850
Connection ~ 8600 2050
Wire Wire Line
	7300 2050 7300 1850
Connection ~ 7950 2050
$EndSCHEMATC
