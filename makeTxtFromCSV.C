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

using namespace std;

void makeTxtFromCSV_Uni(char *csvin);
int countLines(char* filelist);
fstream& goToLine(fstream& file, unsigned int num);
void mpvCrossCheck(char *uniplast, char *gsu);
float findMPV(string tilename);
void makeTxtFromCSV_GSU(char *csvin);
int countLines(string filelist);

void makeTxtFromCSV_Uni(const char *csvin) //Note this is specifically for Uniplast data
{
  fstream outputboi;
  outputboi.open("CSV_output_UNI.txt",ios::app);
  fstream inputbois;
  inputbois.open(csvin);
  int nFiles = countLines(csvin);
  
  for(int i = 0; i < nFiles; i++)
    {
      string inputline;
      inputbois >> inputline;
      fstream csvboi; csvboi.open(inputline.c_str());
      double mpv[8]; memset(mpv,0,sizeof(mpv));
      //
      goToLine(csvboi,12);
      string dummer;
      float mpv_12;
      float mpv_13;
      float dumber;
      string ref_tilename;
      csvboi >> dumber >> dumber >> dummer >> ref_tilename >>mpv_12;
      // cout<< mpv_12<<endl;
      
      goToLine(csvboi,13);
      csvboi >> dumber >> dumber >> dummer >> ref_tilename >> mpv_13;
      // cout<< mpv_12<< '\t' <<mpv_13 <<endl;
      float ref_avg;
      ref_avg = (mpv_12+mpv_13)/2; // Average mpv of the reference tiles.
      cout<< ref_avg << '\t'<< mpv_12<< '\t' << mpv_13 <<endl;
      //
      //
     for(int j = 14; j < 22; j++)
	{
	  goToLine(csvboi,j);
	  string tilename;
	  float MPV;
	  float dumb;
	  string dummy;
	  float PR[8];

	  
	 
	  csvboi >> dumb >> dumb >> dummy >> tilename >> MPV;
	  mpv[j-14] = MPV;
	  PR[j-14] = mpv[j-14]/ref_avg;
	  //cout<< "PR: " <<PR[j-14]<<endl;
	  cout << tilename << '\t' << MPV << '\t' << PR[j-14] <<endl;
	  //cout<< "mpv: "<< mpv[j-14]<<endl;
	  // int mpv_avg = (mpv[0]+mpv[1])/2;
	  //cout<< "mpv: "<< mpv[j-12]<<endl;
	  //cout<< "mpv_avg: "<< mpv_avg<<endl;
	  
	  /*for(int k=0; k<8; k++){
	    // mpv[k] = MPV;
	    // cout<< "mpv: "<< mpv[k]<<endl;
	    PR[k] = mpv[k]/ref_avg;
	    //cout<< mpv_12<< '\t' << mpv_13<< '\t' << ref_avg << '\t' << PR[k] <<endl;
	    // cout<< "PR: " <<PR[k]<<endl;
	    }*/
	  outputboi << tilename << '\t' << mpv[j-14] << '\t' << PR[j-14] <<endl;
	 
      //cout<< "sum: "<<sum[j]<< "\t"<<"avg: " <<avg_ref<<endl;
	  
	}
    }

outputboi.close();
}

void makeTxtFromCSV_GSU(char *csvin)// And this is specifically for GSU data
{
  fstream outputboi;
  outputboi.open("CSV_output_GSU.txt",ios::app);
  fstream inputbois;
  inputbois.open(csvin);

  int nFiles = 12;
  for(int i = 0; i < 12; i++)
    {
      string name;
      inputbois >> name;
      int nLines = countLines(name); cout << "nLines: " << nLines << endl;
      fstream database; database.open(name.c_str());
      for(int j = 1; j < nLines; j++)
	{
	  goToLine(database,j);
	  string tilename,dumb;
	  float dummy, mpv,PR;
	  //float  mpv;
	  database >> dummy >> dummy >> tilename >> PR >> dummy>> dumb >> mpv;
	  outputboi << tilename << '\t' << PR << '\t'  << mpv << endl;
	}
    }
  outputboi.close();
}
		 
  

