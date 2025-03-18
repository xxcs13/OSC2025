# Step
1. Go to your bootloader file and make the bootloader.img, then place bootloader.img in the SD card
2. Go to your kernel file and make the kernel8.img, kernel.img needs to be placed in the same directory as `upload.py`
3. Start monitoring by `minicom` and open another terminal to send kernel8.img to overwrite bootloader.img using `upload.py`
```
cd bootloader
make clean
make
cd kernel
make clean
make
sudo minicom -b 115200 -D /dev/ttyUSB0 // terminal 1
python3 upload.py                      // terminal 2
```   
