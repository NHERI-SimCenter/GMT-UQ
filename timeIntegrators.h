double CentralDifference(double mass,
                      double damping,
                      double stiffness,
                      double disp_init,
                      double vel_init,
                      double time_step,
                      const std::vector<double> &force_hist,
                      std::vector<double> &disps);

double Newmark(double mass,
            double damping,
            double stiffness,
            double disp_init,
            double vel_init,
            double gamma,
            double beta,
            double time_step,
            const std::vector<double> &force_hist,
            std::vector<double> &disps);

double LinearInterpolation(double natural_freq,
                        double damping_ratio,
                        double disp_init,
                        double vel_init,
                        double stiffness,
                        double time_step,
                        const std::vector<double> &force_hist,
                        std::vector<double> &disps);
