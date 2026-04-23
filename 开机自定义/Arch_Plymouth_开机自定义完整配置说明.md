# Arch 开机自定义完整配置说明

## 1. 思路总览

这次不是在改 BIOS/UEFI 的启动菜单图标，而是在改 **Linux 进入系统时的 Plymouth 开机动画层**。

整套思路分成 4 层：

1. **确认改动层级**
   - 先确认机器实际使用的是 `systemd-boot`。
   - 再确认开机动画真正由 `plymouth` 接管。
   - 这样可以把“开机图标 + 右下角文案 + 下半部分滚动日志”都放在 Plymouth 主题里做。

2. **先做主题结构**
   - 保留你喜欢的整体骨架：
     - 上半部分：左企鹅、中间大 Arch、右侧小白龙
     - 下半部分左侧：滚动 boot log
     - 下半部分右侧：文案信息区
   - 右侧文案区从一开始的普通文字，逐步升级成更极客、更亮、更有霓虹感的信息卡。

3. **解决“关机能显示中文，开机乱码”的根因**
   - 关机阶段已经挂载了系统根文件系统，所以系统字体可用。
   - 开机早期还在 `initramfs` 阶段，默认只有 Plymouth 自带字体，没有 `Noto Sans CJK`。
   - 所以最终的关键不是“换个中文句子”，而是 **把中文字体打进 initramfs**。

4. **最后才做正式安装**
   - 本地先做预览、做测试镜像、验证字体真的进了 initramfs。
   - 验证通过后，再正式安装到系统主题目录，并重建 `/boot/initramfs-linux.img`。

一句话总结：

> 这次改的是 **Plymouth 启动主题 + initramfs 中文字体支持**，不是单纯换几张图。

---

## 2. 最终目标和最终结果

### 2.1 最终目标

- 开机顶部显示：
  - 左侧经典 Linux 企鹅
  - 中间更大的 Arch 图标
  - 右侧 Kali 白龙
- 下半部分左侧显示滚动日志
- 下半部分右侧显示更极客、更大胆、更亮的中文信息区
- 开机标题改成：
  - `欢迎来到开源大世界`
- 右下角增加两行：
  - `进入风华的小窝`
  - `联系邮箱：1938355142@qq.com`
- `进入风华的小窝` 后面带一个小猫图标
- 开机阶段不再依赖 PNG 静态中文字，而是回到 **动态中文彩字**
- 通过把 `Noto Sans CJK` 加入 `initramfs`，解决开机中文乱码

### 2.2 当前已经落地的结果

当前系统已经完成这些关键状态：

- `/etc/plymouth/plymouthd.conf`
  - 当前主题为 `arch-triad`
- `/etc/mkinitcpio.conf`
  - `FILES=` 已加入 `/usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc`
- `/boot/initramfs-linux.img`
  - 已重建成功
  - 已确认包含 `NotoSansCJK-Regular.ttc`
- `/usr/share/plymouth/themes/arch-triad/`
  - 已安装新的 `arch-triad` 主题
  - 已包含 `cat-icon.png`
  - 已包含动态中文版本脚本

---

## 3. 关键配置文件总表

这次真正涉及到的配置和资源，按重要性排序如下：

### 3.1 系统级配置

1. `/etc/plymouth/plymouthd.conf`
2. `/etc/mkinitcpio.conf`
3. `/boot/initramfs-linux.img`

### 3.2 系统主题目录

1. `/usr/share/plymouth/themes/arch-triad/arch-triad.plymouth`
2. `/usr/share/plymouth/themes/arch-triad/arch-triad.script`
3. `/usr/share/plymouth/themes/arch-triad/archlinux-logo.png`
4. `/usr/share/plymouth/themes/arch-triad/tux.png`
5. `/usr/share/plymouth/themes/arch-triad/kali-dragon.png`
6. `/usr/share/plymouth/themes/arch-triad/cat-icon.png`
7. `/usr/share/plymouth/themes/arch-triad/line.png`

### 3.3 本地工作目录

1. `/home/tian/plymouth-themes/arch-triad/arch-triad.plymouth`
2. `/home/tian/plymouth-themes/arch-triad/arch-triad.script`
3. `/home/tian/plymouth-themes/arch-triad/install.sh`
4. `/home/tian/plymouth-themes/arch-triad/preview-boot.png`
5. `/home/tian/plymouth-themes/arch-triad/preview-shutdown.png`

