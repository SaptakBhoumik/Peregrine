#include "lexer.hpp"
#include <algorithm>
bool LEXER::advance(){
    if(m_curr_index<(m_input.size()-1)){
        m_curr_index++;
        m_loc++;
        m_curr_item=m_input[m_curr_index];
        return true;
    }
    else{
        return false;
    }
}

std::vector<std::string> LEXER::split_ln(std::string code) {
    std::vector<std::string> split_code;
    std::string character;
    std::string prev;
    std::string temp;
    size_t index = 0;
    while (index < code.length()) {
        character = code.at(index);
        if (character == "\n" || character == "\r") {
            //To prevent counting \r\n as 2 diffrent lines
            if(prev=="\r" && character=="\n"){}
            else{
                split_code.push_back(temp);
                temp = "";
            }
        } else if (character == "\"") {
            temp += "\\\"";
        }else{
            temp += character;
        }
        index++;
        prev=character;
    }
    split_code.push_back(temp);
    return split_code;
}

bool LEXER::is_int(const std::string s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

char LEXER::next(){
    if((m_curr_index+1)<m_input.size()){
        return m_input[m_curr_index+1];
    }
    else{
        return '\0';
    }
}
LEXEME LEXER::result(){
    return m_result;
}