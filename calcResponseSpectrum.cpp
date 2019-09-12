#include <vector>
#include <iostream>
#include <QDebug>

#include <QVector>
#include <timeIntegrators.h>

#define PI 3.14159
/*****
int CalcResponseSpectrum(std::vector<double> &periods,
			 double dampingRatio, 
			 const char *integrator, 
			 const std::vector<double> &groundMotion, 
			 double dT,
             std::vector<double> &dispResponse,
             std::vector<double> &accelResponse) {
  
  //
  // resize some vectors for output
  //

  int numPeriods = periods.size();
  dispResponse.resize(numPeriods,0);
  accelResponse.resize(numPeriods,0);
  std::vector<double> disp;
  disp.resize(groundMotion.size(),0);

  //
  // loop over all periods in period range
  //

  for (auto& period : periods) {

    //
    // Calc natural frequency, stiffness and damping based on period and damping ratio
    //

    double natural_freq = 2.0 * PI / period;
    double mass = 1.0;
    double stiffness = mass * natural_freq*natural_freq;
    double damping = 2.0 * mass * natural_freq * dampingRatio;

    //
    // obtain result of numerical integration
    //

    double dispMax = 0.;
    if (strcmp(integrator, "CentralDifference") == 0) {
      dispMax = CentralDifference(mass, damping, stiffness, 0.0, 0.0, dT, groundMotion, disp);
    } else if (strcmp(integrator, "NewmarkAverageAccel") == 0) {
      dispMax = Newmark(mass, damping, stiffness, 0.0, 0.0, 0.5, 0.25, dT, groundMotion, disp);
    } else if (strcmp(integrator, "NewmarkLinearAccel") == 0) {
      dispMax = Newmark(mass, damping, stiffness, 0.0, 0.0,0.5, 1.0 / 6.0, dT, groundMotion, disp);
    } else if (strcmp(integrator, "LinearInterpolation") == 0) {
      dispMax = LinearInterpolation(natural_freq, dampingRatio, 0.0, 0.0, stiffness, dT, groundMotion, disp);
    } else {
      std::cerr << "Specified time integrator: " << integrator << " does not exist, please check input\n";
      return -1;
    }

    //
    // store results
    //

    dispResponse.push_back(dispMax);
    accelResponse.push_back(dispMax * natural_freq* natural_freq / 9.81);
  }
  return 0;
}
****/

int CalcResponseSpectrum(const QVector<double> &periods,
                         double dampingRatio,
                         const char *integrator,
                         const std::vector<double> &groundMotion,
                         double dT,
                         QVector<double> &dispResponse,
                         QVector<double> &accelResponse) {

  //
  // resize some vectors for output
  //

  int numPeriods = periods.size();
  dispResponse.resize(numPeriods);
  accelResponse.resize(numPeriods);
  std::vector<double> disp;
  disp.resize(groundMotion.size(),0);

  //
  // loop over all periods in period range
  //

  int count = 0;

  for (auto& period : periods) {

    //
    // Calc natural frequency, stiffness and damping based on period and damping ratio
    //

    double natural_freq = 2.0 * PI / period;
    double mass = 1.0;
    double stiffness = mass * natural_freq*natural_freq;
    double damping = 2.0 * mass * natural_freq * dampingRatio;

    //
    // obtain result of numerical integration
    //

    double dispMax = 0.;
    if (strcmp(integrator, "CentralDifference") == 0) {
      dispMax = CentralDifference(mass, damping, stiffness, 0.0, 0.0, dT, groundMotion, disp);
    } else if (strcmp(integrator, "NewmarkAverageAccel") == 0) {
      dispMax = Newmark(mass, damping, stiffness, 0.0, 0.0, 0.5, 0.25, dT, groundMotion, disp);
    } else if (strcmp(integrator, "NewmarkLinearAccel") == 0) {
      dispMax = Newmark(mass, damping, stiffness, 0.0, 0.0,0.5, 1.0 / 6.0, dT, groundMotion, disp);
    } else if (strcmp(integrator, "LinearInterpolation") == 0) {
      dispMax = LinearInterpolation(natural_freq, dampingRatio, 0.0, 0.0, stiffness, dT, groundMotion, disp);
    } else {
      std::cerr << "Specified time integrator: " << integrator << " does not exist, please check input\n";
      return -1;
    }

    //
    // store results
    //
    dispResponse[count] = dispMax;
    //dispResponse.push_back(dispMax);
    accelResponse.push_back(dispMax * natural_freq* natural_freq / 9.81);
    count++;
  }
  return 0;
}





