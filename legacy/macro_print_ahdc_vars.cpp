void macro_print_ahdc_vars()
{
        TFile* f = new TFile("./input-files/023007.hipo_w.root");

        TNtuple* ntuple = (TNtuple*) f->Get("clas12");

        TH1F* h_sumadc = new TH1F("h_sumadc", "", 100, 0, 50*10E2);
        TH1F* h_let    = new TH1F("h_let"   , "", 100, 0, 1000);
        TH1F* h_tot    = new TH1F("h_tot"   , "", 100, -100, 700);

        ntuple->Project("h_sumadc", "AHDC_sumadc"           , "");
        ntuple->Project("h_let"   , "AHDC_leadingEdgeTime"  , "");
        ntuple->Project("h_tot"   , "AHDC_timeOverThreshold", "");

        h_sumadc->SetTitle(";max ADC;");
        h_let->SetTitle(";Leading edge time (ns);");
        h_tot->SetTitle(";Time over threshold (ns);");

        TCanvas* c = new TCanvas("","",800,600);
        h_sumadc->Draw();
        gPad->SetLogy(1);
        c->Print("./plots/sumadc.pdf");
        
        h_let->Draw();
        gPad->SetLogy(0);
        c->Print("./plots/leadingedgetime.pdf");
        
        h_tot->Draw();
        gPad->SetLogy(1);
        c->Print("./plots/timeoverthreshold.pdf");
}