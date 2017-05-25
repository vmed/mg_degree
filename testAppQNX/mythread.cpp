#include "mythread.h"
#include <QtCore/qmath.h>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <time.h>

#include <qtextcodec>
#include <qmessagebox.h>
#include <unistd.h>
#include <QTime>

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <hw/inout.h>
#include <sched.h>

MyThread::MyThread()
{
}
#define CODE_TIMER      2
#define PERIOD			1000

void MyThread::run()
{
    setPriority(QThread::HighPriority);
    int rcvid;
    int chid;
    struct _pulse msg;
    timer_t timerid;
    struct sigevent evt;
    struct itimerspec timer;
    int coid;
    struct _clockperiod clk_per_old;
    struct _clockperiod clk_per_new;
    clk_per_new.fract = 0;
    clk_per_new.nsec = 100000;
    //int z = 0;

    //********************************************************************
    //*******     timer       ********************************************
    //********************************************************************

    //create a channel for timer pulse reception
    if ( (chid = ChannelCreate(0)) == -1 )
    {
        //ui.teLog->append( "error creating a channel!\n");
        //exit(EXIT_FAILURE);
    }


    // create a connection to ourselves
    coid = ConnectAttach(ND_LOCAL_NODE, 0,chid,0,0);
    if (coid == -1)
    {
        //ui.teLog->append( tr("error conecting to a channel! %1\n").arg(strerror( errno )) );
        //exit(EXIT_FAILURE);
    }
    // set up pulse event to be generated by timer
    SIGEV_PULSE_INIT(&evt,coid,/*SIGEV_PULSE_PRIO_INHERIT*/getprio(0),CODE_TIMER,0);
    // create timer and anchor event to it
    if ( timer_create(CLOCK_REALTIME,&evt, &timerid) == -1)
    {
        //ui.teLog->append( "error creating a timer!\n");
        //exit(EXIT_FAILURE);
    }
    //setup timer delay and period
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_nsec = PERIOD*5000*100;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_nsec = PERIOD*5000*100;

    // set thread privelege level to allow port IO operations
    if (ThreadCtl(_NTO_TCTL_IO, 0) == -1)
    {
       // ui.teLog->append( "Couldn't set proper thread privelege level!\n");
        //exit(EXIT_FAILURE);
    }

    // adjust clock period
    clk_per_new.fract = 0;
    clk_per_new.nsec = 10000;

    if ( ClockPeriod(CLOCK_REALTIME, &clk_per_new, &clk_per_old, 0) == -1)
    {
        //ui.teLog->append( " Couldn't set clock period!\n");
        //exit(EXIT_FAILURE);
    }
    //ui.teLog->append( tr("Clock period: old = %1 nsec; new = %2 nsec\n").arg(clk_per_old.nsec).arg(clk_per_new.nsec) );
    //qApp->processEvents();
    // start timer
    timer_settime(timerid,0,&timer, NULL);

    //********************************************************************
    //*******     timer       ********************************************
    //********************************************************************

    double wx = 0.0;


    //Trying to open PCI's
    if(PCI1753_0.open(0)== 0){
        sendMsgToConsole("PCI1753 was opened!");
        PCI1753_0.setDirection(4, 0);
    }
    else
        sendMsgToConsole("Error: I can't open 1753");

    if (PCI1713_3.open(3) == 0)
        sendMsgToConsole("PCI1713_3 was opened!");
    else
        sendMsgToConsole("Error: I can't open 1713_3");



    for(double t = 0.0;;)
        {
        rcvid = MsgReceivePulse(chid, &msg,sizeof(msg),NULL );//get event from QNX to block thread

        reload_PCI1713();

        switch(int(wx)){
            case (0):
                PCI1753_0.writePin(26, 1);
                wx=double(PCI1753_0.readPin(26));
                break;
            case (1):
                PCI1753_0.writePin(26, 0);
                wx=double(PCI1753_0.readPin(26));
                break;
        }

        //wx = qSin(0.05*t)+qSin(0.01*t);
        /*if(qSin(0.05*t)+qSin(0.01*t)>0)
            z=1;
        else
            z=-1;
        switch(z)
        {
        case (1)    : wx = 0.11 + wx; break;
        case (-1)   : wx = -0.11 + wx; break;
        case (0)    : wx = wx; break;
        }*/


        emit send(double(data[0]), t);
        t=t+0.5;
        }
}

void MyThread::reload_PCI1713(){
    for(int i=0; i<32; i++)
        data[i]=PCI1713_3.read(i);
}