#include <iostream>
#include <vector>
#include <math.h>

double CentralDifference(double mass,
			 double damping,
			 double stiffness,
			 double disp0,
			 double v0,
			 double dT,
			 const std::vector<double> &force_hist,
			 std::vector<double> &disps) {
    /*
    This function calculates the displacement, velocity and acceleration time histories
    of single-degree-of-freedom linear system to the input force time history using
    the central finite differences
    
    Inputs:
    mass = mass of single-degree-of-freedom system
    damping = damping coefficient for system specified as a decimal value
    stiffness = linear stiffness of the system
    disp0 = initial displacement
    v0 = initial velocity
    dT = temporal discretization to use in computations
    force_hist = applied force history
    
    Outputs:
    Displacement time history
  */

    // resize output & set to 0
    int numSteps = force_hist.size();
    disps.resize(numSteps,0);

    //Constants
    double dt2 = dT * dT;
    double k_hat = mass / (dt2) + damping / (2.0 * dT);
    double a_coeff = mass / (dt2) - damping / (2.0 * dT);
    double b_coeff = stiffness - 2.0 * mass / (dt2);

    /*
    # Initialize memory for outputs
    disps = np.zeros(len(force_hist) + 1)
    vels = np.zeros(len(force_hist) + 1)
    accels = np.zeros(len(force_hist) + 1)
  */

    //Initialize values required to warm-up time integrator

    disps[0] = disp0;
    double minD = disp0;
    double maxD = disp0;

    double accel_init = (force_hist[0] - damping * v0 - stiffness * disp0) / mass;
    double disp_min1 = disp0 - dT * v0 + (dt2) * accel_init / 2.0;
    // Calculate 2nd time step displacement using leapfrog method (2nd order)
    disps[1] = (force_hist[0] - a_coeff * disp_min1 - b_coeff * disps[0]) / stiffness;

    //Loop over time steps to calcute SDOF response
    for (int index=1; index<numSteps; index++) {
        double force = force_hist[index];
        double currentD  = (force - a_coeff * disps[index - 1] - b_coeff * disps[index]) / k_hat;
        disps[index + 1] = currentD;

        if (currentD < minD)
           minD = currentD;
        else if (currentD > maxD)
            maxD = currentD;

    }

    if (fabs(maxD) > fabs(minD))
      return fabs(maxD);

    return fabs(minD);
}

double Newmark(double mass,
	       double damping,
	       double stiffness,
	       double disp0,
	       double v0,
	       double gamma,
	       double beta,
	       double dT,
	       const std::vector<double> &force_hist,
	       std::vector<double> &disps) {
  
    /*
    This function calculates the displacement, velocity and acceleration time histories
    of single-degree-of-freedom linear system to the input force time history using
    Newmark's method

    Inputs:
       mass = mass of single-degree-of-freedom system
       damping = damping coefficient for system specified as a decimal value
       stiffness = linear stiffness of the system
       disp0 = initial displacement
       v0 = initial velocity
       gamma = Newmark velocity coefficient
       beta = Newmark acceleration coefficient
       dT = temporal discretization to use in computations
       force_hist = applied force history

    Outputs:
       Displacement time history
  */

    int numSteps = force_hist.size();
    disps.resize(numSteps,0);
    std::vector<double> vels(numSteps, 0);
    std::vector<double> accels(numSteps, 0);

    // Set initial values
    double minD = disp0;
    double maxD = minD;
    disps[0] = disp0;
    vels[0] = v0;
    accels[0] = (force_hist[0] - damping * vels[0] - stiffness * disps[0]) / mass;

    // Constants
    double k_hat = stiffness + gamma * damping / (beta * dT) + mass / (beta * dT * dT);

    for (int index = 1; index<numSteps; index++) {
        // Prediction step
        double force = force_hist[index];
        disps[index] = disps[index - 1];
        vels[index] = (1.0 - gamma / beta) * vels[index - 1] +
                dT * (1.0 - gamma / (2.0 * beta)) * accels[index - 1];
        accels[index] = (-1.0 / (beta * dT)) * vels[index - 1] +
                (1.0 - 1.0/ (2.0 * beta)) * accels[index - 1];

        // Correction step
        double d_disp = (force - mass * accels[index]
                         - damping * vels[index]
                         - stiffness * disps[index]) / k_hat;

        double currentD = disps[index] + d_disp;
        vels[index] = vels[index] + gamma * d_disp / (beta * dT);
        accels[index] = accels[index] + d_disp / (beta * dT * dT);

        disps[index] = currentD;

        if (currentD < minD)
           minD = currentD;
        else if (currentD > maxD)
            maxD = currentD;
    }

    if (fabs(maxD) > fabs(minD))
      return fabs(maxD);

    return fabs(minD);
}


