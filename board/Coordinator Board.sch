EESchema Schematic File Version 2
LIBS:Coordinator Board-rescue
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
LIBS:Coordinator Board-cache
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
L LM3671-RESCUE-Coordinator_Board U1
U 1 1 57A0D83A
P 2450 4800
F 0 "U1" H 2200 5025 60  0000 L CNN
F 1 "LM3671" H 2200 4550 60  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 1950 3950 60  0000 C CNN
F 3 "" H 2450 4450 60  0000 C CNN
	1    2450 4800
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
P 3200 4700
F 0 "L1" V 3150 4700 50  0000 C CNN
F 1 "2.2uH" V 3300 4700 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 3400 5000 50  0000 C CNN
F 3 "" H 3200 4700 50  0000 C CNN
	1    3200 4700
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
L R R2
U 1 1 58978884
P 5650 4000
F 0 "R2" V 5730 4000 50  0000 C CNN
F 1 "1k" V 5650 4000 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" H 5150 3400 50  0000 C CNN
F 3 "" H 5650 4000 50  0000 C CNN
	1    5650 4000
	-1   0    0    1   
$EndComp
$Comp
L DIY-OLED-0.96 U4
U 1 1 5898EB2B
P 7850 5800
F 0 "U4" V 7650 5700 60  0000 C CNN
F 1 "DIY-OLED-0.96" H 7850 5900 60  0000 C CNN
F 2 "nebrius-lcd:DIY-OLED-0.96" H 8050 6150 60  0000 C CNN
F 3 "" H 7850 5800 60  0000 C CNN
	1    7850 5800
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
L R R5
U 1 1 58D3649D
P 8150 4150
F 0 "R5" V 8230 4150 50  0000 C CNN
F 1 "10k" V 8150 4150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8000 3800 50  0000 C CNN
F 3 "" H 8150 4150 50  0000 C CNN
	1    8150 4150
	-1   0    0    1   
$EndComp
$Comp
L R R6
U 1 1 58D6E0DB
P 8400 4150
F 0 "R6" V 8480 4150 50  0000 C CNN
F 1 "10k" V 8400 4150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8650 4350 50  0000 C CNN
F 3 "" H 8400 4150 50  0000 C CNN
	1    8400 4150
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR01
U 1 1 58D6E218
P 3850 4600
F 0 "#PWR01" H 3850 4450 50  0001 C CNN
F 1 "+3.3V" H 3850 4740 50  0000 C CNN
F 2 "" H 3850 4600 50  0000 C CNN
F 3 "" H 3850 4600 50  0000 C CNN
	1    3850 4600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR02
U 1 1 58D6E2FE
P 5800 6550
F 0 "#PWR02" H 5800 6400 50  0001 C CNN
F 1 "+3.3V" H 5800 6690 50  0000 C CNN
F 2 "" H 5800 6550 50  0000 C CNN
F 3 "" H 5800 6550 50  0000 C CNN
	1    5800 6550
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR03
U 1 1 58D6E3A2
P 7950 5200
F 0 "#PWR03" H 7950 5050 50  0001 C CNN
F 1 "+3.3V" H 7950 5340 50  0000 C CNN
F 2 "" H 7950 5200 50  0000 C CNN
F 3 "" H 7950 5200 50  0000 C CNN
	1    7950 5200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR04
U 1 1 58D6E5CC
P 5650 3750
F 0 "#PWR04" H 5650 3600 50  0001 C CNN
F 1 "+3.3V" H 5650 3890 50  0000 C CNN
F 2 "" H 5650 3750 50  0000 C CNN
F 3 "" H 5650 3750 50  0000 C CNN
	1    5650 3750
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 58D6E79B
P 8150 3900
F 0 "#PWR05" H 8150 3750 50  0001 C CNN
F 1 "+3.3V" H 8150 4040 50  0000 C CNN
F 2 "" H 8150 3900 50  0000 C CNN
F 3 "" H 8150 3900 50  0000 C CNN
	1    8150 3900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR06
U 1 1 58D6E7CF
P 8400 3900
F 0 "#PWR06" H 8400 3750 50  0001 C CNN
F 1 "+3.3V" H 8400 4040 50  0000 C CNN
F 2 "" H 8400 3900 50  0000 C CNN
F 3 "" H 8400 3900 50  0000 C CNN
	1    8400 3900
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR07
U 1 1 58D6E803
P 7650 3900
F 0 "#PWR07" H 7650 3750 50  0001 C CNN
F 1 "+3.3V" H 7650 4040 50  0000 C CNN
F 2 "" H 7650 3900 50  0000 C CNN
F 3 "" H 7650 3900 50  0000 C CNN
	1    7650 3900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 58D6F166
