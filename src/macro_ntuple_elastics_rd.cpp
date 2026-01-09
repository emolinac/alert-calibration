#include "../include/constants.h"

void macro_ntuple_elastics_rd()
{        
        TFile* fin  = new TFile("../../input-files/022994/022994.root");
        TFile* fout = new TFile("../output-files/ntuple_elastics_D2_022994_rdalgo.root","RECREATE");
        gROOT->cd();

        TNtuple* ntuple_elastics = new TNtuple("ntuple_elastics","","ahdc_trackid:ahdc_kftrackid:ahdc_component:ahdc_layer:ahdc_superlayer:ahdc_sumadc:ahdc_leadingEdgeTime:ahdc_timeOverThreshold:ahdc_kftrackdedx:ahdc_kftrackpath:electron_pt:electron_theta:Q2:W:delta_phi");
        TNtuple* ntuple_electron = new TNtuple("ntuple_electron","","electron_energy:electron_pt:electron_theta:Q2:x:W:is_elastic");

        TTree* clas12 = (TTree*) fin->Get("clas12");

        int   ahdc_hitid[maxahdcadc];
        int   ahdc_trackid[maxahdcadc];
	int   ahdc_wire[maxahdcadc];
	int   ahdc_layer[maxahdcadc];
	int   ahdc_superlayer[maxahdcadc];
	float ahdc_time[maxahdcadc];

        int   ahdc_kftrackid[maxkftracks];
        float ahdc_kftrackx[maxkftracks];
        float ahdc_kftracky[maxkftracks];
        float ahdc_kftrackz[maxkftracks];
        float ahdc_kftrackpx[maxkftracks];
        float ahdc_kftrackpy[maxkftracks];
        float ahdc_kftrackpz[maxkftracks];
        float ahdc_kftrackpath[maxkftracks];
        float ahdc_kftrackdedx[maxkftracks];
        int   ahdc_kftracksumadc[maxkftracks];

        int   ahdc_adc_sector[maxahdcadc];
	int   ahdc_adc_component[maxahdcadc];
        int   ahdc_adc_layer[maxahdcadc];
        int   ahdc_adc_wfType[maxahdcadc];
	int   ahdc_adc_order[maxahdcadc];
	int   ahdc_adc_sumadc[maxahdcadc];
	float ahdc_adc_leadingEdgeTime[maxahdcadc];
	float ahdc_adc_timeOverThreshold[maxahdcadc];

        float rec_track_x[maxrectracks];
        float rec_track_y[maxrectracks];
        float rec_track_z[maxrectracks];
        float rec_track_px[maxrectracks];
        float rec_track_py[maxrectracks];
        float rec_track_pz[maxrectracks];
        int   rec_track_pid[maxrectracks];
        
        int nkftracks;
        int nrectracks;
        int nahdcrows;
        int n_ahdcrecohits;

        clas12->SetBranchAddress("n_ahdcrecohits", &n_ahdcrecohits);
        clas12->SetBranchAddress("ahdc_hitid",     ahdc_hitid);
	clas12->SetBranchAddress("ahdc_trackid",   ahdc_trackid);
	clas12->SetBranchAddress("ahdc_wire",      ahdc_wire);
	clas12->SetBranchAddress("ahdc_layer",     ahdc_layer);
	clas12->SetBranchAddress("ahdc_superlayer",ahdc_superlayer);
	clas12->SetBranchAddress("ahdc_time",      ahdc_time);

        clas12->SetBranchAddress("ahdc_nkftracks",    &nkftracks);
        clas12->SetBranchAddress("ahdc_kftrack_id",    ahdc_kftrackid);
        clas12->SetBranchAddress("ahdc_kftrack_x",     ahdc_kftrackx);
        clas12->SetBranchAddress("ahdc_kftrack_y",     ahdc_kftracky);
        clas12->SetBranchAddress("ahdc_kftrack_z",     ahdc_kftrackz);
        clas12->SetBranchAddress("ahdc_kftrack_px",    ahdc_kftrackpx);
        clas12->SetBranchAddress("ahdc_kftrack_py",    ahdc_kftrackpy);
        clas12->SetBranchAddress("ahdc_kftrack_pz",    ahdc_kftrackpz);
        clas12->SetBranchAddress("ahdc_kftrack_path",  ahdc_kftrackpath);
        clas12->SetBranchAddress("ahdc_kftrack_dedx",  ahdc_kftrackdedx);
        clas12->SetBranchAddress("ahdc_kftrack_sumadc",ahdc_kftracksumadc);

        clas12->SetBranchAddress("ahdc_adc_nadcrows",          &nahdcrows);
	clas12->SetBranchAddress("ahdc_adc_sector",            ahdc_adc_sector);
	clas12->SetBranchAddress("ahdc_adc_component",         ahdc_adc_component);
	clas12->SetBranchAddress("ahdc_adc_layer",             ahdc_adc_layer);
	clas12->SetBranchAddress("ahdc_adc_wfType",            ahdc_adc_wfType);
	clas12->SetBranchAddress("ahdc_adc_order",             ahdc_adc_order);
	clas12->SetBranchAddress("ahdc_adc_sumadc",            ahdc_adc_sumadc);
	clas12->SetBranchAddress("ahdc_adc_leadingEdgeTime",   ahdc_adc_leadingEdgeTime);
	clas12->SetBranchAddress("ahdc_adc_timeOverThreshold", ahdc_adc_timeOverThreshold);

        clas12->SetBranchAddress("nrectracks",   &nrectracks);
        clas12->SetBranchAddress("rec_track_x",  rec_track_x);
        clas12->SetBranchAddress("rec_track_y",  rec_track_y);
        clas12->SetBranchAddress("rec_track_z",  rec_track_z);
        clas12->SetBranchAddress("rec_track_px", rec_track_px);
        clas12->SetBranchAddress("rec_track_py", rec_track_py);
        clas12->SetBranchAddress("rec_track_pz", rec_track_pz);
        clas12->SetBranchAddress("rec_track_pid",rec_track_pid);

        TVector3* p_electron = new TVector3(-999,-999,-999);
        TVector3* p_kftrack  = new TVector3(-999,-999,-999);
                
        for (int i = 0 ; i < clas12->GetEntries() ; i++) {
                clas12->GetEntry(i);

                for (int i = 0 ; i < nrectracks ; i++)
                        if (rec_track_pid[i] == 11)
                                p_electron->SetXYZ(rec_track_px[i], rec_track_py[i], rec_track_pz[i]);

                // Check electron
                if (p_electron->X() == -999)
                        continue;
                
                double electron_theta = p_electron->Theta();

                if (electron_theta < theta_min)
                        continue;

                double electron_energy_final = p_electron->Mag();
                
                double Q2 = 4. * beam_energy * electron_energy_final * std::pow(std::sin(electron_theta/2.), 2);

                double x = Q2 / 2. / (mass_target * (beam_energy - electron_energy_final));

                double W = std::sqrt(Q2 * (1. - x) / x + std::pow(mass_target, 2));

                for (int i = 0 ; i < nkftracks ; i++) {
                        p_kftrack->SetXYZ(ahdc_kftrackpx[i], ahdc_kftrackpy[i], ahdc_kftrackpz[i]);

                        if (std::isnan(ahdc_kftrackpx[i]))
                                continue;

                        if (p_kftrack->Theta() < theta_min)
                                continue;

                        if (std::abs(TMath::RadToDeg() * p_electron->DeltaPhi(*p_kftrack)) < 170.)
                                continue;

                        //  Check the hit list
                        for (int j = 0 ; j < n_ahdcrecohits ; j++) {
                                if (ahdc_trackid[j] == 1)
                                        continue;

                                for (int k = 0 ; k < nahdcrows ; k++) {
                                        // reco-adc connecion
                                        if (ahdc_adc_component[k] != ahdc_wire[j])
                                                continue;

                                        if (ahdc_adc_wfType[k] != 0)
                                                continue;

                                        ntuple_elastics->Fill(ahdc_trackid[j], ahdc_kftrackid[i], ahdc_wire[j], ahdc_layer[j], ahdc_superlayer[j],
                                                              ahdc_adc_sumadc[k], ahdc_adc_leadingEdgeTime[k], ahdc_adc_timeOverThreshold[k], ahdc_kftrackdedx[i], 
                                                              ahdc_kftrackpath[i], p_electron->Pt(), electron_theta * TMath::RadToDeg(), Q2, W, p_electron->DeltaPhi(*p_kftrack) * TMath::RadToDeg());
                                }
                        }
                }

                p_electron->SetXYZ(-999,-999,-999);
        }

        fout->cd();
        ntuple_elastics->Write();
        // ntuple_electron->Write();
        gROOT->cd();
}