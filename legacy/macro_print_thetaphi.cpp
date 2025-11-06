#define maxkftracks 100

void macro_print_thetaphi()
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

        TH2D* h = new TH2D("h","",100,-TMath::Pi(),TMath::Pi(),100,0,TMath::Pi());

        for (int i = 0 ; i < clas12->GetEntries() ; i++) {
                clas12->GetEntry(i);

                for (int j = 0 ; j < nkftracks ; j++) {
                        TVector3* p1 = new TVector3(kftrackpx[j], kftrackpy[j], kftrackpz[j]);

                        if (std::isnan(kftrackpx[j]))
                                continue;

                        if (nkftracks == 1 || nkftracks > 2)
                                continue;

                        h->Fill(p1->Phi(), p1->Theta());

                        delete p1;
                }
        }

        h->SetTitle(";#phi;#theta");

        h->Draw("col");
}