## Examples of code 

### Using only stack

http://pythontutor.com/c.html#code=%23include%20%3Cstdio.h%3E%0A%0Adouble%20multiplyByTwo%20%28double%20input%29%20%7B%0A%20%20double%20twice%20%3D%20input%20*%202.0%3B%0A%20%20return%20twice%3B%0A%7D%0A%0Aint%20main%20%28int%20argc,%20char%20*argv%5B%5D%29%0A%7B%0A%20%20int%20age%20%3D%2030%3B%0A%20%20double%20salary%20%3D%2012345.67%3B%0A%20%20double%20myList%5B3%5D%20%3D%20%7B1.2,%202.3,%203.4%7D%3B%0A%20%20printf%28%22double%20your%20salary%20is%20%25.3f%5Cn%22,%0A%20%20multiplyByTwo%28salary%29%29%3B%0A%20%20return%200%3B%0A%7D%0A&curInstr=0&mode=display&origin=opt-frontend.js&py=c&rawInputLstJSON=%5B%5D


### Using only heap

http://pythontutor.com/c.html#code=%23include%20%3Cstdio.h%3E%0A%0Adouble%20*multiplyByTwo%20%28double%20*input%29%20%7B%0A%20%20double%20*twice%20%3D%20malloc%28sizeof%28double%29%29%3B%0A%20%20*twice%20%3D%20*input%20*%202.0%3B%0A%20%20return%20twice%3B%0A%7D%0A%0Aint%20main%20%28int%20argc,%20char%20*argv%5B%5D%29%0A%7B%0A%20%20int%20*age%20%3D%20malloc%28sizeof%28int%29%29%3B%0A%20%20*age%20%3D%2030%3B%0A%20%20double%20*salary%20%3D%20malloc%28sizeof%28double%29%29%3B%0A%20%20*salary%20%3D%2012345.67%3B%0A%20%20double%20*myList%20%3D%20malloc%283%20*%20sizeof%28double%29%29%3B%0A%20%20myList%5B0%5D%20%3D%201.2%3B%0A%20%20myList%5B1%5D%20%3D%202.3%3B%0A%20%20myList%5B2%5D%20%3D%203.4%3B%0A%20%20double%20*twiceSalary%20%3D%20multiplyByTwo%28salary%29%3B%0A%20%20printf%28%22double%20your%20salary%20is%20%25.3f%5Cn%22,%20*twiceSalary%29%3B%0A%20%20free%28age%29%3B%0A%20%20free%28salary%29%3B%0A%20%20free%28myList%29%3B%0A%20%20free%28twiceSalary%29%3B%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=c&rawInputLstJSON=%5B%5D


### Using global variables, stack and heap

http://pythontutor.com/c.html#code=%23include%20%3Cstdio.h%3E%0A%0Aint%20age%20%3D%2030%3B%0Adouble%20salary%20%3D%2012345.67%3B%0A%0Adouble%20*multiplyByTwo%20%28double%20input%29%20%7B%0A%20%20double%20*twice%20%3D%20malloc%28sizeof%28double%29%29%3B%0A%20%20*twice%20%3D%20input%20*%202.0%3B%0A%20%20return%20twice%3B%0A%7D%0A%0Aint%20main%20%28int%20argc,%20char%20*argv%5B%5D%29%0A%7B%0A%20%20double%20myList%5B3%5D%20%3D%20%7B1.2,%202.3,%203.4%7D%3B%0A%20%20double%20*twiceSalary%20%3D%20multiplyByTwo%28salary%29%3B%0A%20%20printf%28%22double%20your%20salary%20is%20%25.3f%5Cn%22,%20*twiceSalary%29%3B%0A%20%20return%200%3B%0A%7D&curInstr=0&mode=display&origin=opt-frontend.js&py=c&rawInputLstJSON=%5B%5D
