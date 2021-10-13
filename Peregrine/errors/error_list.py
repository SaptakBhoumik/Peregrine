from .errors import *

class message:
    def __init__(self,code,line,col, file_name,expected=""):
        self.code = code
        self.line = line
        self.col = col
        self.file_name = file_name
        self.expected = expected
    def string(self):
        #like for example if the programmer forgets to close the " or '
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Unexpected end of file",f"Expecting a {self.expected}",self.code, hint=f"Adding a {self.expected}",)
        error.display()
    def cpp(self):
        #like for example if the programmer forgets to close the cpp function
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Unexpected end of file",f"Expecting a ) to stop the c++ code",self.code, hint=f"Adding a )",)
        error.display()
    def dictionary(self):
        #like for example if the programmer forgets to close the }
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Unexpected end of file",f"Expecting a {self.expected}",self.code, hint=f"Adding a {self.expected}",)
        error.display()
    def array(self):
        #like for example if the programmer forgets to close the ]
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Unexpected end of file",f"Expecting a {self.expected}",self.code, hint=f"Adding a {self.expected}",)
        error.display()
    def redef_variable(self):
        #a=6
        #int a=6
        #see you redefined an existing variable so you will get an error
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Redefinition an existing variable","Expecting a diffrent name",self.code, hint="Changing the name",)
        error.display()
    def redef_function(self):
        #def name():
        #def name():
        #you cant use same name to define a function
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Redefinition an existing function","Expecting a diffrent name",self.code, hint="Changing the name",)
        error.display()
    def un_defined_function(self):
        #name()
        #you cant use name function as it doesnt exist
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Unknown function","Make sure that the function you are trying to use is spelled properly",self.code, hint="",)
        error.display()
    def un_defined_variable(self):
        #name()
        #you cant use name function as it doesnt exist
        error=PEError(Location(self.line,self.col,self.file_name,self.code),"Unknown variable","Make sure that the variable you are trying to use is spelled properly",self.code, hint="",)
        error.display()