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
https://github.com/amir9480/signalpp
version = 2.0
Auther(s): Amir Alizadeh => (amiralizadeh9480@gmail.com) (telegram.me/amir9480)

****************************************************************************/

#ifndef SIGNALPP_H
#define SIGNALPP_H

#ifndef SIGNALPP_T
#define SIGNALPP_THREAD 1
#else
#define SIGNALPP_THREAD SIGNALPP_T
#endif // SIGNALPP_T

#ifndef SIGNALPP_TMAX
#define SIGNALPP_MAX_THREADS_FOR_ONE_EMIT 2
#else
#define SIGNALPP_MAX_THREADS_FOR_ONE_EMIT SIGNALPP_TMAX
#endif // SIGNALPP_TMAX

#ifndef __cplusplus
#error You need a c++ compiler to use signal++
#endif

#if __cplusplus < 201103L
#error You need c++11 standards or higher to use signal++
#endif

#include <tuple>
#include <string>
#include <typeinfo>
#include <list>
#include <map>
#if SIGNALPP_THREAD == 1
#include <thread>
#endif // SIGNALPP_THREAD

namespace signalpp
{
using namespace std;

#define slots public
#define signals public:std::map<void*,signalpp::_Signal> _signals;public

#define SIGNAL(O,FN,...) __SIGSLOT(O,void,FN, signalpp::RR<decltype(O)>::type ,__VA_ARGS__ )
#define SLOT(O,FN,...) __SIGSLOT(O,void,FN, signalpp::RR<decltype(O)>::type ,__VA_ARGS__ )

#define SIGNAL2(O,RT,FN,...) __SIGSLOT(O,RT,FN, signalpp::RR<decltype(O)>::type ,__VA_ARGS__ )
#define SLOT2(O,RT,FN,...) __SIGSLOT(O,RT,FN, signalpp::RR<decltype(O)>::type ,__VA_ARGS__ )

#define __SIGSLOT(O,RT,FN,TYPE,...) O,static_cast<RT( TYPE::*)(__VA_ARGS__)>(&TYPE::FN)

////////////////////////////////////////////////////////////////////////////////////

#define SIGNALFUNC(FN,...) _SIGSLOTFUNC(void,FN,__VA_ARGS__)
#define SLOTFUNC(FN,...) _SIGSLOTFUNC(void,FN,__VA_ARGS__)

#define SIGNALFUNC2(RT,FN,...) _SIGSLOTFUNC(RT,FN,__VA_ARGS__)
#define SLOTFUNC2(RT,FN,...) _SIGSLOTFUNC(RT,FN,__VA_ARGS__)

#define _SIGSLOTFUNC(RT,FN,...) static_cast<RT(*)(__VA_ARGS__)>(&FN)

////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct RR
{
    typedef T type;
};

template <typename T>
struct RR<T&>
{
    typedef T type;
};
////////////////////////////////////////////////////////////////////////////////////
#if SIGNALPP_THREAD==1
void joinThreads(list<thread*>& signalppthreads)
{
    while(signalppthreads.size()>0)
    {
        if((*signalppthreads.begin())->joinable())
            (*signalppthreads.begin())->join();
        delete (*signalppthreads.begin());
        signalppthreads.erase(signalppthreads.begin());
    }
}

void updateThreads(list<thread*>& signalppthreads)
{
    if(signalppthreads.size()==0)
        return;
    (*signalppthreads.begin())->join();
    delete (*signalppthreads.begin());
    signalppthreads.erase(signalppthreads.begin());
}
#endif // SIGNALPP_THREAD
////////////////////////////////////////////////////////////////////////////////////
template<typename T1, typename... _T2>
struct _typetostring
{
    static string getType()
    {
        string out;
        out+=typeid(T1).name();
        out+=",";
        out+=_typetostring<_T2...>::getType();
        return out;
    }
};
////////////////////////////////////////////////////////////////////////////////////
template<typename T1>
struct _typetostring<T1>
{
    static string getType()
    {
        return string(typeid(T1).name());
    }
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _A>
struct function_traits;
////////////////////////////////////////////////////////////////////////////////////
template<typename _R>
struct function_traits<_R(*)()>
{
    typedef _R _ReturnType;

    static string getType()
{
    return typeid(void).name();
}
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename... _A>
struct function_traits<_R(*)(_A...)>
{
    typedef _R _ReturnType;

