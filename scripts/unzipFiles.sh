#!/bin/bash
capturesZippedFolder="CapturesV2"
unzippedCapturesParentFolderName="CapturasV2"
berny="Berny"
delia="Delia"
vortega="Vortega"
pardinas="Pardi+%A6as"
pardinasRenamed="Pardinas"
userId=0

#clean previous folder
rm -rfv ../$unzippedCapturesParentFolderName
rm -rfv $unzippedCapturesParentFolderName

#clean previous folder
for i in `seq 4 8`; do
  rm -rfv ../networkTrafficCapturesBuildDataSet/user$i
  rm -rfv ../networkTrafficCapturesEvaluationDataSet/user$i
  rm -rfv ../networkTrafficCapturesTrainDataSet/user$i
done

# unzip captures on the same folder that the script is
unzip ../$capturesZippedFolder.zip

# remove folders that do not cotain usefule information
rm $unzippedCapturesParentFolderName/DCastro.rar

# unzip folder in parent folder
for folder in $(ls $unzippedCapturesParentFolderName); do
  folderName=$unzippedCapturesParentFolderName/$folder
  previousName="";
  if [[ $folder =~ .*\.rar ]]; then
    unrar x $folderName -d $unzippedCapturesParentFolderName
  elif [[ $folder =~ .*\.zip ]]; then
    unzip $folderName -d $unzippedCapturesParentFolderName
  else
    echo $folder NOT Processed
  fi
  
  rm -rfv $folderName
  folderNameLength=${#folder}
  unraredFolder=${folder::(folderNameLength-4)}

  if [ "$unraredFolder" = "$berny" ]; then
    previousName="capture-Bernardo "
    userId=4
  elif [ "$unraredFolder" = "$delia" ]; then
    unraredFolder="HostCapture"
    userId=6
  elif [ "$unraredFolder" = "$pardinas" ]; then
    unraredFolder="Pardiñas"
    userId=7
  elif [ "$unraredFolder" = "$vortega" ]; then
    userId=8
  else #cervates
    userId=5
  fi

  # unzip individual folders in every user folder
  totalDataSetElements=0
  for zippedFile in $(ls $unzippedCapturesParentFolderName/$unraredFolder); do
    fileName="$unzippedCapturesParentFolderName/$unraredFolder/$previousName$zippedFile"
    if [[ $zippedFile =~ .*\.zip ]]; then
      unzip "$unzippedCapturesParentFolderName/$unraredFolder/$previousName$zippedFile" -d $unzippedCapturesParentFolderName/$unraredFolder/
      rm $fileName
      ((totalDataSetElements++))
    else
     echo $fileName NOT Processed
    fi
  done

  if [ "$unraredFolder" = "$berny" ]; then
    rm $unzippedCapturesParentFolderName/$unraredFolder/*.zip
  fi

  #move files to it's corresponding dataset and user folder
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
      mv "$unzippedCapturesParentFolderName/$unraredFolder/$previousName$csvFile" $destinationDir/flows-$totalCSV.csv
      #mv "$unzippedCapturesParentFolderName/$unraredFolder/$previousName$csvFile" $destinationDir
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
done