for eps in "1e-5" "5e-6" "2e-6" "1e-6";
do
    SUBMIT="ompsubmit prog $eps"
    echo $SUBMIT
    $SUBMIT

    for i in `seq 2 16`;
    do
        SUBMIT="ompsubmit -w 15 -n $i prog $eps"
        echo $SUBMIT
        $SUBMIT
    done
done