void mpvCrossCheck(char *uniplast, char *gsu)
{
  fstream unidat; unidat.open(uniplast);
  fstream gsudat; gsudat.open(gsu);

  TH2F *crosscheck = new TH2F("Uni Vs GSU","uniplastvgsu",20,400,1600,20,300,900);
  TH2F *PR_check = new TH2F("Uni Vs GSU","univgsu",40,0,2.0,40,0,2.0);
  TH1F* uni_PR=new TH1F("Uniplast","UNI_PR",40,0,2.0);
  TH1F* uni_PR_all=new TH1F("Uniplast","UNI_PR_all",40,0,2.0) ;
  TH1F* gsu_PR=new TH1F("GSU","GSU_PR",40,0,2.0) ;
  // uni_PR= (TH1F *) PR_check->ProjectionX();
  int nGLines = countLines(gsu);
  int nULines = countLines(uniplast);
  int missingTiles = 0;
  for (int k =0; k<nULines;k++){

    string uniTile;
    float uniPr, mPV;
    //goToLine(unidat,j);
    unidat >> uniTile >> mPV >> uniPr;
    //cout<<uniTile<<'\t'<<uniPr<<endl;
    if(uniTile.c_str())
      {
	  uni_PR_all->Fill(uniPr);
	  gStyle->SetOptStat(1);
	  //cout<< k <<'\t'<<uniPr<<endl;
      }
  }
  for(int i = 0; i < nGLines; i++)
    {
      string gsutile;
      string unitile;
      float gsuMPV ,gsuPR, uniPR;
      gsudat >> gsutile >> gsuPR >> gsuMPV;
      int missCheck = 0;
      for(int j = 0; j < nULines; j++)
	{
	  //string unitile;
	  float uniMPV;
	  
	  goToLine(unidat,j);
	  unidat >> unitile >> uniMPV >> uniPR;
	  //cout<<unitile<<'\t'<<uniPR<<endl;
	   
	  //uni_PR->Fill(uniPR);
	  //string tile ={"B,2,3"};
	  // tile
	  //tile.substr(0,2);
	  //cout<<tile<<endl;
	  // cout<<"GSU_PR: "<< gsuPR<<'\t'<<"UNI_PR: "<<uniPR<<endl;
	  // if(!strcmp(unitile.c_str(),gsutile.c_str()))
	  if(!strcmp(gsutile.c_str(), unitile.c_str()))
	    {
	     
	      crosscheck -> Fill(uniMPV,gsuMPV);
	      PR_check->Fill(uniPR,gsuPR);
	      uni_PR->Fill(uniPR);
	      gsu_PR->Fill(gsuPR);
	      break;
	    }
	  else
	    {
	      
	      missCheck++;
	    }
	  if(missCheck == nULines)
	    {
	      missingTiles++;
	      cout << "Missing Tile " << gsutile << endl;
	    }
	 
	}
      // cout<<gsutile<<'\t'<<"GSU_PR: "<< gsuPR<<'\t'<<unitile<<'\t'<<"UNI_PR: "<<uniPR<<endl;
    }
  cout << "Number of tiles not found: " << missingTiles << endl;
  crosscheck -> SetTitle(";Uniplast MPV; GSU MPV");
  crosscheck -> Draw("colz");
  TCanvas *c= new TCanvas();
  PR_check -> SetTitle("B27;Uniplast PR; GSU PR");
  PR_check -> Draw("colz");
  TCanvas *c1= new TCanvas();
  uni_PR->SetTitle("B27;PR;");
  uni_PR->Draw();
  //TCanvas *c2= new TCanvas();
  gsu_PR->SetTitle("B25;PR;");
  gsu_PR->SetLineColor(kRed);
  gsu_PR->Draw("same");
  TLegend *leg = new TLegend();
  gStyle->SetOptStat(0);
  leg->AddEntry(uni_PR,"Uniplast","l");
  leg->AddEntry(gsu_PR,"GSU","l");
  leg->Draw();
  TCanvas *c2= new TCanvas();
  uni_PR_all->SetTitle("B27;PR;");
  uni_PR_all->SetLineColor(kBlue);
  uni_PR_all->Draw();
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

