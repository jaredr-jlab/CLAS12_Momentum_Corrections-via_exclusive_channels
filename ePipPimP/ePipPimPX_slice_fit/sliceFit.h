#ifndef SLICEFIT_H
#define SLICEFIT_H

#include <TStyle.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TH2F.h>
#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>


void SliceHisto2D(TH2F *histo2D, TH1D *histo, const int num_slices);

void FitSlices(TH1D *slice_histo, TGraphErrors *mean_graph,
               const int num_slices, double fit_min, double fit_max);

#endif
