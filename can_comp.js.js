function render(code){document.write(code);}error___AssertionError=0;error___ZeroDivisionError=1

//;

//;
function unknown_type(variable) {
    
//pass;

};
colours___RED = 144;
colours___GREEN = colours___RED+1;
colours___BLUE = 59;
colours___Other = (9 + colours___GREEN);
;
function divide(num1, num2) {
    
while (true) {
if ((num2===0)) {
    throw error___ZeroDivisionError;

}
else {
    return (num1 / num2);

}

break;
};

};
function decorator(func) {
    let h = 9;
    value=function(c){
    h = 8;
    console.log("%d", h);
    console.log("Answer is %d", func(c));

};
    return value;

};
let ggggggg = 9;
let dec_test=decorator(function(x){
    ggggggg = 7;
    console.log("%d", ggggggg);
    let nested_test=decorator(function(x){
    ggggggg = 7;
    console.log("%d", ggggggg);
    nested_test2=function(x){
    ggggggg = 7;
    console.log("%d", ggggggg);
    return (x * x);

};
    nested_test2(7);
    return (x * x);

});
    nested_test(7);
    return (x * x);

});
function test(x) {
    return x;

};
function lambda_test(x) {
    console.log("The value is %d", x(5));

};
function func(g) {
    return g;

};
function fun() {
    
//pass;

};
function main () {
    dec_test(4);
    z=function(){
    console.log("hello");

};
    z();
    {
    console.log("Hello from new scope");

};
    let bddd = [9,7];
    console.log(bddd);
    console.log(bddd[1]);
    let gg = {"k":154,"t":1};
    console.log(gg["k"]);
    let x = false;
    const  cc=false;
    x = true;
    let y;
    y = true;
    if ((x === y)) {
    
//pass;
};
    console.log("%d", test(0));
    let a = 0;
    let b = 7;
    let c = 7;
    
while (true) {
if ((a===5)&&(b===7)&&(c===8)) {
    console.log("a is 5,b is 7 and c is 8");

}
else if ((a===4)&&(b===7)) {
    console.log("a is 4 but b is 7");
    break;

}
else if ((a===4)&&(c===7)) {
    console.log("a is 4 but c is 7");
    break;

}
else if ((a===8)) {
    console.log("a is 8");

}
else {
    console.log("idk");

}
    console.log("Hello");

break;
};
    lambda_test(func);
    if(! (8 === 8)){
console.log("AssertionError : in line 89 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       assert 8==8");throw error___AssertionError;
};
    console.log("%d", divide(6, 2));
    let color = colours___RED;
    console.log("Enum is ", color);
    let TernaryIf = (true)?5:6;
    if(! (TernaryIf === 5)){
console.log("AssertionError : in line 98 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       assert TernaryIf==5");throw error___AssertionError;
};
    TernaryIf = (false)?5:6;
    if(! (TernaryIf === 6)){
console.log("AssertionError : in line 100 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       assert TernaryIf==6");throw error___AssertionError;
};
    let r = 3;
    r--;
    if(! (r === 2)){
console.log("AssertionError : in line 103 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       assert r==2");throw error___AssertionError;
};
    r++;
    try{
    if(! (r === 0)){
console.log("AssertionError : in line 106 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n           assert r==0");throw error___AssertionError;
};
}
catch(__P__exception){
if (__P__exception===error___AssertionError){
    console.log("Assertion error\n");
}
else{throw __P__exception;
}
};
    try{
    console.log("%d", divide(6, 0));
}
catch(__P__exception){
if (__P__exception===error___AssertionError){
    
//pass;
}
else{    console.log("Exception caught\n");
}
};
    try{
    if(! false){
console.log("AssertionError : in line 115 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       try:assert False");throw error___AssertionError;
};
}
catch(__P__exception){
if (__P__exception===error___AssertionError||__P__exception===error___ZeroDivisionError){
let e=__P__exception;
    console.log("Exception caught %d\n", e);
}
else{throw __P__exception;
}
};
    let variable_a = 5;
    let variable_b = 1;
    {let _____P____temp____0=variable_a;let _____P____temp____1=variable_b;variable_b=_____P____temp____0;variable_a=_____P____temp____1;};
    console.log("variable_a is %d and variable_b is %d\n", variable_a, variable_b);
    if(! (variable_a === 1)){
console.log("AssertionError : in line 123 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       assert variable_a==1");throw error___AssertionError;
};
    if(! (variable_b === 5)){
console.log("AssertionError : in line 124 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       assert variable_b==5");throw error___AssertionError;
};
    variable_b+=variable_a;
    if(! (variable_b === 6)){
console.log("AssertionError : in line 126 in file /home/saptak/Desktop/peregrine/can_comp.js.pe\n       assert variable_b==6");throw error___AssertionError;
};
    console.log("variable_b is %d\n", variable_b);
    
//;
    let square = function(x){return(x * x);};
    console.log("Square of 4 is %d\n", square(4));
return 0;
};

main();