---

## 4. 每一个 config 配置都改了什么

## 4.1 `/etc/plymouth/plymouthd.conf`

当前内容：

```ini
[Daemon]
Theme=arch-triad
# Administrator customizations go in this file
#[Daemon]
#Theme=fade-in
```

### 这一步的作用

- 明确告诉 Plymouth：
  - 当前默认主题不是系统自带的 `fade-in`
  - 而是我新做的 `arch-triad`

### 这一步的改动点

- 最核心的一行就是：

```ini
Theme=arch-triad
```

---

## 4.2 `/etc/mkinitcpio.conf`

这次最关键的新增配置是：

```bash
FILES=( /usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc)
```

当前系统里的相关部分是：

```bash
MODULES=(i915)
BINARIES=()
FILES=( /usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc)
HOOKS=(base udev plymouth autodetect microcode modconf kms keyboard keymap consolefont block filesystems fsck)
```

### 这一步的作用

- `HOOKS=(... plymouth ...)`
  - 确保 initramfs 里会启动 Plymouth
- `FILES=( /usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc )`
  - 明确把中文字体文件硬塞进 initramfs

### 这一步为什么是决定性的

因为之前开机时字体不在 initramfs 里，Plymouth 只能用自带英文字体：

- 英文可以
- 中文容易乱码或直接异常显示

把 `Noto Sans CJK` 放进去之后，开机阶段也能直接渲染中文。

---

## 4.3 `/usr/share/plymouth/themes/arch-triad/arch-triad.plymouth`

这是主题入口文件。

核心内容是：

```ini
[Plymouth Theme]
Name=Arch Triad
Description=Triad icon layout with live boot log and localized status text
ModuleName=script

[script]
ImageDir=/usr/share/plymouth/themes/arch-triad
ScriptFile=/usr/share/plymouth/themes/arch-triad/arch-triad.script
```

### 这一步的作用

- 指定这个主题是 `script` 类型
- 指定图片目录
- 指定实际执行的脚本文件是 `arch-triad.script`

---

## 4.4 `/usr/share/plymouth/themes/arch-triad/arch-triad.script`

这是整个主题的核心逻辑文件。

这个文件做了几件关键事情：

### A. 布局

- 上半部分三图标
- 下半部分一条横向分割线
- 下半部分中间一条竖向分割线
- 左边日志区
- 右边信息卡区

### B. 图标尺寸控制

核心变量：

```javascript
small_icon_size = Math.Int(screen_height * 0.15);
big_icon_size = Math.Int(screen_height * 0.24);
```

### 含义

- `small_icon_size`
  - 控制企鹅和白龙
- `big_icon_size`
  - 控制中间 Arch 图标

你后来要求“Arch 图标要大一点”，就是在这里把它放大的。

---

### C. 右侧动态中文区

核心文案变量：

```javascript
if (mode == "shutdown") {
  hero_mode = "SHUTDOWN";
  hero_title = "愿自由与代码，陪你下次再见";
  hero_subtitle = "系统正在安全关机";
  hero_quote = "同步磁盘，保存自由。";
} else if (mode == "reboot") {
  hero_mode = "REBOOT";
  hero_title = "马上回到开源大世界";
  hero_subtitle = "系统正在重新启动";
  hero_quote = "内核即将重新接管世界。";
} else {
  hero_mode = "BOOT";
  hero_title = "欢迎来到开源大世界";
  hero_subtitle = "Arch Linux 正在启动";
  hero_quote = "开源在前，世界可改。";
}

hero_home = "进入风华的小窝";
hero_email = "联系邮箱：1938355142@qq.com";
```

### 这一步的作用

- 分别定义开机、关机、重启三种状态的文案
- 开机时采用你要求的新标题：

```text
欢迎来到开源大世界
```

- 并在下面追加：
  - `进入风华的小窝`
  - `联系邮箱：1938355142@qq.com`

---

### D. 极客霓虹色动态变化

这一部分是通过 `Wave()` + `Mix()` + `RenderHero()` 实现的。

关键逻辑：

```javascript
fun Wave(time, speed, phase) {
  return (Math.Cos(time * speed + phase) + 1) / 2;
}

fun Mix(low, high, ratio) {
  return low + (high - low) * ratio;
}
```

