#define maxkftracks  1000
#define maxrectracks 1000

const double beam_energy = 2.240;
const double mass_target = 4.; // approx

void macro_print_theta_electron()
{
        TFile* fin = new TFile("./input-files/022992.root");
        
        TTree* clas12 = (TTree*) fin->Get("clas12");

        float kftrackx[maxkftracks];
        float kftracky[maxkftracks];
        float kftrackz[maxkftracks];
        float kftrackpx[maxkftracks];
        float kftrackpy[maxkftracks];
        float kftrackpz[maxkftracks];
        float kftrackpath[maxkftracks];
        float kftrackdedx[maxkftracks];

        float rectrackx[maxrectracks];
        float rectracky[maxrectracks];
        float rectrackz[maxrectracks];
        float rectrackpx[maxrectracks];
        float rectrackpy[maxrectracks];
        float rectrackpz[maxrectracks];
        int   rectrackpid[maxrectracks];
        
        int nkftracks;
        int nrectracks;

        clas12->SetBranchAddress("ahdc_nkftracks",  &nkftracks);
        clas12->SetBranchAddress("ahdc_kftrackx",   kftrackx);
        clas12->SetBranchAddress("ahdc_kftracky",   kftracky);
        clas12->SetBranchAddress("ahdc_kftrackz",   kftrackz);
        clas12->SetBranchAddress("ahdc_kftrackpx",  kftrackpx);
        clas12->SetBranchAddress("ahdc_kftrackpy",  kftrackpy);
        clas12->SetBranchAddress("ahdc_kftrackpz",  kftrackpz);
        clas12->SetBranchAddress("ahdc_kftrackpath",kftrackpath);
        clas12->SetBranchAddress("ahdc_kftrackdedx",kftrackdedx);

        clas12->SetBranchAddress("nrectracks",   &nrectracks);
        clas12->SetBranchAddress("rec_track_x",  rectrackx);
        clas12->SetBranchAddress("rec_track_y",  rectracky);
        clas12->SetBranchAddress("rec_track_z",  rectrackz);
        clas12->SetBranchAddress("rec_track_px", rectrackpx);
        clas12->SetBranchAddress("rec_track_py", rectrackpy);
        clas12->SetBranchAddress("rec_track_pz", rectrackpz);
        clas12->SetBranchAddress("rec_track_pid",rectrackpid);

        TH1D* h = new TH1D("h","",200,0,35);
        
        TVector3* p_electron = new TVector3(-999,-999,-999);
                
        for (int i = 0 ; i < clas12->GetEntries() ; i++) {
                clas12->GetEntry(i);

                for (int i = 0 ; i < nrectracks ; i++)
                        if (rectrackpid[i] == 11)
                                p_electron->SetXYZ(rectrackpx[i], rectrackpy[i], rectrackpz[i]);

                if (p_electron->X() == -999)
                        continue;

                double electron_theta = p_electron->Theta();

                h->Fill(electron_theta * TMath::RadToDeg());

                p_electron->SetXYZ(-999,-999,-999);
        }

        h->Draw();
        h->SetTitle(";#theta;");
}