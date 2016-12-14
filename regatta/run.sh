SUBMIT="ompsubmit prog"
echo $SUBMIT
$SUBMIT

for i in `seq 2 16`;
do
    SUBMIT="ompsubmit -w 15 -n $i prog"
    echo $SUBMIT
    $SUBMIT
done
