#!/bin/bash

FileName="FASER_SCTmodule_QA.txt"
if [ ! -e ${FileName} ]
then
  echo $FileName does not exist!!!!
  return 
fi

inPath="/eos/home-f/fasersct/data/"


while read Line
do
  array=(${Line//  /}) 
  ModuleID=${array[0]}
  if [[ ${ModuleID:0:1} == "#" ]] || [[ ${ModuleID:0:8} == "ModuleID" ]]
  then
    continue
  fi
  GainRun1=${array[7]}
  GainRun2=${array[8]}
  GainRun3=${array[9]}

  #if [[ ${ModuleID} != "20220040200538" ]]
  ##if [[ ${ModuleID} != "20220170200413" ]]
  #then
  #  continue
  #fi

  if [ ! -d ${inPath}/${ModuleID}/ ]
  then
    echo inPath: ${inPath}/${ModuleID}/ does not exist!!!!
    return 
  fi

  FileGainRun1=$(printf "feb-802932E1-%06d.root" ${GainRun1})
  FileGainRun2=$(printf "feb-802932E1-%06d.root" ${GainRun2})
  FileGainRun3=$(printf "feb-802932E1-%06d.root" ${GainRun3})

  if [ ! -e ${inPath}/${ModuleID}/${FileGainRun1} ]
  then
    echo inFile: ${inPath}/${ModuleID}/${FileGainRun1} does not exist!!!!
    return 
  fi
  if [ ! -e ${inPath}/${ModuleID}/${FileGainRun2} ]
  then
    echo inFile: ${inPath}/${ModuleID}/${FileGainRun2} does not exist!!!!
    return 
  fi
  if [ ! -e ${inPath}/${ModuleID}/${FileGainRun3} ]
  then
    echo inFile: ${inPath}/${ModuleID}/${FileGainRun3} does not exist!!!!
    return 
  fi

  echo ${ModuleID}: ${GainRun1}, ${GainRun2}, ${GainRun3}
  cp ${inPath}/${ModuleID}/${FileGainRun1} .
  cp ${inPath}/${ModuleID}/${FileGainRun2} .
  cp ${inPath}/${ModuleID}/${FileGainRun3} .

  ./src/ana ${GainRun1} ${GainRun2} ${GainRun3}  | grep -v "Info" > gain.log 2>&1

  root -l -b -q 'DrawHists.cxx("3PointsMeasurement.root")'

  rm ${FileGainRun1}
  rm ${FileGainRun2}
  rm ${FileGainRun3}

  outPath=$(pwd)/outPut/${ModuleID}/
  outPath=${outPath/user/work}
  if [ ! -e $outPath ]
  then
    mkdir -p $outPath
  fi
  echo outPath: $outPath

  
  mv *.pdf $outPath
  mv gain.log $outPath
  mv 3PointsMeasurement.root $outPath/3PointsMeasurement_${GainRun1}_${GainRun2}_${GainRun3}.root

done < ${FileName}
