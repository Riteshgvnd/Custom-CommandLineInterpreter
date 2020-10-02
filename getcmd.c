#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>


int n=0,cont=0;

void call2fun(char *,int,int,int);
void makeargs(char *,int,int,int);
void I_O(char* , int);

int checkcpupercentage(char *args[])
{
    pid_t child;
    int status;
    int n=0;
    child=fork();
    if(child != 0)
    {
        waitpid (child, &status, 0);
        n=WEXITSTATUS(status); 
        if(n==-1)
            printf("child terminated abnormally! Try again.\n");  
    }
    else 
    {
        execve("cut2",args,NULL);
    }    
    //free(args);
    return 1;
}

int checkresidentmemory(char *args[])
{
    char * argrss[6]={"/bin/ps","-o","rss","h",args[1],NULL};
    pid_t child;
    int status;
    int n=0;
    child=fork();
    if(child != 0)
    {
        waitpid (child, &status, 0);
        n=WEXITSTATUS(status); 
        if(n==-1)
            printf("child terminated abnormally! Try again.\n"); 
    }
    else 
    {   
        execve("/bin/ps",argrss,NULL);
    }
    return 1;
}

int listFiles(char * args[])
{
    pid_t child;
    int status,fd;
    
    child=fork();
    if (child!=0)
    {
        waitpid (child, &status, 0); 
    }
    else
    {
        fd= open("files.txt", O_CREAT | O_RDWR, S_IRWXO | S_IRWXG | S_IRWXU );
        if(fd==-1)
        {    printf("file can not be opened");
            exit(0);
        }
        dup2(fd,STDOUT_FILENO);
        char *ar[]={"/bin/ls",NULL};
        execve("/bin/ls",ar,NULL);
    }
    return EXIT_SUCCESS;
}

int sortFile(char * args[])
{
    pid_t child;
    int status,fd;
    
    child=fork();
    if (child!=0)
    {
        waitpid (child, &status, 0); 
    }
    else
    {
        char *ar[]={"/bin/sort",args[1],NULL};
        execve("/bin/sort",ar,NULL);
    }
    return EXIT_SUCCESS;
}

int executeCommands(char * args[])
{
    FILE *fp;
    char c,*cmd, *command;
    int len,flag,csize;
    int x=0,y=0;
    fp=fopen(args[1],"r");
    if(fp == NULL)
    {
        printf("file not open\n");
        return 0;
    }
    
    c='a';
    len=0;
    csize=30;
    c=fgetc(fp);
    cmd=(char*) malloc(csize*sizeof(char));
    while(c!=EOF)
    {
        cmd[len]=c;
        len++;
        c=fgetc(fp);
                
        if(len==csize)
        {   
            csize+=30;
            cmd=(char *) realloc(cmd,csize*sizeof(char));
        }

    }
    cmd[len]='\n';
    command=(char*) malloc(len*sizeof(char));
    for(x=0,y=0;x<=len;x++,y++)
    {   
        if(cmd[x]=='\n')
        {
            x++;
            command[y]='\0';
            makeargs(command,0,y,flag);
            free(command);
            y=0;
            command=(char*) malloc(len*sizeof(char));
        }

        command[y]=cmd[x];
        

    }
    

    
    return EXIT_SUCCESS;    

}



int match(char *args[],int flag)
{
    if(flag==-1)
        return 1;
    else if(strcmp(args[0],strcat("checkcpupercentage","\0"))==0)
        checkcpupercentage(args);
    else if (strcmp(args[0],strcat("checkresidentmemory","\0"))==0)
        checkresidentmemory(args);
    else if (strcmp(args[0],strcat("listFiles","\0"))==0)
        listFiles(args);
    else if (strcmp(args[0],"sortFile")==0)
        sortFile(args);
    else if (strcmp(args[0],"executeCommands")==0)
        executeCommands(args);
    else
        printf("Invalid command : %s\n",args[0]);
    
    return 0 ;
}

