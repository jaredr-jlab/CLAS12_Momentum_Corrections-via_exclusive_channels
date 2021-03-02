{
//--------------------------------------------------------
//------------*****Make Adjustments Here*****-------------
int cut_select = 1;  //Select switch case (1, 2, 3, or 4)
float lcut_me = -0.11;
float rcut_me = 0.25;
float lcut_pim = -0.18;
float rcut_pim = 0.20;
float lcut_pip = -0.20;
float rcut_pip = 0.28;
float lcut_pro = 0.80;
float rcut_pro = 1.06;
//--------------------------------------------------------
//--------------------------------------------------------

bool cut1, cut2, cut3, cut4;

float pipx, pipy, pipz, pimx, pimy, pimz;
float ex, ey, ez, prox, proy, proz;

double pipE, pimE, eleE, proE;
double me_ePipPimPX, mm2_ePipPX, mm2_ePimPX, mm_ePipPimX;
double mpip = 0.13957, mpim = 0.13957;
double mele = 0.00051099891, mpro = 0.938272;

//-----------------Instantiate 4-Vectors-------------------
auto beam = ROOT::Math::PxPyPzEVector(0, 0, 10.604, sqrt(10.604*10.604 + mele*mele));
auto target = ROOT::Math::PxPyPzEVector(0, 0, 0, mpro);
auto pip = ROOT::Math::PxPyPzEVector();
auto pim = ROOT::Math::PxPyPzEVector();
auto ele = ROOT::Math::PxPyPzEVector();
auto pro = ROOT::Math::PxPyPzEVector();
auto ePipPimPX = ROOT::Math::PxPyPzEVector();
auto ePipPX = ROOT::Math::PxPyPzEVector();
auto ePimPX = ROOT::Math::PxPyPzEVector();
auto ePipPimX = ROOT::Math::PxPyPzEVector();
auto PipPimPX = ROOT::Math::PxPyPzEVector();

//-----------------Initialize Histograms------------------
TH1F *H1 = new TH1F("e#pi^{+}#pi^{-}pX",
                    "Missing Energy; me[GeV]; bin count",
                    200, -0.6, 0.6);
TH1F *h1_pass = new TH1F("e#pi^{+}#pi^{-}pX Pass",
                         "Missing Energy; me[GeV]; bin count",
                         200, -0.6, 0.6);
TH1F *h1_fail = new TH1F("e#pi^{+}#pi^{-}pX Fail",
                         "Missing Energy; me[GeV]; bin count",
                         200, -0.6, 0.6);
TH1F *H_pim = new TH1F("e#pi^{+}pX",
                       "Missing Mass^{2}: expecting #pi^{-}; mm^{2}[GeV]; bin count",
                       200, -0.4, 0.6);
TH1F *h_pim_pass = new TH1F("e#pi^{+}pX Pass",
                            "Missing Mass^{2}: expecting #pi^{-}; mm^{2}[GeV]; bin count",
                            200, -0.4, 0.6);
TH1F *h_pim_fail = new TH1F("e#pi^{+}pX Fail",
                            "Missing Mass^{2}: expecting #pi^{-}; mm^{2}[GeV]; bin count",
                            200, -0.4, 0.6);
TH1F *H_pip = new TH1F("e#pi^{-}pX",
                       "Missing Mass^{2}: expecting #pi^{+}; mm^{2}[GeV]; bin count",
                       200, -0.4, 0.6);
TH1F *h_pip_pass = new TH1F("e#pi^{-}pX Pass",
                            "Missing Mass^{2}: expecting #pi^{+}; mm^{2}[GeV]; bin count",
                            200, -0.4, 0.6);
TH1F *h_pip_fail = new TH1F("e#pi^{-}pX Fail",
                            "Missing Mass^{2}: expecting #pi^{+}; mm^{2}[GeV]; bin count",
                            200, -0.4, 0.6);
TH1F *H_pro = new TH1F("e#pi^{+}#pi^{-}X",
                       "Missing Mass: expecting proton; mm[GeV]; bin count",
                       200, 0.6, 1.3);
TH1F *h_pro_pass = new TH1F("e#pi^{+}#pi^{-}X Pass",
                            "Missing Mass: expecting proton; mm[GeV]; bin count",
                            200, 0.6, 1.3);
TH1F *h_pro_fail = new TH1F("e#pi^{+}#pi^{-}X Fail",
                            "Missing Mass: expecting proton; mm[GeV]; bin count",
                            200, 0.6, 1.3);

//-----------------Chain Root Files------------------------
/*
The string "h22" is the tree name from the root file.
*/
TChain chain("h22");
chain.Add("./rootfiles/out_skim8_005032.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005038.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005197.hipo_3.hipo.root");
chain.Add("./rootfiles/out_skim8_005222.hipo_3.hipo.root");

//--------Point Branch Names to Variable Addresses---------
/*
The string "ex" is the branch name from the root file.
The variable address "&ex" points to the float created above.
*/
chain.SetBranchAddress("ex", &ex);
chain.SetBranchAddress("ey", &ey);
chain.SetBranchAddress("ez", &ez);
chain.SetBranchAddress("pipx", &pipx);
chain.SetBranchAddress("pipy", &pipy);
chain.SetBranchAddress("pipz", &pipz);
chain.SetBranchAddress("pimx", &pimx);
chain.SetBranchAddress("pimy", &pimy);
chain.SetBranchAddress("pimz", &pimz);
chain.SetBranchAddress("prox", &prox);
chain.SetBranchAddress("proy", &proy);
chain.SetBranchAddress("proz", &proz);

int nentries = chain.GetEntries();

//-----------------Loop Through Branches------------------
for(int j = 0; j < nentries; j++){
    chain.GetEntry(j);

    pipE = sqrt(pipx*pipx + pipy*pipy + pipz*pipz + mpip*mpip);
    pimE = sqrt(pimx*pimx + pimy*pimy + pimz*pimz + mpim*mpim);
    eleE = sqrt(ex*ex + ey*ey + ez*ez + mele*mele);
    proE = sqrt(prox*prox + proy*proy + proz*proz + mpro*mpro);

    pip.SetPxPyPzE(pipx, pipy, pipz, pipE);
    pim.SetPxPyPzE(pimx, pimy, pimz, pimE);
    ele.SetPxPyPzE(ex, ey, ez, eleE);
    pro.SetPxPyPzE(prox, proy, proz, proE);

    ePipPimPX = beam + target - pip - pim - ele - pro;
    me_ePipPimPX = ePipPimPX.E();

    ePipPX = beam + target - pip - ele - pro;
    mm2_ePipPX = ePipPX.M2();

    ePimPX = beam + target - pim - ele - pro;
    mm2_ePimPX = ePimPX.M2();

    ePipPimX = beam + target - pip - pim - ele;
    mm_ePipPimX = ePipPimX.M();

    //--------Define Cuts------------------------------
    cut1 = me_ePipPimPX > lcut_me && me_ePipPimPX  < rcut_me;
    cut2 = mm2_ePipPX > lcut_pim && mm2_ePipPX < rcut_pim;
    cut3 = mm2_ePimPX > lcut_pip && mm2_ePimPX < rcut_pip;
    cut4 = mm_ePipPimX > lcut_pro && mm_ePipPimX < rcut_pro;

    //--------------Fill Pass/Fail Histograms------
    H1->Fill(me_ePipPimPX);
    H_pim->Fill(mm2_ePipPX);
    H_pip->Fill(mm2_ePimPX);
    H_pro->Fill(mm_ePipPimX);

    //--------------Fill Uncut Histograms----------
    switch(cut_select){
        case 1:
            if(cut2 && cut3 && cut4){
                h1_pass->Fill(me_ePipPimPX);
            }
            else{
                h1_fail->Fill(me_ePipPimPX);
            }
            break;
        case 2:
            if(cut1 && cut3 && cut4){
                h_pim_pass->Fill(mm2_ePipPX);
            }
            else{
                h_pim_fail->Fill(mm2_ePipPX);
            }
            break;
        case 3:
            if(cut1 && cut2 && cut4){
                h_pip_pass->Fill(mm2_ePimPX);
            }
            else{
                h_pip_fail->Fill(mm2_ePimPX);
            }
            break;
        case 4:
            if(cut1 && cut2 && cut3){
                h_pro_pass->Fill(mm_ePipPimX);
            }
            else{
                h_pro_fail->Fill(mm_ePipPimX);
            }
            break;
    }
}

//-------------Set Histogram Line Colors--------------
H1->SetLineColor(1);
H_pim->SetLineColor(1);
H_pip->SetLineColor(1);
H_pro->SetLineColor(1);
h1_pass->SetLineColor(2);
h1_fail->SetLineColor(4);
h_pim_pass->SetLineColor(2);
h_pim_fail->SetLineColor(4);
h_pip_pass->SetLineColor(2);
h_pip_fail->SetLineColor(4);
h_pro_pass->SetLineColor(2);
h_pro_fail->SetLineColor(4);

//-----------------Create Canvas and Draw------------------
TCanvas *c1 = new TCanvas("c1", "c1", 1100, 800);
c1->Divide(2,2);
gStyle->SetOptFit(101);

double ymin, ymax;
TLine lline, rline;
lline.SetLineColor(2);
rline.SetLineColor(2);

//Missing Engergy: ep->e pi+ pi- p X
c1->cd(1);
if(cut_select == 1){
    gPad->SetGrid();
    H1->Draw();
    h1_fail->Draw("SAMES");
    h1_pass->Draw("SAMES");
    TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
    leg->AddEntry(H1, "No Cuts", "l");
    leg->AddEntry(h1_pass, "Passed Cuts", "l");
    leg->AddEntry(h1_fail, "Failed Cuts", "l");
    leg->Draw();
}
else{
    H1->Fit("gaus", "", "", -0.045, 0.080);
    H1->Draw();
    ymin = H1->GetMinimum();
    ymax = H1->GetMaximum();
    lline.DrawLine(lcut_me, ymin, lcut_me, ymax);
    rline.DrawLine(rcut_me, ymin, rcut_me, ymax);
}

//Missing Mass Squared: ep->e pi+ p X
c1->cd(2);
if(cut_select == 2){
    gPad->SetGrid();
    H_pim->Draw();
    h_pim_fail->Draw("SAMES");
    h_pim_pass->Draw("SAMES");
    TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
    leg->AddEntry(H1, "No Cuts", "l");
    leg->AddEntry(h_pim_pass, "Passed Cuts", "l");
    leg->AddEntry(h_pim_fail, "Failed Cuts", "l");
    leg->Draw();
}
else{
    H_pim->Fit("gaus", "", "", -0.05, 0.08);
    H_pim->Draw();
    ymin = H_pim->GetMinimum();
    ymax = H_pim->GetMaximum();
    lline.DrawLine(lcut_pim, ymin, lcut_pim, ymax);
    rline.DrawLine(rcut_pim, ymin, rcut_pim, ymax);
}

//Missing Mass Squared: ep->e pi- p X
c1->cd(3);
if(cut_select == 3){
    gPad->SetGrid();
    H_pip->Draw();
    h_pip_fail->Draw("SAMES");
    h_pip_pass->Draw("SAMES");
    TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
    leg->AddEntry(H1, "No Cuts", "l");
    leg->AddEntry(h_pip_pass, "Passed Cuts", "l");
    leg->AddEntry(h_pip_fail, "Failed Cuts", "l");
    leg->Draw();
}
else{
    H_pip->Fit("gaus", "", "", -0.010, 0.030);
    H_pip->Draw();
    ymin = H_pip->GetMinimum();
    ymax = H_pip->GetMaximum();
    lline.DrawLine(lcut_pip, ymin, lcut_pip, ymax);
    rline.DrawLine(rcut_pip, ymin, rcut_pip, ymax);
}

//Missing Mass: ep->e pi+ pi- X
c1->cd(4);
if(cut_select == 4){
    gPad->SetGrid();
    H_pro->Draw();
    h_pro_fail->Draw("SAMES");
    h_pro_pass->Draw("SAMES");
    TLegend *leg = new TLegend(0.1, 0.7, 0.3, 0.9);
    leg->AddEntry(H1, "No Cuts", "l");
    leg->AddEntry(h_pro_pass, "Passed Cuts", "l");
    leg->AddEntry(h_pro_fail, "Failed Cuts", "l");
    leg->Draw();
}
else{
    H_pro->Fit("gaus", "", "", 0.90, 0.98);
    H_pro->Draw();
    ymin = H_pro->GetMinimum();
    ymax = H_pro->GetMaximum();
    lline.DrawLine(lcut_pro, ymin, lcut_pro, ymax);
    rline.DrawLine(rcut_pro, ymin, rcut_pro, ymax);
}

//-----------------Save the Canvas as a .pdf--------------
c1->Print(Form("mm_me_cuts%i.pdf", cut_select));

delete(H1);
delete(h1_pass);
delete(h1_fail);
delete(H_pim);
delete(h_pim_pass);
delete(h_pim_fail);
delete(H_pip);
delete(h_pip_pass);
delete(h_pip_fail);
delete(H_pro);
delete(h_pro_pass);
delete(h_pro_fail);
delete(c1);
}
