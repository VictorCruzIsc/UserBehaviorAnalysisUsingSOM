#!/bin/bash

berny="Berny"
delia="Delia"

rm DCastro.rar

for folder in $(ls); do
  previousName="";
  if [[ $folder =~ .*\.rar ]]; then
    echo .rar - $folder
    unrar x $folder
  elif [[ $folder =~ .*\.zip ]]; then
    echo .zip - $folder
    unzip $folder
  else
    echo $folder NOT Processed
  fi

  folderNameLength=${#folder}
  unraredFolder=${folder::(folderNameLength-4)}

  if [ "$unraredFolder" = "$berny" ]; then
    previousName="capture-Bernardo "
  elif [ "$unraredFolder" = "$delia" ]; then
    unraredFolder="HostCapture"
  else
    echo "NOTHING IS DONE"
  fi

  for zippedFile in $(ls $unraredFolder); do
    total=0
    if [[ $zippedFile =~ .*\.zip ]]; then
      fileName="$unraredFolder/$previousName$zippedFile"
      unzip "$unraredFolder/$previousName$zippedFile" -d $unraredFolder/
      rm $fileName

    #  fileNameLength=${#fileToUnzip}
    #  obtainedFile=${fileToUnzip::(fileNameLength-4)}
    #  newName=$folder/flows-$total.csv
      
    #  mv $obtainedFile $newName
      #((total++))

    #  echo $newName DONE
    else
      echo $fileToUnzip NOT Processed
    fi
  done

  if [ "$unraredFolder" = "$berny" ]; then
    rm $unraredFolder/*.zip
  fi

  #rm $folder
done
