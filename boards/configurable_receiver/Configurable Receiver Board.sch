EESchema Schematic File Version 4
LIBS:Configurable Receiver Board-cache
EELAYER 29 0
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
L Configurable-Receiver-Board-rescue:LM3671 U1
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
L Configurable-Receiver-Board-rescue:Switch_SPDT_x2 SW1
U 1 1 57A0DADD
P 2550 4150
F 0 "SW1" H 2350 4300 50  0000 C CNN
F 1 "Switch_SPDT_x2" H 2300 4000 50  0000 C CNN
F 2 "nebrius-switch:CL-SA-12C4" H 2850 4450 50  0000 C CNN
F 3 "" H 2550 4150 50  0000 C CNN
	1    2550 4150
	-1   0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:INDUCTOR L1
U 1 1 57A0E1B1
P 3450 4700
F 0 "L1" V 3400 4700 50  0000 C CNN
F 1 "2.2uH" V 3550 4700 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 3700 5200 50  0000 C CNN
F 3 "" H 3450 4700 50  0000 C CNN
	1    3450 4700
	0    -1   -1   0   
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:C C2
U 1 1 57A0E396
P 3850 5200
F 0 "C2" H 3875 5300 50  0000 L CNN
F 1 "10uF" H 4000 5200 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3100 4500 50  0000 C CNN
F 3 "" H 3850 5200 50  0000 C CNN
	1    3850 5200
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:C C1
U 1 1 57A0E3BE
P 1850 4600
F 0 "C1" H 1875 4700 50  0000 L CNN
F 1 "4.7uF" H 1600 4700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 1350 4800 50  0000 C CNN
F 3 "" H 1850 4600 50  0000 C CNN
	1    1850 4600
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:CONN_01X03 P4
U 1 1 584F7038
P 4450 5950
F 0 "P4" H 4450 6150 50  0000 C CNN
F 1 "SERIAL" V 4550 5950 50  0000 C CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x03_P2.00mm_Vertical" V 4700 5150 50  0000 C CNN
F 3 "" H 4450 5950 50  0000 C CNN
	1    4450 5950
	0    1    1    0   
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:GND #PWR01
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
L Configurable-Receiver-Board-rescue:GND #PWR02
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
L Configurable-Receiver-Board-rescue:GND #PWR03
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
L Configurable-Receiver-Board-rescue:GND #PWR04
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
L Configurable-Receiver-Board-rescue:Switch_DPST SW2
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
L Configurable-Receiver-Board-rescue:GND #PWR06
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
L Configurable-Receiver-Board-rescue:+5V #PWR07
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
L Configurable-Receiver-Board-rescue:10118194 U0
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
L Configurable-Receiver-Board-rescue:GND #PWR09
U 1 1 5A6AE250
P 7650 2550
F 0 "#PWR09" H 7650 2300 50  0001 C CNN
F 1 "GND" H 7650 2400 50  0000 C CNN
F 2 "" H 7650 2550 50  0001 C CNN
F 3 "" H 7650 2550 50  0001 C CNN
	1    7650 2550
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:GND #PWR010
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
L Configurable-Receiver-Board-rescue:GND #PWR011
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
L Configurable-Receiver-Board-rescue:+5V #PWR012
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
L Configurable-Receiver-Board-rescue:Fuse F0
U 1 1 5A6BD755
P 1450 5200
F 0 "F0" V 1530 5200 50  0000 C CNN
F 1 "Fuse" V 1375 5200 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 1380 5200 50  0001 C CNN
F 3 "" H 1450 5200 50  0001 C CNN
	1    1450 5200
	0    1    1    0   
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:GND #PWR013
U 1 1 5AB174D5
P 9500 5050
F 0 "#PWR013" H 9500 4800 50  0001 C CNN
F 1 "GND" H 9500 4900 50  0000 C CNN
F 2 "" H 9500 5050 50  0001 C CNN
F 3 "" H 9500 5050 50  0001 C CNN
	1    9500 5050
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:VCC #PWR015
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
L Configurable-Receiver-Board-rescue:VCC #PWR019
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
L Configurable-Receiver-Board-rescue:VCC #PWR020
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
L Configurable-Receiver-Board-rescue:R_PACK4 RP2
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
L Configurable-Receiver-Board-rescue:VCC #PWR021
U 1 1 5AB1C417
P 6950 3050
F 0 "#PWR021" H 6950 2900 50  0001 C CNN
F 1 "VCC" H 6950 3200 50  0000 C CNN
F 2 "" H 6950 3050 50  0001 C CNN
F 3 "" H 6950 3050 50  0001 C CNN
	1    6950 3050
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:R R1
U 1 1 5AB1D5D2
P 5400 4000
F 0 "R1" V 5480 4000 50  0000 C CNN
F 1 "1k" V 5400 4000 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5330 4000 50  0001 C CNN
F 3 "" H 5400 4000 50  0001 C CNN
	1    5400 4000
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:VCC #PWR022
U 1 1 5AB1D659
P 5400 3750
F 0 "#PWR022" H 5400 3600 50  0001 C CNN
F 1 "VCC" H 5400 3900 50  0000 C CNN
F 2 "" H 5400 3750 50  0001 C CNN
F 3 "" H 5400 3750 50  0001 C CNN
	1    5400 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5550 3550 5550 4450
