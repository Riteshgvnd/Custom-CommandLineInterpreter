#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>
int main(int argc, char *argv[])
{
    long utickspsec1,stickspsec1,utickspsec2,stickspsec2;
    double ucpupercetage,scpupercetage;
    FILE *fp;
    int int1,int2,int3,int4,int5,int6,i,j;
    char str[20],ch;
    long long1,long2,long3,long4,long5,long6,long7,cpu_ticks1,cpu_ticks2;
    long total_cpu_tick_diff;
    long u_mode_tick_diff , s_mode_tick_diff;
    char *path1,*path2,*p2;
    path1=(char*) malloc(50);
    path2=(char*) malloc(50);
    p2=(char*) malloc(30);
    strcpy(path1,"/proc/");
    strcpy(p2,"/stat");
    for(i=6;i<strlen(argv[1])+6;i++)
    {
        path1[i]=argv[1][i-6];
    }
    for(j=0;j<strlen(p2);j++,i++)
    {
        path1[i]=p2[j];
    }
    path1[i]='\0';

    fp= fopen(path1,"r");
    if (fp == NULL) 
    {
        perror("ERROR:process not present ");
        fclose(fp);
        return -1;
    }

    rewind(fp);
    fscanf(fp,"%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %ld %ld ",&int1,str,&ch,&int2,&int3,&int4,&int5,&int6,&long1,&long2,&long3,&long4,&long5,&utickspsec1,&stickspsec1);
    fclose(fp);

    strcpy(path2,"/proc/stat");
    path2[10]='\0'; 
    fp =fopen(path2,"r");
    if (fp == NULL) 
    {
        perror("ERROR:process not present ");
        fclose(fp);
        return -1;
    }
    fscanf(fp,"%s %ld %ld %ld %ld %ld %ld %ld",str,&long1,&long2,&long3,&long4,&long5,&long6,&long7);


    cpu_ticks1=long1+long2+long3+long4+long5+long6+long7;
    fclose(fp);


    sleep(1);


    fp= fopen(path1,"r");
    if (fp == NULL) 
    {
        perror("ERROR:process not present ");
        fclose(fp);
        return -1;
    }

    rewind(fp);
    fscanf(fp,"%d %s %c %d %d %d %d %d %lu %lu %lu %lu %lu %ld %ld ",&int1,str,&ch,&int2,&int3,&int4,&int5,&int6,&long1,&long2,&long3,&long4,&long5,&utickspsec2,&stickspsec2);
    fclose(fp);

   fp =fopen(path2,"r");
    if (fp == NULL) 
    {
        perror("ERROR:process not present ");
        fclose(fp);
        return -1;
    }
    fscanf(fp,"%s %ld %ld %ld %ld %ld %ld %ld",str,&long1,&long2,&long3,&long4,&long5,&long6,&long7);


    cpu_ticks2=long1+long2+long3+long4+long5+long6+long7;
    fclose(fp);

    total_cpu_tick_diff= cpu_ticks2-cpu_ticks1;
    u_mode_tick_diff=utickspsec2-utickspsec1 ;
    s_mode_tick_diff=stickspsec2-stickspsec1;

    ucpupercetage=(u_mode_tick_diff*100)/(double)total_cpu_tick_diff;
    scpupercetage=(s_mode_tick_diff*100)/(double)total_cpu_tick_diff;

    printf("user mode cpu percentage: %lf%%\nsystem mode cpu percentage: %lf%%\n",ucpupercetage,scpupercetage);

    free(path1);
    free(path2);
    free(p2);


return EXIT_SUCCESS;

}