P 1850 5250
F 0 "#PWR08" H 1850 5000 50  0001 C CNN
F 1 "GND" H 1850 5100 50  0000 C CNN
F 2 "" H 1850 5250 50  0000 C CNN
F 3 "" H 1850 5250 50  0000 C CNN
	1    1850 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 58D6F261
P 3850 5450
F 0 "#PWR09" H 3850 5200 50  0001 C CNN
F 1 "GND" H 3850 5300 50  0000 C CNN
F 2 "" H 3850 5450 50  0000 C CNN
F 3 "" H 3850 5450 50  0000 C CNN
	1    3850 5450
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 58D6F42E
P 5000 4350
F 0 "#PWR010" H 5000 4100 50  0001 C CNN
F 1 "GND" H 5000 4200 50  0000 C CNN
F 2 "" H 5000 4350 50  0000 C CNN
F 3 "" H 5000 4350 50  0000 C CNN
	1    5000 4350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR011
U 1 1 58D6F4C6
P 4200 5750
F 0 "#PWR011" H 4200 5500 50  0001 C CNN
F 1 "GND" H 4200 5600 50  0000 C CNN
F 2 "" H 4200 5750 50  0000 C CNN
F 3 "" H 4200 5750 50  0000 C CNN
	1    4200 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 58D6F646
P 8100 5400
F 0 "#PWR012" H 8100 5150 50  0001 C CNN
F 1 "GND" H 8100 5250 50  0000 C CNN
F 2 "" H 8100 5400 50  0000 C CNN
F 3 "" H 8100 5400 50  0000 C CNN
	1    8100 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 5953540A
P 5400 6750
F 0 "#PWR013" H 5400 6500 50  0001 C CNN
F 1 "GND" H 5400 6600 50  0000 C CNN
F 2 "" H 5400 6750 50  0000 C CNN
F 3 "" H 5400 6750 50  0000 C CNN
	1    5400 6750
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
L GND #PWR014
U 1 1 59615099
P 5300 3350
F 0 "#PWR014" H 5300 3100 50  0001 C CNN
F 1 "GND" H 5300 3200 50  0000 C CNN
F 2 "" H 5300 3350 50  0000 C CNN
F 3 "" H 5300 3350 50  0000 C CNN
	1    5300 3350
	1    0    0    -1  
$EndComp
NoConn ~ 4700 3350
NoConn ~ 4700 3950
$Comp
L R_PACK4 RP1
U 1 1 59615F7B
P 5550 6350
F 0 "RP1" H 5550 6800 50  0000 C CNN
F 1 "10k" H 5550 6300 50  0000 C CNN
F 2 "nebrius-discrete:R_Array_Concave_4x0805" V 4700 6900 50  0000 C CNN
F 3 "" H 5550 6350 50  0000 C CNN
	1    5550 6350
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR015
U 1 1 59616333
P 6800 5550
F 0 "#PWR015" H 6800 5400 50  0001 C CNN
F 1 "+3.3V" H 6800 5690 50  0000 C CNN
F 2 "" H 6800 5550 50  0000 C CNN
F 3 "" H 6800 5550 50  0000 C CNN
	1    6800 5550
	1    0    0    -1  
$EndComp
$Comp
L Conn_01x04 J1
U 1 1 5A47DDAC
P 3100 1150
F 0 "J1" H 3100 1350 50  0000 C CNN
F 1 "Conn_01x04" H 3100 850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 3100 1150 50  0001 C CNN
F 3 "" H 3100 1150 50  0001 C CNN
	1    3100 1150
	0    -1   -1   0   
$EndComp
$Comp
L Conn_01x04 J2
U 1 1 5A47DE57
P 3750 1150
F 0 "J2" H 3750 1350 50  0000 C CNN
F 1 "Conn_01x04" H 3750 850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 3750 1150 50  0001 C CNN
F 3 "" H 3750 1150 50  0001 C CNN
	1    3750 1150
	0    -1   -1   0   
$EndComp
$Comp
L +5V #PWR016
U 1 1 5A47E663
P 3000 3800
F 0 "#PWR016" H 3000 3650 50  0001 C CNN
F 1 "+5V" H 3000 3940 50  0000 C CNN
F 2 "" H 3000 3800 50  0001 C CNN
F 3 "" H 3000 3800 50  0001 C CNN
	1    3000 3800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR017