    template<unsigned int i>
    struct _Arg
    {
        typedef typename tuple_element<i,tuple<_A...>>::type type;
    };

    static string getType()
{
    return _typetostring<_A...>::getType();
}
            };
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT>
struct function_traits<_R (_OT::*)()>
{
    typedef _R _ReturnType;
    typedef _OT _ObjectType;

    static string getType()
{
    return typeid(void).name();
}
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename... _A>
struct function_traits<_R (_OT::*)(_A...)>
{
    typedef _R _ReturnType;
    typedef _OT _ObjectType;
    template<unsigned int i>
    struct Arg
    {
        typedef typename tuple_element<i,tuple<_A...>>::type type;
    };

    static string getType()
{
    return _typetostring<_A...>::getType();
}
            };
////////////////////////////////////////////////////////////////////////////////////
class _Slot
{
    friend class _Signal;

    template<typename _O1,typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void connect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void connect(_R1(*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _O1,typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void connect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_R2(*_f)(_A2...) );
    template<typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void connect(_R1(*_sfa)(_A1...),_R2(*_f)(_A2...));

    template<typename _O1,typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void disconnect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void disconnect(_R1(*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _O1,typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void disconnect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_R2(*_f)(_A2...) );
    template<typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void disconnect(_R1(*_sfa)(_A1...),_R2(*_f)(_A2...));
public:
    _Slot()
    {

    }
    virtual ~_Slot()
    {

    }
    void call(string _t)
    {
        if(_t==type&&!type.empty())
            this->_call();
    }
    template<typename... _T>
    void call(string _t,_T... _args)
    {
        if(_t==type&&!type.empty())
            this->_call(&_args...);
    }

    virtual void _call()                                                             {}
    virtual void _call(void*)                                                        {}
    virtual void _call(void*,void*)                                                  {}
    virtual void _call(void*,void*,void*)                                            {}
    virtual void _call(void*,void*,void*,void*)                                      {}
    virtual void _call(void*,void*,void*,void*,void*)                                {}
    virtual void _call(void*,void*,void*,void*,void*,void*)                          {}
    virtual void _call(void*,void*,void*,void*,void*,void*,void*)                    {}
    virtual void _call(void*,void*,void*,void*,void*,void*,void*,void*)              {}
    virtual void _call(void*,void*,void*,void*,void*,void*,void*,void*,void*)        {}
    virtual void _call(void*,void*,void*,void*,void*,void*,void*,void*,void*,void*)  {}
protected:
    string type;
    void* _FA;
    void* _o;
};
////////////////////////////////////////////////////////////////////////////////////
template<typename... _T>
class __Slot:public _Slot {};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R>
class __Slot<_R(*)()>:public _Slot
{
public:
    __Slot(_R(*__f)()):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)()>::getType();
    }
    virtual void _call()override
    {
        (*_f)();
    }
    _R(*_f)();
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1>
class __Slot<_R(*)(_A1)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1)>::getType();
    }
    virtual void _call(void* _a1)override
    {
        (*_f)(*((_A1*)_a1));
    }
    _R(*_f)(_A1);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2>
class __Slot<_R(*)(_A1,_A2)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2)>::getType();
    }
    virtual void _call(void* _a1,void* _a2)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2));
    }
    _R(*_f)(_A1,_A2);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3>
class __Slot<_R(*)(_A1,_A2,_A3)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3));
    }
    _R(*_f)(_A1,_A2,_A3);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3,typename _A4>
class __Slot<_R(*)(_A1,_A2,_A3,_A4)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3,_A4)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3,_A4)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4));
    }
    _R(*_f)(_A1,_A2,_A3,_A4);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5>
class __Slot<_R(*)(_A1,_A2,_A3,_A4,_A5)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3,_A4,_A5)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3,_A4,_A5)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5));
    }
    _R(*_f)(_A1,_A2,_A3,_A4,_A5);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6>
class __Slot<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3,_A4,_A5,_A6)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6));
    }
    _R(*_f)(_A1,_A2,_A3,_A4,_A5,_A6);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7>