以及：

```javascript
title_red = Mix(0.58, 1.00, title_wave);
title_green = Mix(0.88, 0.96, subtitle_wave);
title_blue = Mix(1.00, 0.82, quote_wave);
```

```javascript
quote_red = Mix(0.94, 1.00, quote_wave);
quote_green = Mix(0.22, 0.56, title_wave);
quote_blue = Mix(0.68, 1.00, quote_wave);
```

```javascript
home_red = Mix(0.30, 0.56, meta_wave);
home_green = Mix(0.90, 1.00, meta_wave);
home_blue = Mix(0.64, 0.88, title_wave);
```

```javascript
email_red = Mix(0.46, 0.74, email_wave);
email_green = Mix(0.96, 1.00, email_wave);
email_blue = Mix(0.42, 0.72, meta_wave);
```

### 这一步的作用

- 标题、引导句、附加信息、邮箱都不是静止死色
- 会随着时间轻微波动
- 形成一种更极客、更“活”的霓虹感

---

### E. 小猫图标

脚本里通过：

```javascript
hero_cat_source = Image("cat-icon.png");
hero_cat_image = hero_cat_source.Scale(18, 18);
```

把小猫图标放在：

```javascript
hero_cat.SetPosition(hero_left + home_measure.GetWidth() + 10, hero_top + 153, 97);
```

### 这一步的作用

- 把小猫放在 `进入风华的小窝` 这一行后面
- 不是系统 emoji 字体直接渲染
- 而是单独的图像资源，稳定、不会因为 emoji 渲染器不同而坏掉

---

## 4.5 `/home/tian/plymouth-themes/arch-triad/install.sh`

这是我做的安装脚本。

当前逻辑重点如下：

### A. 拷贝主题资源到系统目录

```bash
sudo install -d "${TARGET_DIR}"
sudo install -m 0644 \
  "${SCRIPT_DIR}/arch-triad.plymouth" \
  "${SCRIPT_DIR}/arch-triad.script" \
  "${SCRIPT_DIR}/archlinux-logo.png" \
  "${SCRIPT_DIR}/cat-icon.png" \
  "${SCRIPT_DIR}/boot-quote.png" \
  "${SCRIPT_DIR}/boot-subtitle.png" \
  "${SCRIPT_DIR}/boot-title.png" \
  "${SCRIPT_DIR}/kali-dragon.png" \
  "${SCRIPT_DIR}/line.png" \
  "${SCRIPT_DIR}/reboot-quote.png" \
  "${SCRIPT_DIR}/reboot-subtitle.png" \
  "${SCRIPT_DIR}/reboot-title.png" \
  "${SCRIPT_DIR}/shutdown-quote.png" \
  "${SCRIPT_DIR}/shutdown-subtitle.png" \
  "${SCRIPT_DIR}/shutdown-title.png" \
  "${SCRIPT_DIR}/tux.png" \
  "${TARGET_DIR}/"
```

### B. 自动把中文字体加到 mkinitcpio

```bash
if ! grep -Fq "${FONT_FILE}" /etc/mkinitcpio.conf; then
  sudo perl -0pi -e 's/^FILES=\((.*?)\)$/FILES=($1 \/usr\/share\/fonts\/noto-cjk\/NotoSansCJK-Regular.ttc)/m' /etc/mkinitcpio.conf
fi
```

### C. 重新构建 initramfs

```bash
sudo plymouth-set-default-theme "${THEME_NAME}" --rebuild-initrd
```

### 这一步的作用

- 一次完成：
  - 复制主题
  - 修补字体
  - 重建 initramfs

---

## 5. 详细实施过程：我是怎么一步一步做的

## 第 1 步：先确认机器的引导链路

先查了：

- `bootctl status`
- `efibootmgr -v`
- `/boot` 结构

确认结论：

- 系统引导器是 `systemd-boot`
- 但你真正想改的视觉内容，不是 UEFI 启动菜单，而是进入 Linux 时的 Plymouth

这一步非常重要。

因为如果一开始搞错层级，后面做再多图也会改不到你看到的地方。

---

## 第 2 步：确认 Plymouth 已经启用

检查了：

- `pacman -Qs plymouth`
- `/etc/mkinitcpio.conf`
- `/etc/plymouth/plymouthd.conf`

