make
for file in teste/*.in; do
    echo $file
    ./program < $file > result.out
    diff result.out ${file/.in/.out}
done