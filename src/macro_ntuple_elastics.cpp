#include "../include/constants.h"

void macro_ntuple_elastics()
{        
        TFile* fin  = new TFile("../input-files/022992.root");
        TFile* fout = new TFile("../output-files/ntuple_elastics_D2.root","RECREATE");
        gROOT->cd();

        TNtuple* ntuple_elastics = new TNtuple("ntuple_elastics","","ahdc_component:ahdc_order:ahdc_maxADC:ahdc_leadingEdgeTime:ahdc_timeOverThreshold:kftrackdedx:kftrackpath:electron_energy:electron_pt:electron_theta:Q2:x:W:is_elastic:delta_phi");
        TNtuple* ntuple_electron = new TNtuple("ntuple_electron","","electron_energy:electron_pt:electron_theta:Q2:x:W:is_elastic");

        TTree* clas12 = (TTree*) fin->Get("clas12");

        float kftrackx[maxkftracks];
        float kftracky[maxkftracks];
        float kftrackz[maxkftracks];
        float kftrackpx[maxkftracks];
        float kftrackpy[maxkftracks];
        float kftrackpz[maxkftracks];
        float kftrackpath[maxkftracks];
        float kftrackdedx[maxkftracks];

        int   ahdc_sector[maxahdcadc];
	int   ahdc_component[maxahdcadc];
	int   ahdc_order[maxahdcadc];
	int   ahdc_maxADC[maxahdcadc];
	float ahdc_leadingEdgeTime[maxahdcadc];
	float ahdc_timeOverThreshold[maxahdcadc];

        float rectrackx[maxrectracks];
        float rectracky[maxrectracks];
        float rectrackz[maxrectracks];
        float rectrackpx[maxrectracks];
        float rectrackpy[maxrectracks];
        float rectrackpz[maxrectracks];
        int   rectrackpid[maxrectracks];
        
        int nkftracks;
        int nrectracks;
        int nahdcrows;

        clas12->SetBranchAddress("ahdc_nkftracks",  &nkftracks);
        clas12->SetBranchAddress("ahdc_kftrackx",   kftrackx);
        clas12->SetBranchAddress("ahdc_kftracky",   kftracky);
        clas12->SetBranchAddress("ahdc_kftrackz",   kftrackz);
        clas12->SetBranchAddress("ahdc_kftrackpx",  kftrackpx);
        clas12->SetBranchAddress("ahdc_kftrackpy",  kftrackpy);
        clas12->SetBranchAddress("ahdc_kftrackpz",  kftrackpz);
        clas12->SetBranchAddress("ahdc_kftrackpath",kftrackpath);
        clas12->SetBranchAddress("ahdc_kftrackdedx",kftrackdedx);

        clas12->SetBranchAddress("ahdc_nadcrows",          &nahdcrows);
	clas12->SetBranchAddress("ahdc_sector",            ahdc_sector);
	clas12->SetBranchAddress("ahdc_component",         ahdc_component);
	clas12->SetBranchAddress("ahdc_order",             ahdc_order);
	clas12->SetBranchAddress("ahdc_maxADC",            ahdc_maxADC);
	clas12->SetBranchAddress("ahdc_leadingEdgeTime",   ahdc_leadingEdgeTime);
	clas12->SetBranchAddress("ahdc_timeOverThreshold", ahdc_timeOverThreshold);

        clas12->SetBranchAddress("nrectracks",   &nrectracks);
        clas12->SetBranchAddress("rec_track_x",  rectrackx);
        clas12->SetBranchAddress("rec_track_y",  rectracky);
        clas12->SetBranchAddress("rec_track_z",  rectrackz);
        clas12->SetBranchAddress("rec_track_px", rectrackpx);
        clas12->SetBranchAddress("rec_track_py", rectrackpy);
        clas12->SetBranchAddress("rec_track_pz", rectrackpz);
        clas12->SetBranchAddress("rec_track_pid",rectrackpid);

        TVector3* p_electron = new TVector3(-999,-999,-999);
        TVector3* p_kftrack  = new TVector3(-999,-999,-999);
                
        for (int i = 0 ; i < clas12->GetEntries() ; i++) {
                clas12->GetEntry(i);

                for (int i = 0 ; i < nrectracks ; i++)
                        if (rectrackpid[i] == 11)
                                p_electron->SetXYZ(rectrackpx[i], rectrackpy[i], rectrackpz[i]);

                if (p_electron->X() == -999)
                        continue;
                
                // Elastic condition
                int is_elastic = 0;

                for (int j = 0 ; j < nkftracks ; j++) {
                        p_kftrack->SetXYZ(kftrackpx[j], kftrackpy[j], kftrackpz[j]);

                        if (std::isnan(kftrackpx[j]))
                                continue;

                        if (p_kftrack->Theta() < theta_min)
                                continue;

                        if (std::abs(TMath::RadToDeg() * p_electron->DeltaPhi(*p_kftrack)) > 170.)
                                is_elastic++;
                }

                if (is_elastic == 0)
                        continue;

                double electron_theta = p_electron->Theta();

                if (electron_theta < theta_min)
                        continue;

                double electron_energy_final = p_electron->Mag();
                
                double Q2 = 4. * beam_energy * electron_energy_final * std::pow(std::sin(electron_theta/2.), 2);

                double x = Q2 / 2. / (mass_target * (beam_energy - electron_energy_final));

                double W = std::sqrt(Q2 * (1. - x) / x + std::pow(mass_target, 2));

                for (int j = 0 ; j < nkftracks ; j++) {
                        p_kftrack->SetXYZ(kftrackpx[j], kftrackpy[j], kftrackpz[j]);

                        if (std::isnan(kftrackpx[j]))
                                continue;

                        if (p_kftrack->Theta() < theta_min)
                                continue;
                        
                        ntuple_elastics->Fill(ahdc_component[j], ahdc_order[j], ahdc_maxADC[j], ahdc_leadingEdgeTime[j], ahdc_timeOverThreshold[j], kftrackdedx[j], kftrackpath[j],
                                              electron_energy_final, p_electron->Pt(), electron_theta * TMath::RadToDeg(), Q2, x, W, is_elastic, p_electron->DeltaPhi(*p_kftrack) * TMath::RadToDeg());
                }

                ntuple_electron->Fill(electron_energy_final, p_electron->Pt(), electron_theta, Q2, x, W, is_elastic);

                p_electron->SetXYZ(-999,-999,-999);
                p_kftrack->SetXYZ(-999,-999,-999);
        }

        fout->cd();
        ntuple_elastics->Write();
        ntuple_electron->Write();
        gROOT->cd();
}