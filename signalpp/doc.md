\mainpage Signal++ document

<h2> Quick start </h2> <br>
Welcome to [signal++](https://github.com/amir9480/signalpp) v2.<br>

Signal++ is an easy to use , header-only and open source signal/slot library for c++.<br>

<b> How to install signal++? </b> <br>
   It's easy. just include ```signalpp.h``` in you'r code.<br>
   \code{.cpp}
    #include <signalpp.h>
   \endcode

When you connect a signal(e.g function a) to a slot(e.g function b) and using emit at end of you'r function a . 
you tell to signal++ when i called a , then you call b for me.<br>
you can have many slots for a signal . if you connect 20 slots to signal a. when you called a . all of 20 slots will called.

<br> Let's start signal++ with samples. <br>
Connect a signal function to slot function: <br>
\code{.cpp}
#include <iostream>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

void a()
{
    cout<<"signal a called!"<<endl;
}

void b()
{
    cout<<"slot b called!"<<endl;
}

int main()
{
    connect(SIGNALFUNC(a),SLOTFUNC(b));// connect signal a() to b()
    return 0;
}
\endcode<br>
```connect(...)``` : using to connect a signal to a slot (and parameter is function name)<br>
```SIGNALFUNC(...)``` : using to specify signal function <br>
```SLOTFUNC(...)``` : using to specify slot function <br>

and while program is running signal ```a()``` is connected to slot ```b()```<br>
<br>
Connect a signal function to slot member function:<br>
\code{.cpp}
#include <iostream>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

void a()
{
    cout<<"signal a called!"<<endl;
}

struct B
{
slots:
    void b()
    {
        cout<<"slot b called!"<<endl;
    }
};

int main()
{
    B o1;
    connect(SIGNALFUNC(a),SLOT(o1,b));
    return 0;
}
\endcode
<br>
slots:  : using to specify slots area of struct or class ( it's same public and isn't important and using to make the code more readable! ).<br>
SLOT(...) : using to specify slot member function <br> <br>


Connect a signal member function to slot function:<br>
\code{.cpp}
#include <iostream>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

struct A
{
signals:
    void a()
    {
        cout<<"signal a called!"<<endl;
    }
};

void b()
{
    cout<<"slot b called!"<<endl;
}

int main()
{
    A o1;
    connect(SIGNAL(o1,a),SLOTFUNC(b));
    return 0;
}
\endcode
<br>
signals: : like same public and will define map of signals.(it's important that define signals once inside you'r class/struct . not more!).<br>
SIGNAL : using to specify signal member function.<br>

Connect a signal member function to slot member function:<br>
\code{.cpp}
#include <iostream>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

struct A
{
signals:
    void a()
    {
        cout<<"signal a called!"<<endl;
    }
};

struct B
{
slots:
    void b()
    {
        cout<<"slot b called!"<<endl;
    }
};

int main()
{
    A o1;
    B o2;
    connect(SIGNAL(o1,a),SLOT(o2,b));
    return 0;
}
\endcode
<br>

<b>Make signals responsible</b>:<br>
it's time to using signals and using emit
\code{.cpp}
#include <iostream>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

struct A
{
signals:
    void a()
    {
        cout<<"signal a called!"<<endl;
        emit(SIGNAL(*this,a));
    }
};

struct B
{
slots:
    void b()
    {
        cout<<"slot b called!"<<endl;
    }
};

int main()
{
    A o1;
    B o2;
    connect(SIGNAL(o1,a),SLOT(o2,b));

    //now call o1.a to see the result
    o1.a();
    return 0;
}
\endcode
emit( signal ) : this function tells signal++ that parameter signal is called<br>
in this sample we used SIGNAL ( do not forget first parameter ; set it to <b>*this</b> not <b>this</b>).<br>
and when we call that signal , slots will called.<br><br>
you can use signals/slots and emit for previous samples.

<b>How to specify signals/slot with non-void return type?!</b>
When you try to use SIGNAL|SLOT|SIGNALFUNC|SLOTFUNC with functions that doesn't have void return type
compiler gives you errors. to fix error you need to change SIGNAL|SLOT|SIGNALFUNC|SLOTFUNC to SIGNAL2|SLOT2|SIGNALFUNC2|SLOTFUNC2 
to specify custom return types. <br>
patterns:<br>
\code{.cpp}
SIGNAL2(object,returntype,functionname) 
SLOT2(object,returntype,functionname) 
SIGNALFUNC2(returntype,functionname) 
SLOTFUNC2(returntype,functionname) 
\endcode
sample:<br>
\code{.cpp}
#include <iostream>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

int a()
{
    cout<<"signal a called!"<<endl;
    emit(SIGNALFUNC2(int,a));
    return 0;
}

int b()
{
    cout<<"slot b called!"<<endl;
    return 0;
}

struct A
{
signals:
    int a()
    {
        cout<<"signal A::a called!"<<endl;
        emit(SIGNAL2(*this,int,a));
        return 0;
    }
};

struct B
{
slots:
    int b()
    {
        cout<<"slot B::b called!"<<endl;
        return 0;
    }
};

int main()
{
    A o1;
    B o2;
    connect(SIGNAL2(o1,int,a),SLOT2(o2,int,b));
    connect(SIGNALFUNC2(int,a),SLOTFUNC2(int,b));

    o1.a();
    a();
    return 0;
}
\endcode
<br>
<b>How to specify signals/slot with non-void argument types?!</b>
When you try to use SIGNAL|SLOT|SIGNALFUNC|SLOTFUNC|SIGNAL2|SLOT2|SIGNALFUNC2|SLOTFUNC2 with functions that have arguments
compiler gives you errors. to fix error you need to add function argument types after SIGNAL|SLOT|SIGNALFUNC|SLOTFUNC|SIGNAL2|SLOT2|SIGNALFUNC2|SLOTFUNC2 parameters.<br>
patterns:<br>
\code{.cpp}
SIGNAL(object,functionname,argumenttypes...)
SLOT(object,functionname,argumenttypes...)
SIGNALFUNC(functionname,argumenttypes...)
SLOTFUNC(functionname,argumenttypes...)
\endcode
<br>
sample:<br>
\code{.cpp}
#include <iostream>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

void a(double in)
{
    cout<<"signal a called!"<<endl;
    emit(SIGNALFUNC(a,double));
}

void b(int in)
{
    cout<<"slot b called!"<<endl;
}

struct A
{
signals:
    void a(int in)
    {
        cout<<"signal A::a called!"<<endl;
        emit(SIGNAL(*this,a,int));
    }
};

struct B
{
slots:
    void b(float in)
    {
        cout<<"slot B::b called!"<<endl;
    }
};

int main()
{
    A o1;
    B o2;
    connect(SIGNAL(o1,a,int),SLOT(o2,b,float));
    connect(SIGNALFUNC(a,double),SLOTFUNC(b,int));

    o1.a(10);
    a(10.0);
    return 0;
}
\endcode

\warning You can't use function with more than 10 arguments in signal++. <br>
\note above codes also works with SIGNAL2|SLOT2|SIGNALFUNC2|SLOTFUNC2.<br>


As you can see in output of above code, slots will not called.<br>
<h4>WHY?!</h4> <br>
becuase when you used emit(signal ) it's will call all slots that have not any arguments.
but in above sample slots have parameters. to send parameters to slots you need to add some values after signals parameter of emit.<br>
<b> pattern </b>:<br>
\code{.cpp}
emit( signal, parameters... )
\endcode
<b> sample </b>:<br>
\code
emit ( signal , 10 )
\endcode
<br>
in above sample after call all slots of signal parameter with (int) argument will called
.like void test(int); void T::test(int) ; int t(int) and  ...
<b> sample code</b>:<br>
\code{.cpp}
#include <iostream>
#include <string>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

void a(double in)
{
    cout<<"signal a called!"<<endl;
    emit(SIGNALFUNC(a,double),string("Test"));
}

void b(string in)
{
    cout<<"slot b called! in="<<in<<endl;
}

struct A
{
signals:
    void a(int in)
    {
        cout<<"signal A::a called!"<<endl;
        emit(SIGNAL(*this,a,int),4.12f);
    }
};

struct B
{
slots:
    void b(float in)
    {
        cout<<"slot B::b called! in="<<in<<endl;
    }
};

int main()
{
    A o1;
    B o2;
    connect(SIGNAL(o1,a,int),SLOT(o2,b,float));
    connect(SIGNALFUNC(a,double),SLOTFUNC(b,string));

    o1.a(10);
    a(10.0);
    return 0;
}
\endcode

\warning in emit(SIGNALFUNC(a,double),string("Test")); do not forget to specify that argument is string not const char* and these are diffrent in signal++. 
because of using typeid.
<br>

<h3> How to change multy threading setting?</h3>

You can disable using SIGNALPP_T in you'r code before #include <signalpp.h> 
\code{.cpp}
#define SIGNALPP_T 0

#include <iostream>
#include "signalpp.h"
#include <thread>
using namespace std;
using namespace signalpp;

struct A
{
signals:
    void a()
    {
        emit(SIGNAL(*this,a));
        emit(SIGNAL(*this,a),10);
    }
};

struct B
{
slots:
    void b()
    {
        cout<<"slot B:b called\n";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"slot B:b done\n";
    }
};

void a()
{
    emit(SIGNALFUNC(a));
    emit(SIGNALFUNC(a),10);
}

void b()
{
    cout<<"slot b called\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout<<"slot b done\n";
}

int main()
{
    A _a;
    B _b[7];
    connect(SIGNAL(_a,a),SLOTFUNC(b));
    for(int i=0;i<7;i++)
        connect(SIGNAL(_a,a),SLOT(_b[i],b));
    _a.a();
    return 0;
}
\endcode
When using #define SIGNALPP_T 0 multi threading feature will disable .
default value is 1.

also You also can change maximum threads for 1 emit.
\code{.cpp}
#define SIGNALPP_T 1
#define SIGNALPP_TMAX 32

#include <iostream>
#include "signalpp.h"
#include <thread>
using namespace std;
using namespace signalpp;

struct A
{
signals:
    void a()
    {
        emit(SIGNAL(*this,a));
        emit(SIGNAL(*this,a),10);
    }
};

struct B
{
slots:
    void b()
    {
        cout<<"slot B:b called\n";
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"slot B:b done\n";
    }
};

void a()
{
    emit(SIGNALFUNC(a));
    emit(SIGNALFUNC(a),10);
}

void b()
{
    cout<<"slot b called\n";
    this_thread::sleep_for(chrono::seconds(1));
    cout<<"slot b done\n";
}

int main()
{
    A _a;
    B _b[7];
    connect(SIGNAL(_a,a),SLOTFUNC(b));
    for(int i=0;i<7;i++)
        connect(SIGNAL(_a,a),SLOT(_b[i],b));
    _a.a();
    return 0;
}
\endcode
#define SIGNALPP_TMAX 32 will change maximum of threads for one emit to 32. default value is 2.


<h4>Other functions</h4><br>
disconnect(signal, slot): will disconnect a connected signal to slot.<br>
sample:<br>
\code {.cpp}
#include <iostream>
#include <string>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

void a()
{
    cout<<"signal a called!"<<endl;
    emit(SIGNALFUNC(a));
}

void b()
{
    cout<<"slot b called!"<<endl;
}


int main()
{
    cout<<"Before connect"<<endl;
    a();
    connect(SIGNALFUNC(a),SLOTFUNC(b));
    cout<<"\nAfter connect"<<endl;
    a();
    disconnect(SIGNALFUNC(a),SLOTFUNC(b)); // just like connect
    cout<<"\nAfter disconnect"<<endl;
    a();// slot b will not called any more
    return 0;
}
\endcode

disconnectall(signal) : will disconnect all slots of a signal.<br>
sample:<br>
\code{.cpp}
#include <iostream>
#include <string>
#include "signalpp.h"
using namespace std;
using namespace signalpp;

void a()
{
    cout<<"signal a called!"<<endl;
    emit(SIGNALFUNC(a));
}

void b()
{
    cout<<"slot b called!"<<endl;
}

void c()
{
    cout<<"slot c called!"<<endl;
}


int main()
{
    cout<<"Before connect"<<endl;
    a();
    connect(SIGNALFUNC(a),SLOTFUNC(b));
    connect(SIGNALFUNC(a),SLOTFUNC(c));
    cout<<"\nAfter connect"<<endl;
    a();
    disconnectall(SIGNALFUNC(a));
    cout<<"\nAfter disconnect"<<endl;
    a();// slot b and c will not called any more
    return 0;
}
\endcode


and that was it!!!

\note i'm sorry for my low english knowledge.