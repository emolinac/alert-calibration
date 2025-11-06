#define maxkftracks 100

void macro_print_deltaphitheta()
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

        TH2D* h = new TH2D("h","",100,-TMath::Pi()*TMath::RadToDeg(),TMath::Pi()*TMath::RadToDeg(),100,0,TMath::Pi()*TMath::RadToDeg());

        for (int i = 0 ; i < clas12->GetEntries() ; i++) {
                clas12->GetEntry(i);

                TVector3* p1 = new TVector3(kftrackpx[0], kftrackpy[0], kftrackpz[0]);
                TVector3* p2 = new TVector3(kftrackpx[1], kftrackpy[1], kftrackpz[1]);

                if (std::isnan(kftrackpx[0]) || std::isnan(kftrackpx[1]))
                        continue;

                if (nkftracks == 1 || nkftracks > 2)
                        continue;

                if (p1->Theta() == 0 || p2->Theta() == 0)
                        continue;

                if (p1->DeltaR(*p2) == 0)
                        continue;

                h->Fill(p2->DeltaPhi(*p1)*TMath::RadToDeg(), std::abs(p1->Theta() - p2->Theta())*TMath::RadToDeg());

                delete p1;
                delete p2;
        }

        h->Draw("col");
        h->SetTitle(";#Delta#phi;#Delta#theta");
}