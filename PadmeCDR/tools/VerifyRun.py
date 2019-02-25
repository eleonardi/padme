#!/usr/bin/python -u

import os
import re
import sys
import time
import getopt
import subprocess

# List of available sites
SITE_LIST = [ "LNF", "CNAF" , "DAQ" ]

# User running CDR
CDR_USER = os.environ['USER']

# Access information for DAQ data server
DAQ_USER = "daq"
DAQ_KEYFILE = "/home/%s/.ssh/id_rsa_cdr"%CDR_USER
DAQ_SERVERS = [ "l1padme3", "l1padme4" ]

# Path to adler32 command on DAQ data server
DAQ_ADLER32_CMD = "/home/daq/DAQ/tools/adler32"

# SRM addresses for storage elements at LNF and CNAF
LNF_SRM = "srm://atlasse.lnf.infn.it:8446/srm/managerv2?SFN=/dpm/lnf.infn.it/home/vo.padme.org"
CNAF_SRM = "srm://storm-fe-archive.cr.cnaf.infn.it:8444/srm/managerv2?SFN=/padmeTape"

def print_help():
    print 'RestoreRun -R run_name [-S src_site] [-D dst_site] [-d dst_dir] [-Y year] [-h]'
    print '  -R run_name     Name of run to recover'
    print '  -S src_site     Source site. Default: CNAF. Available %s'%SITE_LIST
    print '  -D dst_site     Destination site. Default: LNF. Available %s'%SITE_LIST
    print '  -s daq_server   Name of data server if src_site or dst_site is DAQ. Available %s'%DAQ_SERVERS
    print '  -Y year         Specify year of data taking to copy. Default: year from run name'
    print '  -c              Enable checksum verification (very time consuming!)'
    print '  -v              Enable verbose mode (repeat to increase level)'
    print '  -h              Show this help message and exit'

def get_checksum_lnf(file):
    a32 = ""
    cmd = "gfal-sum %s%s adler32"%(LNF_SRM,file);
    for line in run_command(cmd):
        try:
            (fdummy,a32) = line.rstrip().split()
        except:
            a32 = ""
    return a32

def get_checksum_cnaf(file):
    a32 = ""
    cmd = "gfal-sum %s%s adler32"%(CNAF_SRM,file);
    for line in run_command(cmd):
        try:
            (fdummy,a32) = line.rstrip().split()
        except:
            a32 = ""
    return a32

def get_checksum_daq(server,file):
    a32 = ""
    cmd = "ssh -n -i %s -l %s %s %s %s"%(DAQ_KEYFILE,DAQ_USER,server,DAQ_ADLER32_CMD,file)
    for line in run_command(cmd):
        try:
            (a32,fdummy) = line.rstrip().split()
        except:
            a32 = ""
    return a32