Wire Wire Line
	2000 4350 2000 4700
Wire Wire Line
	2000 4900 2050 4900
Connection ~ 1850 4800
Wire Wire Line
	7750 2800 7750 2450
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
	1200 4800 1850 4800
Wire Wire Line
	4900 3000 4900 3100
Wire Wire Line
	5100 3000 4900 3000
Wire Wire Line
	5100 3000 5100 3350
Wire Wire Line
	4350 5750 4200 5750
Wire Wire Line
	3850 5350 3850 5450
Wire Wire Line
	7650 3900 7650 4250
Wire Wire Line
	8150 3550 8150 4950
Wire Wire Line
	8050 3650 8050 4750
Wire Wire Line
	8050 4750 7550 4750
Wire Wire Line
	7650 4250 7550 4250
Wire Wire Line
	5000 4250 5750 4250
Connection ~ 5400 4550
Wire Wire Line
	5100 3950 5100 4550
Wire Wire Line
	5000 4250 5000 4650
Wire Wire Line
	5400 4550 5400 4150
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
	3850 4600 3850 4700
Connection ~ 2000 4350
Wire Wire Line
	1850 4350 1850 4450
Wire Wire Line
	3000 4350 2000 4350
Wire Wire Line
	3000 3800 3000 4150
Wire Wire Line
	2950 4700 3150 4700
Wire Wire Line
	1850 4750 1850 4800
Connection ~ 2000 4700
Wire Wire Line
	2000 4700 2050 4700
Wire Wire Line
	2850 4150 3000 4150
Connection ~ 5550 4450
Wire Wire Line
	6500 3000 6500 3050
Wire Wire Line
	6600 3000 6600 3050
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
	5400 3750 5400 3850
Wire Wire Line
	5700 2500 5700 2600
Wire Wire Line
	6900 2700 7100 2700
Wire Wire Line
	7000 2600 6900 2600
Wire Wire Line
	4600 4450 5550 4450
Wire Wire Line
	4600 4450 4600 2800
Wire Wire Line
	4600 2800 5800 2800
Wire Wire Line
	5800 2700 5600 2700
Wire Wire Line
	5700 2600 5800 2600
$Comp
L Configurable-Receiver-Board-rescue:SN74LVC1T45 U7
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
	5100 4550 5400 4550
Wire Wire Line
	5600 2700 5600 2900
Wire Wire Line
	7100 2700 7100 2900
