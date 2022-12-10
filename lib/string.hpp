#ifndef __PEREGRINE__STR__
#define __PEREGRINE__STR__
//TODO: Use peregrine exception instead of c++ throw
#include <cstddef>
#include <cstdint>
#include <iostream>
namespace Peregrine{
class str{
    char* m_data;
    size_t m_size=0;
    size_t m_capacity=0;
    size_t m_iter_index=0;
    public:
    str(){
        m_data=nullptr;
    }
    str(const char* string,size_t size){
        m_data=new char[size];
        for(size_t i=0;i<size;i++){
            m_data[i]=string[i];
        }
        this->m_capacity=size;
    }
    str(const char c){
        m_data=new char[1];
        m_data[0]=c;
        this->m_capacity=1;
    }
    str(const str& other){
        m_data=new char[other.m_size];
        this->m_size=other.m_size;
        this->m_capacity=other.m_capacity;
        for(size_t i=0;i<m_size;i++){
            m_data[i]=other.m_data[i];
        }
    }
    str(str&& other){
        m_data=other.m_data;
        other.m_data=nullptr;
        this->m_size=other.m_size;
        this->m_capacity=other.m_capacity;
    }
    ~str(){
        delete[] m_data;
    }
    str& operator=(const str& other){
        if(this!=&other){
            delete[] m_data;
            m_data=new char[other.m_size];
            this->m_size=other.m_size;
            this->m_capacity=other.m_capacity;
            for(size_t i=0;i<m_size;i++){
                m_data[i]=other.m_data[i];
            }
        }
        return *this;
    }
    str& operator=(str&& other){
        if(this!=&other){
            delete[] m_data;
            m_data=other.m_data;
            other.m_data=nullptr;
            this->m_size=other.m_size;
            this->m_capacity=other.m_capacity;
        }
        return *this;
    }
    char& ____mem____P____P______getitem__(int64_t index){
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
    const char& ____mem____P____P______getitem__(int64_t index)const{
        if(index<0){
            index+=(int64_t)m_size;
        }
        if(index<0||index>=m_capacity){
            throw std::out_of_range("index out of range");
        }
        return m_data[index];
    }
    //TODO: str[1:5]
    size_t ____mem____P____P______len__()const{
        return m_size;
    }
    size_t ____mem____P____P______iter__(){
        m_iter_index=0;//reseting it to 0.Dont remove this line
        return m_size;
    }
    char ____mem____P____P______iterate__(){
        m_iter_index++;
        return m_data[m_iter_index-1];
    }
    str& ____mem____P____P______enter__(){
        return *this;
    }
    void ____mem____P____P______end__(){}
    //TODO: __reverse__
    void append(const str& other){
        if(m_capacity<=(m_size+other.m_size)){
            m_capacity+=other.m_capacity;
            char* new_data=new char[m_capacity];
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
    void append(char value){
        if(m_size==m_capacity){
            if(m_capacity==0){
                m_capacity=1;
            }
            else{
                m_capacity*=2;
            }
            char* new_data=new char[m_capacity];
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