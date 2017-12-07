//
// Created by Angel Zaprianov on 9.5.2017 г..
//

#ifndef ARDUINO_MID_LAN
#define ARDUINO_MID_LAN

#include <Arduino.h>

//
// Maximum size of message is 128 symbols
//
//
const char msg0[] PROGMEM = "";
const char msg72[] PROGMEM = "Довиждане...     ";
const char msg1[] PROGMEM = "до следващия път!";
const char msg2[] PROGMEM = "Преход ";
//
// Car state
const char msg3[] PROGMEM = "Предни на спирачки: ";
const char msg4[] PROGMEM = "Охладителна течност: ";
const char msg5[] PROGMEM = "Течност за чистачки: ";
const char msg6[] PROGMEM = "Ниво моторно  масло: ";
const char msg7[] PROGMEM = "Добре"; // Is fine message
const char msg8[] PROGMEM = "Под 25%"; // Problem message for liquid
const char msg9[] PROGMEM = "Износени"; // Problem message for brakes
const char msg10[] PROGMEM = "Изгорели"; // Problem message for lamps
//
// Titles
const char msg11[] PROGMEM = "НАЧАЛО";
const char msg12[] PROGMEM = "ПЪТУВАНИЯ";
const char msg13[] PROGMEM = "ГОРИВА";
const char msg14[] PROGMEM = "ОБСЛУЖВАНЕ";
const char msg15[] PROGMEM = "ОТНОСНО";
const char msg16[] PROGMEM = "ИНСТУМЕНТИ";
const char msg17[] PROGMEM = "НАСТРОЙКИ";
const char msg18[] PROGMEM = "ТЕКУЩО";
const char msg19[] PROGMEM = "ЗАВОДСКИ";
//
//
const char msg20[] PROGMEM = "За нулиране на пътуването";
const char msg21[] PROGMEM = "Задръжте >R и натиснете спирачката ";
const char msg22[] PROGMEM = "За нови версии: www.git.io/vHlkd";
const char msg23[] PROGMEM = "ArduinoMID версия ";
const char msg24[] PROGMEM = "Натиснете \"S<\" за промяна";
const char msg25[] PROGMEM = "Използвай \"S<\" за навигация ";
const char msg26[] PROGMEM = "Използвай \"R<\" за промяна";
const char msg27[] PROGMEM = "СКОРОСТ НА КОЛА SPEED"; // Vehicle speed sensor
const char msg28[] PROGMEM = "ОБОРОТИ В МИН.";
const char msg29[] PROGMEM = "ОДОМЕТЪР";
const char msg30[] PROGMEM = "КОНСУМАЦИЯ";
const char msg31[] PROGMEM = "РЕЗУЛТАТ";
const char msg32[] PROGMEM = "Изгасетедвигателя за запазване.";
const char msg33[] PROGMEM = "Използвайте спирачката за край";
const char msg34[] PROGMEM = " Направете почивка";
const char msg35[] PROGMEM = " Сънливост засечена";
const char msg36[] PROGMEM = " Неизправност на крушка";
//
// Warning winter
const char msg37[] PROGMEM = " ОПАСНОСТ ОТ ЗАЛЕДЯВАНИЯ ПО ПЪТЯ!";
const char msg38[] PROGMEM = " ";
const char msg39[] PROGMEM = "Моля шофирайте внимателно!";
//
// Warning Oil level
const char msg40[] PROGMEM = "НИВОТО НА МОТОРНОТО МАСЛО!";
const char msg41[] PROGMEM = "Ниво е по-малко от 25% ";
const char msg42[] PROGMEM = "Моля проверете.";
//
// Warning Battery
const char msg43[] PROGMEM = "ЕЛЕКТРИЧЕСИ ПРОБЛЕМ!";
const char msg44[] PROGMEM = "Напрежение от: ";
const char msg45[] PROGMEM = "не е в работните граници.";
//
// Warning breaks
const char msg46[] PROGMEM = "НЕИЗПРАВНОСТ В СПИРАЧКИТЕ!";
const char msg47[] PROGMEM = "Износване на накладките";
const char msg48[] PROGMEM = "при предните спирачки.  ";
//
// Warning coolant
const char msg49[] PROGMEM = "ОХЛАДИТЕЛНАТА ТЕЧНОСТ";
const char msg50[] PROGMEM = "Охладителната течност";
const char msg51[] PROGMEM = "е малко. ";
//
// Warning overheating
const char msg52[] PROGMEM = "ОПАСНОСТ ОТ ПРЕГРЯВАНЕ!";
const char msg53[] PROGMEM = "Температурата в охладителната ";
const char msg54[] PROGMEM = "система е твърде висока!";
//
// Warning window washer
const char msg55[] PROGMEM = "ТЕЧНОСТТА ЗА ЧИСТАЧКИ!";
const char msg56[] PROGMEM = "Ниво е по-малко от 25% ";
const char msg57[] PROGMEM = "Моля проверете.";
//
// Warning window washer
const char msg58[] PROGMEM = " ИЗГОРЯЛА КРУШКА";
const char msg59[] PROGMEM = "Неработаща круша в : ";
const char msg60[] PROGMEM = "Предни светлини";
const char msg61[] PROGMEM = "Задни светлини ";
//
// Warning timing  Belt ware
const char msg62[] PROGMEM = "СМЯНА АНГРЕНАЖЕН РЕМЪК";
const char msg63[] PROGMEM = "Ангренаженият ремък се сменя на над";
const char msg64[] PROGMEM = "85000km (След смяна нулирайте данните)";
//
// Warning Air filter ware
const char msg65[] PROGMEM = " НУЖНА СМЯНА ВЪЗДУШЕН/МАСЛЕН ФИЛТЪР ";
const char msg66[] PROGMEM = "Въздушният филтър се сменя на над";
const char msg67[] PROGMEM = "42000km (След смяна нулирайте данните)";
//
// Value tags
const char msg68[] PROGMEM = "л";
const char msg69[] PROGMEM = "км";
const char msg70[] PROGMEM = "ч";
const char msg71[] PROGMEM = "л/ч";
//
// Generate message table for LCD
const char *const LcdMsgTable[] PROGMEM = {
        msg0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15,
        msg16, msg17, msg18, msg19, msg20, msg21, msg22, msg23, msg24, msg25, msg26, msg27, msg28, msg29, msg30,
        msg31, msg32, msg33, msg34, msg35, msg36, msg37, msg38, msg39, msg40, msg41, msg42, msg43, msg44, msg45,
        msg46, msg47, msg48, msg49, msg50, msg51, msg52, msg53, msg54, msg55, msg56, msg57, msg58, msg59, msg60,
        msg61, msg62, msg63, msg64, msg65, msg66, msg67, msg68, msg69, msg70, msg71, msg72
};
#endif //ARDUINOMID_EN_H_H
