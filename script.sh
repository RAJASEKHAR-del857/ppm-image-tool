#!/bin/bash

images=("1" "2" "3" "4" "5" "6" "7")
runs_count=5
output_prefix="output"

for image_id in "${images[@]}"
do
    sum_smooth=0.0
    sum_details=0.0
    sum_sharp=0.0

    echo "Processing image number ${image_id}..."

    for (( run_idx=1; run_idx<=runs_count; run_idx++ ))
    do
        output_file="${output_prefix}_${image_id}_run${run_idx}.ppm"
        ./a.out "../images/${image_id}.ppm" "../images/${output_file}" > run_log.txt

        smooth_time=$(grep "s1_smoothen" run_log.txt | sed -n 's/.*, Time Taken is \([0-9.]*\)seconds.*/\1/p')
details_time=$(grep "S2_find_details" run_log.txt | sed -n 's/.*, Time Taken is \([0-9.]*\)seconds.*/\1/p')
sharp_time=$(grep "S3_Sharpen" run_log.txt | sed -n 's/.*, Time Taken is \([0-9.]*\)seconds.*/\1/p')

        sum_smooth=$(echo "$sum_smooth + ($smooth_time * 1000)" | bc)
        sum_details=$(echo "$sum_details + ($details_time * 1000)" | bc)
        sum_sharp=$(echo "$sum_sharp + ($sharp_time * 1000)" | bc)
    done

    avg_smooth=$(echo "scale=5; $sum_smooth / $runs_count" | bc)
    avg_details=$(echo "scale=5; $sum_details / $runs_count" | bc)
    avg_sharp=$(echo "scale=5; $sum_sharp / $runs_count" | bc)

    echo "Average S1_smoothen time: ${avg_smooth} ms"
    echo "Average S2_find_details time: ${avg_details} ms"
    echo "Average S3_sharpen time: ${avg_sharp} ms"
    echo -e "\n\n"
done

rm -f run_log.txt
