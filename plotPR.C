#include "cstdlib"
#include "iostream"
#include "TFile.h"
#include "cstring"
#include "TLegend.h"
#include "TGraph.h"
#include "TH3.h"
#include "TH2.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TText.h"
#include "TPaveText.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TAttFill.h"
#include "TColor.h"
#include "string"
#include "fstream"

//This macro plot the uniplast data for the one which are unique tiles and those more than 530 tiles in the uniplast list.
 
using namespace std;
int countLines(char* filelist);
fstream& goToLine(fstream& file, unsigned int num);
int countLines(string filelist);
void plotPR(const char *uni1);

void plotPR(const char *uni1){

  
  fstream data1;data1.open(uni1);
  //fstream data2;data2.open(uni2);
  TH1F* h1=new TH1F("h1","Unique_PR",40,0,2.0);
  //TH1F* h2=new TH1F("h2","B27_All_PR",40,0,2.0);

  int n1lines =countLines(uni1);
  //int n2lines =countLines(uni2);

  for(int i=0; i< n1lines;i++){
    
    string tile1;
    float PR1,mpv1;
    data1>>tile1 >> mpv1 >> PR1;
    h1->Fill(PR1);
    //gStyle->SetOptStat(1);
   

  }
  /* 
     for(int j=0; j< n2lines;j++){
    
    string tile2;
    float PR2,mpv2;
    data2>>tile2 >> mpv2 >> PR2;
    h2->Fill(PR2);
    //gStyle->SetOptStat(1);
   

    }
*/
  
  TCanvas *c = new TCanvas();
  h1->SetLineColor(4);
  h1->GetXaxis()->SetTitle("PR");
  h1->Draw();
  /*TCanvas *c1 = new TCanvas();
  h2->SetLineColor(2);
  h2->GetXaxis()->SetTitle("PR");
  h2->Draw("same");
  */
  TLegend *leg = new TLegend();
  gStyle->SetOptStat(0);
  leg->AddEntry(h1,"Unique from Uniplast","l");
  //leg->AddEntry(h2,"Exact as GSU","l");
  leg->Draw();
  






  

}

int countLines(char *filelist) {
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;

}

int countLines(string filelist) {
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(filelist);

    while (std::getline(myfile, line))++number_of_lines;
    myfile.seekg (0, ios::beg);
    return number_of_lines;

}

fstream& goToLine(fstream& file, unsigned int num)
{
  file.seekg(0);
  for(int i = 0; i < num; i++)
    {
      file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
  return file;
}
