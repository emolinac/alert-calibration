#define maxkftracks 100

void macro_print_deltar()
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

        TH1D* h = new TH1D("h","",100,0,7);

        for (int i = 0 ; i < clas12->GetEntries() ; i++) {
                clas12->GetEntry(i);

                for (int j = 0 ; j < nkftracks ; j++) {
                        TVector3* p1 = new TVector3(kftrackpx[j], kftrackpy[j], kftrackpz[j]);

                        if (std::isnan(kftrackpx[j]))
                                continue;

                        if (nkftracks == 1 || nkftracks > 2)
                                continue;

                        for (int k = j + 1 ; k < nkftracks ; k++) {
                                TVector3* p2 = new TVector3(kftrackpx[k], kftrackpy[k], kftrackpz[k]);

                                if (std::isnan(kftrackpx[k]))
                                        continue;

                                h->Fill(p2->DeltaR(*p1));

                                delete p2;
                        }

                        delete p1;
                }
        }

        h->Draw();
}