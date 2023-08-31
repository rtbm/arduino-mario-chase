#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#ifndef STASSID
#define STASSID "ssid"
#define STAPSK "pwd"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

#define LED_PIN    D4
#define LED_COUNT 256

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int offset = 0;
int counter = 0;

uint32_t buffer[16][16];

int foxBlackFramesCount = 5;
uint32_t foxBlack[5][16][16] = {
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(8,4,1), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(37,27,18), strip.Color(116,99,89), strip.Color(14,0,0), strip.Color(4,0,0), strip.Color(3,0,0), strip.Color(9,0,0), strip.Color(16,1,0), strip.Color(13,0,0), strip.Color(13,0,0), strip.Color(13,0,0), strip.Color(7,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(2,0,1), strip.Color(6,0,0), strip.Color(14,0,0), strip.Color(145,118,101), strip.Color(194,156,135), strip.Color(50,17,0), strip.Color(8,0,0), strip.Color(7,0,0), strip.Color(48,20,8), strip.Color(173,132,112), strip.Color(115,65,42), strip.Color(77,29,6), strip.Color(143,106,88), strip.Color(27,5,0), strip.Color(6,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(75,44,26), strip.Color(221,174,148), strip.Color(228,163,131), strip.Color(156,99,70), strip.Color(28,1,0), strip.Color(15,0,0), strip.Color(45,7,0), strip.Color(202,145,118), strip.Color(232,159,126), strip.Color(212,142,108), strip.Color(189,137,113), strip.Color(35,3,0), strip.Color(8,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(14,0,0), strip.Color(100,52,30), strip.Color(214,140,105), strip.Color(226,129,86), strip.Color(224,135,95), strip.Color(112,61,40), strip.Color(27,0,0), strip.Color(46,1,0), strip.Color(177,116,87), strip.Color(228,140,100), strip.Color(200,115,76), strip.Color(169,112,83), strip.Color(35,2,0), strip.Color(8,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(11,0,0), strip.Color(68,23,4), strip.Color(202,129,97), strip.Color(232,131,87), strip.Color(233,132,88), strip.Color(177,104,72), strip.Color(64,3,0), strip.Color(140,77,46), strip.Color(230,157,122), strip.Color(230,143,100), strip.Color(218,139,100), strip.Color(184,134,107), strip.Color(82,56,41), strip.Color(7,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(18,0,0), strip.Color(113,68,49), strip.Color(201,126,94), strip.Color(224,131,90), strip.Color(221,124,81), strip.Color(204,105,63), strip.Color(212,115,72), strip.Color(234,145,103), strip.Color(244,166,127), strip.Color(221,165,132), strip.Color(135,104,84), strip.Color(41,28,19), strip.Color(3,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(17,0,0), strip.Color(58,7,0), strip.Color(150,74,40), strip.Color(227,126,82), strip.Color(238,126,78), strip.Color(232,125,79), strip.Color(224,132,91), strip.Color(226,161,129), strip.Color(89,48,26), strip.Color(14,0,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(18,0,0), strip.Color(65,14,0), strip.Color(209,123,88), strip.Color(251,150,106), strip.Color(243,147,105), strip.Color(214,135,102), strip.Color(155,107,85), strip.Color(31,4,0), strip.Color(8,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(100,67,52), strip.Color(192,126,100), strip.Color(160,81,48), strip.Color(137,62,30), strip.Color(168,108,82), strip.Color(85,51,39), strip.Color(13,0,0), strip.Color(4,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,2), strip.Color(4,0,0), strip.Color(11,0,0), strip.Color(119,92,81), strip.Color(117,80,64), strip.Color(66,21,2), strip.Color(68,23,2), strip.Color(156,121,102), strip.Color(69,49,42), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0)},
	{strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,2), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(80,63,55), strip.Color(67,48,41), strip.Color(24,4,0), strip.Color(47,25,12), strip.Color(98,80,70), strip.Color(26,15,11), strip.Color(5,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(4,0,0), strip.Color(4,0,0), strip.Color(6,0,0), strip.Color(6,0,0), strip.Color(4,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(1,1,1), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(7,0,0), strip.Color(57,46,40), strip.Color(24,17,11), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(4,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(36,17,3), strip.Color(176,153,135), strip.Color(99,80,66), strip.Color(10,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(10,0,0), strip.Color(11,0,0), strip.Color(8,0,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(10,0,0), strip.Color(108,72,50), strip.Color(237,190,162), strip.Color(204,158,134), strip.Color(47,14,0), strip.Color(10,0,0), strip.Color(3,0,0), strip.Color(6,0,0), strip.Color(28,9,2), strip.Color(120,84,68), strip.Color(70,33,15), strip.Color(85,59,46), strip.Color(18,3,0), strip.Color(4,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(14,0,0), strip.Color(155,98,69), strip.Color(232,152,115), strip.Color(223,138,99), strip.Color(158,89,58), strip.Color(28,1,0), strip.Color(8,0,0), strip.Color(15,0,0), strip.Color(98,57,37), strip.Color(234,163,131), strip.Color(198,125,90), strip.Color(193,145,122), strip.Color(35,7,0), strip.Color(7,0,0), strip.Color(0,0,2)},
	{strip.Color(0,0,0), strip.Color(14,0,0), strip.Color(144,86,62), strip.Color(221,137,100), strip.Color(231,130,86), strip.Color(220,130,93), strip.Color(85,34,13), strip.Color(27,0,0), strip.Color(41,0,0), strip.Color(173,114,84), strip.Color(235,146,104), strip.Color(218,129,87), strip.Color(185,126,96), strip.Color(36,3,0), strip.Color(8,0,0), strip.Color(0,0,2)},
	{strip.Color(0,0,0), strip.Color(8,0,0), strip.Color(65,29,15), strip.Color(182,121,93), strip.Color(217,127,90), strip.Color(231,134,92), strip.Color(190,104,69), strip.Color(128,52,18), strip.Color(138,68,34), strip.Color(217,142,103), strip.Color(215,124,80), strip.Color(198,115,73), strip.Color(167,110,81), strip.Color(63,32,14), strip.Color(8,0,0), strip.Color(0,0,0)},
	{strip.Color(1,1,1), strip.Color(4,0,0), strip.Color(14,0,0), strip.Color(39,0,0), strip.Color(113,42,10), strip.Color(217,125,84), strip.Color(228,127,85), strip.Color(230,129,85), strip.Color(233,142,98), strip.Color(243,160,118), strip.Color(243,165,126), strip.Color(233,168,136), strip.Color(189,147,123), strip.Color(89,66,52), strip.Color(6,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(15,0,0), strip.Color(44,0,0), strip.Color(186,111,79), strip.Color(230,131,90), strip.Color(237,129,83), strip.Color(226,125,79), strip.Color(231,149,109), strip.Color(183,132,105), strip.Color(77,46,28), strip.Color(33,15,5), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(8,0,0), strip.Color(58,25,10), strip.Color(187,129,105), strip.Color(205,122,88), strip.Color(216,122,84), strip.Color(224,135,95), strip.Color(210,141,108), strip.Color(70,37,20), strip.Color(8,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(7,0,0), strip.Color(66,37,23), strip.Color(180,137,118), strip.Color(106,58,38), strip.Color(111,56,35), strip.Color(179,123,98), strip.Color(131,86,63), strip.Color(38,18,7), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(6,0,0), strip.Color(18,0,0), strip.Color(100,73,62), strip.Color(70,45,38), strip.Color(27,0,0), strip.Color(76,47,33), strip.Color(96,73,59), strip.Color(23,12,8), strip.Color(3,2,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(7,0,0), strip.Color(6,0,0), strip.Color(7,0,0), strip.Color(7,0,0), strip.Color(6,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(2,0,1), strip.Color(0,0,0), strip.Color(2,1,0), strip.Color(3,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(7,0,0), strip.Color(7,0,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(21,12,7), strip.Color(74,50,38), strip.Color(67,35,20), strip.Color(31,2,0), strip.Color(22,0,0), strip.Color(10,0,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(80,61,47), strip.Color(106,87,73), strip.Color(10,2,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(35,18,10), strip.Color(172,126,103), strip.Color(209,144,112), strip.Color(196,117,84), strip.Color(179,110,81), strip.Color(75,36,19), strip.Color(14,0,0), strip.Color(4,0,0), strip.Color(0,0,0)},
	{strip.Color(4,0,0), strip.Color(42,25,15), strip.Color(216,174,150), strip.Color(183,139,114), strip.Color(25,1,0), strip.Color(8,0,0), strip.Color(7,0,0), strip.Color(17,0,0), strip.Color(88,32,5), strip.Color(225,146,107), strip.Color(217,122,78), strip.Color(204,119,80), strip.Color(152,102,79), strip.Color(23,0,0), strip.Color(6,0,0), strip.Color(0,0,0)},
	{strip.Color(8,0,0), strip.Color(82,50,35), strip.Color(238,165,130), strip.Color(202,123,84), strip.Color(71,19,0), strip.Color(25,0,0), strip.Color(15,0,0), strip.Color(20,0,0), strip.Color(113,62,35), strip.Color(225,155,119), strip.Color(213,130,88), strip.Color(208,133,94), strip.Color(185,139,113), strip.Color(92,69,55), strip.Color(6,0,0), strip.Color(1,1,1)},
	{strip.Color(10,0,0), strip.Color(90,53,35), strip.Color(225,137,97), strip.Color(230,129,83), strip.Color(205,125,90), strip.Color(157,96,68), strip.Color(90,39,18), strip.Color(80,28,6), strip.Color(157,88,55), strip.Color(228,154,115), strip.Color(242,170,132), strip.Color(207,148,116), strip.Color(135,99,77), strip.Color(47,29,17), strip.Color(4,0,0), strip.Color(0,0,0)},
	{strip.Color(8,0,0), strip.Color(84,48,32), strip.Color(222,138,101), strip.Color(237,129,82), strip.Color(234,131,88), strip.Color(233,132,90), strip.Color(225,126,85), strip.Color(228,127,85), strip.Color(230,125,80), strip.Color(220,128,87), strip.Color(228,167,136), strip.Color(80,44,22), strip.Color(14,0,0), strip.Color(4,0,0), strip.Color(2,1,0), strip.Color(0,0,0)},
	{strip.Color(6,0,0), strip.Color(30,3,0), strip.Color(149,85,57), strip.Color(189,104,67), strip.Color(193,96,54), strip.Color(218,112,70), strip.Color(232,125,81), strip.Color(243,135,89), strip.Color(230,121,78), strip.Color(239,154,117), strip.Color(158,113,90), strip.Color(19,1,0), strip.Color(6,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(7,0,0), strip.Color(22,0,0), strip.Color(36,0,0), strip.Color(59,0,0), strip.Color(174,100,71), strip.Color(206,131,102), strip.Color(195,118,88), strip.Color(210,131,100), strip.Color(151,91,65), strip.Color(37,10,0), strip.Color(6,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(11,0,0), strip.Color(68,29,12), strip.Color(175,124,103), strip.Color(97,48,31), strip.Color(98,51,33), strip.Color(161,113,93), strip.Color(39,3,0), strip.Color(10,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(8,0,0), strip.Color(86,53,38), strip.Color(149,110,93), strip.Color(40,13,2), strip.Color(77,57,48), strip.Color(68,49,42), strip.Color(10,0,0), strip.Color(4,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,2), strip.Color(6,0,0), strip.Color(48,26,15), strip.Color(96,72,60), strip.Color(20,9,3), strip.Color(4,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(6,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(10,0,0), strip.Color(13,0,0), strip.Color(11,0,0), strip.Color(8,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(26,11,4), strip.Color(87,49,30), strip.Color(82,34,11), strip.Color(54,9,0), strip.Color(40,7,0), strip.Color(11,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,2)},
	{strip.Color(7,0,0), strip.Color(10,0,0), strip.Color(15,0,0), strip.Color(15,0,0), strip.Color(13,0,0), strip.Color(11,0,0), strip.Color(11,0,0), strip.Color(45,17,5), strip.Color(188,134,108), strip.Color(216,150,116), strip.Color(194,125,94), strip.Color(177,119,95), strip.Color(73,41,28), strip.Color(10,0,0), strip.Color(3,0,0), strip.Color(0,0,2)},
	{strip.Color(24,0,0), strip.Color(108,71,52), strip.Color(134,74,46), strip.Color(125,61,33), strip.Color(80,30,7), strip.Color(32,0,0), strip.Color(36,0,0), strip.Color(31,0,0), strip.Color(80,25,0), strip.Color(213,143,107), strip.Color(205,117,79), strip.Color(192,109,75), strip.Color(164,112,88), strip.Color(30,2,0), strip.Color(8,0,0), strip.Color(0,0,0)},
	{strip.Color(109,78,58), strip.Color(224,177,151), strip.Color(219,139,102), strip.Color(226,134,95), strip.Color(209,129,94), strip.Color(183,110,78), strip.Color(184,117,88), strip.Color(152,86,54), strip.Color(187,114,79), strip.Color(233,155,116), strip.Color(217,132,93), strip.Color(214,136,100), strip.Color(187,135,111), strip.Color(84,55,41), strip.Color(7,0,0), strip.Color(0,0,0)},
	{strip.Color(67,40,21), strip.Color(187,141,117), strip.Color(220,136,100), strip.Color(235,129,87), strip.Color(236,128,82), strip.Color(233,125,79), strip.Color(231,126,81), strip.Color(230,127,84), strip.Color(234,133,87), strip.Color(240,151,107), strip.Color(231,165,131), strip.Color(202,156,130), strip.Color(134,105,89), strip.Color(42,28,19), strip.Color(4,0,0), strip.Color(0,0,0)},
	{strip.Color(15,0,0), strip.Color(63,28,9), strip.Color(136,69,40), strip.Color(185,93,56), strip.Color(220,115,70), strip.Color(231,120,74), strip.Color(231,124,80), strip.Color(240,137,94), strip.Color(233,132,88), strip.Color(231,149,111), strip.Color(149,102,74), strip.Color(20,0,0), strip.Color(11,0,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(6,0,0), strip.Color(11,0,0), strip.Color(25,0,0), strip.Color(93,41,17), strip.Color(188,115,83), strip.Color(213,134,101), strip.Color(203,129,102), strip.Color(154,84,58), strip.Color(164,97,68), strip.Color(201,145,118), strip.Color(115,82,63), strip.Color(14,0,0), strip.Color(7,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(58,45,37), strip.Color(143,120,106), strip.Color(84,41,24), strip.Color(77,30,10), strip.Color(65,23,9), strip.Color(40,3,0), strip.Color(39,0,0), strip.Color(117,84,67), strip.Color(108,88,77), strip.Color(8,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(9,5,2), strip.Color(14,7,1), strip.Color(15,0,0), strip.Color(13,0,0), strip.Color(10,0,0), strip.Color(10,0,0), strip.Color(13,0,0), strip.Color(25,14,8), strip.Color(14,6,3), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(8,0,0), strip.Color(10,0,0), strip.Color(8,0,0), strip.Color(6,0,0), strip.Color(3,0,0), strip.Color(3,0,0), strip.Color(7,0,0), strip.Color(8,0,0), strip.Color(8,0,0), strip.Color(7,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(50,19,1), strip.Color(82,44,25), strip.Color(58,25,8), strip.Color(20,0,0), strip.Color(8,0,0), strip.Color(15,2,0), strip.Color(40,11,0), strip.Color(39,3,0), strip.Color(24,0,0), strip.Color(15,0,0), strip.Color(6,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0)},
	{strip.Color(4,0,0), strip.Color(75,56,42), strip.Color(211,161,136), strip.Color(216,151,121), strip.Color(194,133,104), strip.Color(95,47,24), strip.Color(18,0,0), strip.Color(46,24,13), strip.Color(169,127,105), strip.Color(173,119,93), strip.Color(143,87,62), strip.Color(126,79,61), strip.Color(37,13,3), strip.Color(7,0,0), strip.Color(1,0,0), strip.Color(0,1,0)},
	{strip.Color(31,21,11), strip.Color(177,148,130), strip.Color(239,173,139), strip.Color(220,131,89), strip.Color(230,133,90), strip.Color(201,119,82), strip.Color(68,23,4), strip.Color(25,0,0), strip.Color(110,62,39), strip.Color(222,156,124), strip.Color(226,141,104), strip.Color(218,138,103), strip.Color(151,100,79), strip.Color(24,0,0), strip.Color(6,0,0), strip.Color(0,0,0)},
	{strip.Color(23,14,5), strip.Color(52,25,8), strip.Color(149,86,55), strip.Color(225,136,96), strip.Color(235,129,81), strip.Color(235,134,90), strip.Color(169,98,68), strip.Color(73,16,0), strip.Color(106,41,9), strip.Color(216,141,102), strip.Color(206,111,67), strip.Color(191,106,67), strip.Color(174,120,94), strip.Color(65,37,23), strip.Color(7,0,0), strip.Color(0,0,0)},
	{strip.Color(3,0,0), strip.Color(11,0,0), strip.Color(47,10,0), strip.Color(180,119,91), strip.Color(223,134,94), strip.Color(231,128,83), strip.Color(231,128,85), strip.Color(219,120,78), strip.Color(201,108,65), strip.Color(242,155,112), strip.Color(244,165,124), strip.Color(228,165,130), strip.Color(178,142,120), strip.Color(79,61,51), strip.Color(4,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(14,0,0), strip.Color(52,15,0), strip.Color(118,53,23), strip.Color(190,102,64), strip.Color(233,128,83), strip.Color(237,126,80), strip.Color(229,126,81), strip.Color(218,130,90), strip.Color(226,163,128), strip.Color(111,69,44), strip.Color(27,5,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(13,0,0), strip.Color(25,0,0), strip.Color(127,73,49), strip.Color(208,129,98), strip.Color(226,134,97), strip.Color(240,147,106), strip.Color(233,157,121), strip.Color(140,98,76), strip.Color(16,0,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(105,77,65), strip.Color(153,102,81), strip.Color(107,41,15), strip.Color(161,86,54), strip.Color(180,119,91), strip.Color(61,32,18), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(39,28,22), strip.Color(38,18,11), strip.Color(52,20,7), strip.Color(79,34,15), strip.Color(154,113,95), strip.Color(74,51,43), strip.Color(12,4,1), strip.Color(1,0,0), strip.Color(0,1,0), strip.Color(1,1,1), strip.Color(1,1,1)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(7,0,0), strip.Color(32,10,0), strip.Color(107,85,74), strip.Color(41,28,22), strip.Color(6,1,0), strip.Color(1,0,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(6,0,0), strip.Color(6,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
}
};

int marioFramesCount = 8;
uint32_t mario[8][16][16] = {
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(13,0,0), strip.Color(15,0,0), strip.Color(14,0,0), strip.Color(10,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(17,0,0), strip.Color(38,0,0), strip.Color(46,0,0), strip.Color(42,0,0), strip.Color(29,0,0), strip.Color(10,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(25,0,0), strip.Color(60,0,0), strip.Color(126,18,18), strip.Color(144,24,23), strip.Color(143,41,36), strip.Color(90,17,11), strip.Color(25,0,0), strip.Color(8,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(24,0,0), strip.Color(92,4,3), strip.Color(146,11,8), strip.Color(199,21,17), strip.Color(227,46,37), strip.Color(255,136,119), strip.Color(171,64,48), strip.Color(49,0,0), strip.Color(27,0,0), strip.Color(8,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(32,0,0), strip.Color(121,21,6), strip.Color(161,12,0), strip.Color(202,15,0), strip.Color(220,31,11), strip.Color(232,78,52), strip.Color(170,51,29), strip.Color(116,36,27), strip.Color(54,8,8), strip.Color(11,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(34,6,0), strip.Color(161,88,45), strip.Color(186,78,29), strip.Color(214,81,36), strip.Color(236,97,58), strip.Color(197,69,32), strip.Color(200,96,69), strip.Color(103,35,22), strip.Color(31,0,0), strip.Color(8,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,1,0), strip.Color(78,58,31), strip.Color(179,130,74), strip.Color(182,107,42), strip.Color(204,105,50), strip.Color(255,148,101), strip.Color(255,157,118), strip.Color(255,172,143), strip.Color(157,105,91), strip.Color(18,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(32,20,0), strip.Color(104,73,19), strip.Color(164,109,45), strip.Color(201,115,64), strip.Color(127,31,0), strip.Color(139,55,19), strip.Color(133,71,46), strip.Color(61,29,18), strip.Color(8,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(10,0,0), strip.Color(42,8,0), strip.Color(139,78,21), strip.Color(170,73,30), strip.Color(179,66,32), strip.Color(177,66,39), strip.Color(119,30,12), strip.Color(34,0,0), strip.Color(13,0,0), strip.Color(4,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(15,0,0), strip.Color(120,66,22), strip.Color(245,153,104), strip.Color(240,112,75), strip.Color(208,48,22), strip.Color(217,30,13), strip.Color(204,40,31), strip.Color(137,44,39), strip.Color(30,0,0), strip.Color(10,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(27,7,0), strip.Color(183,127,94), strip.Color(255,159,120), strip.Color(255,149,122), strip.Color(232,47,27), strip.Color(227,14,0), strip.Color(208,22,9), strip.Color(131,28,19), strip.Color(32,0,0), strip.Color(11,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(10,0,0), strip.Color(72,33,16), strip.Color(151,66,45), strip.Color(207,57,42), strip.Color(207,24,6), strip.Color(212,22,0), strip.Color(179,28,0), strip.Color(85,13,0), strip.Color(20,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(17,0,0), strip.Color(50,0,0), strip.Color(137,24,10), strip.Color(177,36,16), strip.Color(193,58,26), strip.Color(177,74,41), strip.Color(110,65,42), strip.Color(10,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(6,0,0), strip.Color(15,0,0), strip.Color(76,33,14), strip.Color(112,58,30), strip.Color(127,77,42), strip.Color(82,43,12), strip.Color(31,14,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(1,1,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(9,1,0), strip.Color(13,2,0), strip.Color(12,4,0), strip.Color(7,2,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,0)}
},
{
	{strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(11,0,0), strip.Color(22,0,0), strip.Color(28,0,0), strip.Color(25,0,0), strip.Color(18,0,0), strip.Color(8,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(15,0,0), strip.Color(35,0,0), strip.Color(89,0,0), strip.Color(128,11,4), strip.Color(121,17,4), strip.Color(89,11,0), strip.Color(25,0,0), strip.Color(8,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(14,0,0), strip.Color(42,0,0), strip.Color(98,11,4), strip.Color(165,24,15), strip.Color(216,56,42), strip.Color(255,122,103), strip.Color(238,118,102), strip.Color(87,8,3), strip.Color(31,0,0), strip.Color(11,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(43,2,0), strip.Color(112,19,11), strip.Color(145,13,0), strip.Color(173,17,0), strip.Color(200,34,10), strip.Color(224,64,40), strip.Color(207,56,37), strip.Color(161,18,10), strip.Color(106,2,0), strip.Color(26,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(47,3,0), strip.Color(156,58,33), strip.Color(187,57,23), strip.Color(211,69,33), strip.Color(255,111,77), strip.Color(231,88,58), strip.Color(226,86,63), strip.Color(203,61,49), strip.Color(133,26,20), strip.Color(30,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(33,6,0), strip.Color(177,110,57), strip.Color(223,133,70), strip.Color(186,91,35), strip.Color(227,131,83), strip.Color(251,158,117), strip.Color(255,176,145), strip.Color(200,125,106), strip.Color(42,0,0), strip.Color(13,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,1,0), strip.Color(77,58,26), strip.Color(147,97,34), strip.Color(172,104,29), strip.Color(209,131,69), strip.Color(146,60,9), strip.Color(128,40,0), strip.Color(162,87,56), strip.Color(125,77,57), strip.Color(21,0,0), strip.Color(7,0,0), strip.Color(0,0,2), strip.Color(0,1,0), strip.Color(0,1,0)},
	{strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(1,1,0), strip.Color(34,18,0), strip.Color(111,65,13), strip.Color(164,95,30), strip.Color(185,94,39), strip.Color(196,88,42), strip.Color(155,30,0), strip.Color(121,12,0), strip.Color(71,11,0), strip.Color(20,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(15,0,0), strip.Color(120,66,20), strip.Color(243,156,102), strip.Color(255,162,115), strip.Color(240,98,62), strip.Color(204,40,15), strip.Color(190,47,31), strip.Color(117,38,25), strip.Color(26,0,0), strip.Color(8,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(20,0,0), strip.Color(161,86,47), strip.Color(255,139,93), strip.Color(255,123,90), strip.Color(244,58,35), strip.Color(223,15,2), strip.Color(204,24,10), strip.Color(136,31,9), strip.Color(38,0,0), strip.Color(13,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(2,0,1), strip.Color(30,6,0), strip.Color(187,117,81), strip.Color(183,64,22), strip.Color(203,35,6), strip.Color(216,20,0), strip.Color(223,17,0), strip.Color(200,26,1), strip.Color(177,79,42), strip.Color(92,47,14), strip.Color(11,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(12,0,0), strip.Color(82,42,6), strip.Color(151,64,19), strip.Color(184,34,0), strip.Color(204,28,0), strip.Color(194,31,0), strip.Color(179,54,6), strip.Color(151,90,36), strip.Color(78,56,15), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,2), strip.Color(6,0,0), strip.Color(28,9,0), strip.Color(124,70,32), strip.Color(183,75,39), strip.Color(181,54,13), strip.Color(176,68,19), strip.Color(163,86,34), strip.Color(101,69,20), strip.Color(8,2,0), strip.Color(3,1,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(7,0,0), strip.Color(33,12,0), strip.Color(92,51,21), strip.Color(110,62,24), strip.Color(122,80,38), strip.Color(76,46,10), strip.Color(29,17,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(9,2,0), strip.Color(12,2,0), strip.Color(12,4,0), strip.Color(7,2,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(7,0,0), strip.Color(20,0,0), strip.Color(27,0,0), strip.Color(29,0,0), strip.Color(22,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(14,0,0), strip.Color(29,0,0), strip.Color(65,0,0), strip.Color(87,6,5), strip.Color(84,11,5), strip.Color(58,3,0), strip.Color(20,0,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(14,0,0), strip.Color(49,0,0), strip.Color(101,0,0), strip.Color(174,16,17), strip.Color(195,26,23), strip.Color(220,83,67), strip.Color(170,68,53), strip.Color(43,0,0), strip.Color(21,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(2,0,1)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(24,0,0), strip.Color(102,19,11), strip.Color(146,14,2), strip.Color(193,9,0), strip.Color(215,22,7), strip.Color(255,102,79), strip.Color(233,101,80), strip.Color(119,18,10), strip.Color(66,1,0), strip.Color(15,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(1,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(7,0,0), strip.Color(43,5,0), strip.Color(126,40,13), strip.Color(160,37,4), strip.Color(185,38,4), strip.Color(209,55,21), strip.Color(195,47,17), strip.Color(189,45,21), strip.Color(191,43,31), strip.Color(139,26,20), strip.Color(30,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(73,39,12), strip.Color(184,110,63), strip.Color(220,119,65), strip.Color(194,80,28), strip.Color(255,146,103), strip.Color(255,138,104), strip.Color(255,139,116), strip.Color(255,127,110), strip.Color(143,35,25), strip.Color(31,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(84,61,20), strip.Color(149,100,34), strip.Color(198,128,56), strip.Color(203,114,54), strip.Color(179,88,41), strip.Color(130,50,15), strip.Color(195,121,96), strip.Color(195,124,104), strip.Color(56,4,0), strip.Color(14,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,1,0), strip.Color(6,1,0), strip.Color(79,61,21), strip.Color(128,88,19), strip.Color(145,86,10), strip.Color(200,122,60), strip.Color(164,76,28), strip.Color(120,30,0), strip.Color(107,29,6), strip.Color(105,54,35), strip.Color(23,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(34,17,0), strip.Color(112,65,9), strip.Color(177,108,41), strip.Color(217,131,74), strip.Color(220,108,62), strip.Color(219,70,38), strip.Color(182,43,22), strip.Color(90,10,0), strip.Color(27,0,0), strip.Color(8,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,0)},
	{strip.Color(0,0,0), strip.Color(1,1,0), strip.Color(1,0,0), strip.Color(15,0,0), strip.Color(123,67,20), strip.Color(230,144,87), strip.Color(255,159,111), strip.Color(255,130,91), strip.Color(216,26,0), strip.Color(204,32,10), strip.Color(122,26,4), strip.Color(34,0,0), strip.Color(11,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(18,0,0), strip.Color(155,88,46), strip.Color(211,99,51), strip.Color(244,86,51), strip.Color(237,47,23), strip.Color(225,15,0), strip.Color(205,25,0), strip.Color(159,54,6), strip.Color(106,55,8), strip.Color(21,4,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(1,0,0), strip.Color(31,9,0), strip.Color(160,101,57), strip.Color(162,56,4), strip.Color(195,31,0), strip.Color(216,20,0), strip.Color(214,18,0), strip.Color(193,30,0), strip.Color(181,88,27), strip.Color(148,106,48), strip.Color(23,8,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(10,0,0), strip.Color(92,63,7), strip.Color(178,109,40), strip.Color(205,74,18), strip.Color(192,34,0), strip.Color(186,34,0), strip.Color(175,55,5), strip.Color(135,76,18), strip.Color(77,56,9), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(4,1,0), strip.Color(37,26,0), strip.Color(134,95,36), strip.Color(181,94,40), strip.Color(168,61,7), strip.Color(171,71,19), strip.Color(160,84,34), strip.Color(103,70,25), strip.Color(8,2,0), strip.Color(3,1,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(29,13,0), strip.Color(86,52,15), strip.Color(106,64,22), strip.Color(120,81,38), strip.Color(75,45,9), strip.Color(29,17,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,1,0), strip.Color(9,2,0), strip.Color(11,3,0), strip.Color(12,4,0), strip.Color(7,2,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(24,0,0), strip.Color(31,0,0), strip.Color(29,0,0), strip.Color(22,0,0), strip.Color(8,0,0), strip.Color(0,0,0), strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(17,0,0), strip.Color(35,0,0), strip.Color(74,0,0), strip.Color(89,5,3), strip.Color(82,12,4), strip.Color(55,5,0), strip.Color(18,0,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(17,0,0), strip.Color(58,0,0), strip.Color(117,3,3), strip.Color(177,15,13), strip.Color(197,30,22), strip.Color(228,99,80), strip.Color(162,69,51), strip.Color(41,0,0), strip.Color(21,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(27,0,0), strip.Color(107,17,9), strip.Color(153,11,1), strip.Color(200,9,0), strip.Color(221,24,8), strip.Color(255,125,99), strip.Color(236,107,85), strip.Color(119,16,7), strip.Color(66,1,0), strip.Color(15,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(44,4,0), strip.Color(128,36,11), strip.Color(164,30,1), strip.Color(180,14,0), strip.Color(204,25,2), strip.Color(216,50,24), strip.Color(199,38,18), strip.Color(188,30,19), strip.Color(142,25,18), strip.Color(32,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(74,38,12), strip.Color(187,108,65), strip.Color(220,110,59), strip.Color(201,68,25), strip.Color(255,116,80), strip.Color(241,100,72), strip.Color(255,120,98), strip.Color(255,114,99), strip.Color(147,33,23), strip.Color(31,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(85,60,20), strip.Color(159,104,39), strip.Color(202,125,53), strip.Color(189,99,39), strip.Color(183,91,44), strip.Color(167,83,49), strip.Color(220,143,117), strip.Color(206,131,112), strip.Color(57,0,0), strip.Color(14,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,1,0), strip.Color(79,61,21), strip.Color(131,91,21), strip.Color(163,104,26), strip.Color(211,136,71), strip.Color(162,79,29), strip.Color(79,0,0), strip.Color(110,36,11), strip.Color(106,55,38), strip.Color(24,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0)},
	{strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(1,1,0), strip.Color(33,19,0), strip.Color(102,66,6), strip.Color(147,87,17), strip.Color(225,136,80), strip.Color(224,109,64), strip.Color(214,74,41), strip.Color(173,44,23), strip.Color(88,11,1), strip.Color(27,0,0), strip.Color(8,0,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(13,0,0), strip.Color(107,61,12), strip.Color(211,135,77), strip.Color(255,162,113), strip.Color(255,131,94), strip.Color(217,33,7), strip.Color(204,38,16), strip.Color(125,30,10), strip.Color(32,0,0), strip.Color(10,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(18,0,0), strip.Color(156,89,47), strip.Color(209,104,56), strip.Color(255,126,86), strip.Color(255,110,81), strip.Color(232,20,6), strip.Color(213,23,1), strip.Color(161,53,6), strip.Color(106,55,8), strip.Color(21,4,0), strip.Color(1,2,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(33,10,0), strip.Color(166,102,58), strip.Color(149,45,0), strip.Color(179,38,0), strip.Color(210,35,4), strip.Color(220,18,0), strip.Color(207,29,0), strip.Color(193,94,35), strip.Color(150,105,48), strip.Color(24,7,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(20,8,0), strip.Color(128,94,31), strip.Color(169,97,23), strip.Color(186,59,4), strip.Color(191,34,0), strip.Color(183,26,0), strip.Color(173,48,2), strip.Color(147,87,27), strip.Color(80,58,8), strip.Color(8,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,1,0), strip.Color(1,0,0), strip.Color(7,2,0), strip.Color(66,53,0), strip.Color(139,95,30), strip.Color(187,93,41), strip.Color(171,59,11), strip.Color(169,67,19), strip.Color(155,79,30), strip.Color(104,72,25), strip.Color(11,3,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(6,1,0), strip.Color(30,13,0), strip.Color(88,52,16), strip.Color(107,63,24), strip.Color(119,80,39), strip.Color(76,46,10), strip.Color(28,16,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(9,2,0), strip.Color(12,2,0), strip.Color(11,3,0), strip.Color(7,2,0), strip.Color(4,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(24,0,0), strip.Color(31,0,0), strip.Color(29,0,0), strip.Color(22,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(6,0,0), strip.Color(17,0,0), strip.Color(35,0,0), strip.Color(73,0,0), strip.Color(92,4,3), strip.Color(88,9,5), strip.Color(61,2,0), strip.Color(21,0,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(17,0,0), strip.Color(59,0,0), strip.Color(119,3,4), strip.Color(177,15,13), strip.Color(202,27,22), strip.Color(240,89,78), strip.Color(175,61,50), strip.Color(46,0,0), strip.Color(22,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(31,0,0), strip.Color(115,12,13), strip.Color(165,7,6), strip.Color(213,3,2), strip.Color(232,13,7), strip.Color(242,55,38), strip.Color(217,64,48), strip.Color(132,14,10), strip.Color(72,0,0), strip.Color(18,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(52,0,0), strip.Color(137,13,11), strip.Color(177,2,0), strip.Color(225,10,7), strip.Color(252,34,24), strip.Color(255,120,100), strip.Color(248,73,54), strip.Color(202,23,16), strip.Color(153,17,17), strip.Color(37,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(1,0,0)},
	{strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(11,0,0), strip.Color(84,32,18), strip.Color(139,24,6), strip.Color(166,5,0), strip.Color(204,14,0), strip.Color(222,29,11), strip.Color(243,72,46), strip.Color(210,44,20), strip.Color(203,25,13), strip.Color(156,19,13), strip.Color(37,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(1,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(89,54,22), strip.Color(154,77,31), strip.Color(177,67,16), strip.Color(217,81,41), strip.Color(240,98,60), strip.Color(202,68,31), strip.Color(220,95,63), strip.Color(199,87,65), strip.Color(72,0,0), strip.Color(20,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,1,0), strip.Color(81,59,22), strip.Color(179,129,68), strip.Color(183,105,39), strip.Color(218,118,66), strip.Color(255,149,106), strip.Color(251,143,104), strip.Color(255,168,136), strip.Color(232,157,134), strip.Color(61,13,0), strip.Color(11,0,0), strip.Color(1,1,3), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(32,18,0), strip.Color(110,74,16), strip.Color(169,108,43), strip.Color(203,120,70), strip.Color(104,10,0), strip.Color(120,30,0), strip.Color(151,71,44), strip.Color(112,49,32), strip.Color(33,0,0), strip.Color(10,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(13,0,0), strip.Color(98,57,3), strip.Color(146,82,20), strip.Color(161,75,28), strip.Color(177,74,39), strip.Color(181,68,38), strip.Color(162,56,32), strip.Color(116,35,16), strip.Color(38,0,0), strip.Color(13,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(25,2,0), strip.Color(164,102,53), strip.Color(225,136,78), strip.Color(255,152,105), strip.Color(233,100,65), strip.Color(216,42,18), strip.Color(211,41,18), strip.Color(204,80,46), strip.Color(121,46,15), strip.Color(26,0,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(32,9,0), strip.Color(187,123,75), strip.Color(241,146,88), strip.Color(255,152,107), strip.Color(246,97,64), strip.Color(215,21,0), strip.Color(208,23,0), strip.Color(178,53,9), strip.Color(158,89,48), strip.Color(29,6,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(23,6,0), strip.Color(146,99,47), strip.Color(173,91,31), strip.Color(176,59,16), strip.Color(176,27,0), strip.Color(201,26,0), strip.Color(183,31,0), strip.Color(156,74,24), strip.Color(88,54,9), strip.Color(10,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(11,0,0), strip.Color(76,47,5), strip.Color(125,69,18), strip.Color(144,54,17), strip.Color(150,37,3), strip.Color(179,53,15), strip.Color(188,87,45), strip.Color(126,82,37), strip.Color(13,0,0), strip.Color(4,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(34,14,0), strip.Color(92,57,25), strip.Color(105,62,27), strip.Color(116,68,28), strip.Color(112,74,37), strip.Color(37,21,0), strip.Color(6,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,1,0), strip.Color(8,1,0), strip.Color(11,3,0), strip.Color(11,4,0), strip.Color(7,3,0), strip.Color(4,1,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,2), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(22,0,0), strip.Color(29,0,0), strip.Color(31,0,0), strip.Color(22,0,0), strip.Color(8,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(7,0,0), strip.Color(18,0,0), strip.Color(37,0,0), strip.Color(74,0,0), strip.Color(89,6,2), strip.Color(85,10,5), strip.Color(60,2,0), strip.Color(20,0,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(21,0,0), strip.Color(63,0,0), strip.Color(124,6,4), strip.Color(179,22,15), strip.Color(200,36,26), strip.Color(202,62,47), strip.Color(134,28,15), strip.Color(45,0,0), strip.Color(21,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(42,0,0), strip.Color(120,15,11), strip.Color(169,16,8), strip.Color(212,20,7), strip.Color(255,80,64), strip.Color(255,140,120), strip.Color(236,86,71), strip.Color(125,12,6), strip.Color(70,0,0), strip.Color(17,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(7,0,0), strip.Color(20,0,0), strip.Color(75,15,5), strip.Color(132,21,2), strip.Color(166,14,0), strip.Color(204,23,2), strip.Color(229,36,18), strip.Color(241,54,35), strip.Color(217,34,20), strip.Color(205,22,18), strip.Color(154,18,18), strip.Color(37,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(7,0,0), strip.Color(20,0,0), strip.Color(90,33,13), strip.Color(166,68,33), strip.Color(196,67,28), strip.Color(254,106,76), strip.Color(249,89,63), strip.Color(232,67,45), strip.Color(209,38,20), strip.Color(204,23,14), strip.Color(155,16,11), strip.Color(38,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(1,1,1)},
	{strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(13,0,0), strip.Color(109,75,37), strip.Color(210,144,84), strip.Color(183,97,36), strip.Color(232,139,95), strip.Color(248,148,114), strip.Color(255,169,134), strip.Color(247,139,110), strip.Color(190,83,63), strip.Color(72,0,0), strip.Color(20,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(93,66,23), strip.Color(147,92,27), strip.Color(190,115,48), strip.Color(149,62,17), strip.Color(124,30,0), strip.Color(175,77,38), strip.Color(202,109,75), strip.Color(223,147,121), strip.Color(63,12,0), strip.Color(13,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(65,32,1), strip.Color(169,99,47), strip.Color(163,62,8), strip.Color(162,34,0), strip.Color(169,28,0), strip.Color(178,34,0), strip.Color(200,73,41), strip.Color(211,124,97), strip.Color(53,2,0), strip.Color(13,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(91,53,30), strip.Color(229,148,103), strip.Color(247,126,81), strip.Color(196,41,13), strip.Color(219,42,22), strip.Color(226,42,18), strip.Color(198,37,9), strip.Color(206,95,67), strip.Color(86,21,0), strip.Color(15,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(81,43,22), strip.Color(227,140,97), strip.Color(255,126,85), strip.Color(202,27,10), strip.Color(219,16,9), strip.Color(230,12,2), strip.Color(216,19,0), strip.Color(182,38,4), strip.Color(130,44,11), strip.Color(28,0,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(1,1,1)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(39,8,0), strip.Color(186,112,67), strip.Color(172,56,9), strip.Color(166,6,0), strip.Color(192,9,0), strip.Color(215,25,0), strip.Color(202,32,0), strip.Color(178,52,4), strip.Color(164,85,46), strip.Color(32,6,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(24,7,0), strip.Color(132,93,38), strip.Color(139,69,7), strip.Color(137,26,0), strip.Color(157,36,0), strip.Color(181,80,8), strip.Color(179,98,19), strip.Color(148,87,22), strip.Color(90,52,7), strip.Color(11,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(7,1,0), strip.Color(66,49,5), strip.Color(117,82,26), strip.Color(137,70,25), strip.Color(127,58,3), strip.Color(142,97,16), strip.Color(150,122,39), strip.Color(113,91,33), strip.Color(14,0,0), strip.Color(4,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(6,0,0), strip.Color(29,15,0), strip.Color(83,57,24), strip.Color(96,67,25), strip.Color(99,78,23), strip.Color(95,81,32), strip.Color(34,25,0), strip.Color(6,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,0), strip.Color(1,0,0), strip.Color(3,1,0), strip.Color(9,2,0), strip.Color(11,3,0), strip.Color(12,3,0), strip.Color(8,2,0), strip.Color(4,1,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0), strip.Color(0,1,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(13,0,0), strip.Color(28,0,0), strip.Color(34,0,0), strip.Color(31,0,0), strip.Color(22,0,0), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(24,0,0), strip.Color(50,0,0), strip.Color(85,6,2), strip.Color(92,4,2), strip.Color(88,9,5), strip.Color(61,2,0), strip.Color(21,0,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(1,1,1), strip.Color(1,0,0), strip.Color(4,0,0), strip.Color(24,0,0), strip.Color(83,0,0), strip.Color(150,18,14), strip.Color(191,28,21), strip.Color(227,59,48), strip.Color(243,92,81), strip.Color(138,24,14), strip.Color(46,0,0), strip.Color(21,0,0), strip.Color(8,0,0), strip.Color(1,1,1), strip.Color(0,1,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(10,0,0), strip.Color(46,0,0), strip.Color(128,17,6), strip.Color(171,13,1), strip.Color(209,22,5), strip.Color(255,75,57), strip.Color(255,107,89), strip.Color(195,45,31), strip.Color(125,12,8), strip.Color(70,0,0), strip.Color(18,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(17,0,0), strip.Color(74,22,1), strip.Color(140,43,11), strip.Color(167,37,3), strip.Color(210,55,25), strip.Color(210,44,18), strip.Color(200,38,15), strip.Color(205,42,25), strip.Color(197,26,19), strip.Color(152,20,18), strip.Color(35,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(26,4,0), strip.Color(118,72,38), strip.Color(204,126,77), strip.Color(180,79,27), strip.Color(255,140,101), strip.Color(255,139,107), strip.Color(255,136,108), strip.Color(255,138,116), strip.Color(215,51,39), strip.Color(151,18,11), strip.Color(35,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(21,7,0), strip.Color(111,80,34), strip.Color(165,110,43), strip.Color(180,107,38), strip.Color(198,112,65), strip.Color(186,96,59), strip.Color(204,118,83), strip.Color(213,126,96), strip.Color(200,105,83), strip.Color(65,0,0), strip.Color(18,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(5,1,0), strip.Color(14,0,0), strip.Color(94,63,16), strip.Color(136,81,14), strip.Color(168,91,23), strip.Color(173,76,31), strip.Color(148,41,7), strip.Color(145,40,8), strip.Color(150,55,25), strip.Color(225,148,122), strip.Color(74,23,4), strip.Color(13,0,0), strip.Color(0,0,2), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(7,0,0), strip.Color(21,0,0), strip.Color(140,90,55), strip.Color(219,141,92), strip.Color(181,66,19), strip.Color(189,30,0), strip.Color(233,51,30), strip.Color(232,43,23), strip.Color(217,52,32), strip.Color(246,130,107), strip.Color(158,91,72), strip.Color(15,0,0), strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(7,0,0), strip.Color(21,0,0), strip.Color(162,108,82), strip.Color(254,165,125), strip.Color(227,93,56), strip.Color(206,20,0), strip.Color(230,20,5), strip.Color(220,18,0), strip.Color(199,28,0), strip.Color(191,75,36), strip.Color(102,36,4), strip.Color(15,0,0), strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,1,0)},
	{strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(13,0,0), strip.Color(92,47,26), strip.Color(216,127,87), strip.Color(182,46,8), strip.Color(198,15,0), strip.Color(209,22,5), strip.Color(188,39,0), strip.Color(171,58,2), strip.Color(159,79,10), strip.Color(106,60,1), strip.Color(20,5,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(39,8,0), strip.Color(179,108,64), strip.Color(172,60,14), strip.Color(165,9,0), strip.Color(170,16,0), strip.Color(165,62,3), strip.Color(146,77,0), strip.Color(153,102,21), strip.Color(137,106,41), strip.Color(20,10,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(21,8,0), strip.Color(119,86,32), strip.Color(143,79,17), strip.Color(137,32,0), strip.Color(147,41,0), strip.Color(144,81,1), strip.Color(131,94,5), strip.Color(131,102,24), strip.Color(78,58,5), strip.Color(7,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,1,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(6,3,0), strip.Color(52,41,0), strip.Color(110,79,24), strip.Color(135,73,26), strip.Color(122,59,5), strip.Color(135,100,19), strip.Color(143,125,43), strip.Color(108,92,32), strip.Color(12,2,0), strip.Color(4,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,1,1)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(27,15,0), strip.Color(82,58,24), strip.Color(95,68,25), strip.Color(96,79,25), strip.Color(93,83,32), strip.Color(32,25,0), strip.Color(4,1,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,1,0), strip.Color(9,2,0), strip.Color(11,3,0), strip.Color(12,3,0), strip.Color(8,2,0), strip.Color(4,1,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
},
{
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,0,0), strip.Color(18,0,0), strip.Color(49,0,0), strip.Color(54,0,0), strip.Color(39,0,0), strip.Color(22,0,0), strip.Color(8,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(8,0,0), strip.Color(21,0,0), strip.Color(50,0,0), strip.Color(114,16,13), strip.Color(135,27,24), strip.Color(97,8,4), strip.Color(63,0,0), strip.Color(21,0,0), strip.Color(7,0,0), strip.Color(3,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{…olor(7,0,0), strip.Color(26,4,0), strip.Color(122,72,39), strip.Color(213,122,77), strip.Color(191,70,25), strip.Color(255,112,78), strip.Color(249,98,69), strip.Color(255,113,86), strip.Color(255,119,97), strip.Color(219,51,40), strip.Color(150,18,13), strip.Color(35,0,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,2)},
	{strip.Color(0,0,0), strip.Color(4,0,0), strip.Color(20,6,0), strip.Color(112,81,34), strip.Color(173,116,47), strip.Color(171,96,28), strip.Color(199,111,63), strip.Color(209,117,80), strip.Color(246,156,121), strip.Color(241,152,120), strip.Color(190,98,77), strip.Color(63,0,0), strip.Color(17,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(17,0,0), strip.Color(102,69,18), strip.Color(139,87,14), strip.Color(183,111,39), strip.Color(191,99,52), strip.Color(135,35,1), strip.Color(140,43,8), strip.Color(142,56,23), strip.Color(207,138,109), strip.Color(72,25,5), strip.Color(11,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(10,0,0), strip.Color(49,12,0), strip.Color(188,127,83), strip.Color(186,107,50), strip.Color(163,53,0), strip.Color(199,46,15), strip.Color(214,42,22), strip.Color(211,41,16), strip.Color(208,63,34), strip.Color(250,147,114), strip.Color(157,95,70), strip.Color(15,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(11,0,0), strip.Color(66,24,0), strip.Color(228,158,124), strip.Color(232,137,93), strip.Color(154,20,0), strip.Color(206,19,0), strip.Color(234,22,11), strip.Color(227,20,2), strip.Color(206,31,2), strip.Color(211,96,52), strip.Color(152,88,50), strip.Color(15,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(8,0,0), strip.Color(50,19,1), strip.Color(188,127,99), strip.Color(207,104,71), strip.Color(160,12,0), strip.Color(213,17,5), strip.Color(230,11,5), strip.Color(226,14,0), strip.Color(201,28,0), strip.Color(177,71,9), strip.Color(127,75,17), strip.Color(19,3,0), strip.Color(0,1,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(6,0,0), strip.Color(15,0,0), strip.Color(61,14,0), strip.Color(170,81,49), strip.Color(153,23,0), strip.Color(183,3,0), strip.Color(204,8,0), strip.Color(208,24,0), strip.Color(192,47,2), strip.Color(178,95,25), strip.Color(143,105,42), strip.Color(21,9,0), strip.Color(0,1,0), strip.Color(1,1,1), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(6,0,0), strip.Color(30,4,0), strip.Color(130,77,35), strip.Color(133,45,0), strip.Color(148,15,0), strip.Color(165,22,0), strip.Color(171,49,0), strip.Color(157,67,4), strip.Color(146,97,30), strip.Color(80,59,6), strip.Color(8,1,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(10,0,0), strip.Color(62,36,1), strip.Color(121,70,27), strip.Color(148,64,28), strip.Color(140,51,9), strip.Color(148,78,19), strip.Color(145,97,35), strip.Color(109,86,32), strip.Color(10,2,0), strip.Color(4,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(3,0,0), strip.Color(8,0,0), strip.Color(30,12,0), strip.Color(88,55,24), strip.Color(100,65,27), strip.Color(102,71,25), strip.Color(98,77,34), strip.Color(33,23,0), strip.Color(4,1,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)},
	{strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(1,0,0), strip.Color(3,1,0), strip.Color(9,2,0), strip.Color(10,2,0), strip.Color(11,2,0), strip.Color(9,3,0), strip.Color(4,1,0), strip.Color(1,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0), strip.Color(0,0,0)}
}
};

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(10);

  Serial.begin(115200);
  Serial.println("Booting");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setPort(3232);
  ArduinoOTA.setHostname("myesp32");
  ArduinoOTA.setPassword("admin");
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
  
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
}

void setPixelXY(int x, int y, uint32_t color) {
  int i = y*16;

  if (y == 0 || y % 2 == 0) {
    i = i + 16 - (x + 1);
  } else {
    i = i + x;
  }

  strip.setPixelColor(i, color);
}

void setBufferXY(int x, int y, uint32_t color) {
  buffer[x][y] = color;
}

int limiter = 0;

void loop() {
  ArduinoOTA.handle();

  for (int i = 0; i <= 256; i++) {
    strip.setPixelColor(i, strip.Color(0,0,0));
  }

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      int targetOffsetX = x - offset;

      if (targetOffsetX < 0 || targetOffsetX > 15) { 
        continue;        
      }

      setBufferXY(x, y, foxBlack[counter%foxBlackFramesCount][y][targetOffsetX]);
    }
  }

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      int targetOffsetX = (15 - offset) + x;
      
      if (targetOffsetX < 0 || targetOffsetX > 15) { 
        continue;        
      }

      setBufferXY(x, y, mario[counter%marioFramesCount][y][targetOffsetX]);
    }
  }

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      int targetOffsetX = (30 - offset) + x;
      
      if (targetOffsetX < 0 || targetOffsetX > 15) { 
        continue;        
      }

      setBufferXY(x, y, foxBlack[counter%foxBlackFramesCount][y][targetOffsetX]);
    }
  }

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      setPixelXY(x, y, buffer[x][y]);
    }
  }

  strip.show();
  delay(100);

  if (limiter == 2) {
    offset++;
    limiter = 0;
  } else {
    limiter++;
  }

  if (offset == 30) {
    offset = 0;
  }

  counter++;

  if (counter == 40) {
    counter = 0;
  }
}
