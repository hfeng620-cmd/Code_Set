# Arch USB SSD Boot Repair - 2026-05-12

## 问题概述

这次故障不是 systemd-boot 启动项写错，也不是 Arch 根分区丢失。

实际问题是：Arch 安装在外接 USB SSD 上，启动早期需要 initramfs 先加载 USB 存储相关支持，才能识别承载根分区的 Realtek RTL9210B-CG 设备。故障时 initramfs / 内核模块索引不完整，导致启动早期找不到 `PARTUUID=afa9da95-ee18-4ced-9e13-f764f54a392d` 对应的根分区。

启动失败链路大致是：

```text
systemd-boot
-> vmlinuz-linux 或 vmlinuz-linux-lts
-> initramfs
-> USB 控制器 / USB 存储模块
-> Realtek RTL9210B-CG SSD
-> PARTUUID=afa9da95-ee18-4ced-9e13-f764f54a392d
-> /dev/sdb2 挂载为 /
```

故障卡在 `initramfs -> USB SSD 可见` 这一步。

## 修复前证据

- 早期启动报错指向根分区无法挂载：
  `Failed to mount 'PARTUUID=afa9da95-ee18-4ced-9e13-f764f54a392d' on real root`
- `mkinitcpio -P` 曾出现 `ERROR: module not found`，包括 USB 存储相关模块。
- 新内核模块目录一开始缺少 `modules.dep` / `modules.devname` 这类 depmod 索引。
- `pacman` 本地数据库中 `pacman-7.1.0.r9.g54d9411-2` 条目损坏，实际目录缺少 `desc`。

## 已执行修复

### 1. 恢复 pacman

先从官方镜像下载并校验同版本 pacman 包：

```text
pacman-7.1.0.r9.g54d9411-2-x86_64.pkg.tar.zst
SHA256: 2092ec7a0391416e4a1757c455bc071d7b58a124cedc83f582cb7d37d52f211b
```

随后手工提取 `usr/` 下的 pacman 二进制、库和 hooks，让 `pacman` 能重新运行。之后将损坏的本地数据库空目录移到：

```text
/var/lib/pacman/corrupt-backups/pacman-7.1.0.r9.g54d9411-2.corrupt-20260512
```

并用 `pacman -U --dbonly` 重建 pacman 本地数据库条目。

### 2. 重装关键系统包

通过 pacman 标准流程重装：

```text
pacman
linux
linux-lts
mkinitcpio
```

当前关键版本：

```text
pacman 7.1.0.r9.g54d9411-2
linux 7.0.5.arch1-1
linux-lts 6.18.29-1
mkinitcpio 41-3
```

### 3. 重建模块索引和 initramfs

已执行 `depmod` 和 `mkinitcpio -P`。现在两个可启动内核的模块索引存在：

```text
/usr/lib/modules/7.0.5-arch1-1/modules.dep
/usr/lib/modules/7.0.5-arch1-1/modules.devname
/usr/lib/modules/6.18.29-1-lts/modules.dep
/usr/lib/modules/6.18.29-1-lts/modules.devname
```

initramfs 内已确认包含：

```text
usb-storage.ko.zst
uas.ko.zst
modules.dep.bin
modules.devname
```

### 4. 添加 fallback 启动项

新增两个 systemd-boot fallback 菜单项：

```text
/boot/loader/entries/arch-linux-fallback.conf
/boot/loader/entries/arch-lts-fallback.conf
```

它们分别使用：

```text
/boot/initramfs-linux-fallback.img
/boot/initramfs-linux-lts-fallback.img
```

原有默认项未改，当前默认仍是：

```text
arch-linux.conf
```

### 5. 处理 pacnew

已备份相关配置到：

```text
/root/arch-repair-backups/20260512-230252
```

已做的低风险合并：

- `/etc/locale.gen` 合并了 pacnew 中新增的注释行：
  `#en_SE.UTF-8 UTF-8`
- 保留 `zh_CN.UTF-8 UTF-8` 为启用状态。
- 已运行 `locale-gen`。
- `/etc/gdm/custom.conf.pacnew` 已从 `/etc` 移走到备份目录。

保留未合并的高风险 pacnew：

```text
/etc/mkinitcpio.conf.pacnew
/etc/pacman.conf.pacnew
/etc/pacman.d/mirrorlist.pacnew
/etc/locale.gen.pacnew
```

原因：

- `mkinitcpio.conf.pacnew` 会移除当前 USB 启动需要的 `MODULES` 配置，不能覆盖。
- `pacman.conf.pacnew` 会移除当前 `multilib` / `archlinuxcn` 等仓库配置，不能直接覆盖。
- `mirrorlist.pacnew` 只有注释镜像，直接覆盖会导致没有可用镜像。
- `locale.gen.pacnew` 会把当前启用的 `zh_CN.UTF-8` 注释掉，不能直接覆盖。

