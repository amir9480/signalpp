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
#define SIGNALPP_T 0 //disable multi threading

#include <iostream>
#include "../../signalpp/signalpp.h"
using namespace std;
using namespace signalpp;

class TrafficLight
{
public:
    TrafficLight(){}
    enum LightColors{ red,yellow,green } color; // simple enum to hold the traffic light color
signals: // siganls shoud add here (note: you can use signals only once time in one class)
    void SetRed()
    {
        color = red;
        emit(SIGNAL(*this,SetRed));//this mean's SetRed is Called
    }
    void SetYellow()
    {
        color = yellow;
        emit(SIGNAL(*this,SetYellow));//this mean's SetYellow is Called
    }
    void SetGreen()
    {
        color = green;
        emit(SIGNAL(*this,SetGreen));//this mean's SetGreen is Called
    }
};


class Cars
{
public:
    Cars(){}
slots://slots should add here
    void StopCars()
    {
        cout<<"Cars::StopCars() slot is called"<<endl;
    }
    void RunCars()
    {
        cout<<"Cars::RunCars() slot is called"<<endl;
    }
    void SlowerCars()
    {
        cout<<"Cars::SlowerCars() slot is called"<<endl;
    }
};

int main()
{
    TrafficLight light;
    Cars cars;
    cout<<"Before using Connect:"<<endl;
    light.SetRed();
    light.SetYellow();
    light.SetGreen();

    connect(SIGNAL(light,SetRed),SLOT(cars,StopCars)); // connect signal :light.SetRed()  to  slot: cars.StopCars()
    //connect( SIGNAL(Signal Obj,Signal),SLOT(Slot Obj,Slot) )
    connect(SIGNAL(light,SetYellow),SLOT(cars,SlowerCars));
    connect(SIGNAL(light,SetGreen),SLOT(cars,RunCars));
    cout<<endl<<"After using Connect:"<<endl;
    light.SetRed();
    light.SetYellow();
    light.SetGreen();
    disconnect(SIGNAL(light,SetRed),SLOT(cars,StopCars)); // disconnect signal :light.SetRed()  to  slot: cars.StopCars()
    //disConnect( SIGNAL(Signal Obj,Signal),SLOT(Slot Obj,Slot) )
    disconnect(SIGNAL(light,SetYellow),SLOT(cars,SlowerCars));
    disconnect(SIGNAL(light,SetGreen),SLOT(cars,RunCars));
    cout<<endl<<"After using DisConnect:"<<endl;
    light.SetRed();
    light.SetYellow();
    light.SetGreen();

    connect(SIGNAL(light,SetGreen),SLOT(cars,RunCars)); // connect again
    cout<<endl<<"After using Connect again:"<<endl;
    light.SetRed();
    light.SetYellow();
    light.SetGreen();

    return 0;
}
