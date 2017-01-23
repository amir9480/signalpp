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

class Area
{
public:
    Area(){}
    string areaname;
signals:
    void OnAreaClicked(int x,int y)
    {
        cout<<"Area::OnAreaClicked() called"<<endl;
        emit(SIGNAL(*this,OnAreaClicked,int,int),areaname ,x,y);
    }
};
class Button
{
public:
    Button(int _x,int _y,int _w,int _h)
    {
        x=_x;
        y=_y;
        width=_w;
        height=_h;
    }

    int x,y,width,height;
signals:
    void OnButtonClicked(string areaname,int _x,int _y)
    {
        if(_x>=x&&_y>=y&&_x<=x+width&&_y<=y+height)
        {
            emit(SIGNAL(*this,OnButtonClicked,string,int,int),areaname);
        }
    }
};

class SlotDemo
{
public:
    SlotDemo(){}
slots:
    void Called(string areaname)
    {
        cout<<"SlotDemo::Called() called! areaname="<<areaname<<endl;
    }
};

int main()
{
    Area a;
    a.areaname="area";
    Button b(10,10,10,10);
    SlotDemo c;
    connect(SIGNAL(a,OnAreaClicked,int,int),SLOT(b,OnButtonClicked,string,int,int));//Signal to Signal connect is like Signal Slot
    connect(SIGNAL(b,OnButtonClicked,string,int,int),SLOT(c,Called,string));
    a.OnAreaClicked(40,77);//it's not in button area
    a.OnAreaClicked(15,18);//it's in button area
    return 0;
}

