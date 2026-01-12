#include "../include/constants.h"

const int nwires = 100;
const int nlayers = 5;

TCut basic_cut  = Form("W<%f&&ahdc_kftrackpath*ahdc_kftrackdedx>3000",w_max);
        
double fitfunction (double* x, double* par)
{
        double background = par[0] + par[1] * x[0];
        double signal     = par[2] * TMath::Exp(-0.5 * std::pow((x[0] - par[3])/(par[4]),2));

        return background + signal;
}

void macro_create_wire_tot_histos()
{
        gStyle->SetOptFit(1);

        TFile* f = new TFile("../output-files/ntuple_elastics_D2_022994.root");
        TFile* fout = new TFile("../output-files/histos_wire_tot_chisquare-set3-allwires.root","RECREATE");
        gROOT->cd();
        
        TNtuple* ntuple_elastics = (TNtuple*) f->Get("ntuple_elastics");

        TH1F* h_tot = new TH1F("h_tot","",30,350,650);
        TGraphErrors* g_wire_tot_offset   = new TGraphErrors();
        TGraphErrors* g_wire_chisquarendf = new TGraphErrors();

        TF1* fitf = new TF1("fitf", fitfunction, 400, 500, 5);
        fitf->SetParameter(0, 0.01);  
        fitf->SetParameter(1, -0.00003);  
        fitf->SetParameter(2, 0.25);  
        fitf->SetParameter(3, 470);  
        fitf->SetParameter(4, 50);  
        
        // Nominal ranges
        // fitf->SetParLimits(0, 0.005, 0.015); 
        // fitf->SetParLimits(1, -0.00009, -0.000005); 
        // fitf->SetParLimits(2, 0, 0.5);
        // fitf->SetParLimits(3, 400, 500);
        // fitf->SetParLimits(4, 40, 60);

        // Set 1
        // fitf->SetParLimits(0, 0.001, 0.025); 
        // fitf->SetParLimits(1, -0.00009, -0.0000005); 
        // fitf->SetParLimits(2, 0, 0.5);
        // fitf->SetParLimits(3, 400, 500);
        // fitf->SetParLimits(4, 30, 60);

        // Set 2 (BEST ONE YET)
        // fitf->SetParLimits(0, 0.001, 0.025); 
        // fitf->SetParLimits(1, -0.0002, -0.0000001); 
        // fitf->SetParLimits(2, 0, 0.5);
        // fitf->SetParLimits(3, 400, 500);
        // fitf->SetParLimits(4, 20, 70);

        // Set 3
        fitf->SetParLimits(0, 0.0001, 0.035); 
        fitf->SetParLimits(1, -0.0002, -0.0000001); 
        fitf->SetParLimits(2, 0, 0.5);
        fitf->SetParLimits(3, 400, 500);
        fitf->SetParLimits(4, 20, 70);

        int graph_point = 0;

        for (int i = 1 ; i <= nwires ; i++) {
                for (int j = 1 ; j <= nlayers ; j++) {
                        TCut wire_cut = Form("ahdc_component==%i&&ahdc_superlayer==%i",i,j);

                        ntuple_elastics->Project("h_tot", "ahdc_timeOverThreshold", basic_cut + wire_cut);

                        h_tot->Scale(1./h_tot->Integral());

                        h_tot->Fit("fitf","MR");

                        if (fitf->GetChisquare() == 0 || fitf->GetNDF() == 0)
                                continue;

                        fout->cd();
                        h_tot->Write(Form("h_tot%i_%i",i,j));
                        gROOT->cd();

                        g_wire_tot_offset->SetPoint(graph_point, graph_point, fitf->GetParameter(3));
                        g_wire_tot_offset->SetPointError(graph_point, 0, fitf->GetParError(3));

                        g_wire_chisquarendf->SetPoint(graph_point, graph_point, fitf->GetChisquare()/fitf->GetNDF());
                        g_wire_chisquarendf->SetPointError(graph_point, 0, 0);

                        graph_point++;

                        h_tot->Reset();
                }
        }

        g_wire_tot_offset->SetTitle(";wire;ToT Offset");
        g_wire_chisquarendf->SetTitle(";wire;#Chi^{2}_{ndf}");

        fout->cd();
        g_wire_tot_offset->Write("wire_tot_offset");
        g_wire_chisquarendf->Write("wire_chisquarendf");
        gROOT->cd();

        f->Close();
        fout->Close();
}