double LinearInterpolation(double natural_freq,
			   double damping_ratio,
			   double disp0,
			   double v0,
			   double stiffness,
			   double dT,
			   const std::vector<double> &force_hist,
			   std::vector<double> &disps) {
  /*
    """
    This function calculates the displacement and velocity time history for a single-degree-of-freedom
    linear system using piecewise linear interpolation

    Inputs:
       natural_freq = natural frequency of the system
       damping_ratio = damping ratio of the system
       disp0 = initial displacement
       v0 = initial velocity
       dT = time step to use for interpolation
       stiffness = linear stiffness of the system
       force_hist = applied force history

    Outputs:
       Displacement time history
       Velocity time history
  */
    int numSteps = force_hist.size();
    disps.resize(numSteps,0);
    std::vector<double> vels(disps.size(), 0);
    std::vector<double> accels(disps.size(), 0);

    double e_pow = exp(-damping_ratio * natural_freq * dT);
    double damped_freq = natural_freq * sqrt(1.0 - damping_ratio*damping_ratio);
    double sin_freq = sin(damped_freq * dT);
    double cos_freq = cos(damped_freq * dT);

    // Constants
    double dampRatio2 = damping_ratio*damping_ratio;

    double A = e_pow * (damping_ratio / sqrt(1 - dampRatio2) * sin_freq + cos_freq);
    double B = e_pow * sin_freq / damped_freq;
    double C = (2.0 * damping_ratio / (natural_freq * dT) +
                e_pow * (((1.0 - 2.0 * dampRatio2) / (damped_freq * dT) -
                          damping_ratio / sqrt(1.0 - dampRatio2)) * sin_freq -
                         (1.0 + 2.0 * damping_ratio / (natural_freq * dT)) * cos_freq)) / stiffness;

    double D = (1.0 -
                2.0 * damping_ratio / (natural_freq * dT) +
                e_pow * ((2.0 * dampRatio2 - 1) / (damped_freq * dT) * sin_freq +
                         2.0 * damping_ratio / (natural_freq * dT) * cos_freq)) / stiffness;

    double A_p = -e_pow * (natural_freq * sin_freq / sqrt(1.0 - dampRatio2));
    double B_p = e_pow * (cos_freq - damping_ratio * sin_freq / sqrt(1.0 - dampRatio2));
    double C_p = (-1.0 / dT + e_pow * ((natural_freq / sqrt(1.0 - dampRatio2) +
                                               damping_ratio / (dT * sqrt(1.0 - dampRatio2))) *
                                              sin_freq + cos_freq / dT)) / stiffness;
    double D_p = (1.0 - e_pow * (damping_ratio * sin_freq / sqrt(1.0 - dampRatio2) +
                                 cos_freq)) / (stiffness * dT);

    // Initialize memory for outputs
    //disps = np.zeros(len(force_hist))
    //vels = np.zeros(len(force_hist))
    disps[0] = disp0;
    vels[0] = v0;
    double minD = disp0;
    double maxD = disp0;

    for (int index = 1; index<numSteps; index++) {
        double forceC = force_hist[index];
        double forceP = force_hist[index-1];
        double currentD = A * disps[index - 1] + B * vels[index - 1] +
                C * forceP + D * forceC;
        vels[index] = A_p * disps[index - 1] + B_p * vels[index - 1] +
                C_p * forceP + D_p * forceC;

        disps[index] = currentD;

        if (currentD < minD)
           minD = currentD;
        else if (currentD > maxD)
            maxD = currentD;
    }

    if (fabs(maxD) > fabs(minD))
      return fabs(maxD);

    return fabs(minD);
}
