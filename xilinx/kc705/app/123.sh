function reset_by_pin() {
	local pin=$1
	echo "$pin" > "/proc/gpio_test/export";

	echo "0" > "/proc/gpio_test/$pin";
	sleep 0.1;
	echo "1" > "/proc/gpio_test/$pin";

	echo "$pin" > "/proc/gpio_test/unexport";
}

function tune_33_on_off_on() {
	local pin=$1
	echo "$pin" > "/proc/gpio_test/export";

	echo "1" > "/proc/gpio_test/$pin";

	echo "$pin" > "/proc/gpio_test/unexport";
}

tune0_33_on_off=245
tune1_33_on_off=250
tune2_33_on_off=255

tune_33_on_off_list="
$tune0_33_on_off
$tune1_33_on_off
$tune2_33_on_off
"


for pin in $tune_33_on_off_list;do
	tune_33_on_off_on $pin
done

tune0_reset=244
tune1_reset=249
tune2_reset=254
reset_pin_list="
$tune0_reset
$tune1_reset
$tune2_reset
"

for pin in $reset_pin_list;do
	reset_by_pin $pin
done


#sudo ./A6211/A6211 1400 27500 H
sudo ./A6211/A6211 1218 28800 H
