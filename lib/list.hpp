#ifndef __PEREGRINE__LIST__
#define __PEREGRINE__LIST__
//TODO: Use peregrine exception instead of c++ throw
#include <cstddef>
#include <cstdint>
#include <iostream>
namespace Peregrine{
template<typename T>
class list{
    T* m_data;
    size_t m_size=0;
    size_t m_capacity=0;
    size_t m_iter_index=0;
    public:
    list(){
        m_data=nullptr;
    }
    list(size_t size){
        m_data=new T[size];
        this->m_capacity=size;
    }
    list(const list<T>& other){
        m_data=new T[other.m_size];
        this->m_size=other.m_size;
        this->m_capacity=other.m_capacity;
        for(size_t i=0;i<m_size;i++){
            m_data[i]=other.m_data[i];
        }
    }
    list(list<T>&& other){
        m_data=other.m_data;
        other.m_data=nullptr;
        this->m_size=other.m_size;
        this->m_capacity=other.m_capacity;
    }
    ~list(){
        delete[] m_data;
    }
    list<T>& operator=(const list<T>& other){
        if(this!=&other){
            delete[] m_data;
            m_data=new T[other.m_size];
            this->m_size=other.m_size;
            this->m_capacity=other.m_capacity;
            for(size_t i=0;i<m_size;i++){
                m_data[i]=other.m_data[i];
            }
        }
        return *this;
    }
    list<T>& operator=(list<T>&& other){
        if(this!=&other){
            delete[] m_data;
            m_data=new T[other.m_size];
            this->m_size=other.m_size;
            this->m_capacity=other.m_capacity;
            for(size_t i=0;i<m_size;i++){
                m_data[i]=other.m_data[i];
            }
            delete[] other.m_data;
        }
        return *this;
    }
    T& ____mem____P____P______getitem__(int64_t index){
        if(index<0){
            index+=(int64_t)m_size;
        }
        if(index<0||index>=m_capacity){
            throw std::out_of_range("index out of range");
        }
        if(index>=m_size){
            m_size=index+1;
        }
        return m_data[index];
    }
    const T& ____mem____P____P______getitem__(int64_t index)const{
        if(index<0){
            index+=(int64_t)m_size;
        }
        if(index<0||index>=m_capacity){
            throw std::out_of_range("index out of range");
        }
        return m_data[index];
    }
    //TODO: list[1:5]
    size_t ____mem____P____P______len__()const{
        return m_size;
    }
    size_t ____mem____P____P______iter__(){
        m_iter_index=0;//reseting it to 0.Dont remove this line
        return m_size;
    }
    T ____mem____P____P______iterate__(){
        m_iter_index++;
        return m_data[m_iter_index-1];
    }
    list<T>& ____mem____P____P______enter__(){
        return *this;
    }
    void ____mem____P____P______end__(){}
    //TODO: __reverse__
    void extend(const list<T>& other){
        if(m_capacity<=(m_size+other.m_size)){
            m_capacity+=other.m_capacity;
            T* new_data=new T[m_capacity];
            for(size_t i=0;i<m_size;i++){
                new_data[i]=m_data[i];
            }
            delete[] m_data;
            m_data=new_data;
        }
        for (size_t i = 0; i < other.m_size; i++)
        {
            m_size++;
            m_data[m_size-1]=other.m_data[i];
        }
        
    }
    void append(T value){
        if(m_size==m_capacity){
            if(m_capacity==0){
                m_capacity=1;
            }
            else{
                m_capacity*=2;
            }
            T* new_data=new T[m_capacity];
            for(size_t i=0;i<m_size;i++){
                new_data[i]=m_data[i];
            }
            delete[] m_data;
            m_data=new_data;
        }
        m_size++;
        m_data[m_size-1]=value;
    }
    void clear(){
        m_size=0;
        delete[] m_data;
        m_data=nullptr;
        m_capacity=0;
    }
};
}
#endif