确认结果：

- `plymouth` 已安装
- `mkinitcpio` 的 `HOOKS=` 已包含 `plymouth`
- 系统可以直接走 Plymouth 主题路线

---

## 第 3 步：找现有主题基础和可复用素材

检查了：

- `/usr/share/plymouth/themes/arch-stream`
- `/usr/share/plymouth/themes/arch-mac-style`

最后决定：

- 不从零开始写一个陌生结构
- 而是基于现有的 `arch-stream` 思路，扩展成自己的 `arch-triad`

复用了这些基础资源：

- `archlinux-logo.png`
- `tux.png`

---

## 第 4 步：做右上角的小白龙素材

本机没有现成可直接用的 Kali PNG，于是我做了几轮处理：

1. 找到了本机已有的 Kali SVG 资源
2. 转成 PNG
3. 去掉白底
4. 调整成透明背景
5. 对线条做加粗处理

中间踩过的坑：

- 第一次出来带白色底块
- 第二次透明了，但是太淡
- 第三次线条太细，看起来发虚
- 最后才做成现在这种透明背景 + 更清晰轮廓的版本

---

## 第 5 步：做初版主题布局

初版做的是：

- 上面三图标
- 下面横线
- 左边滚动日志
- 右边文案区

然后根据你的反馈继续改：

- 中间 Arch 图标太小 -> 放大
- 关机文案格式你喜欢 -> 保留那个骨架
- 开机文案不满意 -> 改成“欢迎来到开源大世界”
- 右下角不够极客 -> 改成更亮、更霓虹

---

## 第 6 步：发现“关机能显示中文，开机不稳定”

这是整个过程中最关键的技术问题。

你的反馈是：

- 关机时能看到我写的中文
- 开机时右下角会乱码

我当时没有直接拍脑袋说“换字体就行”，而是去查根因。

---

## 第 7 步：第一次临时绕过方案：静态 PNG 中文

在还没确认根因前，我先做过一个中间版本：

- 把右侧中文文案都预生成成 PNG
- 在 Plymouth 里直接显示图片

这个方案的优点：

- 稳
- 不依赖实时中文字体渲染

缺点：

- 不是真正动态文字
- 颜色变化和交互自由度低

这个版本只是过渡方案，不是最终方案。

---

## 第 8 步：精确证明根因

我去检查了 `/boot/initramfs-linux.img` 里到底有什么：

用的是：

```bash
lsinitcpio -l /boot/initramfs-linux.img | rg 'NotoSansCJK|usr/share/fonts|arch-triad'
```

当时看到的是：

- 有 `Plymouth.ttf`
- 有 `Plymouth-monospace.ttf`
- **没有** `Noto Sans CJK`

于是根因就明确了：

### 为什么关机能显示中文？

因为关机时系统根文件系统已经挂载了：

- `/usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc` 可见

### 为什么开机早期不行？

因为开机早期还在 initramfs 阶段：

- 系统字体还没有自动可用
- 默认只有 Plymouth 自带英文字体

这就是“关机正常、开机乱码”的本质原因。

---

## 第 9 步：先做一个本地测试镜像，不直接赌系统

为了避免直接改系统配置后再靠重启碰运气，我先做了一个本地测试：

### 9.1 生成测试版 `mkinitcpio` 配置

我从 `/etc/mkinitcpio.conf` 拷贝出一个测试文件：

```bash
/home/tian/plymouth-themes/arch-triad/mkinitcpio-test.conf
```

然后把：

```bash
FILES=()
```

改成：

```bash
FILES=(/usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc)
```

### 9.2 构建测试镜像

构建命令类似：

```bash
mkinitcpio -c /home/tian/plymouth-themes/arch-triad/mkinitcpio-test.conf -g /tmp/arch-triad-test.img ...
```

### 9.3 验证测试镜像内容

再查：

```bash
lsinitcpio -l /tmp/arch-triad-test.img | rg 'NotoSansCJK|arch-triad'
```

确认结果：

```text
usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc
```

这一步证明了：

> 中文字体确实可以被打进 initramfs，方案技术上是可行的。

---

## 第 10 步：把主题改回“动态中文彩字”

在确认字体方案可行后，我就不再停留在 PNG 静态文字方案上，而是重新回到动态字。

也就是说，最后安装到系统里的主题逻辑是：