$Comp
L Configurable-Receiver-Board-rescue:NeoPixel J4
U 1 1 5AF67484
P 7750 2250
F 0 "J4" V 7300 2250 60  0000 C CNN
F 1 "NeoPixel" V 7450 2250 60  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x03_Pitch2.54mm" H 7750 2250 60  0001 C CNN
F 3 "" H 7750 2250 60  0001 C CNN
	1    7750 2250
	0    1    1    0   
$EndComp
Wire Wire Line
	8150 2550 8150 2400
Wire Wire Line
	7850 2550 7850 2450
Wire Wire Line
	1850 4800 2050 4800
Wire Wire Line
	1850 4800 1850 5250
Wire Wire Line
	3000 4150 3000 4350
Wire Wire Line
	5400 4550 5750 4550
Wire Wire Line
	3850 4700 3850 4900
Wire Wire Line
	3850 4900 3850 5050
Wire Wire Line
	2000 4350 1850 4350
Wire Wire Line
	2000 4700 2000 4900
Wire Wire Line
	5550 4450 5750 4450
Wire Wire Line
	6500 3000 6600 3000
Wire Wire Line
	6600 3000 6700 3000
Wire Wire Line
	7850 2550 8150 2550
Wire Wire Line
	6900 2800 7750 2800
Wire Wire Line
	7650 2450 7650 2550
$Comp
L Configurable-Receiver-Board-rescue:VCC #PWR018
U 1 1 5AB1B5C4
P 9500 4650
F 0 "#PWR018" H 9500 4500 50  0001 C CNN
F 1 "VCC" H 9500 4800 50  0000 C CNN
F 2 "" H 9500 4650 50  0001 C CNN
F 3 "" H 9500 4650 50  0001 C CNN
	1    9500 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 3650 5650 4350
Wire Wire Line
	5650 4350 5750 4350
Wire Wire Line
	5650 3650 6400 3650
Wire Wire Line
	6400 3650 6400 3450
Wire Wire Line
	5550 3550 6500 3550
Wire Wire Line
	6500 3550 6500 3450
Wire Wire Line
	6700 3000 6850 3000
Wire Wire Line
	6850 3000 6850 3150
Wire Wire Line
	6850 3150 6950 3150
Wire Wire Line
	6950 3150 6950 3050
Connection ~ 6700 3000
$Comp
L Configurable-Receiver-Board-rescue:GND #PWR014
U 1 1 5CFC66E8
P 6150 3050
F 0 "#PWR014" H 6150 2800 50  0001 C CNN
F 1 "GND" H 6155 2877 50  0000 C CNN
F 2 "" H 6150 3050 50  0001 C CNN
F 3 "" H 6150 3050 50  0001 C CNN
	1    6150 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3050 6400 3000
Wire Wire Line
	6400 3000 6150 3000
Wire Wire Line
	6150 3000 6150 3050
Wire Wire Line
	7750 5550 7750 5350
Wire Wire Line
	7750 4550 7550 4550
Wire Wire Line
	7550 4350 7950 4350
Wire Wire Line
	7950 4350 7950 5050
$Comp
L Configurable-Receiver-Board-rescue:VCC #PWR016
U 1 1 5D0124E2
P 7450 6150
F 0 "#PWR016" H 7450 6000 50  0001 C CNN
F 1 "VCC" H 7467 6323 50  0000 C CNN
F 2 "" H 7450 6150 50  0001 C CNN
F 3 "" H 7450 6150 50  0001 C CNN
	1    7450 6150
	1    0    0    -1  
$EndComp
Connection ~ 7850 5450
Wire Wire Line
	7850 5450 7850 5550
Connection ~ 7750 5350
Wire Wire Line
	7750 5350 7750 4550
Wire Wire Line
	7450 6150 7450 6250
Wire Wire Line
	7450 6250 7650 6250
Wire Wire Line
	7750 6250 7750 6150
