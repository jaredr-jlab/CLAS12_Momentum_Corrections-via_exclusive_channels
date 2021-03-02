#include "sliceFit.h"


void FitSlices(TH1D *slice_histo, TGraphErrors *mean_graph,
               const int num_slices, double fit_min, double fit_max){

    int range_iterations = 3;
    double mu[num_slices], emu[num_slices], sigma[num_slices];
    double f_min[num_slices], f_max[num_slices];

    // Fill arrays for the initial fit range
    for (int i=0; i<num_slices; i++){
        f_min[i] = fit_min;
        f_max[i] = fit_max;
    }

    // Fit the slices while adjusting the fit range
    TF1 f1("f1", "gaus(0)+pol1(3)", fit_min, fit_max);
    f1.SetParNames("Constant", "Mean", "Sigma", "p0", "p1");
    f1.SetParLimits(2, 0, 1);
    for (int j=0; j<range_iterations; j++){
        for (int i=0; i<num_slices; i++){
            f1.SetParameters((slice_histo + i)->GetMaximum(), (slice_histo + i)->GetMean(), (slice_histo + i)->GetRMS(), 1, 1);
            (slice_histo + i)->Fit(&f1, "", "", f_min[i], f_max[i]);
            mu[i] = f1.GetParameter("Mean");
            emu[i] = f1.GetParError(1);
            sigma[i] = f1.GetParameter("Sigma");
            f_min[i] = mu[i] - 2.8 * sigma[i];
            f_max[i] = mu[i] + 2.8 * sigma[i];
        }
    }

    // Set points for the TGraphErrors
    double x = 0;
    for (int i=0; i<num_slices; i++){
        x = (2 * i + 1.0) / 2.0;
        mean_graph->SetPoint(i, x, mu[i]);
        mean_graph->SetPointError(i, 0, emu[i]);
    }
}
