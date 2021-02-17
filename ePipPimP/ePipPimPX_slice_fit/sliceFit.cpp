#include "sliceFit.h"


void SliceFit(TH2F *histo, const int num_Slices,
              double fit_min, double fit_max){

    int cut_Slices_left = 2;  // number of slices to remove from the left.
    int cut_Slices_right = 1;  // number of slices to remove from the right.
    int nBins = histo->GetNbinsX();
    int widthBins = nBins / num_Slices;
    int minBin = cut_Slices_left * widthBins;
    int maxBin = minBin + widthBins - 1;

    double mu[num_Slices], sigma[num_Slices];
    double f_min[num_Slices], f_max[num_Slices];

    // Fill arrays for the initial fit range
    for (int i=cut_Slices_left; i<(num_Slices-cut_Slices_right); i++){
        f_min[i] = fit_min;
        f_max[i] = fit_max;
    }

    // Slice TH2F histo
    TH1D *hfy[num_Slices];
    for (int i=cut_Slices_left; i<(num_Slices-cut_Slices_right); i++){
        hfy[i] = histo->ProjectionY(Form("Slice Range [%d, %d)", i, i+1), minBin, maxBin);
        minBin += 20;
        maxBin += 20;
    }

    // Fit the slices while adjusting the fit range
    TF1 f1("f1", "gaus(0)+pol1(3)", fit_min, fit_max);
    f1.SetParNames("Constant", "Mean", "Sigma", "p0", "p1");
    f1.SetParLimits(2, 0, 1);
    for (int j=0; j<5; j++){  // using 5 iterations to adjust range.
        for (int i=cut_Slices_left; i<(num_Slices-cut_Slices_right); i++){
            f1.SetParameters(hfy[i]->GetMaximum(), hfy[i]->GetMean(), hfy[i]->GetRMS(), 1, 1);
            hfy[i]->Fit(&f1, "", "", f_min[i], f_max[i]);
            mu[i] = f1.GetParameter("Mean");
            sigma[i] = f1.GetParameter("Sigma");
            f_min[i] = mu[i] - 3.0 * sigma[i];
            f_max[i] = mu[i] + 3.0 * sigma[i];
        }
    }

    // Draw and Print the fitted slices
    TCanvas cf1("cf1", "cf1", 1200, 900);
    cf1.Divide(3, 3);
    gStyle->SetOptFit(111);
    int pad = 0;
    for (int i=cut_Slices_left; i<(num_Slices-cut_Slices_right); i++){
        pad += 1;
        cf1.cd(pad);
        hfy[i]->Draw();
    }
    TString histoname = histo->GetName();
    cf1.Print("sliceFit_" + histoname + ".pdf");
}
