#include "../include/constants.h"

const int nwires = 100;
const int nlayers = 2;

TCut basic_cut  = Form("W<%f&&ahdc_kftrackpath*ahdc_kftrackdedx>3500",w_max);
        
double fitfunction (double* x, double* par)
{
        double signal = par[0] * TMath::Exp(-0.5 * std::pow((x[0] - par[1])/(par[2]),2));

        return signal;
}

void macro_create_wire_tot_histos_simplefit()
{
        // gStyle->SetOptFit(1);

        TFile* f = new TFile("../output-files/ntuple_elastics_D2_022994.root");
        TFile* fout = new TFile("../output-files/histos_wire_tot_simplefit.root","RECREATE");
        gROOT->cd();
        
        TNtuple* ntuple_elastics = (TNtuple*) f->Get("ntuple_elastics");

        TH1F* h_tot = new TH1F("h_tot","",30,350,650);
        TGraphErrors* g_wire_tot_offset   = new TGraphErrors();
        TGraphErrors* g_wire_chisquarendf = new TGraphErrors();

        TF1* fitf = new TF1("fitf", fitfunction, 400, 500, 3);
        fitf->SetParameter(0, 1);  
        fitf->SetParameter(1, 470);  
        fitf->SetParameter(2, 50);  

        fitf->SetParLimits(1, 400, 500);  

        int graph_point = 0;

        for (int i = 1 ; i <= nwires ; i++) {
                for (int j = 1 ; j <= nlayers ; j++) {
                        TCut wire_cut = Form("ahdc_component==%i&&ahdc_layer==%i",i,j);

                        ntuple_elastics->Project("h_tot", "ahdc_timeOverThreshold", basic_cut + wire_cut);

                        h_tot->Scale(1./h_tot->Integral());

                        h_tot->Fit("fitf","QR");

                        if (fitf->GetChisquare() == 0 || fitf->GetNDF() == 0)
                                continue;

                        fout->cd();
                        h_tot->Write(Form("h_tot%i_%i",i,j));
                        gROOT->cd();

                        g_wire_tot_offset->SetPoint(graph_point, i, fitf->GetParameter(3));
                        g_wire_tot_offset->SetPointError(graph_point, 0, fitf->GetParError(3));

                        g_wire_chisquarendf->SetPoint(graph_point, i, fitf->GetChisquare()/fitf->GetNDF());
                        g_wire_chisquarendf->SetPointError(graph_point, 0, 0);

                        graph_point++;

                        h_tot->Reset();
                }
        }

        fout->cd();
        g_wire_tot_offset->Write("wire_tot_offset");
        g_wire_chisquarendf->Write("wire_chisquarendf");
        gROOT->cd();

        f->Close();
        fout->Close();
}