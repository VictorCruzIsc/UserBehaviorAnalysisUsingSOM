#!/bin/bash

for folder in $(ls); do
  total=0
  echo $folder
 
  for file in $(ls $folder); do
    if [[ $file =~ .*\.zip ]]; then
      usedFile=$folder/$file
      unzip $usedFile -d $folder/ 
      rm $usedFile
      
      obtainedFile=${usedFile::-4}
      newName=$folder/flows-$total.csv
      mv $obtainedFile $newName
      ((total++))

      echo $newName DONE
    else
      echo $file NOT Processed
    fi
  done
done
