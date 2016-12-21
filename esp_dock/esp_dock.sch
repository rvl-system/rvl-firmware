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
LIBS:lm3671
LIBS:esp_dock-cache
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
P 6150 3750
F 0 "U1" H 5900 3975 60  0000 L CNN
F 1 "LM3671" H 5900 3500 60  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23-5" H 6150 3400 60  0000 C CNN
F 3 "" H 6150 3400 60  0000 C CNN
	1    6150 3750
	1    0    0    -1  
$EndComp
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
P 5200 3800
F 0 "P1" H 5200 3950 50  0000 C CNN
F 1 "POWER" V 5300 3800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.00mm" H 5700 3600 50  0000 C CNN
F 3 "" H 5200 3800 50  0000 C CNN
	1    5200 3800
	-1   0    0    -1  
$EndComp
$Comp
L CONN_01X04 P2
U 1 1 57A0DCA2
P 6250 4900
F 0 "P2" H 6250 5150 50  0000 C CNN
F 1 "LIGHT" V 6350 4900 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x04" V 6500 4900 50  0000 C CNN
F 3 "" H 6250 4900 50  0000 C CNN
	1    6250 4900
	0    1    1    0   
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
P 5550 3550
F 0 "C1" H 5575 3650 50  0000 L CNN
F 1 "4.7uF" H 5300 3650 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 5050 3750 50  0000 C CNN
F 3 "" H 5550 3550 50  0000 C CNN
	1    5550 3550
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X03 P4
U 1 1 584F7038
P 9050 3750
F 0 "P4" H 9050 3950 50  0000 C CNN
F 1 "SERIAL" V 9150 3750 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.00mm" H 9350 3500 50  0000 C CNN
F 3 "" H 9050 3750 50  0000 C CNN
	1    9050 3750
	1    0    0    -1  
$EndComp
$Comp
L CONN_02X04 P3
U 1 1 5850C0A1
P 7900 3700
F 0 "P3" H 7900 3950 50  0000 C CNN
F 1 "ESP" H 7900 3450 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x04" H 7500 3150 50  0000 C CNN
F 3 "" H 7900 2500 50  0000 C CNN
	1    7900 3700
	-1   0    0    1   
$EndComp
$Comp
L CONN_01X03 P5
U 1 1 58535794
P 7350 2800
F 0 "P5" H 7350 3000 50  0000 C CNN
F 1 "Pgrm" V 7450 2800 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x03_Pitch2.00mm" V 7550 2300 50  0000 C CNN
F 3 "" H 7350 2800 50  0000 C CNN
	1    7350 2800
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5400 3750 5400 3000
Wire Wire Line
	5400 3000 5950 3000
Wire Wire Line
	6550 3100 6700 3100
Wire Wire Line
	5700 3300 5700 4300
Wire Wire Line
	5700 3650 5750 3650
Wire Wire Line
	5700 4300 6400 4300
Connection ~ 5700 3650
Wire Wire Line
	5400 3850 5750 3850
Wire Wire Line
	5550 3700 5550 4600
Wire Wire Line
	5550 4600 8450 4600
Connection ~ 5550 3850
Wire Wire Line
	6550 3650 6600 3650
Wire Wire Line
	6700 3100 6700 3300
Wire Wire Line
	6700 3300 5550 3300
Wire Wire Line
	5550 3300 5550 3400
Connection ~ 5700 3300
Wire Wire Line
	7550 3650 7550 4000
Wire Wire Line
	6550 3850 7650 3850
Connection ~ 7550 3850
Wire Wire Line
	7550 4600 7550 4300
Wire Wire Line
	5750 3750 5700 3750
Connection ~ 5700 3750
Wire Wire Line
	7200 3650 7650 3650
Wire Wire Line
	8150 3550 8450 3550
Wire Wire Line
	8450 4600 8450 3250
Connection ~ 7550 4600
Wire Wire Line
	8250 3750 8250 4400
Wire Wire Line
	8250 3750 8150 3750
Wire Wire Line
	8250 4400 6200 4400
Wire Wire Line
	6200 4400 6200 4700
Wire Wire Line
	8150 3650 8350 3650
Wire Wire Line
	8350 3650 8350 4500
Wire Wire Line
	8350 4500 6300 4500
Wire Wire Line
	6300 4500 6300 4700
Connection ~ 7550 3650
Wire Wire Line
	8150 3850 8850 3850
Wire Wire Line
	7650 3550 7550 3550
Wire Wire Line
	7550 3550 7550 3400
Wire Wire Line
	7550 3400 8750 3400
Wire Wire Line
	8750 3400 8750 3750
Wire Wire Line
	8750 3750 8850 3750
Wire Wire Line
	8850 3650 8450 3650
Connection ~ 8450 3650
Wire Wire Line
	7650 3750 7350 3750
Wire Wire Line
	7350 3750 7350 3000
Wire Wire Line
	7250 3000 7250 3650
Connection ~ 7250 3650
Wire Wire Line
	8450 3250 7450 3250
Wire Wire Line
	7450 3250 7450 3000
Connection ~ 8450 3550
Wire Wire Line
	6400 4300 6400 4700
Wire Wire Line
	6100 4700 6100 4600
Connection ~ 6100 4600
$EndSCHEMATC
