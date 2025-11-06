#define maxkftracks 100

void macro_print_z()
{
        TFile* fin = new TFile("./input-files/022992.root");
        // TFile* fout = new TFile("./input-files/rec_clas_022992.evio.00463.vars.root","RECREATE");

        // gROOT->cd();
        // TNtuple* ntuple = new TNtuple("ntuple","","delta_phi")
        TTree* clas12 = (TTree*) fin->Get("clas12");

        float kftrackx[maxkftracks];
        float kftracky[maxkftracks];
        float kftrackz[maxkftracks];
        float kftrackpx[maxkftracks];
        float kftrackpy[maxkftracks];
        float kftrackpz[maxkftracks];
        float kftrackpath[maxkftracks];
        float kftrackdedx[maxkftracks];

        int nkftracks;

        clas12->SetBranchAddress("kftrackx",kftrackx);
        clas12->SetBranchAddress("kftracky",kftracky);
        clas12->SetBranchAddress("kftrackz",kftrackz);
        clas12->SetBranchAddress("kftrackpx",kftrackpx);
        clas12->SetBranchAddress("kftrackpy",kftrackpy);
        clas12->SetBranchAddress("kftrackpz",kftrackpz);
        clas12->SetBranchAddress("kftrackpath",kftrackpath);
        clas12->SetBranchAddress("kftrackdedx",kftrackdedx);

        clas12->SetBranchAddress("nkftracks",&nkftracks);

        TH1D* h = new TH1D("h","",300,-1000,1000);

        for (int i = 0 ; i < clas12->GetEntries() ; i++) {
                clas12->GetEntry(i);

                if (nkftracks == 1 || nkftracks > 2)
                        continue;

                TVector3* p0 = new TVector3(kftrackpx[0], kftrackpy[0], kftrackpz[0]);
                TVector3* p1 = new TVector3(kftrackpx[1], kftrackpy[1], kftrackpz[1]);

                if (std::abs(p0->DeltaR(*p1)) == 0)
                        continue;
                
                h->Fill(kftrackz[0]-kftrackz[1]);
                // h->Fill(kftrackz[1]);

                // delete p0;
                // delete p1;
        }

        h->Draw();
        h->SetTitle(";#Delta Z;");
}