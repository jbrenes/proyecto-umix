#!/bin/bash
#Lenny,squeeze,sid,Ubuntu 9.04 (jaunty), Ubuntu 9.10 (karmic):
#DISTRO=lenny,squeeze,sid,jaunty

BOARD=beagle
DISTRO=jaunty
KER_REL=2.6.29
PATCH=
#PATCH=.9
KER_GIT=58cf2f1
OER_PATCH=oer44.1
DEB_REL=1.0
MMC_PART=mmcblk0p1

MIRROR=http://www.rcn-ee.net/deb/kernel/${BOARD}/${DISTRO}
FOLDER=v${KER_REL}${PATCH}-${KER_GIT}-${OER_PATCH}
IMAGE=linux-image-${KER_REL}${PATCH}-${OER_PATCH}_${DEB_REL}${DISTRO}_armel.deb

echo "Installing uboot-mkimage"
sudo apt-get install uboot-mkimage

sudo ntpdate pool.ntp.org

echo "Downloading Recommended Kernel & SGX Modules"
wget -c ${MIRROR}/${FOLDER}/${IMAGE}
wget -c ${MIRROR}/${FOLDER}/omaplfb.ko
wget -c ${MIRROR}/${FOLDER}/pvrsrvkm.ko

echo "Installing linux-image"
sudo dpkg -i ${IMAGE}

echo "Mounting Fat32 partition"
sudo mkdir -p /tmp/boot
sudo mount /dev/${MMC_PART} /tmp/boot

echo "Extracting vmlinuz"
dpkg -x ${IMAGE} ./temp

echo "Backing up Previous uImage"
if [ -e /tmp/boot/uImage ]
then
sudo mv /tmp/boot/uImage /tmp/boot/uImage_old
fi

echo "Creating uImage from vmlinuz"
sudo mkimage -A arm -O linux -T kernel -C none -a 0x80008000 -e 0x80008000 -n "Linux" -d ./temp/boot/vmlinuz-*  /tmp/boot/uImage
sudo umount /tmp/boot
rm -rfd temp

echo "Installing SGX Modules" 
sudo mkdir -p /lib/modules/${KER_REL}-${OER_PATCH}/kernel/drivers/gpu/pvr/
sudo mv *.ko /lib/modules/${KER_REL}-${OER_PATCH}/kernel/drivers/gpu/pvr/
sudo depmod -a omaplfb 

echo "Please Reboot"
echo "Note: you might still have to run this after reboot: sudo depmod -a omaplfb"

