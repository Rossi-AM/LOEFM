//Program created to analyze the data relative to the Millikan experiment
//in order to provide a value for the electron's charge.
//
//This program reads a file with the measured electric charges of several minuscule oildrops,
//then evaluates and draws the S(q) function for different values of q.
//
//S(q)=\sum_{i=1}^{N} (Q_i/k_i - q)^2   where   k_i   is the closest integer to the number Q_i/q
//
//The charges are measured in C*10^(-19) to simplify the calculations.
//The program then finds the minimum of the S(q) function q_min.
//The best value of q is the average of the quantities Q_i/k_i   where   k_i   is the closest integer to the number Q_i/q_min
//The relative sigma is the standard deviation.
//
//Made by Lorenzo Ramella lorenzo.ramella@studenti.unimi.it on 05/01/21

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TF1.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TVectorT.h"     	 
#include "TGraph.h"  

#include <iostream>
#include <functional>
#include <cmath>
#include <system_error>
#include <string>
#include <fstream>
#include <vector>

int k(float Q, float q){                //this function returns the closest integer to the number Q/q
  int r = static_cast<int>(Q/q + 0.5);   
  return r;                               
}                                        
                                          
float partial_s(float Q, float q){      //this function returns (Q/k - q)^2
  float r = std::pow(Q/k(Q, q) - q, 2);
  return r;
}

class S_q{                              //a bit confusing but i didn't came up with a better name
  
  public:
  S_q(float q, float s): m_q(q), m_s(s) {};
  ~S_q() {};

  float m_s;
  float m_q;
};

int main(int argc, char **argv){

TApplication MainApp("Millikan", &argc, argv);

//reading input file

std::ifstream in;
std::string name;
	if (argc > 1) {
	name = argv[1];
	} 
  else {
		std::cout<< "Nome file: ";
		std::cin >> name;
		std::cout<< std::endl;
	}	
	
in.open(name);
  if ( !in ) {
	std::error_code the_error(errno, std::system_category());
	std::cout << argv[0] << ": Error opening "
	          << name << ": " << the_error.message() << std::endl;
	return 1;
	}
	
std::vector<float> drops;
while( in.good() ){
  float element;
	in>>element;
	drops.push_back(element);
	}

in.close();

std::vector<S_q> q;
float test_q = 1.5;     //starting point to test q
const int points = 101; //number of points

for(int i=0; i<points; ++i){
  float test_sum=0;
  for(int j=0; j<drops.size(); ++j){
    test_sum += partial_s(drops[j], test_q);
  }
  S_q q_i(test_q, test_sum);
  q.push_back(q_i);
  test_q += 0.002;      //interval
}

int min=0;              //finding the minimum of S(q)
for(int i=0; i<points; ++i){
  std::cout<<std::endl<<q[i].m_q<<"\t"<<q[i].m_s;
  if (q[i].m_s < q[min].m_s) min=i;
  }

float q_e=0;            //calculating q_e
float sigma=0;          //and the relative sigma
for(int i=0; i<drops.size(); ++i){
  q_e += ((drops[i]/k(drops[i], q[min].m_q)) - q_e) / (i+1);
  }
for(int i=0; i<drops.size(); ++i){
  sigma += (std::pow((drops[i]/k(drops[i], q[min].m_q)) - q_e,2) - sigma) / (i+1);
  }
sigma = std::sqrt(sigma);

std::cout<<std::endl<<std::endl<<"q_e = "<<q_e<<std::endl<<"std dev = "<<sigma<<std::endl;

TVectorT<float> x_values(points);
TVectorT<float> y_values(points);

for(int i=0; i<points; ++i){
x_values[i] = q[i].m_q;
y_values[i] = q[i].m_s;
}

TCanvas *canvas = new TCanvas("S(q)", "Graph Canvas", 700., 500.);
TGraph *graph = new TGraph(x_values, y_values);
	graph->SetTitle("S(q)");
	graph->SetLineColor(kRed);
  graph->GetXaxis()->SetTitle("q [C 10^-19]");
  graph->GetYaxis()->SetTitle("S [10^-38]");
	graph->Draw("AC");
  graph->SaveAs("S_q.PNG");

MainApp.Run();

return 0;
}
