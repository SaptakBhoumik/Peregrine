#include <cstdio>
#include <functional>
typedef enum{error___AssertionError,error___ZeroDivisionError} error;
void peregrine1(int*____PEREGRINE____RETURN____0=NULL,int*____PEREGRINE____RETURN____1=NULL) {
    auto peregrine2=[=](int*____PEREGRINE____RETURN____0=NULL,int*____PEREGRINE____RETURN____1=NULL)mutable->void {
    if (____PEREGRINE____RETURN____0!=NULL){
    *____PEREGRINE____RETURN____0=2;
    *____PEREGRINE____RETURN____1=2;
}
;

};
    if (____PEREGRINE____RETURN____0!=NULL){
    *____PEREGRINE____RETURN____0=1;
    *____PEREGRINE____RETURN____1=1;
}
;

};
typedef std::function<void(int*,int*)> a_type;
a_type dec(a_type x) {
    return x;

};
auto peregrine3=dec([](int*,int*)mutable->void{
    auto peregrine2=[=](int*____PEREGRINE____RETURN____0=NULL,int*____PEREGRINE____RETURN____1=NULL)mutable->void {
    if (____PEREGRINE____RETURN____0!=NULL){
    *____PEREGRINE____RETURN____0=3;
    *____PEREGRINE____RETURN____1=3;
}
;

};
    if (____PEREGRINE____RETURN____0!=NULL){
    *____PEREGRINE____RETURN____0=4;
    *____PEREGRINE____RETURN____1=4;
}
;

});
void unknown_type(auto var) {
    auto ____PEREGRINE____VALUE=var;
for (size_t ____PEREGRINE____i=0;____PEREGRINE____i<____PEREGRINE____VALUE.__iter__();++____PEREGRINE____i){
auto x=____PEREGRINE____VALUE.__iterate__();
    printf("Item is %d\n", x);

};
    {
auto CONTEXT____MANAGER____PEREGRINE____0=var;
auto y=CONTEXT____MANAGER____PEREGRINE____0.__enter__();
    printf("Y is %d\n", y);
CONTEXT____MANAGER____PEREGRINE____0.__end__();

}
;
    {
auto CONTEXT____MANAGER____PEREGRINE____0=var;
CONTEXT____MANAGER____PEREGRINE____0.__enter__();
    
//pass;
CONTEXT____MANAGER____PEREGRINE____0.__end__();

}
;
    {
auto CONTEXT____MANAGER____PEREGRINE____0=var;
auto y=CONTEXT____MANAGER____PEREGRINE____0.__enter__();
auto CONTEXT____MANAGER____PEREGRINE____1=var;
auto x=CONTEXT____MANAGER____PEREGRINE____1.__enter__();
    printf("Y is %d\n", y);
    printf("X is %d\n", x);
CONTEXT____MANAGER____PEREGRINE____0.__end__();
CONTEXT____MANAGER____PEREGRINE____1.__end__();

}
;
    printf("5 in var = %d\n", (var.__contains__(5)));
    printf("55 in var = %d\n", (var.__contains__(55)));
    printf("5 not in var = %d\n", (not var.__contains__(5)));
    printf("55 not in var = %d\n", (not var.__contains__(55)));
    printf("var[6] = %d\n", var.__getitem__(6));
    printf("var.__getitem__(7,8) = %d\n", var.__getitem__(7, 8));

};
class iterate_test
{public:
int i = 0;
int x;
iterate_test(auto c){
auto& self=*this;
    self.i = c;
    self.x = self.i;

};
 peregrine4(){
auto& self=*this;
    if (____PEREGRINE____RETURN____0!=NULL){
    *____PEREGRINE____RETURN____0=6;
    *____PEREGRINE____RETURN____1=6;
}
;

};
int __add__(auto item){
auto& self=*this;
    return 0;

};
 int operator+(auto item){
return __add__(item);
};
int __neg__(){
auto& self=*this;
    return 0;

};
 int operator-(){
return __neg__();
};
int __iter__(){
auto& self=*this;
    return (self.x - 1);

};
int __iterate__(){
auto& self=*this;
    self.i = (self.i - 1);
    return self.i;

};
int __enter__(){
auto& self=*this;
    return 6;

};
bool __contains__(int y){
auto& self=*this;
    if ((y < self.x)) {
    return true;
}
else {
    return false;
};

};
void __end__(){
auto& self=*this;
    printf("Context ended\n");

};
int __getitem__(int index1, int index2=0){
auto& self=*this;
    return (index1 + index2);

};

};
typedef enum{
colours___RED = 144,
colours___GREEN,
colours___BLUE = 59,
colours___Another = (colours___GREEN + 9)
}colours;
typedef std::function<int(int)> a;
typedef std::function<void(int)> g;
static int var_static = 8;
static void staticfunc() {
    printf("Hello from static function\n");
    printf("Static var is %d\n", var_static);

};
typedef union{
int item1;
float item2;

}name;
int divide(int num1, int num2) {
    
while (true) {
if ((num2==0)) {
    throw error___ZeroDivisionError;

}
else {
    return (num1 / num2);

}

break;
};

};
g decorator(a func) {
    int h = 9;
    auto value=[=](int c)mutable->void {
    h = 8;
    printf("%d\n", h);
    printf("Answer is %d\n", func(c));

};
    return value;

};
int ggggggg = 9;
auto dec_test=decorator([](int x)mutable->int{
    ggggggg = 7;
    printf("%d\n", ggggggg);
    auto nested_test=decorator([=](int x)mutable->int{
    ggggggg = 7;
    printf("%d\n", ggggggg);
    auto nested_test2=[=](int x)mutable->int {
    ggggggg = 7;
    printf("%d\n", ggggggg);
    return (x * x);

};
    nested_test2(7);
    return (x * x);

});
    nested_test(7);
    return (x * x);

});
static auto static_dec_test=decorator([](int x)mutable->int{
    printf("Static decorator\n");
    ggggggg = 7;
    printf("%d\n", ggggggg);
    auto nested_test=decorator([=](int x)mutable->int{
    ggggggg = 7;
    printf("%d\n", ggggggg);
    auto nested_test2=[=](int x)mutable->int {
    ggggggg = 7;
    printf("%d\n", ggggggg);
    return (x * x);

};
    nested_test2(7);
    return (x * x);

});
    nested_test(7);
    return (x * x);

});
int test(int x) {
    return x;

};
void lambda_test(a x) {
    printf("The value is %d\n", x(5));

};
int func(int g) {
    return g;

};
static inline void static_inline_func() {
    printf("Hello from static inline function\n");

};
inline void inline_func() {
    printf("Hello from inline function\n");

};
extern "C" void exported_func() {
    printf("Hello from peregrine\n");

};
typedef union{
float f;
int i;

}union_name;
int main () {
    peregrine1();
    union_name cc1;
    union_name* arrow_test = (& cc1);
    arrow_test->i = 7;
    printf("arrow_test->i=%d\n", arrow_test->i);
    exported_func();
    unknown_type(iterate_test(6));
    auto ____PEREGRINE____VALUE=iterate_test(6);
for (size_t ____PEREGRINE____i=0;____PEREGRINE____i<____PEREGRINE____VALUE.__iter__();++____PEREGRINE____i){
auto x=____PEREGRINE____VALUE.__iterate__();
    printf("Item is %d\n", x);

};
    colours enum_test = colours___RED;
    static_inline_func();
    inline_func();
    staticfunc();
    dec_test(4);
    static_dec_test(4);
    int test_var = 9;
    int* test_pointer = (& test_var);
    int test_ref = test_var;
    printf("Value of test_pointer is %d is at location %p\n", (* test_pointer), test_pointer);
    printf("Value of test_ref is %d is at location %p\n", test_ref, (& test_ref));
    auto z=[=]()mutable->int {
    printf("hello\n");
    return 5;

};
    printf("value of z() is %d\n", z());
    #ifdef _WIN32
//;
    printf("On windows\n");
    #elif __linux__
//;
    printf("On linux\n");
    #endif
//;
    {
    printf("Hello from new scope\n");

};
    bool x = false;
    const bool cc=false;
    x = true;
    bool y;
    y = true;
    if ((x == y)) {
    
//pass;
};
    printf("%d\n", test(0));
    int a = 0;
    int b = 7;
    int c = 7;
    
while (true) {
if ((a==5)&&(b==7)&&(c==8)) {
    printf("a is 5,b is 7 and c is 8");

}
else if ((a==4)&&(b==7)) {
    printf("a is 4 but b is 7");
    break;

}
else if ((a==4)&&(c==7)) {
    printf("a is 4 but c is 7");
    break;

}
else if ((a==8)) {
    printf("a is 8");

}
else {
    printf("idk");

}
    printf("\nHello\n");

break;
};
    lambda_test(func);
    if(not (8 == 8)){
printf("AssertionError : in line 197 in file ../can_comp.pe\n       assert 8==8\n");fflush(stdout);throw error___AssertionError;
};
    name var1;
    var1.item1 = 8;
    printf("item1 of var1 is %d\n", var1.item1);
    var1.item2 = 6.8;
    printf("item2 of var1 is %f\n", var1.item2);
    int cast_test = 9;
    printf("Value is %f\n", (float)(cast_test));
    printf("%d\n", divide(6, 2));
    int TernaryIf = (true)?5:6;
    if(not (TernaryIf == 5)){
printf("AssertionError : in line 209 in file ../can_comp.pe\n       assert TernaryIf==5\n");fflush(stdout);throw error___AssertionError;
};
    TernaryIf = (false)?5:6;
    if(not (TernaryIf == 6)){
printf("AssertionError : in line 211 in file ../can_comp.pe\n       assert TernaryIf==6\n");fflush(stdout);throw error___AssertionError;
};
    int r = 3;
    r--;
    if(not (r == 2)){
printf("AssertionError : in line 214 in file ../can_comp.pe\n       assert r==2\n");fflush(stdout);throw error___AssertionError;
};
    r++;
    try{
    if(not (r == 0)){
printf("AssertionError : in line 217 in file ../can_comp.pe\n           assert r==0\n");fflush(stdout);throw error___AssertionError;
};
}
catch(error __PEREGRINE__exception){
if (__PEREGRINE__exception==error___AssertionError){
    printf("Assertion error\n");
}
else{throw __PEREGRINE__exception;
}
};
    try{
    printf("%d", divide(6, 0));
}
catch(error __PEREGRINE__exception){
if (__PEREGRINE__exception==error___AssertionError){
    
//pass;
}
else{    printf("Exception caught\n");
}
};
    try{
    if(not false){
printf("AssertionError : in line 226 in file ../can_comp.pe\n       try:assert False\n");fflush(stdout);throw error___AssertionError;
};
}
catch(error __PEREGRINE__exception){
if (__PEREGRINE__exception==error___AssertionError or __PEREGRINE__exception==error___ZeroDivisionError){
auto e=__PEREGRINE__exception;
    printf("Exception caught %d\n", e);
}
else{throw __PEREGRINE__exception;
}
};
return 0;
};
