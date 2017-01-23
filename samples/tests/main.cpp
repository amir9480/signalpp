#include <iostream>
#include "../../signalpp/signalpp.h"
using namespace std;
using namespace signalpp;

void a()
{
    emit(SIGNALFUNC(a),"this is sent from signal a!");
}

void b(const char* in)
{
    cout<<in<<endl;
}

int main()
{
    connect(SIGNALFUNC(a),SLOTFUNC(b,const char*));
    a();
    return 0;
}
