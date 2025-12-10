#include "../include/constants.h"

void macro_print_ahdc_vars()
{
        TFile* f = new TFile("../output-files/ntuple_elastics_D2_022994.root");
        
        TNtuple* ntuple_electron = (TNtuple*) f->Get("ntuple_electron");
        TNtuple* ntuple_elastics = (TNtuple*) f->Get("ntuple_elastics");

        TH1F* h_tot    = new TH1F("h_tot"   ,"",100,0,1000); 
        
        TH2F* h_ept_energy  = new TH2F("h_ept_energy" ,"",150,0.18,0.5,150,0,200);
        TH2F* h_ept_adc     = new TH2F("h_ept_adc"    ,"",100,0.18,0.5,100,500,14000);
        TH2F* h_adc_tot     = new TH2F("h_adc_tot"    ,"",150,0,1000,150,500,14000);
        TH2F* h_adc_let     = new TH2F("h_adc_let"    ,"",150,100,1000,150,500,14000);
        TH2F* h_tot_let     = new TH2F("h_tot_let"    ,"",150,100,1000,150,100,1000);

        // TCut global_cut = Form("W<%f&&ahdc_trackid>0&&ahdc_kftrackpath*ahdc_kftrackdedx>4000&&ahdc_leadingEdgeTime<400&&ahdc_leadingEdgeTime>270&&(ahdc_timeOverThreshold<(800-ahdc_leadingEdgeTime))",w_max);
        // TCut basic_cut  = Form("W<%f&&ahdc_trackid>0&&ahdc_leadingEdgeTime>100&&(ahdc_timeOverThreshold<(800-ahdc_leadingEdgeTime))",w_max);
        TCut basic_cut  = Form("W<%f",w_max);
        TCut let_cut    = "ahdc_leadingEdgeTime>250";
        // TCut tot_cut    = "ahdc_timeOverThreshold>370&&ahdc_timeOverThreshold<550";
        TCut adc_cut    = "ahdc_kftrackpath*ahdc_kftrackdedx>4000";

        ntuple_elastics->Project("h_tot"        , "ahdc_timeOverThreshold"                                  , basic_cut);
        ntuple_elastics->Project("h_ept_energy" , "ahdc_kftrackdedx:electron_pt"                            , basic_cut);
        ntuple_elastics->Project("h_ept_adc"    , "ahdc_kftrackpath*ahdc_kftrackdedx:electron_pt"           , basic_cut);
        ntuple_elastics->Project("h_adc_tot"    , "ahdc_kftrackpath*ahdc_kftrackdedx:ahdc_timeOverThreshold", basic_cut);
        ntuple_elastics->Project("h_adc_let"    , "ahdc_kftrackpath*ahdc_kftrackdedx:ahdc_leadingEdgeTime"  , basic_cut);
        ntuple_elastics->Project("h_tot_let"    , "ahdc_timeOverThreshold:ahdc_leadingEdgeTime"             , basic_cut);

        TCanvas* c = new TCanvas("c","",800,600);
        c->Draw();

        h_tot->Draw();
        h_tot->SetTitle(";ToT;");

        c->Print("../plots/elastics_tot_w-cut.pdf");

        h_ept_energy->Draw("col");
        h_ept_energy->SetTitle(";p_{T,e}(GeV);dedx (adc/mm)");
        gPad->SetLogy(0);

        c->Print("../plots/elastics_pt_electron_adc_w-cut.pdf");
        
        h_ept_adc->Draw("col");
        h_ept_adc->SetTitle(";p_{T,e}(GeV);adc");
        gPad->SetLogy(0);

        c->Print("../plots/elastics_pt_electron_pathadc_w-cut.pdf");

        h_adc_tot->Draw("col");
        h_adc_tot->SetTitle(";ToT;adc");
        gPad->SetLogy(0);

        c->Print("../plots/elastics_tot_pathadc_w-cut.pdf");

        h_adc_let->Draw("col");
        h_adc_let->SetTitle(";let;adc");
        gPad->SetLogy(0);

        c->Print("../plots/elastics_let_pathadc_w-cut.pdf");

        h_tot_let->Draw("col");
        h_tot_let->SetTitle(";let;tot");
        gPad->SetLogy(0);

        c->Print("../plots/elastics_let_tot_w-cut.pdf");
}