# Arch Linux 下 AAGL 无法启动原神的完整排障记录

记录日期：2026-04-30

这份记录是这次在我的机器上排查 `an-anime-game-launcher`（下面简称 `AAGL`）无法启动国际服原神时整理出来的完整过程。目标不是只记“最后怎么修好”，而是把中间的判断、误判、证据、修复动作、为什么这样修、以后怎么自己查，全部串起来。

---

## 1. 问题现象

我的实际现象是：

- 前一天晚上还能正常进入原神。
- 第二天白天之后，AAGL 启动原神失败。
- 我是双系统，游戏目录放在另一个分区里。
- 我当天早上动过 Wine 环境，所以一开始怀疑是不是自己改 Wine 把原神搞坏了。

当时最容易怀疑的几个方向是：

1. 双系统分区是不是只读、没挂好、AAGL 看不到游戏。
2. AAGL 自己坏了。
3. 我早上改的 Wine 把原神这套前缀带坏了。
4. 显卡驱动或者 Flatpak 图形环境出问题了。

---

## 2. 先把环境关系理清

这一步很重要。如果环境边界没分清，后面很容易乱改。

这台机器上实际有三套互相相关但不完全相同的东西：

### 2.1 系统通用 Wine

就是平时在 shell 里直接用的：

- `wine`
- `winetricks`
- 自己建的 `WINEPREFIX`

例如我历史里有过：

```bash
WINEPREFIX=~/wine-netease winecfg
```

这类操作影响的是“通用 Windows 程序运行环境”。

### 2.2 AAGL 自己的 runner

AAGL 不是直接吃系统 Wine，它有自己下载的 Wine runner。

路径在：

```text
/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/runners
```

我当时在用的是：

```text
spritz-wine-cachyos-wow64-10.0-8
```

后面修复时又加了：

```text
spritz-wine-cachyos-wow64-10.0-9
```

### 2.3 AAGL 自己的 Wine prefix

原神实际跑的是 AAGL 自己的前缀，不是我系统里随便哪个 `WINEPREFIX`。

路径在：

```text
/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/prefix-global
```

所以一个核心结论是：

> “我早上改了 Wine” 不自动等于 “原神这套 AAGL 环境也跟着坏了”。

它可能有关，但不能直接画等号。

---

## 3. 第一轮排查：先确认是不是分区或路径问题

因为我是双系统，游戏又放在别的分区，第一轮最自然就是查路径和挂载。

### 3.1 AAGL 的配置文件

关键配置文件：

[`config.json`](/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/config.json:1)

里面确认了几件事：

- 游戏 edition 是 `Global`
- 游戏路径是：

```text
/media/fangfang/Genshin Impact/Genshin Impact game
```

- 使用的前缀是：

```text
/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/prefix-global
```

- 使用的 runner 是：

```text
spritz-wine-cachyos-wow64-10.0-8
```

### 3.2 检查游戏目录本体

后面验证到：

- `GenshinImpact.exe` 存在
- 游戏目录能被 AAGL 访问
- Flatpak 也已经有 `/media`、`/media/fangfang` 的访问权限

所以这一步的结论是：

> 不是“找不到游戏目录”，也不是“路径写错了”。

### 3.3 关于“只读挂载”的误判

最开始在受限执行环境里看到过 `/media` 的挂载视图里有 `ro` 痕迹，所以一度怀疑分区只读。

但这一步后来被你纠正了：

> “你看到的是只读，我看到的可能是 rw”

这句话非常关键。因为我这边能看到的是当前执行环境的视图，不一定等于你桌面会话真实挂载状态。

后面的证据也证明：

- AAGL 能读取游戏目录
- 原神日志能写回游戏目录里的 `driverError.log`

所以后面我不再把“分区只读”当主因。

这也是这次排障里第一个要记住的点：

> 对图形程序、Flatpak、双系统挂载这类问题，CLI 沙箱里看到的状态不一定等于桌面真实状态，必须用后续证据纠偏。

---

## 4. 第二轮排查：看历史和日志，而不是只凭感觉

### 4.1 把旧会话翻出来

我去查了之前的 Codex 会话历史，发现这台机器以前已经跑通过国际服原神。

旧会话的意义是：

- 能证明“这台机器不是天生跑不了”
- 能证明“这套路径以前确实工作过”
- 能缩小成“最近某个状态变化导致退化”

