#ifndef __PEREGRINE__STR__
#define __PEREGRINE__STR__
//TODO: Use peregrine exception instead of c++ throw
#include "list.hpp"
namespace Peregrine{
template<typename T1,typename T2>
struct pair{
    T1 first;
    T2 second;
};    
//Ordered dictionary
template<typename T1,typename T2>
class dict{
    size_t m_iter_index=0;
    list<T1> m_keys;
    list<T2> m_values;
    public:
    dict(list<T1> keys,list<T2> values){
        m_keys=keys;
        m_values=values;
    }
    dict(){}
    dict<T1,T2>& operator=(const dict<T1,T2>& other){
        if(this!=&other){
            m_keys=other.m_keys;
            m_values=other.m_values
        }
        return *this;
    }
    dict<T1,T2>& operator=(dict<T1,T2>&& other){
        if(this!=&other){
            m_keys=other.m_keys;
            m_values=other.m_values;
            other.clear();
        }
        return *this;
    }
    size_t ____mem____P____P______len__()const{
        return m_keys.____mem____P____P______len__();
    }
    size_t ____mem____P____P______iter__(){
        m_iter_index=0;//reseting it to 0.Dont remove this line
        return m_keys.____mem____P____P______len__();
    }
    pair<T1,T2> ____mem____P____P______iterate__(){
        m_iter_index++;
        return pair<T1,T2>{m_keys[m_iter_index-1],m_values[m_iter_index-1]};
    }
    dict<T1,T2>& ____mem____P____P______enter__(){
        return *this;
    }
    void ____mem____P____P______end__(){}
    list<T1>& keys(){
        return m_keys;
    }
    list<T2>& values(){
        return m_values;
    }
    void clear(){
        m_values.clear();
        m_keys.clear();
    }
};
}
#endif