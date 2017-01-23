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


class SignalClass
{
public:
    SignalClass(){}
signals:
    void theSignal(int a)
    {
        cout<<"SignalClass::theSignal(int) called with argument:("<<a<<")"<<endl;
        emit(SIGNAL(*this,theSignal,int),string("some string argument"));
        //Emit tells that theSignal(int) is called and all slots like ***(string)  must call
        //Do not forget use *this   (and *this != this)
        //string("some string argument") is argument and is difference with "some string argument"
        //emit(SIGNAL(signalobj,signal,args...),argvalue1,...)
        emit(SIGNAL(*this,theSignal,int),a,string("another string arguments"));
    }
};

class SlotClass1
{
public:
    SlotClass1(){}
slots:
    void theSlot(string arg)
    {
        cout<<"SlotClass1::theSlot(string) called with argument:("<<arg<<")"<<endl;
    }
};

class SlotClass2
{
public:
    SlotClass2(){}
slots:
    void theSlot(string arg)
    {
        cout<<"SlotClass2::theSlot(string) called with argument:("<<arg<<")"<<endl;
    }
    void theSlot(int arg1,string arg2)
    {
        cout<<"SlotClass2::theSlot(int,string) called with argument1:("<<arg1<<") and argument2:("<<arg2<<")"<<endl;
    }
};

int main()
{
    SignalClass a;
    SlotClass1 b;
    SlotClass2 c;
    cout<<"Before Connect:"<<endl;
    a.theSignal(50);
    connect(SIGNAL(a,theSignal,int),SLOT(b,theSlot,string));
    //connect(SIGNAL(signalobj,signal,args...) , SLOT(slotobj,slot,args...))
    //"disConnect" also Like "connect"
    //note: max argument counts is 10
    connect(SIGNAL(a,theSignal,int),SLOT(c,theSlot,string));
    connect(SIGNAL(a,theSignal,int),SLOT(c,theSlot,int,string));
    cout<<"\n\nAfter Connect:"<<endl;
    a.theSignal(50);
    return 0;
}