U 1 1 5A47E6D7
P 2650 1350
F 0 "#PWR017" H 2650 1200 50  0001 C CNN
F 1 "+5V" H 2650 1490 50  0000 C CNN
F 2 "" H 2650 1350 50  0001 C CNN
F 3 "" H 2650 1350 50  0001 C CNN
	1    2650 1350
	1    0    0    -1  
$EndComp
NoConn ~ 6900 3750
NoConn ~ 6800 3750
$Comp
L Conn_01x04 J4
U 1 1 5A482341
P 5050 1150
F 0 "J4" H 5050 1350 50  0000 C CNN
F 1 "Conn_01x04" H 5050 850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 5050 1150 50  0001 C CNN
F 3 "" H 5050 1150 50  0001 C CNN
	1    5050 1150
	0    -1   -1   0   
$EndComp
$Comp
L 10118194 U0
U 1 1 5A5AABAF
P 1000 4850
F 0 "U0" H 900 5350 60  0000 C CNN
F 1 "10118194" H 900 5150 60  0000 C CNN
F 2 "nebrius-conn:10118194" H 1000 4850 60  0001 C CNN
F 3 "" H 1000 4850 60  0001 C CNN
	1    1000 4850
	0    -1   -1   0   
$EndComp
NoConn ~ 1200 4850
NoConn ~ 1200 4950
NoConn ~ 1200 5050
NoConn ~ 2250 4050
$Comp
L GND #PWR018
U 1 1 5A6AE250
P 5250 1650
F 0 "#PWR018" H 5250 1400 50  0001 C CNN
F 1 "GND" H 5250 1500 50  0000 C CNN
F 2 "" H 5250 1650 50  0001 C CNN
F 3 "" H 5250 1650 50  0001 C CNN
	1    5250 1650
	1    0    0    -1  
$EndComp
$Comp
L SN74LVC1T45 U6
U 1 1 5A6B65DD
P 6600 2050
F 0 "U6" H 6600 2050 60  0000 C CNN
F 1 "SN74LVC1T45 - MOSI" H 6600 2350 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6_Handsoldering" H 6600 2050 60  0001 C CNN
F 3 "" H 6600 2050 60  0001 C CNN
	1    6600 2050
	1    0    0    -1  
$EndComp
$Comp
L SN74LVC1T45 U7
U 1 1 5A6B663C
P 6600 2700
F 0 "U7" H 6600 2700 60  0000 C CNN
F 1 "SN74LVC1T45 - SCLK" H 6600 3000 60  0000 C CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-6_Handsoldering" H 6600 2700 60  0001 C CNN
F 3 "" H 6600 2700 60  0001 C CNN
	1    6600 2700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5A6B6F9C
P 5850 2900
F 0 "#PWR019" H 5850 2650 50  0001 C CNN
F 1 "GND" H 5850 2750 50  0000 C CNN
F 2 "" H 5850 2900 50  0001 C CNN
F 3 "" H 5850 2900 50  0001 C CNN
	1    5850 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 5A6B7018
P 7350 2900
F 0 "#PWR020" H 7350 2650 50  0001 C CNN
F 1 "GND" H 7350 2750 50  0000 C CNN
F 2 "" H 7350 2900 50  0001 C CNN
F 3 "" H 7350 2900 50  0001 C CNN
	1    7350 2900
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR021
U 1 1 5A6B779E
P 5950 1850
F 0 "#PWR021" H 5950 1700 50  0001 C CNN
F 1 "+5V" H 5950 1990 50  0000 C CNN
F 2 "" H 5950 1850 50  0001 C CNN
F 3 "" H 5950 1850 50  0001 C CNN
	1    5950 1850
	1    0    0    -1  
$EndComp
$Comp
L Fuse F0
U 1 1 5A6BD755
P 1450 5150
F 0 "F0" V 1530 5150 50  0000 C CNN
F 1 "Fuse" V 1375 5150 50  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 1380 5150 50  0001 C CNN
F 3 "" H 1450 5150 50  0001 C CNN
	1    1450 5150
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR022
U 1 1 5A6BE1B6
P 7250 1850
F 0 "#PWR022" H 7250 1700 50  0001 C CNN
F 1 "+3.3V" H 7250 1990 50  0000 C CNN
F 2 "" H 7250 1850 50  0001 C CNN
F 3 "" H 7250 1850 50  0001 C CNN
	1    7250 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 4150 3000 4150
Wire Wire Line
	2000 4350 2000 4800
Wire Wire Line
	2000 4700 2050 4700
