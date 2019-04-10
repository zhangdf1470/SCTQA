# Author: Dengfeng Zhang(dengfeng.zhang@cern.ch)

### Setup enviroment
```
source setup.sh
```

### FASER_SCTmodule_QA.txt contains Module IDs and their measurement results(leakage current, gain, noise, bad strips).

### Read FASER_SCTmodule_QA.txt to a Root file for analysis:
```
  root -l -b -q ReadFASERSCTModule.cxx
```

### Analyse results in FASER_SCTmodule_QA.txt and make plots:
```
  root -l -b -q AnalyseFASERSCTModule.cxx
```

### Analyse the collected data of each Module:
```
  source analyse.sh
```

### Make the plots of distribution of gain/nosie/offset of each strip, this step has been included in analyse.sh:
```
  root -l -b -q 'DrawHists.cxx(inFileName)'
```
