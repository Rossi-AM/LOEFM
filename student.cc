#include <iostream>
#include <fstream>
#include <cmath>
#include "TCanvas.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include "TStyle.h"

using namespace std;

int main(){

    float a[31];
    fstream inputData("student.dat", ios::in);

    for (int i = 0; i < 31; i++)
        inputData >> a[i];
    
    TGraph *graf = new TGraph(31);
    TCanvas *c1 = new TCanvas("c1", "canva", 800, 800);

    for(int i = 0; i < 31; i++)
        graf->SetPoint(i, ((float)i/10), a[i]);
    
    graf->Draw("A*");
    graf->GetXaxis()->SetTitle("t");
    graf->GetYaxis()->SetTitle("probabilità");
    graf->SetLineWidth(0);
    c1->SaveAs("student.png");

    TF1 *f1 = new TF1("f1", "sqrt(x)" , 0, 3);

    f1->Draw("same");
    f1->SetLineWidth(2);

    // graf->Fit("f1");
    // gStyle->SetOptFit();



    delete c1;

    return 0;
}