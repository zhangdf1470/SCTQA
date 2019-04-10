#!/bin/bash

FileName="../FASER_SCTmodule_QA.txt"
inPath="/Users/dengfeng/Desktop/WorkDir/FASER/"

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

  if [[ ${ModuleID} != "20220040200538" ]] 
  then
    continue
  fi 
  echo ${ModuleID}: ${GainRun1}, ${GainRun2}, ${GainRun3}
  outPath="./outPut/"${ModuleID}/
  echo $outPath

  cp ${inPath}/${ModuleID}/feb-802932E1-000${GainRun1}.root .
  cp ${inPath}/${ModuleID}/feb-802932E1-000${GainRun2}.root .
  cp ${inPath}/${ModuleID}/feb-802932E1-000${GainRun3}.root .
  mkdir -p $outPath
  
  ./ana ${GainRun1} ${GainRun2} ${GainRun3}  | grep -v "Info" > gain.log 2>&1

  rm feb-802932E1-000${GainRun1}.root
  rm feb-802932E1-000${GainRun2}.root
  rm feb-802932E1-000${GainRun3}.root

  mv *.pdf $outPath
  mv gain.log $outPath

done < ${FileName}