class __Slot<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7));
    }
    _R(*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7,typename _A8>
class __Slot<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7,void* _a8)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7),*((_A8*)_a8));
    }
    _R(*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7,typename _A8,typename _A9>
class __Slot<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7,void* _a8,void* _a9)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7),*((_A8*)_a8),*((_A9*)_a9));
    }
    _R(*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7,typename _A8,typename _A9,typename _A10>
class __Slot<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10)>:public _Slot
{
public:
    __Slot(_R(*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10)):_f(__f)
    {
        _o=0;
        _FA =(void*)__f;
        type = function_traits<_R(*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7,void* _a8,void* _a9,void* _a10)override
    {
        (*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7),*((_A8*)_a8),*((_A9*)_a9),*((_A10*)_a10));
    }
    _R(*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10);
};
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT>
class __Slot<_R(_OT::*)()>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)()):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)()>::getType();
    }
    virtual void _call()override
    {
        ((*(_OT*)(_o)).*_f)();
    }
    _R(_OT::*_f)();
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1>
class __Slot<_R(_OT::*)(_A1)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1)>::getType();
    }
    virtual void _call(void* _a1)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1));
    }
    _R(_OT::*_f)(_A1);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2>
class __Slot<_R(_OT::*)(_A1,_A2)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2)>::getType();
    }
    virtual void _call(void* _a1,void* _a2)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2));
    }
    _R(_OT::*_f)(_A1,_A2);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3>
class __Slot<_R(_OT::*)(_A1,_A2,_A3)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3));
    }
    _R(_OT::*_f)(_A1,_A2,_A3);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3,typename _A4>
class __Slot<_R(_OT::*)(_A1,_A2,_A3,_A4)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3,_A4)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3,_A4)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4));
    }
    _R(_OT::*_f)(_A1,_A2,_A3,_A4);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5>
class __Slot<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3,_A4,_A5)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5));
    }
    _R(_OT::*_f)(_A1,_A2,_A3,_A4,_A5);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6>
class __Slot<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3,_A4,_A5,_A6)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6));
    }
    _R(_OT::*_f)(_A1,_A2,_A3,_A4,_A5,_A6);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7>
class __Slot<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7));
    }
    _R(_OT::*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7,typename _A8>
class __Slot<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7,void* _a8)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7),*((_A8*)_a8));
    }
    _R(_OT::*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7,typename _A8,typename _A9>
class __Slot<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7,void* _a8,void* _a9)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7),*((_A8*)_a8),*((_A9*)_a9));
    }
    _R(_OT::*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9);
};
////////////////////////////////////////////////////////////////////////////////////
template<typename _R,typename _OT,typename _A1,typename _A2,typename _A3,typename _A4,typename _A5,typename _A6,typename _A7,typename _A8,typename _A9,typename _A10>
class __Slot<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10)>:public _Slot
{
public:
    __Slot(_OT& __o,_R(_OT::*__f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10)):_f(__f)
    {
        _o=&__o;
        _FA =(void*)__f;
        type = function_traits<_R(_OT::*)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10)>::getType();
    }
    virtual void _call(void* _a1,void* _a2,void* _a3,void* _a4,void* _a5,void* _a6,void* _a7,void* _a8,void* _a9,void* _a10)override
    {
        ((*(_OT*)(_o)).*_f)(*((_A1*)_a1),*((_A2*)_a2),*((_A3*)_a3),*((_A4*)_a4),*((_A5*)_a5),*((_A6*)_a6),*((_A7*)_a7),*((_A8*)_a8),*((_A9*)_a9),*((_A10*)_a10));
    }
    _R(_OT::*_f)(_A1,_A2,_A3,_A4,_A5,_A6,_A7,_A8,_A9,_A10);
};
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
class _Signal
{
    template<typename _R1,typename... _A1>
    friend void disconnectall(_R1(*_sfa)(_A1...) );
    template<typename _O1,typename _R1,typename... _A1>
    friend void disconnectall(_O1& _o1,_R1(_O1::*_sfa)(_A1...) );

    template<typename _O,typename _R,typename... _A>
    friend void emit(_O& _o,_R(_O::*_fa)(_A...) );
    template<typename _O,typename _R,typename... _A1,typename... _A2>
    friend void emit(_O& _o,_R(_O::*_fa)(_A1...),_A2... _as);
    template<typename _R,typename... _A>
    friend void emit(_R(*_fa)(_A...) );
    template<typename _R,typename... _A1,typename... _A2>
    friend void emit(_R(*_fa)(_A1...),_A2... _as);

