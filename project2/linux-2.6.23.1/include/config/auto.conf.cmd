deps_config := \
	lib/Kconfig \
	arch/s390/crypto/Kconfig \
	drivers/crypto/Kconfig \
	crypto/async_tx/Kconfig \
	crypto/Kconfig \
	security/selinux/Kconfig \
	security/Kconfig \
	lib/Kconfig.debug \
	arch/i386/Kconfig.debug \
	arch/i386/oprofile/Kconfig \
	fs/dlm/Kconfig \
	fs/nls/Kconfig \
	fs/partitions/Kconfig \
	fs/ncpfs/Kconfig \
	fs/gfs2/Kconfig \
	fs/xfs/Kconfig \
	fs/Kconfig \
	drivers/lguest/Kconfig \
	drivers/uio/Kconfig \
	drivers/kvm/Kconfig \
	drivers/auxdisplay/Kconfig \
	drivers/dma/Kconfig \
	drivers/rtc/Kconfig \
	drivers/edac/Kconfig \
	drivers/infiniband/ulp/iser/Kconfig \
	drivers/infiniband/ulp/srp/Kconfig \
	drivers/infiniband/ulp/ipoib/Kconfig \
	drivers/infiniband/hw/mlx4/Kconfig \
	drivers/infiniband/hw/cxgb3/Kconfig \
	drivers/infiniband/hw/amso1100/Kconfig \
	drivers/infiniband/hw/ehca/Kconfig \
	drivers/infiniband/hw/ipath/Kconfig \
	drivers/infiniband/hw/mthca/Kconfig \
	drivers/infiniband/Kconfig \
	drivers/leds/Kconfig \
	drivers/mmc/host/Kconfig \
	drivers/mmc/card/Kconfig \
	drivers/mmc/core/Kconfig \
	drivers/mmc/Kconfig \
	drivers/usb/gadget/Kconfig \
	drivers/usb/atm/Kconfig \
	drivers/usb/misc/sisusbvga/Kconfig \
	drivers/usb/misc/Kconfig \
	drivers/usb/serial/Kconfig \
	drivers/usb/mon/Kconfig \
	drivers/usb/image/Kconfig \
	drivers/usb/storage/Kconfig \
	drivers/usb/class/Kconfig \
	drivers/usb/host/Kconfig \
	drivers/usb/core/Kconfig \
	drivers/usb/Kconfig \
	drivers/hid/usbhid/Kconfig \
	drivers/hid/Kconfig \
	sound/oss/Kconfig \
	sound/soc/codecs/Kconfig \
	sound/soc/sh/Kconfig \
	sound/soc/s3c24xx/Kconfig \
	sound/soc/pxa/Kconfig \
	sound/soc/at91/Kconfig \
	sound/soc/Kconfig \
	sound/parisc/Kconfig \
	sound/sparc/Kconfig \
	sound/pcmcia/Kconfig \
	sound/usb/Kconfig \
	sound/sh/Kconfig \
	sound/mips/Kconfig \
	sound/arm/Kconfig \
	sound/aoa/soundbus/Kconfig \
	sound/aoa/codecs/Kconfig \
	sound/aoa/fabrics/Kconfig \
	sound/aoa/Kconfig \
	sound/ppc/Kconfig \
	sound/pci/Kconfig \
	sound/isa/Kconfig \
	sound/drivers/Kconfig \
	sound/core/Kconfig \
	sound/oss/dmasound/Kconfig \
	sound/Kconfig \
	drivers/video/logo/Kconfig \
	drivers/video/console/Kconfig \
	drivers/video/omap/Kconfig \
	drivers/video/geode/Kconfig \
	drivers/video/display/Kconfig \
	drivers/video/backlight/Kconfig \
	drivers/video/Kconfig \
	drivers/media/common/Kconfig \
	drivers/media/dvb/frontends/Kconfig \
	drivers/media/dvb/pluto2/Kconfig \
	drivers/media/dvb/bt8xx/Kconfig \
	drivers/media/dvb/b2c2/Kconfig \
	drivers/media/dvb/cinergyT2/Kconfig \
	drivers/media/dvb/ttusb-dec/Kconfig \
	drivers/media/dvb/ttusb-budget/Kconfig \
	drivers/media/dvb/dvb-usb/Kconfig \
	drivers/media/dvb/ttpci/Kconfig \
	drivers/media/dvb/dvb-core/Kconfig \
	drivers/media/dvb/Kconfig \
	drivers/media/radio/Kconfig \
	drivers/media/video/pwc/Kconfig \
	drivers/media/video/zc0301/Kconfig \
	drivers/media/video/sn9c102/Kconfig \
	drivers/media/video/et61x251/Kconfig \
	drivers/media/video/usbvideo/Kconfig \
	drivers/media/video/usbvision/Kconfig \
	drivers/media/video/em28xx/Kconfig \
	drivers/media/video/pvrusb2/Kconfig \
	drivers/media/video/ivtv/Kconfig \
	drivers/media/video/cx88/Kconfig \
	drivers/media/video/saa7134/Kconfig \
	drivers/media/video/cpia2/Kconfig \
	drivers/media/video/bt8xx/Kconfig \
	drivers/media/video/cx25840/Kconfig \
	drivers/media/video/Kconfig \
	drivers/media/Kconfig \
	drivers/mfd/Kconfig \
	drivers/hwmon/Kconfig \
	drivers/power/Kconfig \
	drivers/w1/slaves/Kconfig \
	drivers/w1/masters/Kconfig \
	drivers/w1/Kconfig \
	drivers/spi/Kconfig \
	drivers/i2c/chips/Kconfig \
	drivers/i2c/busses/Kconfig \
	drivers/i2c/algos/Kconfig \
	drivers/i2c/Kconfig \
	drivers/s390/char/Kconfig \
	drivers/char/tpm/Kconfig \
	drivers/char/pcmcia/Kconfig \
	drivers/char/drm/Kconfig \
	drivers/char/agp/Kconfig \
	drivers/char/hw_random/Kconfig \
	drivers/char/watchdog/Kconfig \
	drivers/char/ipmi/Kconfig \
	drivers/serial/Kconfig \
	drivers/char/Kconfig \
	drivers/input/gameport/Kconfig \
	drivers/input/serio/Kconfig \
	drivers/input/misc/Kconfig \
	drivers/input/touchscreen/Kconfig \
	drivers/input/tablet/Kconfig \
	drivers/input/joystick/iforce/Kconfig \
	drivers/input/joystick/Kconfig \
	drivers/input/mouse/Kconfig \
	drivers/input/keyboard/Kconfig \
	drivers/input/Kconfig \
	drivers/telephony/Kconfig \
	drivers/isdn/hardware/eicon/Kconfig \
	drivers/isdn/hardware/avm/Kconfig \
	drivers/isdn/hardware/Kconfig \
	drivers/isdn/capi/Kconfig \
	drivers/isdn/gigaset/Kconfig \
	drivers/isdn/hysdn/Kconfig \
	drivers/isdn/act2000/Kconfig \
	drivers/isdn/sc/Kconfig \
	drivers/isdn/pcbit/Kconfig \
	drivers/isdn/icn/Kconfig \
	drivers/isdn/hisax/Kconfig \
	drivers/isdn/i4l/Kconfig \
	drivers/isdn/Kconfig \
	drivers/s390/net/Kconfig \
	drivers/atm/Kconfig \
	drivers/net/wan/Kconfig \
	drivers/net/pcmcia/Kconfig \
	drivers/net/usb/Kconfig \
	drivers/net/wireless/zd1211rw/Kconfig \
	drivers/net/wireless/bcm43xx/Kconfig \
	drivers/net/wireless/hostap/Kconfig \
	drivers/net/wireless/Kconfig \
	drivers/net/tokenring/Kconfig \
	drivers/net/ixp2000/Kconfig \
	drivers/net/fs_enet/Kconfig \
	drivers/net/fec_8xx/Kconfig \
	drivers/net/tulip/Kconfig \
	drivers/net/arm/Kconfig \
	drivers/net/phy/Kconfig \
	drivers/net/arcnet/Kconfig \
	drivers/net/Kconfig \
	drivers/macintosh/Kconfig \
	drivers/message/i2o/Kconfig \
	drivers/firewire/Kconfig \
	drivers/ieee1394/Kconfig \
	drivers/message/fusion/Kconfig \
	drivers/md/Kconfig \
	drivers/ata/Kconfig \
	drivers/scsi/pcmcia/Kconfig \
	drivers/scsi/arm/Kconfig \
	drivers/scsi/qla4xxx/Kconfig \
	drivers/scsi/qla2xxx/Kconfig \
	drivers/scsi/megaraid/Kconfig.megaraid \
	drivers/scsi/aic94xx/Kconfig \
	drivers/scsi/aic7xxx/Kconfig.aic79xx \
	drivers/scsi/aic7xxx/Kconfig.aic7xxx \
	drivers/scsi/libsas/Kconfig \
	drivers/scsi/Kconfig \
	drivers/ide/Kconfig \
	drivers/misc/Kconfig \
	drivers/s390/block/Kconfig \
	drivers/block/paride/Kconfig \
	drivers/block/Kconfig \
	drivers/pnp/pnpacpi/Kconfig \
	drivers/pnp/pnpbios/Kconfig \
	drivers/pnp/isapnp/Kconfig \
	drivers/pnp/Kconfig \
	drivers/parport/Kconfig \
	drivers/of/Kconfig \
	drivers/mtd/ubi/Kconfig.debug \
	drivers/mtd/ubi/Kconfig \
	drivers/mtd/onenand/Kconfig \
	drivers/mtd/nand/Kconfig \
	drivers/mtd/devices/Kconfig \
	drivers/mtd/maps/Kconfig \
	drivers/mtd/chips/Kconfig \
	drivers/mtd/Kconfig \
	drivers/connector/Kconfig \
	drivers/base/Kconfig \
	drivers/Kconfig \
	net/9p/Kconfig \
	net/rfkill/Kconfig \
	net/ieee80211/softmac/Kconfig \
	net/ieee80211/Kconfig \
	net/mac80211/Kconfig \
	net/wireless/Kconfig \
	net/rxrpc/Kconfig \
	drivers/bluetooth/Kconfig \
	net/bluetooth/hidp/Kconfig \
	net/bluetooth/cmtp/Kconfig \
	net/bluetooth/bnep/Kconfig \
	net/bluetooth/rfcomm/Kconfig \
	net/bluetooth/Kconfig \
	drivers/net/irda/Kconfig \
	net/irda/ircomm/Kconfig \
	net/irda/irnet/Kconfig \
	net/irda/irlan/Kconfig \
	net/irda/Kconfig \
	drivers/net/hamradio/Kconfig \
	net/ax25/Kconfig \
	net/sched/Kconfig \
	net/wanrouter/Kconfig \
	net/econet/Kconfig \
	net/lapb/Kconfig \
	net/x25/Kconfig \
	drivers/net/appletalk/Kconfig \
	net/ipx/Kconfig \
	net/llc/Kconfig \
	net/decnet/Kconfig \
	net/8021q/Kconfig \
	net/bridge/Kconfig \
	net/atm/Kconfig \
	net/tipc/Kconfig \
	net/sctp/Kconfig \
	net/dccp/ccids/Kconfig \
	net/dccp/Kconfig \
	net/bridge/netfilter/Kconfig \
	net/decnet/netfilter/Kconfig \
	net/ipv6/netfilter/Kconfig \
	net/ipv4/netfilter/Kconfig \
	net/netfilter/Kconfig \
	net/netlabel/Kconfig \
	net/ipv6/Kconfig \
	net/ipv4/ipvs/Kconfig \
	net/ipv4/Kconfig \
	net/iucv/Kconfig \
	net/xfrm/Kconfig \
	net/unix/Kconfig \
	net/packet/Kconfig \
	net/Kconfig \
	fs/Kconfig.binfmt \
	drivers/pci/hotplug/Kconfig \
	drivers/pcmcia/Kconfig \
	drivers/mca/Kconfig \
	drivers/eisa/Kconfig \
	drivers/pci/Kconfig \
	drivers/pci/pcie/aer/Kconfig \
	drivers/pci/pcie/Kconfig \
	drivers/cpufreq/Kconfig \
	arch/i386/kernel/cpu/cpufreq/Kconfig \
	drivers/acpi/Kconfig \
	kernel/power/Kconfig \
	kernel/Kconfig.hz \
	mm/Kconfig \
	drivers/firmware/Kconfig \
	kernel/Kconfig.preempt \
	arch/i386/Kconfig.cpu \
	arch/i386/xen/Kconfig \
	kernel/time/Kconfig \
	block/Kconfig.iosched \
	block/Kconfig \
	usr/Kconfig \
	init/Kconfig \
	arch/i386/Kconfig

include/config/auto.conf: \
	$(deps_config)

$(deps_config): ;
