# kenny
A basic Thompson constructor for creating NFAs.

### NFAs, Regex, and the Thompson Construction
Thompson's construction algorithm is a way of systematically converting regular expressions to non-deterministic finite automata. We, of course, are aware by Kleene's theorem that every regular expression is associated with some finite automaton, and vice versa. Thompson's construction is algorithmic proof of at least one half of Kleene's theorem. It is usually attributed to the man also credited with the popularisation of regular expressions, Ken Thompson.   
  
The algorithm works recursively, by defining simple NFAs for atomic transitions, and recursive builds for NFAs for the three major regular expression operations - concatenation, disjunction, and Kleene closure. A more thorough reading on Thompson's construction algorithm can be found [here](https://medium.com/swlh/visualizing-thompsons-construction-algorithm-for-nfas-step-by-step-f92ef378581b).  
  
### Using the Constructor
To use this, simply clone into your local directory with
```
$ git clone https://github.com/hrishitchaudhuri/kenny
```
and run
```
$ make
```
The Makefile provided will take care of the generation of object files and binary files. The binary executable will be located at `bin/nfa` and the object files will be located in the `obj` folder.   
  
To debug, use the make command
```
$ make debug
```
  
Finally, to clean, use 
```
$ make clean
```  
This will clean out your obnoxious object files, binaries, and other unnecessary things that you desperately do not need or want.

### Inputs and Outputs
Once you have the binary executable all set up in your `bin` folder, it's time to use the constructor. When run, a prompt will show up as follows:
```
[IN]: 
```
Enter your favourite regular expression into this. **Concatenation MUST be represented by '.', disjunction MUST be represented by '+', and Kleene closure MUST be represented by '\*'.**  
Thus, for example, to denote the regex ab+(a+b)\*, it must be represented as a.b+(a+b)\*. Also note that outside brackets, the operator precedence followed is **\* > + > . **.

The above regex outputs the following NFA:
```
[IN]: a.b+(a+b)*
ab.ab+*+
START STATE: 14

 0  --  a <=> 1
 1  --  e <=> 2
 2  --  b <=> 3
 3  --  e <=> 5
 4  --  e <=> 0
 5  --  e <=> 15*
 6  --  a <=> 7
 7  --  e <=> 11
 8  --  b <=> 9
 9  --  e <=> 11
 10  --  e <=> 6 e <=> 8
 11  --  e <=> 13 e <=> 10
 12  --  e <=> 10 e <=> 13
 13  --  e <=> 15*
 14  --  e <=> 4 e <=> 12
 15* --
```
The first line of the output is a postfix expression denoting the same regular expression. The second line denotes the start state of the regular expression. Each state has a maximum of two outward transitions (something guaranteed by Thompson's construction). Thus, the transitions can be read as follows: 
```
0 -- a <==> 1 : On state 0, an input 'a' transits to state 1
```
States denoted by a `*` are final states.

NOTE: Thompson's construction does not provide the absolute minimal NFA. 
