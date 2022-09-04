AC-Tube
=======
Open Source CAPWAP WLC + WTP

Current status: development.

See also http://7u83.cauwersin.com/?cat=28

What you can do so far ...

Read INSTALL and compile actube.

Then ...

Create certificates:
    
     cd ssl
     sh all.sh
     cd ..

Edit src/actube/config.ckv and set your IP in
capwap/control-ip-address/address.0

Start actube with some debug options:
    
     cd src/ac
     ./actube -d std -d mod -d elem\_detail -d elem\_dmp

If you have an AP with IOS 7.3x it should connect. Others
might connect, too.

Make sure on the AP you heve made
clear capwap private-config.

In another terminal window connect to the RPC interface:
    
     cd src/ac
     ./act -s 127.0.0.1:5000

Send some commands to bring up a WLAN:
    
     list                # list all connected APs
     select <apname>     # set <apname> to the name of AP you want to select
     status 		# show status of selected AP

     # the following sequence activates a WLAN called actube on AP

     load wlan1
     send
     clear
     load activate
     send

     status		# This should display now oper status for 
			    # interface 0 enabled


CWAPWAP data layer is still not finished, so you can't use the WLAN





