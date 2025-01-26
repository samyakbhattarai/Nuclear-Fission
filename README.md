# Nuclear-Fission
A program that simulates nuclear-fission. The following nuclear-fission is uncontrolled and can be considered the simulation of the <b>chernobyl disaster.</b>
<h2>What is it based on?</h2>
When a neutron strikes a uranium atom it splits the uranium atom and 3 neutron is generated in this process. These 3 neutrons go on to collide with other uranium atoms creating a <b>Chain Reaction.</b>
<br><br><h2>How the program works?</h2>
The program is purely written in C using the <b>SDL</b> Library for graphic implementation. The program generates a neutron and a grid of uranium. On striking the uranium a loop is initiated which generates 3 more neutrons deactivating the previous neutron and the striked uranium. The further created neutrons go on to create a uncontrolled chain reaction striking more uranium. This process continues endlessly.
On striking changes the uranium color to gray.

<h2>Running the program</h2>
You can directly run the <b>nuclear.exe</b> file to start the simulation.<br><br>Incase of any error encountered you can run the <b>Makefile</b> 
<br><b>On windows:</b>
<b>Make sure mingw32 and gcc is installed</b>

```
mingw32-make
```

In case of this not working you can compile your code manually:

```
gcc -Isrc/Include -Lsrc/lib -o nuclear nuclear.c -lmingw32 -lSDL2main -lSDL2
```
<h3>This program is not of the most efficient simulations.</h3>