### 6. 清理旧内核模块残留

已更新 `virtualbox-host-modules-arch` 到当前仓库版本：

```text
virtualbox-host-modules-arch 7.2.8-11
```

确认 VirtualBox 模块已匹配当前普通内核：

```text
vboxdrv -> /lib/modules/7.0.5-arch1-1/extramodules/vboxdrv.ko.zst
```

已删除 pacman 不拥有的旧模块残留：

```text
/usr/lib/modules/6.18.26-2-lts
/usr/lib/modules/7.0.3-arch1-1
```

清理后只保留：

```text
/usr/lib/modules/6.18.29-1-lts
/usr/lib/modules/7.0.5-arch1-1
```

### 7. 修复 /boot FAT dirty bit

日志曾提示：

```text
FAT-fs (sdb1): Volume was not properly unmounted. Some data may be corrupt. Please run fsck.
```

含义：`/boot` 是 vfat/FAT 分区，上次没有干净卸载，dirty bit 被置位。它不等于启动文件已经损坏，但需要 fsck 清理。

处理过程：

- 安装 `dosfstools`，提供 `fsck.vfat`。
- 临时卸载 `/boot`。
- 运行 `fsck.vfat -vn /dev/sdb1` 只读检查。
- 运行 `fsck.vfat -a /dev/sdb1` 自动修复 dirty bit。
- 再次运行离线 `fsck.vfat -vn /dev/sdb1` 确认干净。
- 重新挂载 `/boot`。

最终只读检查结果：

```text
/dev/sdb1: 33 files, 168940/261627 clusters
```

无 dirty bit 报告。

## Timeshift 问题说明

`timeshift-backup.service` 失败不是备份数据损坏，而是重复启动冲突：

```text
Another instance of this application is running
```

已有一个 Timeshift 进程在跑时，systemd 又启动了一次服务，第二个实例立刻失败。

当前已执行：

```text
systemctl reset-failed timeshift-backup.service
```

现在 `systemctl --failed` 为 0 个失败单元。

现有快照：

```text
/home/timeshift/snapshots/2026-05-12_22-00-00  daily
/home/timeshift/snapshots/2026-05-11_20-04-51  daily
/home/timeshift/snapshots/2026-05-01_07-35-24  ondemand
```

Timeshift 目录占用约：

```text
39G /home/timeshift
```

Timeshift 目标 UUID：

```text
5ebf9876-c9d0-4e69-b399-fa77d8ef3c09
```

这就是 `/dev/sdb3`，也就是同一块外接盘上的 `/home`。

结论：

- 它可以用于回滚坏包、坏配置、误改系统文件。
- 它不能作为硬盘损坏保护，因为 `/boot`、`/`、`/home`、Timeshift 快照都在同一块外接盘 `/dev/sdb` 上。
- 如果这块外接盘物理损坏，Timeshift 快照也会一起丢。

## 当前剩余隐患

### 1. 根分区空间仍偏紧

当前空间：

```text
/      32G  used 27G  avail 4.8G  85%
/boot   1G  used 660M avail 363M  65%
/home 202G used 134G avail 58G   70%
```

根分区 `/` 只有 32G，Arch 滚动更新、内核、显卡驱动、浏览器、Electron 软件会持续占空间。建议长期目标是让 `/` 至少保持 8-10G 空闲。

### 2. 仍有高风险 pacnew 未合并

这些保留是刻意的，不是忘记处理：

```text
/etc/mkinitcpio.conf.pacnew
/etc/pacman.conf.pacnew
/etc/pacman.d/mirrorlist.pacnew
/etc/locale.gen.pacnew
```

后续合并必须人工逐项比较，不能直接覆盖。

### 3. Timeshift 不防整盘故障

当前 Timeshift 是系统回滚，不是灾备。真正防丢数据需要另一个物理盘或远端备份。

### 4. 日志里仍有非启动类噪声

见到的非致命项包括：

- ACPI BIOS warning
- NVIDIA / i915 图形相关 warning
- `spark-update-notifier.service` unit 语法警告
- Syncthing 某个 folder path missing
- xdg-desktop-portal-gtk 无 display 失败

这些不影响这次 USB SSD 启动修复，但之后可以单独清理。

## 最终验证摘要

- 当前运行内核：`7.0.5-arch1-1`
- 当前根分区：`/dev/sdb2`
- 根分区 PARTUUID：`afa9da95-ee18-4ced-9e13-f764f54a392d`
- `/boot`：`/dev/sdb1`
- `/home`：`/dev/sdb3`
- `systemctl --failed`：0 个失败单元
- `pacman -Qkk` 对关键包显示 0 altered files
- `lsinitcpio` 确认普通版和 LTS initramfs 都包含 USB 存储模块
- `fsck.vfat -vn /dev/sdb1` 离线检查干净

