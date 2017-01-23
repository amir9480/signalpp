/* ***************************************************************************
MIT License

Copyright (c) 2016 Signal++ .

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
==============================================================================
github.com/amir9480/signalpp
version = 2.0
Auther(s): Amir Alizadeh => (amiralizadeh9480@gmail.com) (telegram.me/amir9480)

****************************************************************************/
#define SIGNALPP_T 0

#include <iostream>
#include "../../signalpp/signalpp.h"
using namespace std;
using namespace signalpp;


class Signalsclass
{
public:
    Signalsclass(){}
signals:
    void samplesignal()
    {
        cout<<"samplesignal() is called"<<endl;
        emit(SIGNAL(*this,samplesignal));
    }
slots:
    void sampleslot1()
    {
        cout<<"Signalsclass::sampleslot1() is called"<<endl;
    }
};

class Slotsclass
{
public:
    Slotsclass(){}
    string cname;
slots:
    void sampleslot2()
    {
        cout<<"Slotsclass::sampleslot2() is called  (cname="<<cname<<")"<<endl;
    }
    void sampleslot3()
    {
        cout<<"Slotsclass::sampleslot3() is called  (cname="<<cname<<")"<<endl;
    }
};

int main()
{
    Signalsclass a;
    Slotsclass b,c;
    b.cname = "B";
    c.cname = "C";
    cout<<"Before connects:"<<endl;
    a.samplesignal();
    connect(SIGNAL(a,samplesignal),SLOT(a,sampleslot1));//connect signal a.samplesignal() to slot a.sampleslot1()
    connect(SIGNAL(a,samplesignal),SLOT(b,sampleslot2));//connect signal a.samplesignal() to slot b.sampleslot2()
    connect(SIGNAL(a,samplesignal),SLOT(b,sampleslot3));//connect signal a.samplesignal() to slot b.sampleslot3()
    connect(SIGNAL(a,samplesignal),SLOT(c,sampleslot2));//connect signal a.samplesignal() to slot c.sampleslot2()
    connect(SIGNAL(a,samplesignal),SLOT(c,sampleslot3));
    connect(SIGNAL(a,samplesignal),SLOT(a,sampleslot1));//connect signal a.samplesignal() to slot a.sampleslot1()
    connect(SIGNAL(a,samplesignal),SLOT(b,sampleslot2));//connect signal a.samplesignal() to slot b.sampleslot2()
    connect(SIGNAL(a,samplesignal),SLOT(b,sampleslot3));//connect signal a.samplesignal() to slot b.sampleslot3()
    connect(SIGNAL(a,samplesignal),SLOT(c,sampleslot2));//connect signal a.samplesignal() to slot c.sampleslot2()
    connect(SIGNAL(a,samplesignal),SLOT(c,sampleslot3));//connect signal a.samplesignal() to slot c.sampleslot3()
    cout<<"After connects:"<<endl;
    a.samplesignal();
    disconnectall(SIGNAL(a,samplesignal));
    cout<<"After DisconnectAll:"<<endl;
    a.samplesignal();

    return 0;
}