    template<typename _O1,typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void connect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void connect(_R1(*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _O1,typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void connect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_R2(*_f)(_A2...) );
    template<typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void connect(_R1(*_sfa)(_A1...),_R2(*_f)(_A2...));

    template<typename _O1,typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void disconnect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
    friend void disconnect(_R1(*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...));
    template<typename _O1,typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void disconnect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_R2(*_f)(_A2...) );
    template<typename _R1,typename... _A1,typename _R2,typename... _A2>
    friend void disconnect(_R1(*_sfa)(_A1...),_R2(*_f)(_A2...));
public:
    _Signal()
    {

    }
    ~_Signal()
    {
        while(_slots.size()>0)
        {
            delete (*_slots.begin());
            _slots.erase(_slots.begin());
        }
    }

private:
    list<_Slot*> _slots;
};
map<void*,_Signal> _fsignals;
////////////////////////////////////////////////////////////////////////////////////
template<typename _O,typename _R,typename... _A>
void emit(_O& _o,_R(_O::*_fa)(_A...) )
{
#if SIGNALPP_THREAD ==0
    for(auto i=_o._signals[(void*)_fa]._slots.begin(); i!=_o._signals[(void*)_fa]._slots.end(); i++)
        (*i)->call(typeid(void).name());
#else
    list<thread*> st;
    for(auto i=_o._signals[(void*)_fa]._slots.begin(); i!=_o._signals[(void*)_fa]._slots.end(); i++)
    {
        if(st.size()>SIGNALPP_MAX_THREADS_FOR_ONE_EMIT)
            updateThreads(st);
        st.push_back(new thread(static_cast<void(_Slot::*)(string)>(&_Slot::call),(*i),typeid(void).name()));
    }
    joinThreads(st);
#endif// SIGNALPP_THREAD
}
template<typename _O,typename _R,typename... _A1,typename... _A2>
void emit(_O& _o,_R(_O::*_fa)(_A1...),_A2... _as)
{
#if SIGNALPP_THREAD ==0
    for(auto i=_o._signals[(void*)_fa]._slots.begin(); i!=_o._signals[(void*)_fa]._slots.end(); i++)
        (*i)->call(_typetostring<_A2...>::getType(),_as...);
#else
    list<thread*> st;
    for(auto i=_o._signals[(void*)_fa]._slots.begin(); i!=_o._signals[(void*)_fa]._slots.end(); i++)
    {
        if(st.size()>SIGNALPP_MAX_THREADS_FOR_ONE_EMIT)
            updateThreads(st);
        st.push_back(new thread(static_cast<void(_Slot::*)(string,_A2...)>(&_Slot::call),(*i),_typetostring<_A2...>::getType(),_as...));
    }
    joinThreads(st);
#endif // SIGNALPP_THREAD
}

