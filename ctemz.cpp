#include <cstdlib>
#include <iostream>

#include "blzlog.h"
#include "blzsim.h"
#include "blzsiminputreader.h"

using namespace std;

void setLogLevel(char* ll);
bool parseArgs(int argc, char* argv[], 
               char** pLogLevel,
               char** pDays, 
               char** pSpec, 
               char** pInputFile,
	       bool* pSingleThreadFlag,
               char** pTestOut
               );

/**
   This is the top-level function of the ctemz executable
 */
int main (int argc, char* argv[])  {
  int result = -1;

  if(argc < 1) {
    cout << "Usage: ctemz -l [loglevel] -d [daysToSimulate] -i [numTimeStepsBetweenOutputLightCurves]" << endl;
    cout << "logLevel = \"debug\", \"info\", \"warn\", or \"error\"" << endl;
    return result;
  }

  char *pLogLevel, *pDays, *pSpec, *pInputFile, *pTestOut;
  char logLevelDefault[10] = "warn";
  char inputFileDefault[128] = "temzinp.txt";
  char daysDefault[10] = ".4";
  char specDefault[10] = "1";
  char testOutDefault[10] = "0";
  bool bSingleThreadFlag = false;
  pLogLevel = logLevelDefault;
  pInputFile =  inputFileDefault;
  pDays = daysDefault;
  pSpec = specDefault;
  pTestOut = testOutDefault;
  bool bTestMode = parseArgs(argc, argv, &pLogLevel, &pDays, &pSpec, &pInputFile, &bSingleThreadFlag, &pTestOut);
  setLogLevel(pLogLevel);
  double days = atof(pDays);
  double spec = atof(pSpec); // not used yet
  int nTestOut = atof(pTestOut);

  time_t starttime;
  time(&starttime);
  cout << "% Time start: " << ctime(&starttime);
  BlzSim* pSim = new BlzSim();
  // Get the input parameters from the input file
  const string inputFile(pInputFile);
  BlzSimInput inp;
  BlzSimInputReader inputReader(inputFile);
  inputReader.read(inp);
  pSim->run(inp, days, bTestMode, bSingleThreadFlag, nTestOut);
  result = 0;
  delete pSim;

  time_t endtime;
  time(&endtime);
  cout << "% Time end: " << ctime(&endtime);
  
  return result;
}


bool parseArgs(int argc, char* argv[], 
               char** pLogLevel,
               char** pDays, 
               char** pSpec, 
               char** pInputFile,
	       bool* pSingleThreadFlag,
               char** pTestOut
               ) {
  int c;
  bool retVal = false;
  *pSingleThreadFlag = false; 

  while ((c = getopt (argc, argv, "l:d:i:tn:s")) != -1)
    switch (c) {
    case 'l':
      cout << "% Log level: " << optarg << endl;
      *pLogLevel = optarg;
      break;
    case 'd':
      cout << "% Days to simulate: " << optarg << endl;
      *pDays = optarg;
      break;
    case 'i':
      cout << "% Spectrum output interval: " << optarg << endl;
      *pSpec = optarg;
      break;
    case 'n':
      cout << "% nTestOut: " << optarg << endl;
      *pTestOut = optarg;
      break;
    case 's':
      cout << "% Single thread  mode ON " << endl;
      *pSingleThreadFlag = true;
      break;
    case 't':
      cout << "% Test mode ON " << endl;
      retVal = true;
      break;
    default:
      // do nothing -ignore unknown options
      break;
    }

  // The one non-option argument should be the input file
  for (int index = optind; index < argc; index++) {
    cout << "% Input file: " << argv[index] << endl;
    *pInputFile = argv[index];
  }

  return retVal;
}

void setLogLevel(char* ll) {
  BlzLogLevel level;
  if(strcmp(ll, "psycho")==0) {
    level = PSYCHO;
  }
  else if(strcmp(ll, "debug")==0) {
    level = DEBUG;
  }
  else if(strcmp(ll, "info")==0) {
    level = INFO;
  }
  else if(strcmp(ll, "error")==0) {
    level = ERROR;
  }
  else {
    // default to WARN level
    level = WARN;
  }
  BlzLog::setLevel(level);
  cout << "% Set logging level to: " << level << endl;
}



