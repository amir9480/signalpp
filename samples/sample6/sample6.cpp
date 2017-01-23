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


class SignalParent
{
public:
    SignalParent(){}
signals:
    virtual void OnSig()
    {

    }
};

class SignalChild:public SignalParent
{
public:
    SignalChild(int a){value=a;}
    int value;
//do not use signals again . just OnSig Must be public
    virtual void OnSig()
    {
        emit(SIGNAL(*this,OnSig),value);
    }
};

class SlotDemo
{
public:
    SlotDemo(){}
slots:
    void SlotDemoCall(int v)
    {
        cout<<"SlotDemo:SlotDemoCall called! with argument=("<<v<<")"<<endl;
    }
};

int main()
{
    SignalParent* a = new SignalChild(150);
    SlotDemo b;
    a->OnSig();
    connect(SIGNAL(*a,OnSig),SLOT(b,SlotDemoCall,int));
    a->OnSig();
    return 0;
}