Connection ~ 2000 4700
Wire Wire Line
	1600 4900 2050 4900
Wire Wire Line
	1850 4750 1850 5250
Connection ~ 1850 4900
Wire Wire Line
	2850 4700 2900 4700
Wire Wire Line
	3000 4350 3000 3800
Wire Wire Line
	1850 4350 3000 4350
Wire Wire Line
	1850 4350 1850 4450
Connection ~ 2000 4350
Wire Wire Line
	3850 4600 3850 5050
Wire Wire Line
	2850 4900 3850 4900
Connection ~ 3850 4900
Wire Wire Line
	2000 4800 2050 4800
Wire Wire Line
	3500 4700 3850 4700
Connection ~ 3850 4700
Wire Wire Line
	4450 4950 5750 4950
Wire Wire Line
	7550 4950 8400 4950
Wire Wire Line
	4450 4950 4450 5750
Wire Wire Line
	4550 4850 5750 4850
Wire Wire Line
	4550 4850 4550 5750
Wire Wire Line
	5100 4550 5750 4550
Wire Wire Line
	5650 4150 5650 4550
Wire Wire Line
	5000 4250 5000 4350
Wire Wire Line
	5100 3950 5100 4550
Connection ~ 5650 4550
Wire Wire Line
	5000 4250 5750 4250
Wire Wire Line
	7650 4250 7550 4250
Wire Wire Line
	7850 5400 7850 5300
Wire Wire Line
	7550 4750 8150 4750
Wire Wire Line
	8150 4750 8150 4300
Wire Wire Line
	8400 4950 8400 4300
Wire Wire Line
	7950 5200 7950 5400
Wire Wire Line
	7650 3900 7650 4250
Wire Wire Line
	8400 3900 8400 4000
Wire Wire Line
	8150 3900 8150 4000
Wire Wire Line
	5750 4350 5200 4350
Wire Wire Line
	3850 5350 3850 5450
Wire Wire Line
	4350 5750 4200 5750
Wire Wire Line
	7850 5300 8100 5300
Wire Wire Line
	8100 5300 8100 5400
Wire Wire Line
	5650 3750 5650 3850
Wire Wire Line
	5100 3250 5100 3350
Wire Wire Line
	5100 3250 5300 3250
Wire Wire Line
	5300 3250 5300 3350
Wire Wire Line
	1600 4900 1600 4750
Wire Wire Line
	1600 4750 1200 4750
Connection ~ 3000 4150
Wire Wire Line
	2250 4250 1700 4250
Wire Wire Line
	1700 4250 1700 5150
Wire Wire Line
	3000 1450 3000 1350
Wire Wire Line
	2650 1450 4950 1450
Wire Wire Line
	2650 1450 2650 1350
Wire Wire Line
	3650 1450 3650 1350
Connection ~ 3000 1450
Wire Wire Line
	4950 1450 4950 1350
Connection ~ 3650 1450
Wire Wire Line
	5250 1350 5250 1650
Wire Wire Line
	3950 1550 3950 1350
Connection ~ 5250 1550
Wire Wire Line
	3300 1550 3300 1350
Connection ~ 3950 1550
Wire Wire Line
	5950 1850 5950 2600
Wire Wire Line
	5950 1950 6050 1950
Wire Wire Line
	5950 2600 6050 2600
Connection ~ 5950 1950
Wire Wire Line
	7250 1850 7250 2600
Wire Wire Line
	7250 1950 7150 1950
Wire Wire Line
	7250 2600 7150 2600
Connection ~ 7250 1950
Wire Wire Line
	7150 2050 7350 2050
Wire Wire Line
	7350 2050 7350 2900
Wire Wire Line
	7150 2700 7350 2700
Connection ~ 7350 2700
Wire Wire Line
	6050 2050 5850 2050
Wire Wire Line
	5850 2050 5850 2900
Wire Wire Line
	6050 2700 5850 2700
Connection ~ 5850 2700
Wire Wire Line
	3200 2150 6050 2150
Wire Wire Line
	3100 2800 6050 2800
Wire Wire Line
	1200 5150 1300 5150
Wire Wire Line
	1700 5150 1600 5150
Wire Wire Line
	7750 2150 7750 4350
Wire Wire Line
	7750 4350 7550 4350
Wire Wire Line
	7850 2800 7850 4550
Wire Wire Line
	7850 4550 7550 4550
NoConn ~ 6700 3750
NoConn ~ 6600 3750
NoConn ~ 6500 3750
NoConn ~ 6400 3750
Wire Wire Line
	5200 4350 5200 6150
