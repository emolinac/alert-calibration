// Notice the use of relative path
TString input_file_name = "../output-files/histos_wire_tot_chisquare-allwires.root";
        
void macro_create_table()
{
        TFile* fin = new TFile(input_file_name.c_str());

        Float_t sector, layer, component, totCorr, dtotCorr, chi2, chi2ndf, extra;

        TNtuple* ntuple = (TNtuple*) fin->Get("ntuple_fit");
        ntuple->SetBranchAddress("sector", &sector);
        ntuple->SetBranchAddress("layer", &layer);
        ntuple->SetBranchAddress("component", &component);
        ntuple->SetBranchAddress("totCorr", &totCorr);
        ntuple->SetBranchAddress("dtotCorr", &dtotCorr);
        ntuple->SetBranchAddress("chi2", &chi2);
        ntuple->SetBranchAddress("chi2ndf", &chi2ndf);
        ntuple->SetBranchAddress("extra", &extra);

        TH1F* h = new TH1F("h", "", 100, 300, 500);

        ntuple->Project("h","totCorr","");

        double nominal_peak     = h->GetMean();
        double nominal_peak_err = h->GetMeanError();

        std::cout<<"-------------------------------------------------"<<std::endl;
        std::cout<<"Nominal peak position = "<<nominal_peak<<"+-"<<nominal_peak_err<<std::endl;
        std::cout<<"-------------------------------------------------"<<std::endl;

        std::cout<<"#& sector layer component totCorr dtotCorr chi2 chi2ndf extra3"<<std::endl;
        for (int i = 0 ; i < ntuple->GetEntries() ; i++) {
                ntuple->GetEntry(i);

                double correction     = nominal_peak / totCorr;
                double correction_err = std::sqrt(std::pow(nominal_peak * dtotCorr,2) + std::pow(totCorr * nominal_peak_err,2)) / totCorr / totCorr;
                
                std::cout<<sector<<" "<<layer<<" "<<component<<" "<<correction<<" "<<correction_err<<" "<<\
                           chi2<<" "<<chi2ndf<<" "<<extra<<std::endl;
        }
}