### 4.2 旧状态给出的关键信息

旧线索说明：

- 国际服路径当时就是现在这个 `/media/fangfang/Genshin Impact/Genshin Impact game`
- 之前确实已经进过游戏
- 之前还出现过“跑到 Intel 核显”的相关问题

后来在：

[`info.txt`](/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/prefix-global/drive_c/users/tian/AppData/LocalLow/miHoYo/Genshin%20Impact/info.txt:1)

里也能看到曾经成功跑到 NVIDIA 的记录。

这一步的结论是：

> 机器以前能跑，当前故障是回归问题，不是从来没支持过。

---

## 5. 第三轮排查：把“失败类型”分开看

这次不能启动，实际上不是只有一种错误。

### 5.1 第一类错误：图形初始化失败

我在：

[`output_log.txt`](/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/prefix-global/drive_c/users/tian/AppData/LocalLow/miHoYo/Genshin%20Impact/output_log.txt:1)

里看到过：

```text
d3d11: failed to create factory (80004005)
```

这说明有一段时间原神是卡在 `d3d11/dxgi` 图形初始化阶段。

### 5.2 第二类错误：HoYoProtect / WDFLDR

在：

[`game.log`](/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/game.log:1)

和：

[`driverError.log`](/media/fangfang/Genshin%20Impact/Genshin%20Impact%20game/driverError.log:1)

里又能看到另一种错误：

```text
Library WDFLDR.SYS ... needed by HoYoKProtect.sys not found
initDriver Failed
```

也就是：

- `HoYoKProtect.sys` 想加载
- 依赖 `WDFLDR.SYS`
- 这条链在 Wine 里没起来

### 5.3 为什么要把两类错误分开

如果混在一起看，很容易以为只有一个根因。

但实际上这次是两层问题叠在一起：

1. 一层是显卡 / Flatpak / Vulkan 可见性问题
2. 另一层是 Wine runner / HoYoProtect 兼容问题

这就是后面为什么修完一个问题，日志又会露出第二个问题。

---

## 6. 一次错误尝试：把旧 `prefix` 复制成 `prefix-global`

当时我为了尽快排除前缀损坏，做过一次尝试：

- 先备份当前 `prefix-global`
- 再用旧的 `prefix` 复制成新的 `prefix-global`

备份目录是：

```text
/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/prefix-global.bak-20260430T094631Z
```

这一步不是完全没价值，但结果证明：

- 它消掉了之前的一些图形初始化痕迹
- 但也让 AAGL 更稳定地掉进了 `HoYoProtect/WDFLDR` 这条路径

为什么？

因为旧 `prefix` 不是“昨晚那套真的在跑国际服的完整运行态”，它少了很多原神实际运行时留下的状态文件。

后来比对发现：

- 备份出来的旧 `prefix-global` 里有完整的 `AppData/LocalLow/miHoYo/Genshin Impact` 运行态
- 我复制进去的新前缀只有很少的基础文件

这说明：

> 前缀不是只看“能不能启动 Wine”，还要看是不是保留了游戏实际跑过后的状态。

所以后面我把真正的备份前缀又恢复回去了。

---

## 7. 关键纠偏：宿主机 NVIDIA 其实没坏

中途一度怀疑过：

- 你白天升级了 `nvidia-open`
- 升级了内核
- 升级了 Wayland / Xwayland / Hyprland 相关组件

所以最开始我有个判断是“宿主机 NVIDIA 可能挂了”。

但你随后在自己的桌面终端给出更强的证据：

```bash
nvidia-smi
prime-run vulkaninfo --summary
```

这两条都正常。

这一步非常关键，因为它推翻了“宿主机 NVIDIA 坏掉”这个猜测。

正确结论变成：

> 宿主机 NVIDIA 正常，问题出在 Flatpak 容器内部看不到正确的 NVIDIA 图形运行时。

这个点是整次排障的核心转折。

---

## 8. 真正的第一主因：Flatpak 里的 NVIDIA runtime 版本不匹配

### 8.1 查 Flatpak runtime

我查了 Flatpak 里的图形 runtime，发现是：

- 宿主机驱动版本：`595.71.05`
- Flatpak 里已有的 NVIDIA GL runtime：`595-58-03`

也就是版本不匹配。

当时系统里能看到的是：

