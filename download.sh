#!/bin/bash

# https://pi2e.ch/blog/2017/03/10/pi-digits-download/#download
# Each file is 100 billion digits

for i in {01..04}
do
	echo pi_dec_1t_${i}.zip
	wget --continue https://archive.org/download/pi_dec_1t/pi_dec_1t_${i}.zip
done

echo "6b3a28f1f191f6f7726e3b78152935fd  pi_dec_1t_01.zip"  > checks.txt
echo "2aeb9b3e0d2e999c581977e85a74f729  pi_dec_1t_02.zip" >> checks.txt
echo "11aac595f3b21c375a1dee59265aef78  pi_dec_1t_03.zip" >> checks.txt
echo "cdd6ec53f92cf84f3f6924e4892662c4  pi_dec_1t_04.zip" >> checks.txt

md5sum --check checks.txt

for i in {01..04}
do
	unzip pi_dec_1t_${i}.zip
	#rm pi_dec_1t_${i}.zip
done

cat pi_dec_1t_*.txt | tr -d "\n" > pi_digits.txt
