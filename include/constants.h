const int maxkftracks  = 1000;
const int maxrectracks = 1000;
const int maxahdcadc   = 1000;

const double beam_energy = 2.240; // Run 22992
const double mass_target = 0.938; // Q2~0.1GeV2 has nucleon mass debroglie wavelength

const double delta_phi_min = 170.; // between electron and KF track
const double theta_min = 5. * TMath::DegToRad(); // clas12 acceptance
const double w_max = 1.;

const int nwires              = 100;
const int nlayers_adc         = 8;
const int layers[nlayers_adc] = {11, 21, 22, 31, 32, 41, 42, 51};