- 右下角：
  - 不是一张整块图片
  - 而是 `Image.Text(...)` 动态渲染的中文
- 同时通过 `Wave()` + `Mix()` 让颜色轻微流动

这一步才是你想要的真正形态：

- 中文
- 动态
- 有颜色变化
- 更极客

---

## 第 11 步：加入“小猫”和更极客的信息区

你后面追加了两个新要求：

1. 把开机句子改成带“开源”字样
2. 再多加两行：
   - `进入风华的小窝`
   - `联系邮箱：1938355142@qq.com`

以及：

- 右下角颜色要更大胆
- 更极客

这一步做了这些改动：

- 把开机标题改成：
  - `欢迎来到开源大世界`
- 把开机引导句改成：
  - `开源在前，世界可改。`
- 新增：
  - `进入风华的小窝`
  - `联系邮箱：1938355142@qq.com`
- 新增小猫图标 `cat-icon.png`
- 加了一块半透明深色信息卡底板
- 左边加了一条亮色竖边，增强信息卡感
- 使用青色、紫色、绿色几组颜色做动态波动

---

## 第 12 步：正式安装并重建 initramfs

你最后执行了：

```bash
sudo /home/tian/plymouth-themes/arch-triad/install.sh
```

这一步实际做了：

1. 把主题文件装进 `/usr/share/plymouth/themes/arch-triad/`
2. 把 `NotoSansCJK-Regular.ttc` 写入 `/etc/mkinitcpio.conf` 的 `FILES=`
3. 调用：

```bash
plymouth-set-default-theme arch-triad --rebuild-initrd
```

4. 触发重新构建：

```text
/boot/initramfs-linux.img
```

构建日志显示成功。

---

## 6. 过程中遇到的所有重要问题

## 问题 1：一开始容易误判成 UEFI 启动图标问题

### 现象

你说的是“开机图标”，很容易第一反应去改 EFI 启动菜单。

### 实际情况

你真正想改的，是 Linux 进系统时的视觉层。

### 解决方式

先查 `bootctl status`、`efibootmgr -v`、`plymouth` 配置，确认层级之后才开始动手。

---

## 问题 2：Kali 白龙素材不好直接拿来用

### 现象

- 有白底
- 透明不对
- 线条太淡

### 解决方式

- 找到本机 SVG
- 去底
- 转透明背景
- 加粗线条

---

## 问题 3：关机有中文，开机乱码

### 现象

- 关机能显示中文
- 开机右下角乱码

### 真正原因

- 关机：已经能访问系统字体
- 开机：还在 initramfs，只有 Plymouth 自带字体

### 解决方式

- 先用 PNG 文案临时兜底
- 再验证并加入 `Noto Sans CJK` 到 initramfs
- 最后恢复动态中文渲染

---

## 问题 4：彩色 emoji 路线不稳定

### 现象

我试过直接让 ImageMagick 用 `Noto Color Emoji` 渲染猫 emoji，报错：

```text
invalid pixel size
```

### 解决方式

不再赌 emoji 字体渲染，改成单独的小猫图像资源。

---

## 问题 5：第一版小猫图标太像方块，不像猫

### 现象

最初使用的符号猫图标，在小尺寸下视觉效果很差。

### 解决方式

换成了本机 `kitty` 图标裁小后的版本，风格更“极客小猫”。

---

## 问题 6：尝试用 OMX team 并行继续做，但环境不满足

### 现象

我按你的 `$team` 指令尝试走 `omx team 3:executor`，但 runtime 报：

```text
Team mode requires tmux
```

### 真正原因

- 虽然机器装了 `tmux`
- 但当前 leader 进程不在一个可用的 tmux 运行上下文里

### 解决方式

取消 team，改回 solo 继续完成。

这不影响最终主题配置，只影响执行方式。

---

## 7. 最终验证是怎么做的

我做了 3 类验证。

## 7.1 配置验证

检查：

```bash
sed -n '1,220p' /etc/plymouth/plymouthd.conf
```

确认：

```ini
Theme=arch-triad
```

---

## 7.2 initramfs 内容验证

检查：

```bash
lsinitcpio -l /boot/initramfs-linux.img | rg 'NotoSansCJK|arch-triad|cat-icon|ttc'
```

确认结果包括：