```text
org.freedesktop.Platform.GL.nvidia-595-58-03
org.freedesktop.Platform.GL32.nvidia-595-58-03
```

但没有：

```text
org.freedesktop.Platform.GL.nvidia-595-71-05
org.freedesktop.Platform.GL32.nvidia-595-71-05
```

### 8.2 这会导致什么

即使宿主机 `nvidia-smi` 正常，AAGL 作为 Flatpak 应用，也可能：

- 在容器里看不到匹配的 NVIDIA Vulkan/GL 驱动
- 只能看到 Intel 或 llvmpipe
- 导致 DXVK 初始化异常，或者只能走错误 GPU

### 8.3 直接证据

我在 AAGL 的 Flatpak 环境里直接启动游戏，抓到了这样的输出：

```text
Found device: Intel(R) UHD Graphics 630
Found device: llvmpipe
DXVK: No adapters found. Please check your device filter settings
```

这说明那时容器里根本没有正确的 NVIDIA 设备链。

### 8.4 修复动作

我安装了匹配版本的 Flatpak runtime：

```bash
flatpak install -y flathub-official \
  org.freedesktop.Platform.GL.nvidia-595-71-05 \
  org.freedesktop.Platform.GL32.nvidia-595-71-05
```

这里还踩到一个点：

- 你的 `flathub` system remote 指向 `https://mirror.sjtu.edu.cn/flathub`
- 这个镜像当时 SSL 出错

所以后来改为用已有的：

```text
flathub-official -> https://dl.flathub.org/repo/
```

### 8.5 修复后验证

修完后再进容器检查，已经能看到：

- `nvidia-595-71-05`
- `nvidia_icd.json`

再直接跑游戏时，DXVK 输出变成：

```text
Found device: NVIDIA GeForce GTX 1660 Ti
Creating device: NVIDIA GeForce GTX 1660 Ti
```

这一刻可以明确宣布：

> 第一层问题已经解决：AAGL 终于能在 Flatpak 里看到并使用宿主机的 NVIDIA 了。

---

## 9. 第二主因：旧 runner `10.0-8` 仍然会撞 `HoYoProtect/WDFLDR`

虽然图形链修好了，但当时继续测试，旧 runner：

```text
spritz-wine-cachyos-wow64-10.0-8
```

还是会在某些情况下撞：

```text
WDFLDR.SYS
HoYoKProtect.sys
initDriver Failed
```

这说明第一层不是全部问题。

### 9.1 为什么怀疑 runner

我看了 AAGL 的 components 索引，里面已经有更新的版本：

```text
spritz-wine-cachyos-wow64-10.0-9
```

并且资源时间是比较新的。

这时候合理推断是：

- `10.0-8` 对你当前这版原神 / 这套环境不够稳
- `10.0-9` 可能已经修了你现在踩到的兼容问题

### 9.2 修复动作

我没有删旧 runner，而是把新 runner 并排装进去：

```text
/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/runners/spritz-wine-cachyos-wow64-10.0-9
```

然后把 AAGL 配置里的选中项从：

```text
spritz-wine-cachyos-wow64-10.0-8
```

改成：

```text
spritz-wine-cachyos-wow64-10.0-9
```

### 9.3 修复后验证

换到 `10.0-9` 后，再直接在 AAGL 的 Flatpak 环境里启动原神，输出已经明显不同：

- 不再一上来就掉进 `WDFLDR.SYS` 失败
- DXVK 成功绑定 NVIDIA
- 成功创建 swapchain
- 原神自己的 `output_log.txt` 开始继续往后写

关键输出类似：

```text
Found device: NVIDIA GeForce GTX 1660 Ti
Creating device: NVIDIA GeForce GTX 1660 Ti
Presenter: Actual swapchain properties...
```

这意味着：

> 第二层问题也被绕过去了，启动过程已经进入真正的图形渲染和登录页初始化阶段。

---

## 10. 为了更稳，我顺手改了哪些启动参数

除了图形 runtime 和 runner，我还调了 AAGL 的配置，让它先走更保守、更稳的窗口路径，而不是独占全屏。

改动在：

[`config.json`](/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/config.json:1)

主要改了：

- `borderless: false -> true`
- `virtual_desktop.enabled: false -> true`
- `fsr.enabled: true -> false`

### 10.1 为什么这么改

