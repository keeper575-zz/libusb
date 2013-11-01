/* Function to read and write the PEN_DRIVE300 board USB port */

#include <stdio.h>
#include <usb.h>

usb_dev_handle *locate_pend(void);

int main (int argc,char **argv)
{
	struct usb_dev_handle *pend_handle;
	struct usb_device *pend_device;
	int send_status;
	int open_status;
	unsigned char send_data=0xff;
	unsigned char receive_data=0;

	usb_init();
	//usb_set_debug(2);
 	if ((pend_handle = locate_pend())==0) 
	{
		printf("Could not open the PEN_DRIVE device\n");
		return (-1);
	}  

	open_status = usb_set_configuration(pend_handle,1);
	printf("conf_stat=%d\n",open_status);
	
	open_status = usb_claim_interface(pend_handle,0);
	printf("claim_stat=%d\n",open_status);
	
	open_status = usb_set_altinterface(pend_handle,0);
	printf("alt_stat=%d\n",open_status);

	send_status=usb_bulk_write(pend_handle,4,&send_data,1,500);
	printf("TX stat=%d\n",send_status);
	
	usb_bulk_read(pend_handle,3,&receive_data,1,500);	
	printf("RX stat=%d -> RX char=%d\n",send_status,receive_data);

	/* Write the data2send bytes to the 7-segs */
    /*	send_status = usb_control_msg(pend_handle,0x20,0x09,0x0200,0x0001,send_data,2,500); 
	printf("TX stat=%d\n",send_status);
	usleep(10000);
   */
	/* Read the bytes that were just sent to the 7-segs */
  /*	
    	send_status = usb_control_msg(pend_handle,0xA0,0x01,0x0100,0x0001,receive_data,2,500); 
	printf("RX stat=%d data=0x%x,0x%x\n",send_status,receive_data[0],receive_data[1]);
	usleep(10000);
  */	
	usb_close(pend_handle);
}	

usb_dev_handle *locate_pend(void) 
{
	unsigned char located = 0;
	struct usb_bus *bus;
	struct usb_device *dev;
	usb_dev_handle *device_handle = 0;
 		
	usb_find_busses();
	usb_find_devices();
 
 	for (bus = usb_busses; bus; bus = bus->next)
	{
		for (dev = bus->devices; dev; dev = dev->next)	
		{
			if (dev->descriptor.idVendor == 0x0930) 
			{	
				located++;
				device_handle = usb_open(dev);
				printf("PEN_DRIVE Device Found @ Address %s \n", dev->filename);
				printf("PEN_DRIVE Vendor ID 0x0%x\n",dev->descriptor.idVendor);
				printf("PEN_DRIVE Product ID 0x0%x\n",dev->descriptor.idProduct);
			}
			else printf("** usb device %s found **\n", dev->filename);			
		}	
  }

  if (device_handle==0) return (0);
  else return (device_handle);  	
}

