#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>


int main(int argc,char** argv)
{
    int Ard;
    unsigned char c,v;

    struct termios old_stdi;
    struct termios old_stdo;
    struct termios stdio;

    tcgetattr(STDIN_FILENO,&old_stdi);
    tcgetattr(STDOUT_FILENO,&old_stdo);

    //open the serial port and check if the open was successful
    Ard = open(argv[1],O_RDWR | O_NOCTTY | O_NDELAY);
    if(Ard == -1){ // if open is unsuccessful
        printf("Error: can not open Arduino.\n");
        return EXIT_SUCCESS;
    }
    else
        printf("Arduino is connected successfully.\npower on.\n");


    //setup the Arduino communication
    //4 steps that are specified in the example problem
    struct termios options;                    //Declare a termios structure
    tcgetattr(Ard,&options);                //read the current port configurations
    options.c_cflag |= (CLOCAL | CREAD);    //manipulate the structure to set up specific attributes

    fcntl(Ard, F_SETFL, O_NONBLOCK);                    // make the reads blocking
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);                    // make the reads blocking
    cfsetispeed(&options, B115200);            // set baud rates
    cfsetospeed(&options, B115200);
    tcsetattr(Ard,TCSANOW,&options);        //write back the termios structure to the port so that modifications take place
    memset(&stdio,0,sizeof(stdio));
    stdio = old_stdo;

    do{
        if (read(STDIN_FILENO,&c,1)>0)        write(Ard,&c,1);            //read from keyboard and send to Arduino
        if (read(Ard,&v,1)>0)                write(STDOUT_FILENO,&v,1);    //read from Arduino and send to output terminal

    }while (c!='q');




    //restore the old configuration and exit
    write(STDOUT_FILENO,"\n",1);
    tcsetattr(STDIN_FILENO,TCSANOW,&old_stdi);
    tcsetattr(STDOUT_FILENO,TCSANOW,&old_stdo);
    return  EXIT_SUCCESS;
}
