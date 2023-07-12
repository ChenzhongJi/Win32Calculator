#pragma once
#include <time.h>
#include <string>


void getLogStr(std::string& logStr)
{
    time_t timep; //本质上time_t是长整型
    struct tm now_time;

    time(&timep); //利用函数time()获取从1970年至今的秒数

    localtime_s(&now_time, &timep);
    //{ "%d/%d/%d %02d:%02d:%02d\t", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec };
    char timeStr[100] = { 0 };
    snprintf(timeStr, sizeof(timeStr) - 1, "%d/%d/%d %02d:%02d:%02d ", 1900 + now_time.tm_year, 1 + now_time.tm_mon, now_time.tm_mday, now_time.tm_hour, now_time.tm_min, now_time.tm_sec);
    logStr.insert(0, timeStr);
}
