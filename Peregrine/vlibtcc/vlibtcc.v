module vlibtcc
#flag -ltcc -ldl
#include <libtcc.h>

pub struct C.TCCState{
}


// create a new TCC compilation context //
fn C.tcc_new() &C.TCCState
// TCCState *tcc_new(void);


// free a TCC compilation context //
fn C.tcc_delete(&C.TCCState)
// void tcc_delete(TCCState *s);


// set CONFIG_TCCDIR at runtime //
fn C.tcc_set_lib_path(&C.TCCState, &char)
// void tcc_set_lib_path(TCCState *s, const char *path);


// set options as from command line (multiple supported) //
fn C.tcc_set_options(&C.TCCState ,&char)
// void tcc_set_options(TCCState *s, const char *str);


/* add include path */
fn C.tcc_add_include_path(&C.TCCState,&char) int
// int tcc_add_include_path(TCCState *s, const char *pathname);


/* add in system include path */
fn C.tcc_add_sysinclude_path(&C.TCCState , &char) int
// int tcc_add_sysinclude_path(TCCState *s, const char *pathname);


/* define preprocessor symbol 'sym'. Can put optional value */
fn C.tcc_define_symbol(&C.TCCState, &char, &char) 
// void tcc_define_symbol(TCCState *s, const char *sym, const char *value);


/* undefine preprocess symbol 'sym' */
fn C.tcc_undefine_symbol(&C.TCCState, &char)
// void tcc_undefine_symbol(TCCState *s, const char *sym);


/* add a file (C file, dll, object, library, ld script). Return -1 if error. */
fn C.tcc_add_file(&C.TCCState, &char) int
// int tcc_add_file(TCCState *s, const char *filename);


/* compile a string containing a C source. Return -1 if error. */
fn C.tcc_compile_string(&C.TCCState, &char) int
// int tcc_compile_string(TCCState *s, const char *buf);


/* set output type. MUST BE CALLED before any compilation */
fn C.tcc_set_output_type(&C.TCCState,int) int
// int tcc_set_output_type(TCCState *s, int output_type);
//1 output will be run in memory (default)
//2 executable file 
//3 dynamic library 
//4 object file
//5 only preprocess (used internally)


/* equivalent to -Lpath option */
fn C.tcc_add_library_path(&C.TCCState,&char) int
// int tcc_add_library_path(TCCState *s, const char *pathname);


/* the library name is the same as the argument of the '-l' option */
fn C.tcc_add_library(&C.TCCState,&char) int
// int tcc_add_library(TCCState *s, const char *libraryname);


/* add a symbol to the compiled program */
fn C.tcc_add_symbol(&C.TCCState,&char,voidptr) int
// int tcc_add_symbol(TCCState *s, const char *name, const void *val);


/* output an executable, library or object file. DO NOT call tcc_relocate() before. */
fn C.tcc_output_file(&C.TCCState,&char) int
// int tcc_output_file(TCCState *s, const char *filename);


/* link and run main() function and return its value. DO NOT call tcc_relocate() before. */
fn C.tcc_run(&C.TCCState,int, &&char) int
// int tcc_run(TCCState *s, int argc, char **argv);


/* do all relocations (needed before using tcc_get_symbol()) */
fn C.tcc_relocate(&C.TCCState,voidptr) int
// int tcc_relocate(TCCState *s1, void *ptr);
/* possible values for 'ptr':
   - voidptr(1) : Allocate and manage memory internally
   - none            : return required memory size for the step below
   - memory address    : copy code to memory passed by the caller
   returns -1 if error. */


/* return symbol value or NULL if not found */
fn C.tcc_get_symbol(&C.TCCState,&char) voidptr
// void *tcc_get_symbol(TCCState *s, const char *name);

pub fn tcc_new() &C.TCCState{
	return C.tcc_new()
}
pub fn tcc_delete(state &C.TCCState){
	C.tcc_delete(state)
}
pub fn tcc_set_lib_path(state &C.TCCState,path &char){
	C.tcc_set_lib_path(state,path)
}
pub fn tcc_set_options(state &C.TCCState,option &char){
	C.tcc_set_options(state,option)
}
pub fn tcc_add_include_path(state &C.TCCState,path &char) int {
	return C.tcc_add_include_path(state,path)
}
pub fn tcc_add_sysinclude_path(state &C.TCCState, pathname &char ) int{
	return C.tcc_add_sysinclude_path(state,pathname)
}
pub fn tcc_define_symbol(state &C.TCCState,sym &char,value &char){
	C.tcc_define_symbol(state,sym,value)	
}
pub fn tcc_undefine_symbol(state &C.TCCState,sym &char){
	C.tcc_undefine_symbol(state, sym)
}
pub fn tcc_add_file(state &C.TCCState, filename &char) int{
	return C.tcc_add_file(state, filename)
}
pub fn tcc_compile_string(state &C.TCCState,buffer &char) int{
	return C.tcc_compile_string(state, buffer)
}
pub fn tcc_set_output_type(state &C.TCCState,output_type int) int{
	return C.tcc_set_output_type(state,output_type)
}
pub fn tcc_add_library_path(state &C.TCCState,pathname &char) int{
	return C.tcc_add_library(state,pathname)
}
pub fn tcc_add_library( state &C.TCCState,name &char) int{
	return C.tcc_add_library(state,name)
}
pub fn tcc_add_symbol(state &C.TCCState, name &char,val voidptr) int{
	return C.tcc_add_symbol(state,name,val)
}
pub fn tcc_output_file(state &C.TCCState,name &char) int {
	return C.tcc_output_file(state,name)
}
pub fn tcc_run(state &C.TCCState,argc int,argv &&char)int{
	return C.tcc_run(state,argc,argv)
}
pub fn tcc_relocate(state &C.TCCState,pointer voidptr)  int{
	return C.tcc_relocate(state,pointer)
}
pub fn tcc_get_symbol(state &C.TCCState,name &char) voidptr{
	return C.tcc_get_symbol(state,name)
}