#!/bin/bash
cat defects_all.lst | grep "Meškanie\|Dátum" | sed -e 's@Pridal: .*@@g' > defects_time-late.lst
lines=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | wc -l`
time=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | awk '{s+=$1} END {print s}'`
#echo "Time for removing defect is approximately: $(($time/$lines)) minutes."

#defect lvl1 <= 10 min
#def_lvl1=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | while read line; do num=`echo $line | sed 's/[^0-9]//g'`; if [ $num -le 10 ]; then echo $num; fi; done | wc -l`

#def_lvl2=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | while read line; do num=`echo $line | sed 's/[^0-9]//g'`; if [ $num -gt 10 ] && [ $num -lt 30 ]; then echo $num; fi; done | wc -l`

#def_lvl3=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | while read line; do num=`echo $line | sed 's/[^0-9]//g'` if [ $num -ge 30 ]; then echo $num; fi done | wc -l`

#def_lvl3=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | while read line; do num=`echo $line | sed 's/[^0-9]//g'`; if [ $num -ge 30 ]; then echo $num; fi; done | wc -l`

#def_lvl1_all=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | while read line; do num=`echo $line | sed 's/[^0-9]//g'`; if [ $num -le 10 ]; then echo $num; fi; done | awk '{s+=$1} END {print s}'`

#def_lvl2_all=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | while read line; do num=`echo $line | sed 's/[^0-9]//g'`; if [ $num -gt 10 ] && [ $num -lt 30 ]; then echo $num; fi; done | awk '{s+=$1} END {print s}'`

#def_lvl3_all=`cat defects_all.lst | grep "Meškanie" | sed -e 's@Meškanie:@@g' | sed -e 's@min.@@g' | while read line; do num=`echo $line | sed 's/[^0-9]//g'`; if [ $num -ge 30 ]; then echo $num; fi; done | awk '{s+=$1} END {print s}'`

#echo "Time for removing small defect is approximately: $(($def_lvl1_all/$def_lvl1)) minutes."
#echo "Time for removing small defect is approximately: $(($def_lvl2_all/$def_lvl2)) minutes."
#echo "Time for removing small defect is approximately: $(($def_lvl3_all/$def_lvl3)) minutes."
if [ -f defects.level ]
then
  rm defects.level
fi

# small defect 
echo "Small defect takes approximatelly $((452/170)) minutes." >> defects.level
# medium defect
echo "Medium defect takes approximatelly $((1959/101)) minutes." >> defects.level
# large defect 29307/366
echo "Large defect takes approximatelly $((29307/366)) minutes." >> defects.level

# It is 4185 days from the start date to the end date, end date included
# Or 11 years, 5 months, 15 days including the end date
days=4185
#echo "Defect happens every $(($days/$lines)) days."
echo "Small defect happens every $(($days/170)) days." >> defects.level
echo "Medium defect happens every $(($days/101)) days." >> defects.level
echo "Large defect happens every $(($days/366)) days." >> defects.level


