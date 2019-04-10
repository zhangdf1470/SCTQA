# Author: Dengfeng Zhang(dengfeng.zhang@cern.ch)

## Setup enviroment
```
source setup.sh
```

## FASER_SCTmodule_QA.txt contains Module IDs and their measurement results(leakage current, gain, noise, bad strips)

## Read FASER_SCTmodule_QA.txt to a Root file for analysis:
```
  root -l -b -q ReadFASERSCTModule.cxx
```

## Analyse results in FASER_SCTmodule_QA.txt and make plots:
```
  root -l -b -q AnalyseFASERSCTModule.cxx
```

