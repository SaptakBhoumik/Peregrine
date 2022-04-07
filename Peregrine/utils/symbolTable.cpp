#include "symbolTable.hpp"

MangleName::MangleName(MangleName const &other){
    *this=other;
}
void MangleName::set_local(std::string original){
    m_local_names[original]="____P____P____"+original;
}
void MangleName::set_local(std::string original,std::string mangled){
    m_local_names[original]=mangled;
}
void MangleName::set_global(std::string original,std::string mangled){
    m_global_names[original]=mangled;
}
bool MangleName::contains(std::string name){
    if(m_local_names.count(name)!=0){
        return true;
    }
    else if(m_global_names.count(name)!=0){
        return true;
    }
    return false;
}
std::string MangleName::operator[](std::string name){
    if(m_local_names.count(name)!=0){
        return m_local_names[name];
    }
    else if(m_global_names.count(name)!=0){
        return m_global_names[name];
    }
    else{
        return name;
    }
}
void MangleName::print(){
    std::cout<<"Local{\n";
    for(auto const &p:m_local_names){
        std::cout<<"    "<<p.first<<":"<<p.second<<std::endl;
    }
    std::cout<<"}\nGLOBAL{\n";
    for(auto const &p:m_global_names){
        std::cout<<"    "<<p.first<<":"<<p.second<<std::endl;
    }
    std::cout<<"}";
}