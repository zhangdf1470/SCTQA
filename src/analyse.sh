#!/bin/bash
if [[ "$#" != "5" ]] && [[ "$#" != "6" ]]; then
  echo "Usage: analyse.sh NOISE_RUN_NUMBER GAIN_RUN_1 GAIN_RUN_2 GAIN_RUN_3 MODULE_ID"
  echo "Example: ./analyse.sh [-w] 272 273 274 275 MODULE_ID"
  echo "    -w: If you give this option, you will be prompted to confirm after each file,"
  echo "        this allows you to convert the files that are already done while continuing"
  echo "        the measurements."
  exit
fi

shopt -s expand_aliases
source $HOME/.bashrc
set -o errexit
set -o nounset

err_report() {
  echo -e "\e[31mAn error occured during the script (in line $1)\e[39m"
  exit 1
}
trap 'err_report $LINENO' ERR

WAIT_AFTER_EACH_FILE=0
if [[ "$1" == "-w" ]]; then
  shift
  WAIT_AFTER_EACH_FILE=1
  echo -e "\e[33mThe script will wait for your input after each file before continuing.\e[39m"
fi

BASE_FOLDER=/home/faser/data
cd $BASE_FOLDER

NOISE_SCAN_RN=$1
NOISE_SCAN_FILENAME=$(printf "feb-802932E1-%06d" $NOISE_SCAN_RN)
GAIN_1_RN=$2
GAIN_1_FILENAME=$(printf "feb-802932E1-%06d" $GAIN_1_RN)
GAIN_2_RN=$3
GAIN_2_FILENAME=$(printf "feb-802932E1-%06d" $GAIN_2_RN)
GAIN_3_RN=$4
GAIN_3_FILENAME=$(printf "feb-802932E1-%06d" $GAIN_3_RN)
MODULE_ID=$5
MODULE_FOLDER=/home/faser/cernbox/data/${MODULE_ID}

# Check if all files exist
if [[ "${WAIT_AFTER_EACH_FILE}" != "1" ]]; then
  if [ ! -f ${NOISE_SCAN_FILENAME}.mdf ]; then
    echo -e "\e[31mThe file for noise scan results does not exist!\e[39m"
    exit 1
  fi
  if [ ! -f ${GAIN_1_FILENAME}.mdf ]; then
    echo -e "\e[31mThe file for gain at 1.5fC results does not exist!\e[39m"
    exit 1
  fi
  if [ ! -f ${GAIN_2_FILENAME}.mdf ]; then
    echo -e "\e[31mThe file for gain at 2.0fC results does not exist!\e[39m"
    exit 1
  fi
  if [ ! -f ${GAIN_3_FILENAME}.mdf ]; then
    echo -e "\e[31mThe file for gain at 2.5fC results does not exist!\e[39m"
    exit 1
  fi
  if [ ! -d ${MODULE_FOLDER} ]; then
    echo -e "\e[31mThe module folder (${MODULE_FOLDER}) does not exist!\e[39m"
    exit 1
  fi
fi

echo -e "Please check if the following settings are correct:"
echo -e "Noise Scan:     \e[32m$NOISE_SCAN_FILENAME\e[39m"
echo -e "Gain for 1.5fC: \e[32m$GAIN_1_FILENAME\e[39m"
echo -e "Gain for 2.0fC: \e[32m$GAIN_2_FILENAME\e[39m"
echo -e "Gain for 2.5fC: \e[32m$GAIN_3_FILENAME\e[39m"
echo -e "Module folder:  \e[32m$MODULE_FOLDER\e[39m"

read -p "Is this correct? [y/N] " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
  for FILENAME in {$NOISE_SCAN_FILENAME,${GAIN_1_FILENAME},${GAIN_2_FILENAME},${GAIN_3_FILENAME}}; do
    if [ -f "${FILENAME}.root" ]; then
      echo -e "\e[32mSkipped converting ${FILENAME} (already exists)!\e[39m"
      if [ -f "${MODULE_FOLDER}/${FILENAME}.root" ]; then
        if cmp --silent ${FILENAME}.root "${MODULE_FOLDER}/${FILENAME}.root"; then
          echo -e "\e[32mSkipped copying ${FILENAME} (already in cernbox)!\e[39m"
        else
          echo -e "\e[31mFile exists but is not the same, copy it again!\e[39m"
          cp ${FILENAME}.root ${MODULE_FOLDER}/
        fi
      else
        cp ${FILENAME}.root ${MODULE_FOLDER}/
      fi
    else
      echo -e "\e[32mConverting ${FILENAME}!\e[39m"
      conv ${FILENAME} >> conv_${MODULE_ID}.log
      if grep -i "error" conv_${MODULE_ID}.log; then
        echo -e "\e[31mThe data file (${FILENAME}) is corrupted!\e[39m"
        exit 1
      fi
      cp ${FILENAME}.root ${MODULE_FOLDER}/
    fi
    if [[ "${WAIT_AFTER_EACH_FILE}" == "1" ]] && [[ "${FILENAME}" != "${GAIN_3_FILENAME}" ]]; then
      read -p $'\e[33mPress any char to continue with the next file (pressing a will disable waiting)!\e[39m ' -n 1 -r
      if [[ $REPLY =~ ^[Aa]$ ]]; then
        echo -e "\e[33mOk, not waiting any more!\e[39m"
        WAIT_AFTER_EACH_FILE="0"
      fi
    fi
  done

  echo -e "\e[32mConverted all the files and copied to module folder!\e[39m"
  echo
  echo -e "\e[32mRun analysis (in ${MODULE_FOLDER})!\e[39m"
  cd ${MODULE_FOLDER}
  ana ${NOISE_SCAN_RN} > noise_scan.log 2>&1
  ana ${GAIN_1_RN} ${GAIN_2_RN} ${GAIN_3_RN} > gain.log 2>&1
  echo -e "\e[32mNoise scan results:\e[39m"
  cat noise_scan.log | grep --after-context=3 "=== Noise & Vt50(@Gain=50mV/fC) =="
  echo -e "\n"
  echo -e "\e[32m3 point gain results:\e[39m"
  cat gain.log | grep --after-context=3 "=== Gain & Noise ============="
  cat gain.log | grep --after-context=9 "=== Bad strips ==============="
else
  echo -e "\e[31mCheck your input and run again.\e[39m"
fi
echo
