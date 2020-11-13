#include "Riostream.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TObjArray.h"

#include "TRawEvent.hh"

#include "Configuration.hh"
#include "ECalMonitor.hh"
#include "TargetMonitor.hh"

int main(int argc, char* argv[])
{
  
  int c;
  int verbose = 0;
  int nevents = 0;

  TString inputFileName;
  TString outputFileName;
  TObjArray inputFileNameList;
  struct stat filestat;

  struct timespec now;
  
  // Connect to configuration handler
  Configuration* cfg = Configuration::GetInstance();

  // Parse options
  while ((c = getopt (argc, argv, "i:l:o:n:v:h")) != -1) {
    switch (c)
      {
      case 'i':
        inputFileNameList.Add(new TObjString(optarg));
        fprintf(stdout,"Added input data file '%s'\n",optarg);
	break;
      case 'l':
	if ( stat(Form(optarg),&filestat) == 0 ) {
	  fprintf(stdout,"Reading list of input files from '%s'\n",optarg);
	  std::ifstream inputList(optarg);
	  while( inputFileName.ReadLine(inputList) ){
	    if ( stat(Form(inputFileName.Data()),&filestat) == 0 ) {
	      inputFileNameList.Add(new TObjString(inputFileName.Data()));
	      fprintf(stdout,"Added input data file '%s'\n",inputFileName.Data());
	    } else {
	      fprintf(stdout,"WARNING: file '%s' is not accessible\n",inputFileName.Data());
	    }
	  }
	} else {
	  fprintf(stdout,"WARNING: file list '%s' is not accessible\n",optarg);
	}
        break;
      case 'o':
        outputFileName = optarg;
        fprintf(stdout,"Output PadmeMonitor file: '%s'\n",optarg);
	break;
      case 'n':
        if ( sscanf(optarg,"%d",&nevents) != 1 ) {
          fprintf (stderr, "Error while processing option '-n'. Wrong parameter '%s'.\n", optarg);
          exit(1);
        }
        if (nevents<0) {
          fprintf (stderr, "Error while processing option '-n'. Required %d events (must be >=0).\n", nevents);
          exit(1);
        }
	if (nevents) {
	  fprintf(stdout,"Will read first %d events in file\n",nevents);
	} else {
	  fprintf(stdout,"Will read all events in file\n");
	}
        break;
      case 'v':
        if ( sscanf(optarg,"%d",&verbose) != 1 ) {
          fprintf (stderr, "Error while processing option '-v'. Wrong parameter '%s'.\n", optarg);
          exit(1);
        }
        if (verbose<0) {
          fprintf (stderr, "Error while processing option '-v'. Verbose level set to %d (must be >=0).\n", verbose);
          exit(1);
        }
        break;
      case 'h':
        fprintf(stdout,"\nReadTest [-i input root file] [-l list of input files] [-v verbosity] [-h]\n\n");
        fprintf(stdout,"  -i: define an input file in root format\n");
        fprintf(stdout,"  -l: define a list of input files\n");
        fprintf(stdout,"  -o: define name of PadmeMonitor output file\n");
        fprintf(stdout,"  -n: define number of events to read from input file (0: all events)\n");
        fprintf(stdout,"  -v: define verbose level\n");
        fprintf(stdout,"  -h: show this help message and exit\n\n");
        exit(0);
      case '?':
        if (optopt == 'v') {
          // verbose with no argument: increase verbose level by 1
          verbose++;
          break;
        } else if (optopt == 'i' || optopt == 'l' || optopt == 'o')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint(optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
        exit(1);
      default:
        abort();
      }
  }

  // Set general verbose level
  if (verbose>0) fprintf(stdout,"Set verbose level to %d\n",verbose);
  cfg->SetVerbose(verbose);

  // Check if at least one input file was specified
  if ( inputFileNameList.GetEntries() == 0 ) {
    perror(Form("ERROR No Input File specified"));
    exit(1);
  }

  // Create chain of input files
  fprintf(stdout,"=== === === Chain of input files === === ===\n");
  TChain* inputChain = new TChain("RawEvents");
  for (Int_t iFile = 0; iFile < inputFileNameList.GetEntries(); iFile++) {
    fprintf(stdout,"%4d %s\n",iFile,((TObjString*)inputFileNameList.At(iFile))->GetString().Data());
    inputChain->AddFile(((TObjString*)inputFileNameList.At(iFile))->GetString());
  }
  if (inputChain->GetEntries() == 0) {
    perror(Form("ERROR No events found for tree 'RawEvents' in input chain"));
    exit(1);
  }

  // Get some info about the input chain
  Int_t runNEntries = inputChain->GetEntries();
  std::cout << "Found Tree 'RawEvents' with " << runNEntries << " entries" << std::endl;
  //for(Int_t i=0; i < inputChain->GetListOfBranches()->GetEntries(); i++) {
  //  std::cout << "Branch " << i << " is " << inputChain->GetListOfBranches()->At(i)->GetName() << std::endl;
  //}
  TRawEvent* rawEv = new TRawEvent();
  inputChain->SetBranchAddress("RawEvent",&rawEv);

  // Set number of events to read
  Int_t ntoread = runNEntries;
  if (nevents && nevents<runNEntries) {
    ntoread = nevents;
    printf("Reading first %d events\n",ntoread);
  }

  // Set and create output file
  if (! outputFileName.IsNull()) cfg->SetOutputFile(outputFileName);
  FILE* outf = fopen(cfg->OutputFile(),"w");
  fprintf(outf,"# OnlineMonitor - %s\n",cfg->FormatTime(time(0)));
  fclose(outf);

  // Initialize anlyses
  ECalMonitor* ecal_mon = new ECalMonitor();
  TargetMonitor* target_mon = new TargetMonitor();

  if( clock_gettime(CLOCK_REALTIME,&now) == -1 ) {
    perror("clock gettime");
    exit(EXIT_FAILURE);
  }
  TTimeStamp t_start = TTimeStamp(now.tv_sec,now.tv_nsec);
  printf("=== OnlineMonitor starting on %s\n",cfg->FormatTime(now.tv_sec));

  for(Int_t iev=0;iev<ntoread;iev++){

    // Read event
    //printf("Reading event %d\n",iev);
    inputChain->GetEntry(iev);
    //printf("Event %d read\n",iev);

    // Show event header when in verbose mode or once in a while
    if ( (cfg->Verbose()>0) || (iev%cfg->DebugScale() == 0) ) {
      TTimeStamp tts = rawEv->GetEventAbsTime();
      printf("%7d Run %7d Event %7d Time %8d-%06d.%09d RunTime %12llu TrigMask 0x%02x EvtStatus 0x%04x Boards %2d MissBoard 0x%04x\n",
	     iev,rawEv->GetRunNumber(),rawEv->GetEventNumber(),tts.GetDate(),tts.GetTime(),tts.GetNanoSec(),
	     rawEv->GetEventRunTime(),(rawEv->GetEventTrigMask() & 0xff),(rawEv->GetEventStatus() & 0xffff),
	     rawEv->GetNADCBoards(),(rawEv->GetMissingADCBoards() & 0xffff));
    }

    // Save event information
    cfg->SetRunNumber(rawEv->GetRunNumber());
    cfg->SetEventNumber(rawEv->GetEventNumber());
    cfg->SetEventAbsTime(rawEv->GetEventAbsTime());
    cfg->SetEventRunTime(rawEv->GetEventRunTime());
    cfg->SetEventTrigMask(rawEv->GetEventTrigMask());
    cfg->SetEventStatus(rawEv->GetEventStatus());

    // Call "start of event" procedures for all detectors
    ecal_mon->StartOfEvent();
    target_mon->StartOfEvent();

    // Only accept BTF events
    //if ( (rawEv->GetEventTrigMask() & (1 << 0)) == 0 ) continue;

    // Only accept cosmics events
    //if ( (rawEv->GetEventTrigMask() & (1 << 1)) == 0 ) continue;

    // Loop over boards
    UChar_t nBoards = rawEv->GetNADCBoards();
    for(UChar_t b=0;b<nBoards;b++){

      // Get board info
      TADCBoard* adcB = rawEv->ADCBoard(b);
      UChar_t boardId = adcB->GetBoardId();
      UInt_t activeMsk = adcB->GetActiveChannelMask();
      UInt_t acceptMsk = adcB->GetAcceptedChannelMask();
      UChar_t nChn = adcB->GetNADCChannels();

      // Loop over channels
      for(UChar_t c=0;c<nChn;c++){
	TADCChannel* chn = adcB->ADCChannel(c);
	UChar_t chNr = chn->GetChannelNumber();
	if (activeMsk & (1 << chNr)) {
	  if (acceptMsk & (1 << chNr)) {
	    if ( (boardId <= 9) || (boardId >= 14 && boardId <= 23) ) {
	      ecal_mon->Analyze(boardId,chNr,chn->GetSamplesArray());
	    } else if (boardId >= 10 && boardId <= 12) {
	      // PVeto
	    } else if (boardId == 13) {
	      // HEPVeto
	    } else if (boardId >= 24 && boardId <= 26) {
	      // EVeto
	    } else if (boardId == 27) {
	      // SAC + Cosmics pads
	    } else if (boardId == 28) {
	      // Target
	      target_mon->Analyze(boardId,chNr,chn->GetSamplesArray());
	    }
	  }
	}
      }

    }

    // Clear event
    rawEv->Clear("C");

    // Call "end of event" procedures for all detectors
    ecal_mon->EndOfEvent();
    target_mon->EndOfEvent();

  } // End loop over events

  ecal_mon->Finalize();
  target_mon->Finalize();

  if( clock_gettime(CLOCK_REALTIME,&now) == -1 ) {
    perror("clock gettime");
    exit(EXIT_FAILURE);
  }
  TTimeStamp t_end = TTimeStamp(now.tv_sec,now.tv_nsec);
  printf("=== OnlineMonitor ending on %s\n",cfg->FormatTime(now.tv_sec));

  Double_t t_start_f = 1.*t_start.GetSec()+1.E-9*t_start.GetNanoSec();
  Double_t t_end_f = 1.*t_end.GetSec()+1.E-9*t_end.GetNanoSec();
  Double_t t_run_f = t_end_f-t_start_f;
  printf("Total run time %.3fs\n",t_run_f);
  if (t_run_f>0.) printf("Event processing time %.3fms/evt\n",1000.*t_run_f/ntoread);

  delete rawEv;
  delete ecal_mon;
  delete target_mon;

  exit(0);

}