这是为了先降低变量数量：

- `exclusive fullscreen` 在 Wayland / 合成器环境里更容易多出一层问题
- 先用 `borderless + virtual desktop` 更适合做“能不能启动进窗口”的验证
- 先关掉 FSR，避免又把图形问题和放大/注入类问题混在一起

等完全稳定后，这些功能以后可以再逐步开回去测试。

---

## 11. 我额外做的整理：把“通用 Wine”和“AAGL 专用环境”分开

因为你说了一个很重要的目标：

> “我想的是后面可以大部分 wine 用我的 windows 都可以正常用”

这个目标是合理的，但必须和 AAGL 专用环境分开。

所以我在：

[`~/.zshrc`](/home/tian/.zshrc:1)

里加了几组辅助函数，用来明确区分：

- 通用 Wine 区
- AAGL 专用原神区

加的命令有：

- `wg`
- `wgt`
- `wwine`
- `wwinetricks`
- `aaglpaths`

另外还写了说明：

[`wine-usage-separation.md`](/home/tian/文档/wine-usage-separation.md:1)

目的就是：

> 以后你折腾通用 Wine，不要再误碰 AAGL 的 `prefix-global`、`prefix`、`runners`。

这不是本次“能启动原神”的主修复，但它是防止以后再次误伤的重要整理。

---

## 12. 最终修复动作清单

如果以后只想快速知道“我这次到底改了什么”，看这一节就够。

### 12.1 恢复前缀

恢复了真正有运行态的 `prefix-global`，不再用那个临时复制出来的瘦身前缀。

### 12.2 安装匹配的 Flatpak NVIDIA runtime

新增：

- `org.freedesktop.Platform.GL.nvidia-595-71-05`
- `org.freedesktop.Platform.GL32.nvidia-595-71-05`

### 12.3 新增并切换 AAGL runner

新增 runner：

```text
spritz-wine-cachyos-wow64-10.0-9
```

并把 AAGL 从 `10.0-8` 切到了 `10.0-9`。

### 12.4 调整 AAGL 启动参数

- 打开 `borderless`
- 打开 `virtual desktop`
- 暂时关闭 `FSR`

### 12.5 整理通用 Wine 与 AAGL 环境边界

更新了：

- `~/.zshrc`
- `~/文档/wine-usage-separation.md`

---

## 13. 本次真正改动了哪些文件和目录

### 13.1 配置文件

- [`config.json`](/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/config.json:1)
- [`~/.zshrc`](/home/tian/.zshrc:1)
- [`wine-usage-separation.md`](/home/tian/文档/wine-usage-separation.md:1)

### 13.2 AAGL 数据目录

- 恢复了：
  `~/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/prefix-global`
- 保留了旧备份：
  `prefix-global.bak-20260430T094631Z`
- 保留了中途试验前缀：
  `prefix-global.from-old-prefix-20260430T1805`

### 13.3 runner 目录

- 旧版还在：
  `spritz-wine-cachyos-wow64-10.0-8`
- 新增：
  `spritz-wine-cachyos-wow64-10.0-9`

### 13.4 Flatpak runtime

新增安装：

- `org.freedesktop.Platform.GL.nvidia-595-71-05`
- `org.freedesktop.Platform.GL32.nvidia-595-71-05`

---

## 14. 这次排障里最值得学的几个方法

### 14.1 不要把“最近改过 X”直接当成根因

你早上改过 Wine，这当然值得怀疑。

但不能因为“刚改过”就直接断定“就是它坏的”。

后面真正的第一主因其实是：

- Flatpak 里的 NVIDIA runtime 版本不匹配

### 14.2 宿主机正常，不等于容器里正常

这是这次最关键的一课。

哪怕：

- `nvidia-smi` 正常
- `prime-run vulkaninfo` 正常

也不代表 Flatpak 里的 AAGL 一定正常。

因为 Flatpak 还依赖自己那套图形 runtime。

### 14.3 日志要分层看

这次至少有三层日志：

1. AAGL 自己的启动日志
2. Wine / DXVK 输出
3. 原神自己的 `output_log.txt` 和 `driverError.log`

只看其中一层，很容易误判。

### 14.4 一个错误修掉后，第二个错误才会露出来

最开始我们只看到：

- `d3d11/dxgi` 初始化问题

修完图形链后，才更稳定地暴露出：

