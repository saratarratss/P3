#!/bin/bash
#umaxnorm=${1:+-m $1}
# Put here the program (maybe with path)
GETF0="get_pitch --umaxnorm=0.40 --unorm=0.85 --umaxpot=5"   
#$umaxnorm después de get_pitch 
for fwav in pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    echo "$GETF0 $fwav $ff0 ----"
    $GETF0 $fwav $ff0 > /dev/null || (echo "Error in $GETF0 $fwav $ff0"; exit 1)
done

exit 0