$Comp
L SF303GJ26 U5
U 1 1 59534F8F
P 6250 5800
F 0 "U5" H 6100 5850 60  0000 C CNN
F 1 "SF303GJ26" H 6100 5600 60  0000 C CNN
F 2 "nebrius-switch:SF303GJ26" H 6550 5350 60  0000 C CNN
F 3 "" H 6250 5800 60  0000 C CNN
	1    6250 5800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 6550 5400 6750
Wire Wire Line
	5300 6550 5300 6650
Connection ~ 5400 6650
Wire Wire Line
	5200 6550 5200 6650
Connection ~ 5300 6650
Wire Wire Line
	5750 4650 5300 4650
Wire Wire Line
	5300 4650 5300 6150
Wire Wire Line
	5750 4750 5400 4750
Wire Wire Line
	5400 4750 5400 6150
Wire Wire Line
	6600 5750 6800 5750
Wire Wire Line
	6800 5750 6800 5550
Wire Wire Line
	5600 5750 5200 5750
Connection ~ 5200 5750
Wire Wire Line
	6700 5500 6700 5850
Wire Wire Line
	6700 5500 5300 5500
Connection ~ 5300 5500
Wire Wire Line
	7650 4450 7650 5400
Wire Wire Line
	5200 6650 5400 6650
Wire Wire Line
	5500 6550 5500 6650
Wire Wire Line
	5500 6650 5800 6650
Wire Wire Line
	5800 6650 5800 6550
Wire Wire Line
	5750 4450 5500 4450
Wire Wire Line
	5500 4450 5500 6150
$Comp
L POT RV1
U 1 1 5A8C8F1E
P 9000 4850
F 0 "RV1" V 8825 4850 50  0000 C CNN
F 1 "POT" V 8900 4850 50  0000 C CNN
F 2 "nebrius-discrete:Model-35-Pot" H 9000 4850 50  0001 C CNN
F 3 "" H 9000 4850 50  0001 C CNN
	1    9000 4850
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR023
U 1 1 5A8C9057
P 9000 5100
F 0 "#PWR023" H 9000 4850 50  0001 C CNN
F 1 "GND" H 9000 4950 50  0000 C CNN
F 2 "" H 9000 5100 50  0001 C CNN
F 3 "" H 9000 5100 50  0001 C CNN
	1    9000 5100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR024
U 1 1 5A8C90D3
P 9000 4600
F 0 "#PWR024" H 9000 4450 50  0001 C CNN
F 1 "+3.3V" H 9000 4740 50  0000 C CNN
F 2 "" H 9000 4600 50  0001 C CNN
F 3 "" H 9000 4600 50  0001 C CNN
	1    9000 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9000 4600 9000 4700
Wire Wire Line
	9000 5000 9000 5100
Wire Wire Line
	7550 4850 8850 4850
Wire Wire Line
	3300 1550 5250 1550
$Comp
L Conn_01x04 J3
U 1 1 5A8CA884
P 4400 1150
F 0 "J3" H 4400 1350 50  0000 C CNN
F 1 "Conn_01x04" H 4400 850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04_Pitch2.54mm" H 4400 1150 50  0001 C CNN
F 3 "" H 4400 1150 50  0001 C CNN
	1    4400 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4300 1350 4300 1450
Connection ~ 4300 1450
Wire Wire Line
	4600 1350 4600 1550
Connection ~ 4600 1550
Wire Wire Line
	5150 2150 5150 1350
Wire Wire Line
	5050 2800 5050 1350
Wire Wire Line
	4500 2150 4500 1350
Connection ~ 5150 2150
Wire Wire Line
	3850 2150 3850 1350
Connection ~ 4500 2150
Wire Wire Line
	3200 2150 3200 1350
Connection ~ 3850 2150
Wire Wire Line
	4400 2800 4400 1350
Connection ~ 5050 2800
Wire Wire Line
	3750 2800 3750 1350
Connection ~ 4400 2800
Wire Wire Line
	3100 2800 3100 1350
Connection ~ 3750 2800
Wire Wire Line
	7650 4450 7550 4450
Wire Wire Line
	7550 4650 7750 4650
Wire Wire Line
	7750 4650 7750 5400
NoConn ~ 6600 5650
NoConn ~ 5600 5850
Wire Wire Line
	5600 5650 5400 5650
Connection ~ 5400 5650
Wire Wire Line
	6700 5850 6600 5850
Wire Wire Line
	7750 2150 7150 2150
Wire Wire Line
	7150 2800 7850 2800
$EndSCHEMATC