```text
usr/share/fonts/noto-cjk/NotoSansCJK-Regular.ttc
usr/share/plymouth/themes/arch-triad/arch-triad.script
usr/share/plymouth/themes/arch-triad/cat-icon.png
```

这说明：

- 字体进去了
- 主题进去了
- 小猫资源也进去了

---

## 7.3 视觉预览验证

本地预览文件：

- `/home/tian/plymouth-themes/arch-triad/preview-boot.png`
- `/home/tian/plymouth-themes/arch-triad/preview-shutdown.png`

这两张图用于在不重启的情况下先看版式是否合理。

---

## 8. 当前主题里哪些资源是“正在用的”，哪些是“历史遗留的”

## 8.1 当前正在用的

- `arch-triad.script`
- `arch-triad.plymouth`
- `archlinux-logo.png`
- `tux.png`
- `kali-dragon.png`
- `cat-icon.png`
- `line.png`

## 8.2 仍然被复制进系统，但现在脚本不再依赖的

- `boot-title.png`
- `boot-subtitle.png`
- `boot-quote.png`
- `shutdown-title.png`
- `shutdown-subtitle.png`
- `shutdown-quote.png`
- `reboot-title.png`
- `reboot-subtitle.png`
- `reboot-quote.png`

这些是中间阶段的静态文案资源。

现在脚本已经回到动态中文字路线，所以它们属于：

> 保留但不再是主方案依赖

如果以后想彻底精简，可以从脚本和安装脚本里一起清理掉。

---

## 9. 如果以后你自己要继续改，应该改哪里

## 9.1 改开机/关机文案

改这里：

```javascript
if (mode == "shutdown") {
  ...
} else if (mode == "reboot") {
  ...
} else {
  ...
}
```

关键变量：

- `hero_title`
- `hero_subtitle`
- `hero_quote`
- `hero_home`
- `hero_email`

---

## 9.2 改右下角颜色风格

改 `RenderHero(time)` 里的颜色混合参数：

- `title_red / title_green / title_blue`
- `subtitle_red / subtitle_green / subtitle_blue`
- `quote_red / quote_green / quote_blue`
- `home_red / home_green / home_blue`
- `email_red / email_green / email_blue`

你如果想更夸张一点，可以把：

- 青色更亮
- 紫色更粉
- 绿色更荧光

---

## 9.3 改中间 Arch 图标大小

改：

```javascript
big_icon_size = Math.Int(screen_height * 0.24);
```

放大就增加 `0.24`。

---

## 9.4 改白龙和企鹅大小

改：

```javascript
small_icon_size = Math.Int(screen_height * 0.15);
```

---

## 9.5 改信息卡位置和大小

改：

```javascript
hero_panel_width = Math.Int(screen_width * 0.29);
hero_panel_height = Math.Int(screen_height * 0.24);
hero_left = center_x + screen_width * 0.06;
hero_top = divider_y + 40;
```

---

## 10. 建议你以后记住的关键经验

这次最有价值的经验，不是“Plymouth 脚本怎么写”，而是这几个判断：

1. **先判断你改的是哪一层**
   - UEFI 菜单层
   - bootloader 层
   - Plymouth 层
   - desktop 登录层

2. **开机阶段和关机阶段不是同一个资源环境**
   - 开机早期很多系统资源还没挂载
   - 关机时系统字体往往已经可用

3. **遇到乱码，先查资源是否真的进了 initramfs**
   - 不要只靠猜

4. **先做本地测试镜像，再动系统正式镜像**
   - 这一步非常值

---

## 11. 最后的状态总结

到目前为止，这套自定义已经完成了下面这些核心目标：

- 开机顶部三图标布局完成
- 中间 Arch 图标已加大
- 右侧信息区改成你要的“更极客、更大胆”风格
- 开机标题改成：
  - `欢迎来到开源大世界`
- 右下角加入：
  - `进入风华的小窝`
  - `联系邮箱：1938355142@qq.com`
- 小猫图标已加入
- 中文字体已写入 `mkinitcpio` 配置
- `initramfs` 已重建
- 已确认新镜像中包含 `Noto Sans CJK`

如果后面你还想继续加强，我建议优先改这三个方向：

1. 右下角颜色动态更激进一点
2. 白龙图标再精细一点
3. 清理掉旧的 PNG 文案遗留文件，让主题目录更干净

