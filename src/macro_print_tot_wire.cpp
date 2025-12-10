#include "../include/constants.h"

const int nwires = 7;
const int wire_offset = 11;

void macro_print_tot_wire()
{
        TFile* f = new TFile("../output-files/ntuple_elastics_D2_022994.root");
        
        TNtuple* ntuple_elastics = (TNtuple*) f->Get("ntuple_elastics");

        TCut basic_cut  = Form("W<%f",w_max);
        
        THStack* hs = new THStack();
        TH1F* h_tot[nwires];
        for (int i = 0 ; i < nwires ; i++) {
                h_tot[i] = new TH1F(Form("h_tot%i",i),"",30,300,650); 
                h_tot[i]->SetMarkerColor(i+1);
                h_tot[i]->SetLineColor(i+1);

                TCut wire_cut = Form("ahdc_component==%i",i + wire_offset + 1);

                ntuple_elastics->Project(Form("h_tot%i",i), "ahdc_timeOverThreshold", basic_cut + wire_cut);

                h_tot[i]->Scale(1./h_tot[i]->Integral());

                hs->Add(h_tot[i], "AP");
        }

        TCanvas* c = new TCanvas("c","",800,600);
        c->Draw();
        hs->Draw("NOSTACK");
        hs->SetTitle(";ToT;");

        c->Print(Form("../plots/tot-wires-%i-to-%i.pdf",wire_offset,wire_offset + nwires));
}