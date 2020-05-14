This is a compiler for the ParaCL language. 
Syntax and features for the first level:
1) All variable values are integers.
2) while - execute code in curly brackets as long as the condition in parentheses (the while condition) is true.
3) if - similar to while, but executed once.
4) print - print a number, variable, string, or line feed;
5) ENDL - line feed.
6) ? - read a number from the standard input stream.  

You can compile the program using the command:
```
g++ main.cpp token.cpp node.cpp block.cpp
```
You can run the program by submitting the desired file using the command:  
```
a.exe Example_programs\Prog1.pcl
```
You can output the program parsed for tokens using the third command line parameter ParFile:  
```
a.exe Example_programs\Prog1.pcl ParFile
```  
Enjoy:)