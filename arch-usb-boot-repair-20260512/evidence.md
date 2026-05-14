# Evidence Summary

## Current Kernel

```text
Linux Tian 7.0.5-arch1-1 #1 SMP PREEMPT_DYNAMIC Fri, 08 May 2026 09:29:23 +0000 x86_64 GNU/Linux
```

## Block Devices

```text
/dev/sdb1  vfat  /boot  PARTUUID=12b36bd5-8498-45fb-9414-c9a4b50977b2
/dev/sdb2  ext4  /      PARTUUID=afa9da95-ee18-4ced-9e13-f764f54a392d
/dev/sdb3  ext4  /home  PARTUUID=46aa10ec-62c8-41d9-9e55-0a500d4c0a52
```

## Installed Kernel Packages

```text
linux 7.0.5.arch1-1
linux-lts 6.18.29-1
linux-headers 7.0.5.arch1-1
linux-lts-headers 6.18.29-1
mkinitcpio 41-3
pacman 7.1.0.r9.g54d9411-2
virtualbox-host-modules-arch 7.2.8-11
dosfstools 4.2-5
```

## Remaining Module Trees

```text
6.18.29-1-lts
7.0.5-arch1-1
```

## Initramfs USB Module Evidence

`/boot/initramfs-linux.img`:

```text
usr/lib/modules/7.0.5-arch1-1/kernel/drivers/usb/storage/uas.ko.zst
usr/lib/modules/7.0.5-arch1-1/kernel/drivers/usb/storage/usb-storage.ko.zst
usr/lib/modules/7.0.5-arch1-1/modules.dep.bin
usr/lib/modules/7.0.5-arch1-1/modules.devname
```

`/boot/initramfs-linux-lts.img`:

```text
usr/lib/modules/6.18.29-1-lts/kernel/drivers/usb/storage/uas.ko.zst
usr/lib/modules/6.18.29-1-lts/kernel/drivers/usb/storage/usb-storage.ko.zst
usr/lib/modules/6.18.29-1-lts/modules.dep.bin
usr/lib/modules/6.18.29-1-lts/modules.devname
```

## systemd-boot Entries

```text
arch-linux.conf
arch-linux-fallback.conf
arch-lts.conf
arch-lts-fallback.conf
```

Default:

```text
arch-linux.conf
```

All entries use:

```text
root=PARTUUID=afa9da95-ee18-4ced-9e13-f764f54a392d
usb-storage.quirks=0bda:9210:u
usbcore.autosuspend=-1
```

## /boot fsck

Before repair:

```text
Dirty bit is set. Fs was not properly unmounted and some data may be corrupt.
```

Repair:

```text
fsck.vfat -a /dev/sdb1
*** Filesystem was changed ***
```

Final offline check:

```text
/dev/sdb1: 33 files, 168940/261627 clusters
```

## Timeshift

Failed service cause:

```text
Another instance of this application is running
```

Current snapshot target:

```text
backup_device_uuid=5ebf9876-c9d0-4e69-b399-fa77d8ef3c09
```

This maps to `/dev/sdb3`, mounted as `/home`.

Snapshot directory:

```text
39G /home/timeshift
```

Snapshots:

```text
2026-05-12_22-00-00 daily
2026-05-11_20-04-51 daily
2026-05-01_07-35-24 ondemand
```

## Final Health Checks

```text
systemctl --failed: 0 loaded units listed
pacman -Qkk linux linux-lts mkinitcpio pacman virtualbox-host-modules-arch dosfstools: 0 altered files for package payloads
```

Remaining pacnew files:

```text
/etc/locale.gen.pacnew
/etc/mkinitcpio.conf.pacnew
/etc/pacman.conf.pacnew
/etc/pacman.d/mirrorlist.pacnew
```