Wire Wire Line
	7750 6250 7850 6250
Wire Wire Line
	7850 6250 7850 6150
Connection ~ 7750 6250
$Comp
L Device:LED D1
U 1 1 5D0CA042
P 5300 5500
F 0 "D1" V 5339 5383 50  0000 R CNN
F 1 "LED" V 5248 5383 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5300 5500 50  0001 C CNN
F 3 "~" H 5300 5500 50  0001 C CNN
	1    5300 5500
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D2
U 1 1 5D0CB0A1
P 5650 5500
F 0 "D2" V 5689 5383 50  0000 R CNN
F 1 "LED" V 5598 5383 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 5650 5500 50  0001 C CNN
F 3 "~" H 5650 5500 50  0001 C CNN
	1    5650 5500
	0    -1   -1   0   
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:R R2
U 1 1 5D0CC208
P 5300 5900
F 0 "R2" H 5370 5946 50  0000 L CNN
F 1 "68" V 5300 5850 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5230 5900 50  0001 C CNN
F 3 "" H 5300 5900 50  0001 C CNN
	1    5300 5900
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:R R3
U 1 1 5D0CC9E1
P 5650 5900
F 0 "R3" H 5720 5946 50  0000 L CNN
F 1 "68" V 5650 5850 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5580 5900 50  0001 C CNN
F 3 "" H 5650 5900 50  0001 C CNN
	1    5650 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 5350 5300 4650
Wire Wire Line
	5300 4650 5750 4650
Wire Wire Line
	5650 5350 5650 4750
Wire Wire Line
	5650 4750 5750 4750
Wire Wire Line
	5300 5650 5300 5750
Wire Wire Line
	5650 5650 5650 5750
$Comp
L Configurable-Receiver-Board-rescue:GND #PWR05
U 1 1 5D0DBA88
P 5300 6150
F 0 "#PWR05" H 5300 5900 50  0001 C CNN
F 1 "GND" H 5305 5977 50  0000 C CNN
F 2 "" H 5300 6150 50  0001 C CNN
F 3 "" H 5300 6150 50  0001 C CNN
	1    5300 6150
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:GND #PWR023
U 1 1 5D0DC46C
P 5650 6150
F 0 "#PWR023" H 5650 5900 50  0001 C CNN
F 1 "GND" H 5655 5977 50  0000 C CNN
F 2 "" H 5650 6150 50  0001 C CNN
F 3 "" H 5650 6150 50  0001 C CNN
	1    5650 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 6050 5300 6150
Wire Wire Line
	5650 6050 5650 6150
Wire Wire Line
	7650 5250 7650 5550
Connection ~ 7650 5250
Wire Wire Line
	7650 6150 7650 6250
Connection ~ 7650 6250
Wire Wire Line
	7650 6250 7750 6250
$Comp
L Configurable-Receiver-Board-rescue:ESP-12F U3
U 1 1 5CFB0A81
P 6650 4650
F 0 "U3" H 6650 3885 50  0000 C CNN
F 1 "ESP-12F" H 6650 3976 50  0000 C CNN
F 2 "ESP8266:ESP-12F_SMD" H 6650 4650 50  0001 C CNN
F 3 "" H 6650 4650 50  0001 C CNN
	1    6650 4650
	-1   0    0    1   
$EndComp
$Comp
L Switch:SW_DIP_x03 SW3
U 1 1 5D2EEC89
P 7650 5850
F 0 "SW3" V 7604 6180 50  0000 L CNN
F 1 "SW_DIP_x03" V 7695 6180 50  0000 L CNN
F 2 "Package_DIP:SMDIP-6_W9.53mm" H 7650 5850 50  0001 C CNN
F 3 "~" H 7650 5850 50  0001 C CNN
	1    7650 5850
	0    1    1    0   
$EndComp
Wire Wire Line
	8050 5550 7950 5550
Wire Wire Line
	8050 5450 7850 5450