void makeargs(char *cmd,int init,int len,int flag)
{   
    pid_t child;
    int i,j,k,flag2;
    char *myargv[6];
    int status, fd[2];
    char buf[10];
    char * cm;
    if (flag>0)
    {   
        call2fun(cmd, init, len, flag);
    }

    else
    {
        
        j=0;
        k=0;
        cmd[len]='\0';
        myargv[j]=(char*)malloc(20*sizeof(char));
        while(cmd[init]==' ')
            init++;
        for (i=init;i<len;i++,k++)
        {   
            
            if(k==20 && flag2==1)
            {
                myargv[j]=(char*)realloc(myargv[j],40*sizeof(char));
                k=0;
            }

            if(cmd[i]==' ')
            {   while(cmd[i+1]==' ')
                    i++;
                flag2=0;
                myargv[j][k]='\0';
                k=-1;
                myargv[++j]= (char*)malloc(20*sizeof(char));
            }

            else
            {
                
                flag2=1;
                myargv[j][k]=cmd[i];
            }
            
        }
        if(flag>=0)
        {
            myargv[++j]=NULL;
          
           if(strcmp(myargv[0],"cat")==0)
            {
                cm=(char *) malloc(strlen(cmd)*sizeof(char));
                i=0;
                j=4;
                while(cmd[i]==' ')
                    i++;
                strcat(cm,"cat <");
                while(cmd[i]!=' ')
                     i++;
                while(cmd[i]!='\0')
                {
                    cm[++j]=cmd[i++];
                }
                       
                I_O(cm,0);
                fflush(stdin);
                fflush(stdout);
                return;
            }
            else    
            match(myargv,0);
        }

        else
        {   
            myargv[++j]=NULL;
              
        }
        
    }
    
}   


void call2fun(char *cmd,int init,int len,int flag)
{   
    pid_t child1;
    pid_t child2;
    pid_t par=getpid();
    int i=flag-1,j=flag+1,status;

     while(cmd[i]==' ')
    {
        cmd[i]='\0';
        i--;
    }

    while(cmd[j]==' ')
    {
        j++;
    }

    child1=fork();
    
    if (child1!=0)
    {   
        child2=fork();
        if(child2!=0)
        {
            waitpid (child1, &status, 0);
        }
        else
        {
            makeargs(cmd,j,len,0);
            exit(0);    
        }
        
        waitpid (child1, &status, 0); 
    }
    else
    {
        makeargs(cmd,0,i+1,0);  
        exit(0); 
    }

        
    if(getpid()!=par)
    {
        exit(0);
    }


}

