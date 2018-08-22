back=$PWD
prefix=/t2k/users/suvorov/figure/ndUP/Michel/
out_name="out_test"
in_name="/t2k/users/suvorov/AnalysisResults/ndUP/SuperFGD/proto2/degrader_4_mu_500.root"

cd $bars/dev/t2k-nd280-upgrade/macro/suvorov/

first=0
last=25000
segm=250

step=$(((last-first) / segm))

for i in `seq 0 $((segm-1))`;
do
	root -J1 -l -q -b /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrackPoint.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpTrack.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpVertex.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpHit.cc+ /t2k/users/suvorov/dev/t2k-nd280-upgrade/src/app/nd280UpEvent/TND280UpEvent.cc+ "MIEle.C($first, $step, $i, \"${out_name}\", \"${in_name}\")"
	first=$((first+step))
done

hadd -f "$prefix/out-v3.root" "$prefix/out_"*
rm "$prefix/out_"*

cd $back

#echo "Plot for MiEle ${out_name}" | mailx -s "Pile up task complete" suvorov@inr.ru