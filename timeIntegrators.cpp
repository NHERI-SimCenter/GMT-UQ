#include <iostream>
#include <vector>

int centralDifference(double mass, 
		      double damping, 
		      double stiffness, 
		      double disp_init, 
		      double vel_init, 
		      double time_step, 
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
    disp_init = initial displacement
    vel_init = initial velocity
    time_step = temporal discretization to use in computations
    force_hist = applied force history
    
    Outputs:
    Displacement time history
  */

  //Constants
  double k_hat = mass / (time_step**2) + damping / (2.0 * time_step);
  double a_coeff = mass / (time_step**2) - damping / (2.0 * time_step);
  double b_coeff = stiffness - 2.0 * mass / (time_step**2);

  /*
    # Initialize memory for outputs
    disps = np.zeros(len(force_hist) + 1)
    vels = np.zeros(len(force_hist) + 1)
    accels = np.zeros(len(force_hist) + 1)
  */
  
  //Initialize values required to warm-up time integrator
  disps[0] = disp_init;
  vels[0] = vel_init;
    
  double accel_init = (force_hist[0] - damping * vel_init - stiffness * disp_init) / mass;
  double disp_min1 = disp_init - time_step * vel_init + (time_step**2) * accel_init / 2.0;
  // Calculate 2nd time step displacement using leapfrog method (2nd order)
  disps[1] = (force_hist[0] - a_coeff * disp_min1 - b_coeff * disps[0]) / stiffness;

  //Loop over time steps to calcute SDOF response
  for (int i=1; i<force_hist.size(); i++) {
    double force = force_hist[i];
    disps[index + 1] = (force - a_coeff * disps[index - 1] - b_coeff * disps[index]) / k_hat;
    vels[index] = (disps[index + 1] - disps[index - 1]) / (2.0 * time_step);
    accels[index] = (disps[index + 1] - 2.0 * disps[index] + disps[index - 1]) / (time_step**2);
  }
}

int newmarkMethod(double mass, 
		  double damping, 
		  double stiffness, 
		  double disp_init, 
		  double vel_init, 
		  double gamma, 
		  double beta, 
		  double time_step, 
		  const vector<double> &force_hist,
		  vector<double> &disps) {

  /*
    This function calculates the displacement, velocity and acceleration time histories
    of single-degree-of-freedom linear system to the input force time history using 
    Newmark's method

    Inputs:
       mass = mass of single-degree-of-freedom system
       damping = damping coefficient for system specified as a decimal value
       stiffness = linear stiffness of the system
       disp_init = initial displacement
       vel_init = initial velocity
       gamma = Newmark velocity coefficient
       beta = Newmark acceleration coefficient
       time_step = temporal discretization to use in computations
       force_hist = applied force history

    Outputs:
       Displacement time history
       Velocity time history       
       Acceleration time history
  */
  
  /*
  // Initialize memory for outputs
  disps = np.zeros(len(force_hist) + 1);
  vels = np.zeros(len(force_hist) + 1);
  accels = np.zeros(len(force_hist) + 1);
  */

  // Set initial values
  disps[0] = disp_init;
  vels[0] = vel_init;
  accels[0] = (force_hist[0] - damping * vels[0] - stiffness * disps[0]) / mass;

  // Constants
  double k_hat = stiffness + gamma * damping / (beta * time_step) + mass / (beta * time_step * time_step);

  for (int index = 1; index<force_hist.size(); index++) {
    // Prediction step
    double force = force_hist[index];
    disps[index] = disps[index - 1];
    vels[index] = (1.0 - gamma / beta) * vels[index - 1] +		
      time_step * (1.0 - gamma / (2.0 * beta)) * accels[index - 1];
    accels[index] = (-1.0 / (beta * time_step)) * vels[index - 1] +  
      (1.0 - 1.0/ (2.0 * beta)) * accels[index - 1];

    // Correction step
    double d_disp = (force - mass * accels[index] 
		     - damping * vels[index]  
		     - stiffness * disps[index]) / k_hat;

    disps[index] = disps[index] + d_disp;
    vels[index] = vels[index] + gamma * d_disp / (beta * time_step);
    accels[index] = accels[index] + d_disp / (beta * time_step * time_step);
  }
}


int linearInterp(double natural_freq, 
		 double damping_ratio, 
		 double disp_init, 
		 double vel_init, 
		 double stiffness, 
		 double time_step, 
		 const vector<double> &force_hist,
		 vector<double> &disps,
		 vector<double> &vels,
		 vector<double> &accels) {
  /*
    """
    This function calculates the displacement and velocity time history for a single-degree-of-freedom
    linear system using piecewise linear interpolation

    Inputs:
       natural_freq = natural frequency of the system
       damping_ratio = damping ratio of the system
       disp_init = initial displacement
       vel_init = initial velocity
       time_step = time step to use for interpolation
       stiffness = linear stiffness of the system
       force_hist = applied force history

    Outputs:
       Displacement time history
       Velocity time history
  */

  double e_pow = np.exp(-damping_ratio * natural_freq * time_step);
  double damped_freq = natural_freq * sqrt(1.0 - damping_ratio*damping_ratio);
  double sin_freq = sin(damped_freq * time_step);
  double cos_freq = cos(damped_freq * time_step);
  
  // Constants
  double A = e_pow * (damping_ratio / np.sqrt(1 - damping_ratio**2) * sin_freq + cos_freq);
  double B = e_pow * sin_freq / damped_freq;
  double C = (2.0 * damping_ratio / (natural_freq * time_step) + 
	      e_pow * (((1.0 - 2.0 * damping_ratio**2) / (damped_freq * time_step) - 
			damping_ratio / np.sqrt(1.0 - damping_ratio**2)) * sin_freq - 
		       (1.0 + 2.0 * damping_ratio / (natural_freq * time_step)) * cos_freq)) / stiffness;

  double D = (1.0 - 
	      2.0 * damping_ratio / (natural_freq * time_step) + 
	      e_pow * ((2.0 * damping_ratio**2 - 1) / (damped_freq * time_step) * sin_freq + 
		       2.0 * damping_ratio / (natural_freq * time_step) * cos_freq)) / stiffness;

  double A_p = -e_pow * (natural_freq * sin_freq / np.sqrt(1.0 - damping_ratio**2));
  double B_p = e_pow * (cos_freq - damping_ratio * sin_freq / np.sqrt(1.0 - damping_ratio**2));
  double C_p = (-1.0 / time_step + e_pow * ((natural_freq / np.sqrt(1.0 - damping_ratio**2) + 
					     damping_ratio / (time_step * np.sqrt(1.0 - damping_ratio**2))) * 
					    sin_freq + cos_freq / time_step)) / stiffness;
  double D_p = (1.0 - e_pow * (damping_ratio * sin_freq / np.sqrt(1.0 - damping_ratio**2) + 
			       cos_freq)) / (stiffness * time_step);

  // Initialize memory for outputs
  //disps = np.zeros(len(force_hist))
  //vels = np.zeros(len(force_hist))
  disps[0] = disp_init;
  vels[0] = vel_init;

  for (index = 1; index<force_hist.size(); index++) {
    double force = force_hist[index];
    disps[index] = A * disps[index - 1] + B * vels[index - 1] +		
      C * force_hist[index - 1] + D * force_hist[index];
    vels[index] = A_p * disps[index - 1] + B_p * vels[index - 1] +
      C_p * force_hist[index - 1] + D_p * force_hist[index];
  }
}
