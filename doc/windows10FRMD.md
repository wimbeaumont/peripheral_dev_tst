As I use often the FRDM KL05Z  and FRDM KL25Z  here some notes that can be helpfull to get it stable running under windows 10 . ( on windows 7 I used it often during development without any specific problems)  

Still it seems that all solutions found on the web don't work always. So just use it as a guidline.

One of the useful articles was 

https://mcuoneclipse.com/2016/08/01/bricking_and_recovering_opensda_boards_in_windows_8_and_10/

Although I don't have the problems with the bootloader that where mentioned here 

The follwing I did for a FRDM25Z  


Windows version  

Windows 10 home   1903  2019-08-03    OS build 18362.720  

First I used a linux  machine  (Lubuntu 18.4) to load the LDAP firmware : 

   * Download the latest version LDAP ( in my case 0253_release_package_64359f5c.zip) 
   * Extract 0253_k20dx_frdmkl25z_0x8000.bin  from the zip file 
   * start a the file manager
   * start the FRDM in bootloader mode by holding the push button when connecting the FRDM to the USB bus 
   * a window pop up click on it.  Check if the "disk" identify itself as bootloader 
   * drag the file 0253_k20dx_frdmkl25z_0x8000.bin  to the bootloader disk 
   * unplug the FDRM 
   * check if  the new firmware is loaded ( didn't check this ) 


Now connect it to the windows 10 machine. 

An extra drive  (in my case)  with the name MBED (E:)   ( of course E can be any other char) 

BUT after a time ( < 1 minute)  the drive disconnect and comes then back with a fail.txt file that has the contents :  TIMEOUT  
Then after a short time it repeats. 

So I had to to do two actions on the windows 10 machine : 

   * stop and  disable ( the later is important !!!) the storage server . 
      * start with  services.cmst
      * look for the  "storage service"  and right clik on it and select properties
      * in startup type set it on disable
      * click on stop 
      * clik on apply 

Only doing this was in my case not sufficient. But I would suggest to give it a try.  As the following stap can cause a system failure if not correcly done.  Also I recomendent to restore the storage service to manual mode startup once you don't use the FDRM . 

One of the problems that cause the behaviour of disconnecting / connectimg the MBED "drive" is that Windows tries to index the drive.  To stop this the register has to be edited .  TAKE CARE if not done correctly the system coudl be corrupted !!! So do this only when you understand the details you are doing and how you can recover ( restore a easier configuration) . 

Also realise that regedit acts DIRECTLY on the register.  So there is no save or undo 


   * open regedit ( tyep  regedit in the search window) 
   * Click on edit and then select find
   * in the find window select only  key  and fill in Windows Search
	* in my computer it was found in  Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\Windows Search  . If found in another location I would advice not to change it
   * In my case there was only the default so I had do add a key 
   * right click in the right window  
   * select new -> DWORD 32 bit value
   * set the name of the new entry to DisableRemovableDriveIndexing
   * right click on ( the new) DisableRemovableDriveIndexing and set the value to 1. 

Not sure what are the consequences are from this change.  But I dislike it that windows is always writing to my USB disks so I leave this setting for the moment. 

Restart the computer. 

In my case if I connect the FDRM KL25Z the "drive" doesn't disconnect anymore.


Finally you have to install the serial driver for windows following 

https://os.mbed.com/handbook/Windows-serial-configuration

( with the FDRM connected) 

For windows (10)  it is not always clear to which com port the FDRM is connected. 

So I check it in the Device manager 

   * open the file explorer
   * right click on this computer (or pc) 
   * click on device manager
   * look in the devices for Ports ( COM & LPT) 
   * expand it and check for mbed and note the com nr. 


I use putty to connect 

( https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html )

Start putty , in putty  , select Serial 

Edit the number after COM   ( in my case 3  so the Serial line is set to COM3 ) 
check the speed is 9600  

Save the session  ( I use mbed3 as name ) 

If you now open there should be communication with the application ( of course you have to down load in the useal way first an application that communicates with the serial port , like the  "hello world" example 

Issue with not finding the serial port. 

One of the students reported that the serial port was not found.

He installed the serial port driver 

The MBED was seen a a disk 

He installed an other driver 

(after consulting 

https://www.nxp.com/document/guide/get-started-with-the-frdm-kl25z:NGS-FRDM-KL25Z?fbclid=IwAR1uYIifVd2KZ5ykBCsqT8xffEhp2Qp9enV3M8edLb_J-rIKe-v-8fbqqeI

)

 He followed the link in  "2.3 PC Configuration". to the webiste  http://www.pemicro.com/opensda/
 
 
 comment on the web site : 
 
OpenSDA Support - P&E Microcomputer Systems
GDB SERVER PLUG-IN for Eclipse-based ARM® IDEs: PEmicro's GDB Server Plug-In for Eckipse-based ARM IDEs allows GNU GDB tools to work with the OpenSDA and OSJTAG embedded debug circuitry incorporated into many of NXP's Tower and Freedom development boards, as well as PEmicro's ARM Cortex-compatible hardware interfaces. A free download of the plug-in is available.
www.pemicro.com










