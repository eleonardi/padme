#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
//#include <CLHEP/Units/SystemOfUnits.h>

#include "HistoSvc.hh"
//#include "DatacardManager.hh"
//#include "MyEvent.hh"
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoSvc* HistoSvc::fInstance = 0;
HistoSvc* HistoSvc::GetInstance()
{
  if ( fInstance == 0 ) { fInstance = new HistoSvc(); }
  return fInstance;
}

HistoSvc::HistoSvc()
 :fRootOutputFile(0),ntupl(0)
{
  // histograms
  fHisto1DMap.clear();
  fHisto2DMap.clear();
  // ntuple
  ntupl = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoSvc::~HistoSvc()
{
  if ( fRootOutputFile ) delete fRootOutputFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoSvc::book()
{ 
 
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  //
  std::string fileName = "OutputFileRoot.root";
  fRootOutputFile = new TFile(fileName.c_str(),"RECREATE");
  if(!fRootOutputFile) {
    std::cout << " HistoSvc::book :"<<" problem creating the ROOT TFile "<< std::endl;
    return;
  }
  std::cout << " HistoSvc::book :"<<" output ROOT TFile "<<fileName<<" created"<< std::endl;

  std::string hname;

  /////////////////////////////// Target histograms
  Double_t Emin = 0.; Double_t Emax = 500.; Int_t nEbin = 100;
  hname = "Target_Yprofile";
  TH1D* hT1y = new TH1D(hname.c_str(), hname.c_str(), 19, 0.5, 19.5/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT1y;
  hname = "Target_YstripTime";
  TH1D* hT2y = new TH1D(hname.c_str(), hname.c_str(), 100, 0, 500./*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT2y;
  hname = "Target_YstripEne";
  TH1D* hT3y = new TH1D(hname.c_str(), hname.c_str(), nEbin, Emin, Emax/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT3y;
  hname = "Target_YtotEne";
  TH1D* hT4y = new TH1D(hname.c_str(), hname.c_str(), nEbin, Emin, Emax/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT4y;
  hname = "Target_Xprofile";
  TH1D* hT1x = new TH1D(hname.c_str(), hname.c_str(), 19, 0.5, 19.5/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT1x;
  hname = "Target_XstripTime";
  TH1D* hT2x = new TH1D(hname.c_str(), hname.c_str(), 100, 0, 500./*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT2x;
  hname = "Target_XstripEne";
  TH1D* hT3x = new TH1D(hname.c_str(), hname.c_str(), nEbin, Emin, Emax/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT3x;
  hname = "Target_XtotEne";
  TH1D* hT4x = new TH1D(hname.c_str(), hname.c_str(), nEbin, Emin, Emax/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT4x;
  hname = "Target_LeadingHitMap";
  TH2D* hT2D_1 = new TH2D(hname.c_str(), hname.c_str(), 19, 0.5, 19.5, 19, 0.5, 19.5/*CLHEP::MeV*/);
  fHisto2DMap[hname]=hT2D_1;
  hname = "Target_LeadingEneMap";
  TH2D* hT2D_2 = new TH2D(hname.c_str(), hname.c_str(), 19, 0.5, 19.5, 19, 0.5, 19.5/*CLHEP::MeV*/);
  fHisto2DMap[hname]=hT2D_2;
  hname = "Target_maxDt";
  TH1D* hT1D_1 = new TH1D(hname.c_str(), hname.c_str(), 100, -50., 50./*CLHEP::MeV*/);
  fHisto1DMap[hname]=hT1D_1;

  
  /////////////////////////////// ECal histograms 
  hname = "ECal_Etot";
  TH1D* h1 = new TH1D(hname.c_str(), hname.c_str(), 1100, 0., 1100./*CLHEP::MeV*/);
  fHisto1DMap[hname]=h1;

  hname = "ECal_HitEnergy";
  TH1D* h1_0 = new TH1D(hname.c_str(), hname.c_str(), 1100, 0., 1.1/*CLHEP::MeV*/);
  fHisto1DMap[hname]=h1_0;

  hname = "ECal_LeadingHitEnergy";
  TH1D* h1_1 = new TH1D(hname.c_str(), hname.c_str(), 1100, 0., 1.1/*CLHEP::MeV*/);
  fHisto1DMap[hname]=h1_1;

  hname = "ECal_HitTime";
  TH1D* h1_2 = new TH1D(hname.c_str(), hname.c_str(), 250, 0., 500.);
  fHisto1DMap[hname]=h1_2;

  hname = "ECal_LeadingHitTime";
  TH1D* h1_3 = new TH1D(hname.c_str(), hname.c_str(), 250, 0., 500.);
  fHisto1DMap[hname]=h1_3;

  hname = "ECal_HitMap";
  TH2D* h2 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h2;

  hname = "ECal_EneMap";
  TH2D* h3 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h3;

  hname = "ECal_LeadingHitMap";
  TH2D* h4 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h4;

  hname = "ECal_LeadingEneMap";
  TH2D* h5 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h5;

  /////////////////////////////// PVeto histograms 
  hname = "PVeto_Etot";
  TH1D* h6 = new TH1D(hname.c_str(), hname.c_str(), 1100, 0., 1100./*CLHEP::MeV*/);
  fHisto1DMap[hname]=h6;

  hname = "PVeto_HitEnergy";
  TH1D* h2_0 = new TH1D(hname.c_str(), hname.c_str(), 1100, 0., 1.1/*CLHEP::MeV*/);
  fHisto1DMap[hname]=h2_0;

  hname = "PVeto_LeadingHitEnergy";
  TH1D* h2_1 = new TH1D(hname.c_str(), hname.c_str(), 1100, 0., 1.1/*CLHEP::MeV*/);
  fHisto1DMap[hname]=h2_1;

  hname = "PVeto_HitTime";
  TH1D* h2_2 = new TH1D(hname.c_str(), hname.c_str(), 250, 0., 500.);
  fHisto1DMap[hname]=h2_2;

  hname = "PVeto_LeadingHitTime";
  TH1D* h2_3 = new TH1D(hname.c_str(), hname.c_str(), 250, 0., 500.);
  fHisto1DMap[hname]=h2_3;

  hname = "PVeto_HitMap";
  TH2D* h7 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h7;

  hname = "PVeto_EneMap";
  TH2D* h8 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h8;

  hname = "PVeto_LeadingHitMap";
  TH2D* h9 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h9;

  hname = "PVeto_LeadingEneMap";
  TH2D* h10 = new TH2D(hname.c_str(), hname.c_str(), 31, -1.5, 29.5, 31, -1.5, 29.5);
  fHisto2DMap[hname]=h10; 

  //////
  hname = "SAC_NHits";
  TH1D* hSAC_001 = new TH1D(hname.c_str(), hname.c_str(), 101, -1., 200.1/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hSAC_001; 
  hname = "SAC_NClusters";
  TH1D* hSAC_002 = new TH1D(hname.c_str(), hname.c_str(), 101, -1., 200.1/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hSAC_002; 
  hname = "SAC_Cluster_size";
  TH1D* hSAC_01 = new TH1D(hname.c_str(), hname.c_str(), 51, -0.5, 50.5/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hSAC_01; 
  hname = "SAC_Cluster_time";
  TH1D* hSAC_02 = new TH1D(hname.c_str(), hname.c_str(), 101, -2.5, 502.5/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hSAC_02; 
  hname = "SAC_Cluster_chId";
  TH1D* hSAC_03 = new TH1D(hname.c_str(), hname.c_str(), 101, -0.5, 100.5/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hSAC_03; 
  hname = "SAC_Cluster_energy";
  TH1D* hSAC_04 = new TH1D(hname.c_str(), hname.c_str(), 101, -2.5, 502.5/*CLHEP::MeV*/);
  fHisto1DMap[hname]=hSAC_04; 
  hname = "SAC_Cluster_XY";
  TH2D* hSAC_05 = new TH2D(hname.c_str(), hname.c_str(), 100, -250., 250., 100, -250., 250./*CLHEP::MeV*/);
  fHisto2DMap[hname]=hSAC_05; 
  hname = "SAC_Cluster_EwXY";
  TH2D* hSAC_06 = new TH2D(hname.c_str(), hname.c_str(), 100, -250., 250., 100, -250., 250./*CLHEP::MeV*/);
  fHisto2DMap[hname]=hSAC_06; 


  std::cout<<" N. of 1D histo booked is "<<fHisto1DMap.size()<<std::endl;

  std::cout<<" N. of 2D histo booked is "<<fHisto2DMap.size()<<std::endl;									  

  
  // create 1 ntuple in subdirectory "tuples"
  //
  ntupl = new TTree("U102", "PADME_Event");
  ntupl->Branch("NTevent"       , &(myEvt.NTNevent)      , "NTevent/I"       );
  ntupl->Branch("NTNCluster"    , &(myEvt.NTNCluster)    , "NTNCluster/I"    );
  ntupl->Branch("NTNPVetoTracks", &(myEvt.NTNPVetoTracks), "NTNPVetoTracks/I");
  ntupl->Branch("NTSACNHit"     , &(myEvt.NTSACNHit)     , "NTSACNHit/I"     );
  ntupl->Branch("NTNTarget"     , &(myEvt.NTNTarget)     , "NTNTarget/I"     );

  ntupl->Branch("NTECluster", &(myEvt.NTECluster), "NTECluster[NTNCluster]/D");
  ntupl->Branch("NTTCluster", &(myEvt.NTTCluster), "NTTCluster[NTNCluster]/D");

  ntupl->Branch("NTXCluster", &(myEvt.NTXCluster), "NTXCluster[NTNCluster]/D");
  ntupl->Branch("NTYCluster", &(myEvt.NTYCluster), "NTYCluster[NTNCluster]/D");
  
  ntupl->Branch("NTPVetoTrkFinger", &(myEvt.NTPVetoTrkFinger), "NTPVetoTrkFinger[NTNPVetoTracks]/I");
  ntupl->Branch("NTPVetoTrkTime"  , &(myEvt.NTPVetoTrkTime  ), "NTPVetoTrkTime[NTNPVetoTracks]/D"  );

 
  ntupl->Branch("NTSACE" , &(myEvt.NTSACE) , "NTSACE[NTSACNHit]/D" );
  ntupl->Branch("NTSACT" , &(myEvt.NTSACT) , "NTSACT[NTSACNHit]/D" );
  ntupl->Branch("NTSACCh", &(myEvt.NTSACCh), "NTSACCh[NTSACNHit]/I");

  ntupl->Branch("NTETarget" , &(myEvt.NTETarget) , "NTETarget/D" );



  /*
  // create 1 ntuple in subdirectory "tuples"
  //
  ntupl = new TTree("U102", "Envent");
  ntupl->Branch("Nevent", &(myEvt.NTNevent), "Nevent/I");
  ntupl->Branch("ETot", &(myEvt.NTEtot), "ETot/D");

  ntupl->Branch("IDProc", &(myEvt.NTIDProc), "IDProc/D");
  ntupl->Branch("PBeam", &(myEvt.NTPBeam), "PBeam/D");
  ntupl->Branch("PPrim", &(myEvt.NTPriBeam), "PPrim/D");
  ntupl->Branch("XBeam", &(myEvt.NTXBeam), "XBeam/D");
  ntupl->Branch("YBeam", &(myEvt.NTYBeam), "YBeam/D");
  ntupl->Branch("PosMomX", &(myEvt.PMomX), "PosMomX/D");
  ntupl->Branch("PosMomY", &(myEvt.PMomY), "PosMomY/D");
  ntupl->Branch("PosMomZ", &(myEvt.PMomZ), "PosMomZ/D");

  ntupl->Branch("NClusters", &(myEvt.NTNCluster), "NClusters/I");
  ntupl->Branch("NTracks",   &(myEvt.NTNTracks), "NTracks/I");
  ntupl->Branch("NHEPVetoTracks", &(myEvt.NTNHEPVetoTracks), "NHEPVetoTracks/I");

  ntupl->Branch("NPVetoTracks", &(myEvt.NTNPVetoTracks), "NPVetoTracks/I");
  ntupl->Branch("NEVetoTracks", &(myEvt.NTNEVetoTracks), "NEVetoTracks/I");

  ntupl->Branch("NSAC", &(myEvt.NTSACNHit), "NSAC/I");
  ntupl->Branch("NCal", &(myEvt.NTNCal), "NCal/I");
  ntupl->Branch("NLAV", &(myEvt.NTLAVNHit), "NLAV/I");
  ntupl->Branch("NTarget", &(myEvt.NTNTarget), "NTarget/I");

  ntupl->Branch("ESAC", (myEvt.NTSACE), "ESAC[100]/D");
  ntupl->Branch("TSAC", (myEvt.NTSACT), "TSAC[100]/D");
  ntupl->Branch("PTypeSAC", (myEvt.NTSACPType), "PTypeSAC[100]/D");
  ntupl->Branch("XSAC", (myEvt.NTSACX), "XSAC[100]/D");
  ntupl->Branch("YSAC", (myEvt.NTSACY), "YSAC[100]/D");
  ntupl->Branch("SACCh", (myEvt.NTSACCh), "SACCh[100]/I");


 ntupl->Branch("ELAV", (myEvt.NTLAVE), "ELAV[100]/D");
 ntupl->Branch("TLAV", (myEvt.NTLAVT), "TLAV[100]/D");
 ntupl->Branch("PTypeLAV", (myEvt.NTLAVPType), "PTypeLAV[100]/D");
 ntupl->Branch("XLAV", (myEvt.NTLAVX), "XLAV[100]/D");
 ntupl->Branch("YLAV", (myEvt.NTLAVY), "YLAV[100]/D");

  ntupl->Branch("EPartCal",     (myEvt.NTCalPartE),     "CalE[20]/D");
  ntupl->Branch("TPartCal",     (myEvt.NTCalPartT),     "CalT[20]/D");
  ntupl->Branch("PTypePartCal", (myEvt.NTCalPartPType), "CalPType[20]/I");
  ntupl->Branch("XPartCal",     (myEvt.NTCalPartX),     "CalX[20]/D");
  ntupl->Branch("YPartCal",     (myEvt.NTCalPartY),     "CalY[20]/D");

  ntupl->Branch("ECluster", (myEvt.NTECluster), "ECluster[20]/D"); 
  ntupl->Branch("QCluster", (myEvt.NTQCluster), "QCluster[20]/D");
  ntupl->Branch("XCluster", (myEvt.NTXCluster), "XCluster[20]/D");
  ntupl->Branch("YCluster", (myEvt.NTYCluster), "YCluster[20]/D");
  ntupl->Branch("ThCluster",(myEvt.NTThCluster),"ThCluster[20]/D");
  ntupl->Branch("M2Cluster",(myEvt.NTM2Cluster),"M2Cluster[20]/D");
  ntupl->Branch("TCluster", (myEvt.NTTCluster), "TCluster[20]/D");  //last modified
  ntupl->Branch("NClusCells",(myEvt.NTNClusCells), "NClusCells[20]/D");  //last modified

//  ntupl->Branch("ECell",    (myEvt.NTECell), "ECell[1000]/D");
//  ntupl->Branch("QCell",    (myEvt.NTQCell), "QCell[1000]/D");
//  ntupl->Branch("TCell",    (myEvt.NTTCell), "TCell[1000]/D");

//  ntupl->Branch("ETracker", (myEvt.NTETracker), "ETracker[100]/D");
//  ntupl->Branch("TrackerLay",(myEvt.NTTrackerLay), "TrackerLay[100]/D");
//  ntupl->Branch("TTracker", (myEvt.NTTrackerTime), "TTracker[100]/D");
//  ntupl->Branch("ZTracker", (myEvt.NTTrackerZ), "ZTracker[100]/D");
//  ntupl->Branch("XTracker", (myEvt.NTTrackerX), "XTracker[100]/D");
//  ntupl->Branch("YTracker", (myEvt.NTTrackerY), "YTracker[100]/D");

  ntupl->Branch("ETarget", &(myEvt.NTETarget), "ETarget/D");
  ntupl->Branch("TTarget", &(myEvt.NTTTarget), "TTarget/D");
  ntupl->Branch("XTarget", &(myEvt.NTXTarget), "XTarget/D");
  ntupl->Branch("YTarget", &(myEvt.NTYTarget), "YTarget/D");

  ntupl->Branch("HEPVetoTrEne" ,(myEvt.NTHEPVetoTrkEne),   "NTHEPVetoTrkEne[100]/D");
  ntupl->Branch("HEPVetoNFing" ,(myEvt.NTHEPVetoTrkFinger),"NTHEPVetoTrkFinger[100]/I");
  ntupl->Branch("HEPVetoTrTime",(myEvt.NTHEPVetoTrkTime),  "NTHEPVetoTrkTime[100]/D");
  ntupl->Branch("HEPVetoFingE", (myEvt.NTHEPVetoFingerE),  "NTHEPVetoFingE[100]/D");
  ntupl->Branch("HEPVetoX", (myEvt.NTHEPVetoX),  "NTHEPVetoX[100]/D");
  ntupl->Branch("HEPVetoY", (myEvt.NTHEPVetoY),  "NTHEPVetoY[100]/D");
  ntupl->Branch("HEPVetoClIndex",   (myEvt.NTHEPVetoClIndex),  "NTHEPVetoClIndex[100]/I");
  ntupl->Branch("HEPVetoECl",       (myEvt.NTHEPVetoECl),      "NTHEPVetoECl[100][10]/D");
  ntupl->Branch("HEPVetoTimeCl",    (myEvt.NTHEPVetoTimeCl),   "NTHEPVetoTimeCl[100][10]/D");

  ntupl->Branch("PVetoTrEne" ,(myEvt.NTPVetoTrkEne),   "NTPVetoTrkEne[100]/D");
  ntupl->Branch("PVetoNFing" ,(myEvt.NTPVetoTrkFinger),"NTPVetoTrkFinger[100]/I");
  ntupl->Branch("PVetoTrTime",(myEvt.NTPVetoTrkTime),  "NTPVetoTrkTime[100]/D");
  ntupl->Branch("PVetoFingE", (myEvt.NTPVetoFingerE),  "NTPVetoFingE[100]/D");
  ntupl->Branch("PVetoX",     (myEvt.NTPVetoX),        "NTPVetoX[100]/D");
  ntupl->Branch("PVetoY",     (myEvt.NTPVetoY),        "NTPVetoY[100]/D");
  ntupl->Branch("PVetoBarE",     (myEvt.NTPVetoBarEnergy),        "NTPVetoBarEnergy[100]/D");
  ntupl->Branch("PVetoBarT",     (myEvt.NTPVetoBarTime),        "NTPVetoBarTime[100]/D");

  ntupl->Branch("PVetoClIndex",   (myEvt.NTPVetoClIndex),  "NTPVetoClIndex[100]/I");
  ntupl->Branch("PVetoECl",       (myEvt.NTPVetoECl),      "NTPVetoECl[100][10]/D");
  ntupl->Branch("PVetoTimeCl",    (myEvt.NTPVetoTimeCl),   "NTPVetoTimeCl[100][10]/D");


  ntupl->Branch("EVetoTrEne" ,(myEvt.NTEVetoTrkEne),   "NTEVetoTrkEne[100]/D");
  ntupl->Branch("EVetoNFing" ,(myEvt.NTEVetoTrkFinger),"NTEVetoTrkFinger[100]/I");
  ntupl->Branch("EVetoTrTime",(myEvt.NTEVetoTrkTime),  "NTEVetoTrkTime[100]/D");
  ntupl->Branch("EVetoFingE", (myEvt.NTEVetoFingerE),  "NTEVetoFingE[100]/D");
  ntupl->Branch("EVetoX",     (myEvt.NTEVetoX),        "NTEVetoX[100]/D");
  ntupl->Branch("EVetoY",     (myEvt.NTEVetoY),        "NTEVetoY[100]/D");
  ntupl->Branch("EVetoClIndex",   (myEvt.NTEVetoClIndex),  "NTEVetoClIndex[100]/I");
  ntupl->Branch("EVetoECl",       (myEvt.NTEVetoECl),      "NTEVetoECl[100][10]/D");
  ntupl->Branch("EVetoTimeCl",    (myEvt.NTEVetoTimeCl),   "NTEVetoTimeCl[100][10]/D");


  //MySimEvent *mySim = (MyEvent::GetInstance())->GetSimEvent();
  //MyEventGenerator *myGen = (MyEvent::GetInstance())->GetGenEvent();
  
  if(IsTrackerRecoON==1){
    ntupl->Branch("NTNTrClus", &(myEvt.NTNTrClus),"NTNTrClus/I");
    ntupl->Branch("NTTrClusX", myEvt.NTTrClusX,"NTTrClusX[1000]/D");
    ntupl->Branch("NTTrClusY", myEvt.NTTrClusY,"NTTrClusY[1000]/D");
    ntupl->Branch("NTTrClusZ", myEvt.NTTrClusZ,"NTTrClusZ[1000]/D");
    ntupl->Branch("NTTrClusLayer", myEvt.NTTrClusLayer,"NTTrClusLayer[1000]/I");
  } 
  */

  std::cout << "\n----> Histogram file is opened in " << fileName << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoSvc::save()
{ 
  if (fRootOutputFile) {
    fRootOutputFile->Write();        // Writing the histograms to the file
    fRootOutputFile->Close();        // and closing the tree (and the file)
    std::cout << "\n----> Histogram Tree is saved \n" << std::endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoSvc::FillHisto(std::string hname, Double_t xbin, Double_t weight)
{
  if (fHisto1DMap.find(hname)==fHisto1DMap.end()) {
    std::cout << "---> warning from HistoSvc::FillHisto() : histo " << hname
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << std::endl;
    return;
  }
 if  (fHisto1DMap[hname]) {fHisto1DMap[hname]->Fill(xbin, weight); }
 else
   {
      std::cout << "---> warning from HistoSvc::FillHisto() : histo " << hname
           << " found in the map with a NULL pointer "
           << std::endl;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoSvc::FillHisto2(std::string hname, Double_t xbin, Double_t ybin, Double_t weight)
{
   if (fHisto2DMap.find(hname)==fHisto2DMap.end()) {
    std::cout << "---> warning from HistoSvc::FillHisto() : histo2D " << hname
           << " does not exist. (xbin=" << xbin << " ybin=" << ybin <<" weight=" << weight << ")"
           << std::endl;
    return;
  }
   if  (fHisto2DMap[hname]) {fHisto2DMap[hname]->Fill(xbin, ybin, weight); }
 else
   {
      std::cout << "---> warning from HistoSvc::FillHisto() : histo " << hname
           << " found in the map with a NULL pointer "
           << std::endl;
   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoSvc::Normalize(std::string hname, Double_t fac)
{
   if (fHisto1DMap.find(hname)!=fHisto1DMap.end()) {
     if  (fHisto1DMap[hname]) {
       fHisto1DMap[hname]->Scale(fac);
     }
     else{
       std::cout << "---> warning from HistoSvc::FillHisto() : histo " << hname
		 << " found in the map with a NULL pointer "
		 << std::endl;
     }
     //    std::cout << "---> warning from HistoSvc::FillHisto() : histo2D " << hname
     //           << ", to be normalized, does not exist. " << xbin << " ybin=" << ybin <<" weight=" << weight << ")"
     //           << std::endl;
    return;
   }
   else{
     if (fHisto2DMap.find(hname)!=fHisto2DMap.end()) {
       if  (fHisto2DMap[hname]) {
	 fHisto2DMap[hname]->Scale(fac);
       }
       else{
	 std::cout << "---> warning from HistoSvc::NormalizeHisto() : histo2D " << hname
		   << " found in the map with a NULL pointer "
		   << std::endl;
       }
       return;
     }
     else
       {
	 std::cout << "---> warning from HistoSvc::NormalizeHisto() : histo " << hname
		   << " NOT found in the map of 1D histos nor in the map of 2D histos"
		   << std::endl;
	 return;
       }
   }
}
   

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoSvc::FillNtuple()
{
  if(ntupl) ntupl->Fill();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


