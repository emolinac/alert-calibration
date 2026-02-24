TString input_file_name = "../output-files/ntuple_elastics_D2_022994.root";

void macro_print_scattering_kinematics()
{
        TFile* f = new TFile(input_file_name.c_str());
        
        TNtuple* ntuple = (TNtuple*) f->Get("ntuple_electron");

        TH1F* h_Q2 = new TH1F("h_Q2","",500,-0.1,1);
        TH1F* h_x  = new TH1F("h_x" ,"",500,0,3);
        TH1F* h_W  = new TH1F("h_W" ,"",500,0.9,1.5); 

        ntuple->Project("h_Q2" , "Q2");
        ntuple->Project("h_x"  , "x" );
        ntuple->Project("h_W"  , "W" );

        TCanvas* c = new TCanvas("c","",800,600);
        c->Draw();

        h_Q2->Draw();
        h_Q2->SetTitle(";Q^{2} (GeV^{2});");
        gPad->SetLogy(1);
        
        c->Print("../plots/elastics_Q2.pdf");

        h_x->Draw();
        h_x->SetTitle(";x;");
        gPad->SetLogy(0);

        c->Print("../plots/elastics_x.pdf");
        
        h_W->Draw();
        h_W->SetTitle(";W (GeV);");

        c->Print("../plots/elastics_W.pdf");
}