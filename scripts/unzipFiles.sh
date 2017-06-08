#!/bin/bash
capturesZippedFolder="CapturasV2"
unzippedCapturesParentFolderName="CapturasV2"
berny="Berny"
delia="Delia"
vortega="Vortega"
luis="Luis"
ivan="ivan"
cervantes="Cervantes"

os=`uname`
if [ "$os" = "Linux" ]; then
  pardinas="Pardi├▒as"
else
  pardinas="Pardi+%A6as"
fi

pardinasRenamed="Pardinas"
userId=0

echo "+++++++++ 1) FINISH: setup +++++++++"

#clean previous folder
rm -rfv ../$unzippedCapturesParentFolderName
rm -rfv $unzippedCapturesParentFolderName

#clean previous folder
for i in `seq 4 11`; do
  rm -rfv ../networkTrafficCapturesBuildDataSet/user$i
  rm -rfv ../networkTrafficCapturesEvaluationDataSet/user$i
  rm -rfv ../networkTrafficCapturesTrainDataSet/user$i
done

echo "+++++++++ 2) FINISH: clean previous folders +++++++++"

# unzip captures on the same folder that the script is
unzip ../$capturesZippedFolder.zip

echo "+++++++++ 1) FINISH: unzipping main folder +++++++++"

# remove folders that do not cotain usefule information
rm $unzippedCapturesParentFolderName/DCastro.rar

# process "Capture Luis Gtz.rar"
# unrar "Capture Luis Gtz.rar"
unrar x $unzippedCapturesParentFolderName/Captur* -d $unzippedCapturesParentFolderName/Luis/
# rm "Capture Luis Gtz.rar"
rm -rfv $unzippedCapturesParentFolderName/Captur*.rar
# move current directory to "CapturasV2"
cd $unzippedCapturesParentFolderName
# make "Luis" normal folder a .rar file
rar a Luis Luis/
# rm "Luis" normal folder
rm -rfv Luis/
cd ..

echo "+++++++++ 1) FINISH: cleaning data (unused users and special folder names) +++++++++"

# unzip folders in parent folder
for folder in $(ls $unzippedCapturesParentFolderName); do
  folderName=$unzippedCapturesParentFolderName/$folder
  echo "------------------------------ Processing $folderName ------------------------------"
  previousNameWhenUnzipped="";
  if [[ $folder =~ .*\.rar ]]; then
    unrar x $folderName -d $unzippedCapturesParentFolderName
  elif [[ $folder =~ .*\.zip ]]; then
    unzip $folderName -d $unzippedCapturesParentFolderName
  else
    echo "*** $folder NOT PROCESSED ***"
  fi
  
  rm -rfv $folderName

  folderNameLength=${#folder}
  
  unraredFolder=${folder::(folderNameLength-4)}

  if [ "$unraredFolder" = "$berny" ]; then
    previousNameWhenUnzipped="capture-Bernardo "
    userId=4
  elif [ "$unraredFolder" = "$luis" ]; then
    userId=5
  elif [ "$unraredFolder" = "$cervantes" ]; then
    userId=6
  elif [ "$unraredFolder" = "$delia" ]; then
    unraredFolder="HostCapture"
    userId=8
  elif [ "$unraredFolder" = "$ivan" ]; then
    unraredFolder="capturasIvan"
    userId=9
  elif [ "$unraredFolder" = "$pardinas" ]; then
    unraredFolder="Pardiñas"
    userId=10
  elif [ "$unraredFolder" = "$vortega" ]; then
    userId=11
  fi

  # unzip individual folders in every user folder
  totalDataSetElements=0
  for zippedFile in $(ls $unzippedCapturesParentFolderName/$unraredFolder); do
    fileName="$unzippedCapturesParentFolderName/$unraredFolder/$previousNameWhenUnzipped$zippedFile"
    if [[ $zippedFile =~ .*\.zip ]]; then
      unzip "$unzippedCapturesParentFolderName/$unraredFolder/$previousNameWhenUnzipped$zippedFile" -d $unzippedCapturesParentFolderName/$unraredFolder/
      rm $fileName
      ((totalDataSetElements++))
    elif [[ $zippedFile =~ .*\.csv ]]; then
      ((totalDataSetElements++))
    else
     echo $fileName NOT Processed
    fi
  done

  if [ "$unraredFolder" = "$berny" ]; then
    rm $unzippedCapturesParentFolderName/$unraredFolder/*.zip
  fi

  #move files to its corresponding dataset and user folder
  totalElementsPerDataSetType=$((totalDataSetElements/3))
  totalCSV=0
  namingElement=0
  currentDestination=1;

  buildDir="../networkTrafficCapturesBuildDataSet/user$userId"
  trainDir="../networkTrafficCapturesTrainDataSet/user$userId"
  evaluationDir="../networkTrafficCapturesEvaluationDataSet/user$userId"

  destinationDir=$buildDir

  mkdir $buildDir
  mkdir $trainDir
  mkdir $evaluationDir

  for csvFile in $(ls $unzippedCapturesParentFolderName/$unraredFolder); do
    if [[ $csvFile =~ .*\.csv ]]; then
      mv "$unzippedCapturesParentFolderName/$unraredFolder/$previousNameWhenUnzipped$csvFile" $destinationDir/flows-$totalCSV.csv
      #mv "$unzippedCapturesParentFolderName/$unraredFolder/$previousNameWhenUnzipped$csvFile" $destinationDir
      ((totalCSV++))

      if [ $totalCSV -eq $totalElementsPerDataSetType ]; then
        ((currentDestination++))
        if [ $currentDestination -eq 2 ]; then
          destinationDir=$trainDir
        elif [ $currentDestination -eq 3 ]; then
          destinationDir=$evaluationDir
        fi

        totalCSV=0
      fi
    fi
  done
  echo "------------------------------ FINISH Processing $folderName ------------------------------"
done

rm -rfv $unzippedCapturesParentFolderName

cat ../networkTrafficCapturesTrainDataSet/user9/flows-11.csv | tail -r | tail -n +3 | tail -r

echo "+++++++++ 1) FINISH: running script +++++++++"