void I_O(char * cmd , int flag)
{
     pid_t child1;
     int len=strlen(cmd);
     int fd[2],status,ffl1=0,ffl2=0;
     int i=0,j=0,k,gt_place=-1,lt_place=-1,pip=-1,apnd=0;
     char *comar1[8] ,* comar2[8];
     char * cm=(char *) malloc(len*sizeof(char));
     char * input_file=(char *) malloc(30*sizeof(char));
     char * output_file=(char *) malloc(30*sizeof(char));
     char * command1=(char *) malloc(60*sizeof(char));
     char * command2=(char *) malloc(60*sizeof(char));
    
    while(cmd[i]==' ')
        i++;
    while(cmd[i]!='\0')
    {   
        if(cmd[i]!=' ')
        {
            cm[j]=cmd[i];
            j++;
        }
        else
        {
            if(cmd[i+1]!=' ')
            {
                cm[j]=cmd[i];
                j++;
            }
            
        }
        i++;

    }
    cm[j]='\0';

    for(i=0;i<len;i++)
    {
        if(cm[i]=='<')
        {
            lt_place=i;
        }
        if(cm[i]=='>')
         {  
            gt_place=i;
         }
        if(cm[i]=='|')
         {
            pip=i;
         }
     }
    if(gt_place>0)
        if (cm[gt_place-1]=='>')
            apnd=1;
    i=lt_place+1;
    while(cm[i]==' ')
        i++;

    for(j=0;i<strlen(cm) && lt_place>0 && cm[i]!=' ' && cm[i]!='\n';i++,j++)
    {
        input_file[j]=cm[i];
    }
    input_file[j]='\0';

    i=gt_place+1;

    while(cm[i]==' ')
        i++;

    for(j=0;i<strlen(cm) && gt_place>0 && cm[i]!=' ' && cm[i]!='\n';i++,j++)
    {
        output_file[j]=cm[i];
    }
    output_file[j]='\0';

    i=pip+1;
    while(cm[i]==' ')
        i++;
    for(j=0;i<strlen(cm) && pip>0 && cm[i]!='>'  && cm[i]!='\n';i++,j++)
    {
        command2[j]=cm[i];
    }
    command2[j]='\0';

    if(lt_place>0)
    {
        for(i=0;i<lt_place;i++)
        {
            command1[i]=cm[i];
        }
        command1[i]='\0';
    }
    else
    {
        if(pip>0 && pip<gt_place)
        {
            for(i=0;i<pip;i++)
            {
                command1[i]=cm[i];
            }
            command1[i]='\0';
        }
        else if(gt_place>0)
        {
            for(i=0;i<gt_place;i++)
            {
                command1[i]=cm[i];
            }
            command1[i]='\0';
        }
    }
    
    comar1[0]=(char *) malloc(30);

    for (i=0,j=0,k=0;i<strlen(command1)-1;i++,k++)
        {   
                    
            if(command1[i]==' ')
            {   while(command1[i+1]==' ')
                    i++;

                comar1[j][k]='\0';
                k=-1;
                comar1[++j]= (char*)malloc(30*sizeof(char));
            }
            else
            {
                comar1[j][k]=command1[i];
            }            
        }
        comar1[++j]=NULL;
    
    if(pip>0)
    {
        comar2[0]=(char *) malloc(30);

        for (i=0,j=0,k=0;i<strlen(command2)-1;i++,k++)
        {   
            if(command2[i]==' ')
            {   while(command2[i+1]==' ')
                    i++;

                comar2[j][k]='\0';
                k=-1;
                comar2[++j]= (char*)malloc(30*sizeof(char));
            }
            else
            {
                comar2[j][k]=command2[i];
            }            
        }
        comar2[++j]=NULL;
    }
        
    if(match(comar1,-1)==1)
    {
        
        if(pip=-1)
        {              
                child1=fork();
                pipe(fd);
                if(child1!=0)
                {
                    waitpid(child1,&status,0);
                }
                else
                {
                
                if(lt_place!=-1)
                {   
                    ffl1=1;
                    fd[0]= open(input_file,O_CREAT | O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU );
                    if(fd[0]==-1)
                        {printf("file can not be opened");
                            exit(0);
                        }
                    dup2(fd[0],STDIN_FILENO);

                }
                if(gt_place!=-1)
                {
                    ffl2=1;
                    if(apnd!=1)
                    {
                        fd[1]= open(output_file, O_CREAT | O_RDWR| O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU );
                        if(fd[1]==-1)
                        {
                            printf("file can not be opened");
                            exit(0);
                        }
                        dup2(fd[1],STDOUT_FILENO);
                    }
                    else
                    {
                        fd[1]=open(output_file,O_CREAT |O_RDWR| O_APPEND,S_IRWXO | S_IRWXG | S_IRWXU);
                        if(fd[1]==-1)
                        {printf("file can not be opened");
                            exit(0);
                        }
                        dup2(fd[1],STDOUT_FILENO);
                    }

                }
                
                if(strcmp(strcat(comar1[0],"\0"),strcat("sortFile","\0"))==0 || strcmp(comar1[0],"sort")==0)
                {
                    strcpy(comar1[0],"/bin/sort");
                    //strcpy(comar1[1],input_file);
                }
                if(strcmp(comar1[0],"cat")==0 )
                {
                    strcpy(comar1[0],"/bin/cat");
                    //strcpy(comar1[1],input_file);
                    //comar1[2]=NULL;
                }
                if(strcmp(comar1[0],strcat("grep","\0"))==0 )
                {
                    strcpy(comar1[0],"/bin/grep");                  
                    execvp("/bin/grep",comar1);
                    if(ffl1==1){close(fd[0]); ffl1=0;}
                    if(ffl2==1){close(fd[1]); ffl2=0;}

                    free(cm);
                    free(command1);
                    free(command2);
                    return;

                }
                execve(comar1[0],comar1,NULL);

                if(ffl1==1){close(fd[0]); ffl1=0;}
                if(ffl2==1){close(fd[1]); ffl2=0;}
                }  
        }
        else if(pip>0)
        {
                child1=fork();
                pipe(fd);
                if(child1!=0)
                {
                    waitpid(child1,&status,0);
                }
                else
                {
                
                if(lt_place!=-1)
                {   
                    ffl1=1;
                    fd[0]= open(input_file,O_CREAT | O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU );
                    if(fd[0]==-1)
                        {printf("file can not be opened");
                            exit(0);
                        }
                    dup2(fd[0],STDIN_FILENO);
                }
                
                    ffl2=1;
                    fd[1]= open("tempo.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU );
                    if(fd[1]==-1)
                        {printf("file can not be opened");
                            exit(0);
                        }
                    dup2(fd[1],STDOUT_FILENO);
                
                
                if(strcmp(strcat(comar2[0],"\0"),strcat("sortFile","\0"))==0 || strcmp(comar2[0],"sort")==0)
                {
                    strcpy(comar2[0],"/bin/sort");
                    //strcpy(comar1[1],input_file);
                }
                if(strcmp(comar2[0],"cat")==0 )
                {
                    strcpy(comar2[0],"/bin/cat");
                    //strcpy(comar1[1],input_file);
                    //comar1[2]=NULL;
                }
                if(strcmp(comar2[0],strcat("grep","\0"))==0 )
                {
                    strcpy(comar2[0],"/bin/grep");                  
                    execvp("/bin/grep",comar2);
                    if(ffl1==1){close(fd[0]); ffl1=0;}
                    if(ffl2==1){close(fd[1]); ffl2=0;}

                    return;

                }
                execve(comar2[0],comar2,NULL);

                if(ffl1==1){close(fd[0]); ffl1=0;}
                if(ffl2==1){close(fd[1]); ffl2=0;}


                ffl1=1;
                fd[0]= open("tempo.txt",O_CREAT | O_RDONLY, S_IRWXO | S_IRWXG | S_IRWXU );
                if(fd[0]==-1)
                        {printf("file can not be opened");
                            exit(0);
                        }
                dup2(fd[0],STDIN_FILENO);
                if(gt_place!=-1)
                {
                    ffl2=1;
                    if(apnd!=1)
                    {
                        fd[1]= open(output_file, O_CREAT | O_RDWR|O_TRUNC, S_IRWXO | S_IRWXG | S_IRWXU );
                        if(fd[1]==-1)
                        {printf("file can not be opened");
                            exit(0);
                        }
                        dup2(fd[1],STDOUT_FILENO);
                    }
                    else{
                        fd[1]=open(output_file,O_CREAT |O_RDWR| O_APPEND,S_IRWXO | S_IRWXG | S_IRWXU);
                        if(fd[1]==-1)
                        {printf("file can not be opened");
                            exit(0);
                        }
                        dup2(fd[1],STDOUT_FILENO);
                    }
                }
                
                if(strcmp(strcat(comar2[0],"\0"),strcat("sortFile","\0"))==0 || strcmp(comar2[0],"sort")==0)
                {
                    strcpy(comar2[0],"/bin/sort");
                    //strcpy(comar1[1],input_file);
                }
                if(strcmp(comar2[0],"cat")==0 )
                {
                    strcpy(comar2[0],"/bin/cat");
                    //strcpy(comar1[1],input_file);
                    //comar1[2]=NULL;
                }
                if(strcmp(comar2[0],strcat("grep","\0"))==0 )
                {
                    strcpy(comar2[0],"/bin/grep");                  
                    execvp("/bin/grep",comar2);
                    if(ffl1==1){close(fd[0]); ffl1=0;}
                    if(ffl2==1){close(fd[1]); ffl2=0;}

                    free(cm);
                    free(command1);
                    free(command2);
                    return;

                }
                execve(comar2[0],comar2,NULL);

                if(ffl1==1){close(fd[0]); ffl1=0;}
                if(ffl2==1){close(fd[1]); ffl2=0;}
                


                }  

            remove("tempo.txt");
        }
    
    }
    
    
       

    

    free(cm);
    free(command1);
    free(command2);
    //free(comar1);
    //free(comar2);

 }

void sighandler1(int x)
{
    write(STDOUT_FILENO,"\nthe program is interrupted, do you want to exit [Y/N] ",54);
    char ch = getchar();
    while (1)
    {
        if(ch=='y'|| ch=='y')
            exit(3);
        else
        {
            fflush(stdout);
            fflush(stdin);
            return;
        }
    }
}

void sighandler2(int x)
{
    write(STDOUT_FILENO,"\nGot SIGTERM-Leaving\n",21);
         exit(3);
    
}


int main()
{
    int i,j,len,csize,pp=0,spcmd=0;
    signed int flag;
    char c,*cmd,*cm;
     
    signal(SIGINT,sighandler1);
    signal(SIGTERM,sighandler2); 
    while(1)
    {   
        len=0;
        spcmd=0;
        flag=0;
        csize=30;
        c='a';
        cmd= (char*) malloc(csize*sizeof(char));
       
        printf("myShell> ");
        
        while(c!='\n')
        {           
            c=getchar();
            
            if(c==';')
            {
                    flag=len;
            }

            if(c=='<' || c=='>' || c=='|')
            {
                spcmd=-1;
            }

            cmd[len]=c;
            len++;

            if(len==csize)
            {
                csize+=30;
                cmd= (char*) realloc(cmd,csize*sizeof(char));
            } 
        }
        

        cmd[len-1]='\0';
         if(cmd[0]==0)
        {
            cmd[0]=' ';
            continue;
        }
        cm= (char*) malloc(csize*strlen(cmd));
        i=0;
        while(cmd[i]==' ')
            i++;
        for(j=0;i<len;i++,j++)
        {
            cm[j]=cmd[i];
                       
        }

        if(strcmp(cm,"exit")==0)
            { 
                free(cm);
                free(cmd);
                exit(0);
            }

        if(spcmd==-1)
        {
            I_O(cmd,flag);
        }
        else
            makeargs(cmd,0,len,flag);

        free(cmd);
    }

   

    return EXIT_SUCCESS;
}