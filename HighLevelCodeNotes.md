# `ctemz.cpp` #
This is the highest level of the `ctemz` executable. It contains the `main()` function which parses the command-line arguments, instantiates a `BlzSim` object and calls its `run()` method. The following code excerpt summarizes what `ctemz.cpp:main()` does:

```
  BlzSim* pSim = new BlzSim();
  // Get the input parameters from the input file
  const string inputFile(pInputFile);
  BlzSimInput inp;
  BlzSimInputReader inputReader(inputFile);
  inputReader.read(inp);
  pSim->run(inp, days, bTestMode, bSingleThreadFlag, nTestOut);
  delete pSim;
```

# `BlzSim` #
This class defined in the files `blzsim.cpp` and `blzsim.h`.

The meat of `ctemz` simulation is in here. The `run()` method calls all the other modules to load the input parameters from a file (current named `temzinp.txt`), set up the simulation, execute the time loop, and output the results to a set of files.  The `run()` method is declared as such:
```
void run(BlzSimInput& blzSimInput, double ndays=20.0, bool bTestMode = false, 
         bool bSingleThreaded = false, int nTestOut = 0);
```

This method is extremely long because it is a fairly direct translation of the Fortran `temz.f` program, which is primarily one long main program with a few things broken out into separate functions/subroutines.

In the C++ program `ctemz`, a few functionalities have been moved into other classes or subroutines. For example, reading the simulation parameters from the input file `temzinp.txt` has been moved to a separate class `BlzSimInputReader`.