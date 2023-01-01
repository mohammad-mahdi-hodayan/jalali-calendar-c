#include <stdio.h>
#include <stdlib.h>
// detect current time
#include <time.h>
//for coloring current time
#include <windows.h>

// jalali lib
long *gregorian_to_jalali(long gy, long gm, long gd, long out[]) {
  long days;
  {
    long gy2 = (gm > 2) ? (gy + 1) : gy;
    long g_d_m[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    days = 355666 + (365 * gy) + ((int)((gy2 + 3) / 4)) - ((int)((gy2 + 99) / 100)) + ((int)((gy2 + 399) / 400)) + gd + g_d_m[gm - 1];
  }
  long jy = -1595 + (33 * ((int)(days / 12053)));
  days %= 12053;
  jy += 4 * ((int)(days / 1461));
  days %= 1461;
  if (days > 365) {
    jy += (int)((days - 1) / 365);
    days = (days - 1) % 365;
  }
  out[0] = jy;
  if (days < 186) {
    out[1]/*jm*/ = 1 + (int)(days / 31);
    out[2]/*jd*/ = 1 + (days % 31);
  } else {
    out[1]/*jm*/ = 7 + (int)((days - 186) / 30);
    out[2]/*jd*/ = 1 + ((days - 186) % 30);
  }
  return out;
}
long *jalali_to_gregorian(long jy, long jm, long jd, long out[]) {
  jy += 1595;
  long days = -355668 + (365 * jy) + (((int)(jy / 33)) * 8) + ((int)(((jy % 33) + 3) / 4)) + jd + ((jm < 7) ? (jm - 1) * 31 : ((jm - 7) * 30) + 186);
  long gy = 400 * ((int)(days / 146097));
  days %= 146097;
  if (days > 36524) {
    gy += 100 * ((int)(--days / 36524));
    days %= 36524;
    if (days >= 365) days++;
  }
  gy += 4 * ((int)(days / 1461));
  days %= 1461;
  if (days > 365) {
    gy += (int)((days - 1) / 365);
    days = (days - 1) % 365;
  }
  long gd = days + 1;
  long gm;
  {
    long sal_a[13] = {0, 31, ((gy % 4 == 0 && gy % 100 != 0) || (gy % 400 == 0)) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    for (gm = 0; gm < 13 && gd > sal_a[gm]; gm++) gd -= sal_a[gm];
  }
  out[0] = gy;
  out[1] = gm;
  out[2] = gd;
  return out;
} 


int get_1st_weekday(int year){
  int d;
  d = (((year - 1) * 365) + ((year - 1) / 4) - ((year - 1) / 100) + ((year) / 400) + 1) % 7;
  return d;
}

int main()
{
    // current date ----------------------------------
    int tarikh_j[] = {};
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	gregorian_to_jalali(tm.tm_year + 1900,tm.tm_mon + 1,tm.tm_mday,tarikh_j);
	int jy=tarikh_j[0];
	int jm=tarikh_j[1];
	int jd=tarikh_j[2];
    printf("--------- taghvim shamsi --------- \n");
    printf("emroz : ");
	printf("%d/%d/%d\n",jy,jm,jd);
    printf("\n note : the red number in output is today ... \n");

    //set color map for output: -----------------------
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    WORD saved_attributes;
     /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
    //-------------------------------------------------

   int year,month,day,daysInMonth,weekDay=0,startingDay;
   printf("\nEnter year that you want to see :");
   scanf("%d",&year);

   char *months[]={" farvardin ","ordibehesht","  khordad  ","    tir    ","  mordad   "," shahrivar ","    mehr   ","    aban   ","    azar   ","    dey    ","   bahman  ","  esfand   "};
   int monthDay[]={31,31,31,31,31,31,30,30,30,30,30,29};

   if((year%4==0&&year%100!=0)||year%400==0)
       monthDay[1]=29;

   startingDay=get_1st_weekday(year);

   for(month=0;month<12;month++){
    
      daysInMonth=monthDay[month];
      printf("\n\n-------------%s-------------\n",months[month]);
      printf("\n  sha  1sh  2sh  3sh  4sh  5sh  jom\n");

      for(weekDay=0;weekDay<startingDay;weekDay++)
        printf("     ");

      for(day=1;day<=daysInMonth;day++){
        
        if(day == jd && month+1 == jm){
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            printf("%5d",day);
            SetConsoleTextAttribute(hConsole, saved_attributes);
        }else{
            printf("%5d",day);
        }
        if(++weekDay>6){
            printf("\n");
            weekDay=0;
        }
        startingDay=weekDay;
      }

   }


}