Wire Wire Line
	8050 5350 7750 5350
Wire Wire Line
	7650 5250 8050 5250
$Comp
L Configurable-Receiver-Board-rescue:R_PACK4 RP1
U 1 1 5CC0A048
P 8250 5600
F 0 "RP1" H 8250 6050 50  0000 C CNN
F 1 "10k" H 8250 5550 50  0000 C CNN
F 2 "nebrius-discrete:R_Array_Concave_4x2012.kicad_mod" V 8250 4300 50  0000 C CNN
F 3 "" H 8250 5600 50  0000 C CNN
	1    8250 5600
	1    0    0    -1  
$EndComp
$Comp
L Configurable-Receiver-Board-rescue:GND #PWR017
U 1 1 5D0138A9
P 8550 5650
F 0 "#PWR017" H 8550 5400 50  0001 C CNN
F 1 "GND" H 8555 5477 50  0000 C CNN
F 2 "" H 8550 5650 50  0001 C CNN
F 3 "" H 8550 5650 50  0001 C CNN
	1    8550 5650
	1    0    0    -1  
$EndComp
Connection ~ 8550 5550
Wire Wire Line
	8550 5550 8450 5550
Wire Wire Line
	8550 5650 8550 5550
Connection ~ 8550 5450
Wire Wire Line
	8550 5450 8450 5450
Wire Wire Line
	8550 5550 8550 5450
Connection ~ 8550 5350
Wire Wire Line
	8550 5350 8450 5350
Wire Wire Line
	8550 5450 8550 5350
Wire Wire Line
	8550 5250 8550 5350
Wire Wire Line
	8450 5250 8550 5250
$Comp
L Configurable-Receiver-Board-rescue:R R4
U 1 1 5D331195
P 8300 4500
F 0 "R4" H 8350 4500 50  0000 L CNN
F 1 "0" H 8270 4500 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 8230 4500 50  0001 C CNN
F 3 "" H 8300 4500 50  0001 C CNN
	1    8300 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 5050 8300 5050
Wire Wire Line
	8300 5050 8300 4650
Connection ~ 7950 5050
Wire Wire Line
	7950 5050 7950 5550
$Comp
L power:VCC #PWR024
U 1 1 5D336940
P 8300 4250
F 0 "#PWR024" H 8300 4100 50  0001 C CNN
F 1 "VCC" H 8317 4423 50  0000 C CNN
F 2 "" H 8300 4250 50  0001 C CNN
F 3 "" H 8300 4250 50  0001 C CNN
	1    8300 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	8300 4250 8300 4350
$Comp
L Configurable-Receiver-Board-rescue:CONN_01X03 P1
U 1 1 5D3494C0
P 9800 4850
F 0 "P1" H 9800 5050 50  0000 C CNN
F 1 "SENSOR" V 9900 4850 50  0000 C CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_1x03_P2.00mm_Vertical" V 10050 4050 50  0000 C CNN
F 3 "" H 9800 4850 50  0000 C CNN
	1    9800 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 4850 9600 4850
Wire Wire Line
	9500 4750 9600 4750
Wire Wire Line
	9500 4650 9500 4750
Wire Wire Line
	9500 4950 9600 4950
Wire Wire Line
	9500 4950 9500 5050
$Comp
L Configurable-Receiver-Board-rescue:+5V #PWR08
U 1 1 5A47E6D7
P 8150 2400
F 0 "#PWR08" H 8150 2250 50  0001 C CNN
F 1 "+5V" H 8150 2540 50  0000 C CNN
F 2 "" H 8150 2400 50  0001 C CNN
F 3 "" H 8150 2400 50  0001 C CNN
	1    8150 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7650 4450 7550 4450
Wire Wire Line
	7650 4450 7650 5250
Wire Wire Line
	7550 4650 7850 4650
Wire Wire Line
	7850 4650 7850 5450
$EndSCHEMATC