template<typename _R,typename... _A>
void emit(_R(*_fa)(_A...) )
{
#if SIGNALPP_THREAD ==0
    for(auto i=_fsignals[(void*)_fa]._slots.begin(); i!=_fsignals[(void*)_fa]._slots.end(); i++)
        (*i)->call(typeid(void).name());
#else
    list<thread*> st;
    for(auto i=_fsignals[(void*)_fa]._slots.begin(); i!=_fsignals[(void*)_fa]._slots.end(); i++)
    {
        if(st.size()>SIGNALPP_MAX_THREADS_FOR_ONE_EMIT)
            updateThreads(st);
        st.push_back(new thread(static_cast<void(_Slot::*)(string)>(&_Slot::call),(*i),typeid(void).name()));
    }
    joinThreads(st);
#endif// SIGNALPP_THREAD
}
template<typename _R,typename... _A1,typename... _A2>
void emit(_R(*_fa)(_A1...),_A2... _as)
{
#if SIGNALPP_THREAD ==0
    for(auto i=_fsignals[(void*)_fa]._slots.begin(); i!=_fsignals[(void*)_fa]._slots.end(); i++)
        (*i)->call(_typetostring<_A2...>::getType(),_as...);
#else
    list<thread*> st;
    for(auto i=_fsignals[(void*)_fa]._slots.begin(); i!=_fsignals[(void*)_fa]._slots.end(); i++)
    {
        if(st.size()>SIGNALPP_MAX_THREADS_FOR_ONE_EMIT)
            updateThreads(st);
        st.push_back(new thread(static_cast<void(_Slot::*)(string,_A2...)>(&_Slot::call),(*i),_typetostring<_A2...>::getType(),_as...));
    }
    joinThreads(st);
#endif // SIGNALPP_THREAD
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _O1,typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
void connect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...))
{
    for(auto i=_o1._signals[(void*)_sfa]._slots.begin(); i!=_o1._signals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_o==&_o2 && (*i)->_FA==(void*)_f)
            return;
    _o1._signals[(void*)_sfa]._slots.push_back(new __Slot<_R2(_O2::*)(_A2...)>(_o2,_f));
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
void connect(_R1(*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...))
{
    for(auto i=_fsignals[(void*)_sfa]._slots.begin(); i!=_fsignals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_o==&_o2 && (*i)->_FA==(void*)_f)
            return;
    _fsignals[(void*)_sfa]._slots.push_back(new __Slot<_R2(_O2::*)(_A2...)>(_o2,_f));
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _O1,typename _R1,typename... _A1,typename _R2,typename... _A2>
void connect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_R2(*_f)(_A2...) )
{
    for(auto i=_o1._signals[(void*)_sfa]._slots.begin(); i!=_o1._signals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_FA==(void*)_f)
            return;
    _o1._signals[(void*)_sfa]._slots.push_back(new __Slot<_R2(*)(_A2...)>(_f));
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _R1,typename... _A1,typename _R2,typename... _A2>
void connect(_R1(*_sfa)(_A1...),_R2(*_f)(_A2...))
{
    for(auto i=_fsignals[(void*)_sfa]._slots.begin(); i!=_fsignals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_FA==(void*)_f)
            return;
    _fsignals[(void*)_sfa]._slots.push_back(new __Slot<_R2(*)(_A2...)>(_f));
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _O1,typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
void disconnect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...))
{
    for(auto i=_o1._signals[(void*)_sfa]._slots.begin(); i!=_o1._signals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_o==&_o2 && (*i)->_FA==(void*)_f)
        {
            delete (*i);
            _o1._signals[(void*)_sfa]._slots.erase(i);
            return;
        }
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _R1,typename... _A1,typename _O2,typename _R2,typename... _A2>
void disconnect(_R1(*_sfa)(_A1...),_O2& _o2,_R2(_O2::*_f)(_A2...))
{
    for(auto i=_fsignals[(void*)_sfa]._slots.begin(); i!=_fsignals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_o==&_o2 && (*i)->_FA==(void*)_f)
        {
            delete (*i);
            _fsignals[(void*)_sfa]._slots.erase(i);
            return;
        }
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _O1,typename _R1,typename... _A1,typename _R2,typename... _A2>
void disconnect(_O1& _o1,_R1(_O1::*_sfa)(_A1...),_R2(*_f)(_A2...) )
{
    for(auto i=_o1._signals[(void*)_sfa]._slots.begin(); i!=_o1._signals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_FA==(void*)_f)
        {
            delete (*i);
            _o1._signals[(void*)_sfa]._slots.erase(i);
            return;
        }
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _R1,typename... _A1,typename _R2,typename... _A2>
void disconnect(_R1(*_sfa)(_A1...),_R2(*_f)(_A2...))
{
    for(auto i=_fsignals[(void*)_sfa]._slots.begin(); i!=_fsignals[(void*)_sfa]._slots.end(); i++)
        if((*i)->_FA==(void*)_f)
        {
            delete (*i);
            _fsignals[(void*)_sfa]._slots.erase(i);
            return;
        }
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _O1,typename _R1,typename... _A1>
void disconnectall(_O1& _o1,_R1(_O1::*_sfa)(_A1...) )
{
    while(_o1._signals[(void*)_sfa]._slots.size()>0)
    {
        delete (*_o1._signals[(void*)_sfa]._slots.begin());
        _o1._signals[(void*)_sfa]._slots.erase(_o1._signals[(void*)_sfa]._slots.begin());
    }
    _o1._signals[(void*)_sfa]._slots.clear();
}
////////////////////////////////////////////////////////////////////////////////////
template<typename _R1,typename... _A1>
void disconnectall(_R1(*_sfa)(_A1...) )
{
    while(_fsignals[(void*)_sfa]._slots.size()>0)
    {
        delete (*_fsignals[(void*)_sfa]._slots.begin());
        _fsignals[(void*)_sfa]._slots.erase(_fsignals[(void*)_sfa]._slots.begin());
    }
}

}
#endif // SIGNALPP_H

