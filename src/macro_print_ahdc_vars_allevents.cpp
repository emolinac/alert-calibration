#include "../include/constants.h"

std::string input_file_name = "../output-files/ntuple_allevents_He4_10600mev.root";

TCut basic_cut  = Form("W<%f",w_max);
TCut let_cut    = "ahdc_leadingEdgeTime>250";
TCut adc_cut    = "ahdc_kftrackpath*ahdc_kftrackdedx>4000";
        
void macro_print_ahdc_vars_allevents()
{
        TFile* f = new TFile(input_file_name.c_str());
        
        TNtuple* ntuple_allevents = (TNtuple*) f->Get("ntuple_allevents");

        TH1F* h_tot    = new TH1F("h_tot"   ,"",100,0,1000); 
        
        TH2F* h_tracktheta_adc  = new TH2F("h_tracktheta_adc" ,"",150,0,180,250,0,15000);
        TH2F* h_trackpt_adc     = new TH2F("h_trackpt_adc"    ,"",150,0,.5,250,0,15000);

        TH2F* h_adc_tot     = new TH2F("h_adc_tot"    ,"",100,0,1000,100,500,40000);
        TH2F* h_adc_let     = new TH2F("h_adc_let"    ,"",100,100,1000,100,500,14000);
        TH2F* h_tot_let     = new TH2F("h_tot_let"    ,"",100,100,1000,100,100,1000);

        ntuple_allevents->Project("h_tracktheta_adc", "ahdc_kftrackpath*ahdc_kftrackdedx:kftrack_theta"   , "");
        ntuple_allevents->Project("h_trackpt_adc"   , "ahdc_kftrackpath*ahdc_kftrackdedx:kftrack_pt/1000.", "");
        
        ntuple_allevents->Project("h_tot"        , "ahdc_timeOverThreshold"                                  , "kftrack_pt/1000.<1.5");
        ntuple_allevents->Project("h_adc_tot"    , "ahdc_kftrackpath*ahdc_kftrackdedx:ahdc_timeOverThreshold", "kftrack_pt/1000.<1.5");
        ntuple_allevents->Project("h_adc_let"    , "ahdc_kftrackpath*ahdc_kftrackdedx:ahdc_leadingEdgeTime"  , "kftrack_pt/1000.<1.5");
        ntuple_allevents->Project("h_tot_let"    , "ahdc_timeOverThreshold:ahdc_leadingEdgeTime"             , "kftrack_pt/1000.<1.5");

        TCanvas* c = new TCanvas("c","",800,600);
        c->Draw();

        h_tot->Draw();
        h_tot->SetTitle(";ToT;");

        c->Print("../plots/allevents_tot_10600mev.pdf");

        h_tracktheta_adc->Draw("col");
        h_tracktheta_adc->SetTitle(";#theta_{kftrack};adc");
        // h_tracktheta_adc->Smooth();
        gPad->SetLogy(0);

        c->Print("../plots/allevents_kftrack_theta_adc_10600mev.pdf");
        
        h_trackpt_adc->Draw("col");
        h_trackpt_adc->SetTitle(";p_{T, kftrack}(GeV);adc");
        // h_trackpt_adc->Smooth();
        gPad->SetLogy(0);

        c->Print("../plots/allevents_kftrack_pt_pathadc_10600mev.pdf");

        h_adc_tot->Draw("col");
        h_adc_tot->SetTitle(";ToT;adc");
        gPad->SetLogy(0);

        c->Print("../plots/allevents_tot_pathadc_10600mev.pdf");

        h_adc_let->Draw("col");
        h_adc_let->SetTitle(";let;adc");
        gPad->SetLogy(0);

        c->Print("../plots/allevents_let_pathadc_10600mev.pdf");

        h_tot_let->Draw("col");
        h_tot_let->SetTitle(";let;tot");
        gPad->SetLogy(0);

        c->Print("../plots/allevents_let_tot_10600mev.pdf");
}