- `WDFLDR/HoYoProtect`

再换 runner 后，才真正进入游戏渲染阶段。

所以排障不是“一次看到一个日志就宣布根因”，而是：

> 修一层，验证一层，再看下一层。

### 14.5 重要运行态要备份

`prefix-global` 这种东西不能随便删，因为里面有大量“已经跑起来后留下的状态”。

这次如果没有先备份，再回滚就麻烦很多。

---

## 15. 以后如果再遇到类似问题，建议按这个顺序查

### 第一步：确认 AAGL 配置

看：

[`config.json`](/home/tian/.var/app/moe.launcher.an-anime-game-launcher/data/anime-game-launcher/config.json:1)

确认：

- 游戏路径
- prefix
- selected runner
- borderless / virtual desktop / FSR

### 第二步：确认宿主机显卡

跑：

```bash
nvidia-smi
prime-run vulkaninfo --summary
```

如果这两步都不对，那先修宿主机显卡链。

### 第三步：确认 Flatpak 里的 NVIDIA runtime 是否匹配

跑：

```bash
flatpak list --runtime --columns=application,branch,origin | rg 'nvidia|GL'
```

检查：

- 宿主机 NVIDIA 版本
- Flatpak `GL.nvidia-*` 版本

是不是一致。

### 第四步：直接在 AAGL 容器里看它看到什么 GPU

如果能运行 `vulkaninfo` 最好。

不行的话，也可以像这次一样直接启动游戏，看 DXVK 输出里是：

- 只看到 Intel / llvmpipe
- 还是已经能看到 NVIDIA

### 第五步：再看 runner 是否过旧

如果图形链没问题，但还卡在 Wine/兼容层：

- 看 components 索引里有没有更新的 runner
- 优先并排安装，不要直接删旧版本

### 第六步：最后才考虑前缀损坏

前缀是重要状态，不能一上来就删。

正确顺序应当是：

1. 先备份
2. 再做替换或回滚
3. 保留可退回的版本

---

## 16. 目前我建议保持的状态

当前建议先保持这样：

- AAGL 使用 `spritz-wine-cachyos-wow64-10.0-9`
- 保留已经安装好的 `595-71-05` Flatpak NVIDIA runtime
- 暂时保持 `borderless + virtual desktop`
- 暂时保持 `FSR` 关闭

如果后面确认非常稳定，再逐步尝试：

1. 关闭 virtual desktop
2. 再试是否要打开 FSR
3. 再试是否要回到更激进的显示模式

不要一次同时改三四项，不然下次出问题又很难定位。

---

## 17. 一句话总结这次的根因

这次不是单一问题，而是两层叠加：

1. 宿主机 NVIDIA 升级后，Flatpak 里的 NVIDIA runtime 没同步，导致 AAGL 容器内最开始看不到正确的 NVIDIA。
2. 即使图形链修好，旧 runner `spritz-wine-cachyos-wow64-10.0-8` 仍然不够稳；切到 `10.0-9` 后才真正进入游戏渲染和登录流程。

所以这次最准确的结论不是：

- “就是你改 Wine 改坏了”

而是：

> “你的通用 Wine 改动不是主因；真正的主链路问题是 Flatpak NVIDIA runtime 不匹配，加上 AAGL 旧 runner 对当前状态兼容不稳。”

---

## 18. 附：这次我自己会记住的坑

### 坑 1

看到双系统分区、挂载信息时，不能过早把 `ro/rw` 当成主结论。

### 坑 2

宿主机 `nvidia-smi` 正常，不代表 Flatpak 应用就一定能看到 NVIDIA。

### 坑 3

`prefix` 和 `prefix-global` 不能混着当成一回事，它们可能包含完全不同的真实运行态。

### 坑 4

升级系统 NVIDIA 驱动后，如果 Flatpak 里的 `GL.nvidia-*` 没跟上，图形类 Flatpak 应用会先出问题。

### 坑 5

runner 更新时，优先“并排新增再切换”，不要“先删旧版本再赌新版本”。

---

如果以后我还要继续补这份笔记，最值得加的一部分会是：

- 一个专门的“Flatpak 图形 runtime 自查清单”
- 一个专门的“AAGL 前缀备份与回滚流程”
- 一个“通用 Wine 与游戏专用 Wine 的隔离规范”
