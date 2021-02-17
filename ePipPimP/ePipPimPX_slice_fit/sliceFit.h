#ifndef SLICEFIT_H
#define SLICEFIT_H

#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TH2.h>
#include <TH2F.h>
#include <TH1.h>
#include <TH1D.h>
#include <TF1.h>


void SliceFit(TH2F *histo, const int num_Slices,
              double fit_min, double fit_max);

#endif