def main(argv):

    run = ""
    src_site = "CNAF"
    src_srm = ""
    src_dir = ""
    src_string = ""
    dst_site = "LNF"
    dst_srm = ""
    dst_dir = ""
    dst_string = ""
    daq_srv = ""
    daq_ssh = ""
    year = ""
    fake = False
    checksum = False
    verbose = 0

    try:
        opts,args = getopt.getopt(argv,"R:S:D:s:Y:cfvh")
    except getopt.GetoptError:
        print_help()
        sys.exit(2)

    for opt,arg in opts:
        if opt == '-h':
            print_help()
            sys.exit()
        elif opt == '-R':
            run = arg
        elif opt == '-S':
            if (not arg in SITE_LIST):
                print "ERROR - Invalid source site %s"%arg
                print_help()
                sys.exit(2)
            src_site = arg
        elif opt == '-D':
            if (not arg in SITE_LIST):
                print "ERROR - Invalid destination site %s"%arg
                print_help()
                sys.exit(2)
            dst_site = arg
        elif opt == '-s':
            if (not arg in DAQ_SERVERS):
                print "ERROR - Invalid DAQ data server %s"%arg
                print_help()
                sys.exit(2)
            daq_srv = arg
        elif opt == '-Y':
            year = arg
        elif opt == '-f':
            fake = True
        elif opt == '-c':
            checksum = True
        elif opt == '-v':
            verbose += 1

    if (not run):
        print "ERROR - No run name specified"
        print_help()
        sys.exit(2)

    if (not year):
        m = re.match("run_\d+_(\d\d\d\d)\d\d\d\d_\d\d\d\d\d\d",run)
        if m:
            year = m.group(1)
        else:
            print "ERROR - No year specified and unable to extract year from run name %s"%run
            print_help()
            sys.exit(2)

    if (src_site == dst_site):
        print "ERROR - Source and destination sites are the same: %s and %s"%(src_site,dst_site)
        print_help()
        sys.exit(2)

    if ( (src_site == "DAQ" or dst_site == "DAQ") and daq_srv == "" ):
        print "ERROR - One site is DAQ but no data server specified"
        print_help()
        sys.exit(2)

    # Define command to access DAQ data server
    daq_ssh = "ssh -n -i %s -l %s %s"%(DAQ_KEYFILE,DAQ_USER,daq_srv)

    # Define top directory, identification string, and SRM (if needed) of source and destination sites
    # Source and destination dirs are always the official directory of desired run in given year
    if (src_site == "DAQ"):
        src_dir = "/data/DAQ/%s/rawdata/%s"%(year,run)
        src_string = "%s(%s)"%(src_site,daq_srv)
    else:
        src_dir = "/daq/%s/rawdata/%s"%(year,run)
        src_string = src_site
        if (src_site == "LNF"):
            src_srm = LNF_SRM
        elif (src_site == "CNAF"):
            src_srm = CNAF_SRM
    if (dst_site == "DAQ"):
        dst_dir = "/data/DAQ/%s/rawdata/%s"%(year,run)
        dst_string = "%s(%s)"%(dst_site,daq_srv)
    else:
        dst_dir = "/daq/%s/rawdata/%s"%(year,run)
        dst_string = dst_site
        if (dst_site == "LNF"):
            dst_srm = LNF_SRM
        elif (dst_site == "CNAF"):
            dst_srm = CNAF_SRM

    if verbose:
        print
        print "=== VerifyRun %s between %s and %s ==="%(run,src_string,dst_string)
        if checksum:
            print "WARNING - Checksum is enabled: verification will take a long time..."

    # Get list of files at source site
    src_file_list = []
    src_file_size = {}
    src_missing = False
    if ( src_site == "DAQ" ):
        cmd = "%s \'( ls -l %s )\'"%(daq_ssh,src_dir)
    else:
        cmd = "gfal-ls -l %s%s"%(src_srm,src_dir)
    for line in run_command(cmd):
        if ( re.match("^ls: cannot access ",line) or re.match("^gfal-ls error: ",line) ):
            src_missing = True
            if verbose:
                print line.rstrip()
                print "%s - run %s is (probably) missing from %s"%(now_str(),run,src_site)
        m = re.match("^\s*\S+\s+\S+\s+\S+\s+\S+\s+(\d+)\s+\S+\s+\S+\s+\S+\s+(\S+)\s*$",line.rstrip())
        if (m):
            src_file_list.append(m.group(2))
            src_file_size[m.group(2)] = int(m.group(1))
    if verbose: print "%s - at %-13s run %s contains %d files"%(now_str(),src_string,run,len(src_file_list))

    # Get list of files at destination site
    dst_file_list = []
    dst_file_size = {}
    dst_missing = False
    if ( dst_site == "DAQ" ):
        cmd = "%s \'( cd %s; ls -l )\'"%(daq_ssh,dst_dir)
    else:
        cmd = "gfal-ls -l %s%s"%(dst_srm,dst_dir)
    for line in run_command(cmd):
        if ( re.match("^ls: cannot access ",line) or re.match("^gfal-ls error: ",line) ):
            dst_missing = True
            if verbose:
                print line.rstrip()
                print "%s - run %s is (probably) missing from %s"%(now_str(),run,dst_site)
        m = re.match("^\s*\S+\s+\S+\s+\S+\s+\S+\s+(\d+)\s+\S+\s+\S+\s+\S+\s+(\S+)\s*$",line.rstrip())
        if (m):
            dst_file_list.append(m.group(2))
            dst_file_size[m.group(2)] = int(m.group(1))
    if verbose: print "%s - at %-13s run %s contains %d files"%(now_str(),dst_string,run,len(dst_file_list))

    if (src_missing or dst_missing):
        if (src_missing and dst_missing):
            print "=== WARNING: Run %s is missing at %s and %s ==="%(run,src_string,dst_string)
        elif src_missing:
            print "=== WARNING: Run %s is missing at %s ==="%(run,src_string)
        elif dst_missing:
            print "=== WARNING: Run %s is missing at %s ==="%(run,dst_string)
        sys.exit()

    if (len(src_file_list) != len(dst_file_list)):
        if verbose: print "%s - run %s has %d files at %s and %d files at %s"%(now_str(),run,len(src_file_list),src_string,len(dst_file_list),dst_string)

    # Merge source and destination lists removing duplicates
    file_list = list(set(src_file_list).union(set(dst_file_list)))
    file_list.sort()

    # Check file lists for differences
    if (verbose > 1): print "%s - Starting verification of run %s (%d files) between %s and %s"%(now_str(),run,len(file_list),src_site,dst_site)
    warnings = 0
    for rawfile in file_list:

        # Check if file is at source site
        if not rawfile in src_file_list:
            warnings += 1
            if verbose: print "%s - not at %s"%(rawfile,src_site)

        # Check if file is at destination site
        elif not rawfile in dst_file_list:
            warnings += 1
            if verbose: print "%s - not at %s"%(rawfile,dst_site)

        # Check if files have the same size
        elif src_file_size[rawfile] != dst_file_size[rawfile]:
            warnings += 1
            if verbose: print "%s - file sizes are different: %d at %s vs. %d at %s"%(rawfile,src_file_size[rawfile],src_site,dst_file_size[rawfile],dst_site)

        else:

            # Verify checksum only if requested by the user
            if checksum:

                # Get checksum at source site
                src_checksum = ""
                if (src_site == "DAQ"):
                    src_checksum = get_checksum_daq(daq_srv,"%s/%s"%(src_dir,rawfile))
                elif (src_site == "LNF"):
                    src_checksum = get_checksum_lnf("%s/%s"%(src_dir,rawfile))
                elif (src_site == "CNAF"):
                    src_checksum = get_checksum_cnaf("%s/%s"%(src_dir,rawfile))

                # Get checksum at destination site
                dst_checksum = ""
                if (dst_site == "DAQ"):
                    dst_checksum = get_checksum_daq(daq_srv,"%s/%s"%(dst_dir,rawfile))
                elif (dst_site == "LNF"):
                    dst_checksum = get_checksum_lnf("%s/%s"%(dst_dir,rawfile))
                elif (dst_site == "CNAF"):
                    dst_checksum = get_checksum_cnaf("%s/%s"%(dst_dir,rawfile))

                # Check if checksums are consistent
                if (src_checksum == "" and dst_checksum == ""):
                    warnings += 1
                    if verbose: print "%s - unable to get checksum at %s and %s"%(rawfile,src_site,dst_site)
                elif (src_checksum == ""):
                    warnings += 1
                    if verbose: print "%s - unable to get checksum at %s - checksum at %s is %s"%(rawfile,src_site,dst_site,dst_checksum)
                elif (dst_checksum == ""):
                    warnings += 1
                    if verbose: print "%s - unable to get checksum at %s - checksum at %s is %s"%(rawfile,dst_site,src_site,src_checksum)
                elif (src_checksum != dst_checksum):
                    warnings += 1
                    if verbose: print "%s - checksums are different: %s at %s vs. %s at %s"%(rawfile,src_checksum,src_site,dst_checksum,dst_site)
                else:
                    if (verbose > 1): print "%s - OK - size %d checksum %s"%(rawfile,src_file_size[rawfile],src_checksum)

            else:
                if (verbose > 1): print "%s - OK - size %d"%(rawfile,src_file_size[rawfile])

    if warnings:
        print "=== WARNING: Run %s DOES NOT MATCH between %s and %s ==="%(run,src_string,dst_string)
    else:
        print "=== Run %s matches between %s and %s ==="%(run,src_string,dst_string)

def run_command(command):
    #print "> %s"%command
    p = subprocess.Popen(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,shell=True)
    return iter(p.stdout.readline, b'')

def now_str():
    return time.strftime("%Y-%m-%d %H:%M:%S",time.gmtime())

# Execution starts here
if __name__ == "__main__":
   main(sys.argv[1:])
