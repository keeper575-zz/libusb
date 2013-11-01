all: libusb

libusb:	test_usb.c
	gcc -o libusb test_usb.c -lusb
clean:
	rm -f libusb
