#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
#include <Units.h>

using namespace std;

#include <jansson.h>  // for Json

int main(int argc, char **argv)
{

  char *filenameEVENT = NULL;
  char *filenameEDP = NULL;
  bool getRV = false;

  int arg = 1;
  while (arg < argc) {
      if (strcmp(argv[arg], "--filenameEVENT") ==0) {
	arg++;
	filenameEVENT = argv[arg];
      }
      else if (strcmp(argv[arg], "--filenameEDP") ==0) {
	arg++;
	filenameEDP = argv[arg];
      }
      else if (strcmp(argv[arg], "--getRV") ==0) {
	getRV = true;
      }
      
      arg++;
    }

    //
    // if not all args present, exit with error
    //

    if (filenameEVENT == 0 || filenameEDP == 0) {
      std::cerr << "ERROR - missing input args\n";
      exit(-1);
    }

    int index;
    json_t *event;

    json_t *rootEVENT = json_load_file(filenameEVENT, 0, &error);
    json_t *rootEDP = json_load_file(filenameEDP, 0, &error);

    json_t *events = json_object_get(rootEVENT,"Events");  
    json_t *edps = json_object_get(rootEDP,"EngineeringDemandParameters");  

    int numEvents = json_array_size(events);
    std::cerr << "numEvents: " << numEvents << "\n";
    int numEDPs = json_array_size(edps);
    std::cerr << "numEDPs: " << numEDPs << "\n";

    for (int i=0; i<numEvents; i++) {
      
      json_t *event = json_array_get(events,i);
      
      json_error_t error;
      int index;
      json_t *value;
      
      // check earthquake
      json_t *type = json_object_get(event,"type");  
      const char *eventType = json_string_value(type);
      
      if (strcmp(eventType,"Seismic") != 0) {
	json_object_clear(rootEVENT);
	printf("WARNING event type %s not Seismic NO OUTPUT", eventType);
	
      } else {
	
	double unitConversion = 1.0;
	
	// get unitConevrsion factor
	json_t* evtUnitsJson = json_object_get(event, "units");
	<Units::UnitSystem eventUnits;
	
	if(evtUnitsJson != NULL) {
	  json_t* evtLengthJson = json_object_get(evtUnitsJson, "length");
	  if(NULL != evtLengthJson)
	    eventUnits.lengthUnit = Units::ParseLengthUnit(json_string_value(evtLengthJson));
	  
	  json_t* evtTimeJson = json_object_get(evtUnitsJson, "time");
	  if(NULL != evtTimeJson)
	    eventUnits.timeUnit = Units::ParseTimeUnit(json_string_value(evtTimeJson));
	  
	  unitConversionFactor = Units::GetAccelerationFactor(eventUnits, bimUnits);
	} else {
	  std::cerr << "Warning! Event file has no units!, assuming acceleration and units in g units" << std::endl;
	  eventUnits.lengthUnit = Units::LengthUnit::Meter;
	  eventUnits.timeUnit = Units::TimeUnit::Second;
	  
	  unitConversionFactor = 9.81 * Units::GetAccelerationFactor(eventUnits, bimUnits);
	}
	
	//
	// loop over patterns getting dof dirn & pattern name
	//   then find timeSeries and determin max PGA for the pattern
	//
	
	int numDOF = 0;
	json_t *theDOFs = json_array();
	int *tDOFs = 0;
	json_t *patternArray = json_object_get(event,"pattern");
	json_t *timeSeriesArray = json_object_get(event,"timeSeries");
	int numSeries = json_array_size(timeSeriesArray);
	int numPattern = json_array_size(patternArray);
	  
	tDOFs = new int[numPattern];
	double maxPGAs = new double[numPattern];
	
	for (int i=0; i<numPattern; i++) {
	  // get array dof and time series name
	  json_t *thePattern = json_array_get(patternArray, ii);
	  json_t *theDof = json_object_get(thePattern, "dof");
	  json_t *theSeries = json_object_get(thePattern, "timeSeries");
	  tDOFs[ii] = json_integer_value(theDof);
	  if (theDof != 0) {
	    json_array_append(theDOFs, theDof);
	    numDOF++;
	  } else {
	    
	  }

	  double PGA = 0.0;
	  
	  char *timeSeriesName = json_string_value(theSeries);

	  // loop over time series & find one wmatching name, loop over data & get Value
	  for (int i=0; i<numSeries; i++) {
	    json_t *theSeries = json_array_get(timeSeriesArray, i);
	    json_t *theName = json_object_get(theSeries, "name");	    
	    
	    if (strcmp(timeSeriesName, json_string_value(theName)) == 0) {
	      i = numSeries;
	      const char *subType = json_string_value(json_object_get(timeSeries,"type"));
	      std::cerr << "subType: " << subType << "\n";
	      if (strcmp(subType,"Value")  == 0) {
		
		double seriesFactor = 1.0;
		json_t *seriesFactorObj = json_object_get(timeSeries,"factor");
		if (seriesFactorObj != NULL) {
		  if (json_is_real(seriesFactorObj))
		    seriesFactor = json_number_value(seriesFactorObj);
		}
		
		double dt = json_number_value(json_object_get(timeSeries,"dT"));
		json_t *data = json_object_get(timeSeries,"data");
		
		json_t *dataV;
		int dataIndex;
		json_array_foreach(data, dataIndex, dataV) {
		  double accel = json_number_value(dataV) * unitConversionFactor * eventFactor << " " ;
		  double absAccel = fabs(accel);
		  if (absAccel > PGA)
		    PGA = absAccel;
		}
	      }
	    }
	  }

	  maxPGA[ii] = PGA;
	}
	// put values into EDP
	/*
	  // max ground acceleration
	  json_t *responseA = json_object();
	  json_object_set(responseA,"type",json_string("PGA"));      
	  json_object_set(responseA,"dofs",theDOFs);
	  json_t *dataArrayA = json_array(); 
	  json_object_set(responseA,"scalar_data",dataArrayA);
	  json_array_append(responsesArray,responseA);
	  numEDP += numDOF;

	  json_object_set(eventObj,"responses",responsesArray);
	
	  json_array_append(eventArray,eventObj);
	  
	  if (tDOFs != 0)
	    delete [] tDOFs;
	}
      }
	
	*/
      return 0;
      }
    }

    /*
      json_object_set(rootEDP,"total_number_edp",json_integer(numEDP));  
      json_object_set(rootEDP,"EngineeringDemandParameters",eventArray);  
      json_dump_file(rootEDP,filenameEDP,0);   
    */

}
