#include "sliceFit.h"


void SliceHisto2D(TH2F *histo2D, TH1D *histo, const int num_slices){

    int nBins = histo2D->GetNbinsX();
    int widthBins = nBins / num_slices;
    int minBin = 0;
    int maxBin = widthBins - 1;

    // Slice TH2F histo
    for (int i=0; i<num_slices; i++){
        histo[i] = *(histo2D->ProjectionY(Form("Slice Range [%d, %d)", i, i+1), minBin, maxBin));
        minBin += widthBins;
        maxBin += widthBins;
    }

}
