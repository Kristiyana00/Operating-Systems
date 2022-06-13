#!/bin/bash

if [ $# -ne 2 ]; then
        echo "Invalid number of arguments."
        exit 1
fi

if [ -e $1 ]; then
        echo "$1 already exists."
        exit 2
fi

if [ ! -d $2 ]; then
        echo "$2 is not a directory."
        exit 3
fi

if [ ! -r $2 ]; then
        echo "$2 is not readable."
        exit 4
fi

touch $1

if [ ! -w $1 ]; then
        echo "$1 is not writable."
        exit 5
fi

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" >> $1

while read line; do
        hostname=$(basename $line | rev | cut -d . -f 2- | rev)
        phy=$(cat $line | egrep "Maximum Physical Interfaces" | tr -s ' ' | tr -d ' ' | cut -d ":" -f 2)
        vlans=$(cat $line | egrep "VLANs" | tr -s ' ' | tr -d ' ' | cut -d ":" -f 2)
        hosts=$(cat $line | egrep "Inside Hosts" | tr -s ' ' | tr -d ' ' | cut -d ":" -f 2)
        failover=$(cat $line | egrep "Failover" | tr -s ' ' | tr -d ' ' | cut -d ":" -f 2)
        VPN_3DES_AES=$(cat $line | egrep "VPN-3DES-AES" | tr -s ' ' | tr -d ' ' | cut -d ":" -f 2)
        peers=$(cat $line | egrep "*Total VPN Peers" | tr -s ' ' | tr -d ' ' | cut -d ":" -f 2)
        VLAN_Trunk_Ports=$(cat $line | egrep "VLAN Trunk Ports" | tr -s ' ' | tr -d ' ' | cut -d ":" -f 2)
        license=$(cat $line | egrep "This platform has a" | cut -d ' ' -f 5- | rev | cut -d ' ' -f 2- | rev)
        SN=$(cat $line | egrep "Serial Number:" | tr -s ' ' | tr -d ' ' | cut -d ':' -f 2)
        key=$(cat $line | egrep "Running Activation Key:" | tr -s ' ' | tr -d ' ' | cut -d ':' -f 2)
        echo "$hostname,$phy,$vlans,$hosts,$failover,$VPN_3DES_AES,$peers,$VLAN_Trunk_Ports,$license,$SN,$key"  >> $1
done < <(find $2 -type f -name "*.log" -printf "%p\n")
