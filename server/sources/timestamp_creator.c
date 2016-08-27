#include "../headers/timestamp_creator.h"

int get_timestamp (char time_ch[23])
{
        time_t rawtime;
        struct tm *time_info;
        rawtime = time(NULL);
        time_info = localtime(&rawtime);

        struct timeval tv;
        gettimeofday(&tv, NULL);

        int millisecond = tv.tv_usec / 1000;
        int second = time_info->tm_sec;
        int minute = time_info->tm_min;
        int hour = time_info->tm_hour;
        int day = time_info->tm_mday;
        int month = 1 + time_info->tm_mon;
        int year = 1900 + time_info->tm_year;

        char millisecond_ch[4];
        bzero(millisecond_ch, 4);
        char second_ch[4];
        bzero(second_ch, 4);
        char minute_ch[4];
        bzero(minute_ch, 4);
        char hour_ch[4];
        bzero(hour_ch, 4);
        char day_ch[4];
        bzero(day_ch, 4);
        char month_ch[4];
        bzero(month_ch, 4);
        char year_ch[4];
        bzero(year_ch, 4);

        convert_int_to_string(millisecond_ch, millisecond);
        convert_int_to_string(second_ch, second);
        convert_int_to_string(minute_ch, minute);
        convert_int_to_string(hour_ch, hour);
        convert_int_to_string(day_ch, day);
        convert_int_to_string(month_ch, month);
        convert_int_to_string(year_ch, year);


        memcpy((void *)(&time_ch[0]), (void *)year_ch, 4);
        time_ch[4] = '-';
        memcpy((void *) (&time_ch[5]), (void *) (month_ch + 2), 2);
        time_ch[7] = '-';
        memcpy((void *) (&time_ch[8]), (void *) (day_ch + 2), 2);
        time_ch[10] = 'T';
        memcpy((void *) (&time_ch[11]), (void *) (hour_ch + 2), 2);
        time_ch[13] = ':';
        memcpy((void *) (&time_ch[14]), (void *) (minute_ch + 2), 2);
        time_ch[16] = ':';
        memcpy((void *) (&time_ch[17]), (void *) (second_ch + 2), 2);
        time_ch[19] = ':';
        memcpy((void *) (&time_ch[20]), (void *) (millisecond_ch + 1), 3);



}

//only works on number
convert_int_to_string (char *string, int number)
{
        int thousand, hundred, ten, one;
        thousand = 0;
        hundred = 0;
        ten = 0;
        one = 0;

        if(number/1000 > 0)
        {
            thousand = number/1000;
            number = number % 1000;
        }
        if(number/100 > 0)
        {
            hundred = number/100;
            number = number % 100;
        }
        if(number/10 > 0)
        {
            ten = number/10;
            number = number % 10;
        }

        one = number;

        string[0] = (char) (48 + thousand);
        string[1] = (char) (48 + hundred);
        string[2] = (char) (48 + ten);
        string[3] = (char) (48 + one);
}
