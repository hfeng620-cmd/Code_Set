# Clash Verge 配置总览

> 更新日期：2026年05月04日
> 配置文件：`~/.local/share/io.github.clash-verge-rev.clash-verge-rev/clash-verge.yaml`

---

## 1. 配置文件位置

| 项目 | 路径 |
|------|------|
| Clash Verge 主配置 | `~/.local/share/io.github.clash-verge-rev.clash-verge-rev/clash-verge.yaml` |
| GUI 内部状态 | `~/.local/share/io.github.clash-verge-rev.clash-verge-rev/config.yaml` |
| GUI 设置 | `~/.local/share/io.github.clash-verge-rev.clash-verge-rev/verge.yaml` |
| DNS 配置 | `~/.local/share/io.github.clash-verge-rev.clash-verge-rev/dns_config.yaml` |
| 规则模板 | `~/.local/share/io.github.clash-verge-rev.clash-verge-rev/profiles/rbNfRMyQHAqC.yaml` |
| 订阅数据 | `~/.local/share/io.github.clash-verge-rev.clash-verge-rev/profiles/R5r0SekydxCr.yaml` |
| Clash Verge 日志 | `~/.local/share/io.github.clash-verge-rev.clash-verge-rev/logs/service/` |
| Clash 核心 | `verge-mihomo`（mihomo = clash-meta 的继任者） |
| 混合代理端口 | `127.0.0.1:7897`（HTTP + SOCKS5） |
| DNS 监听 | `127.0.0.1:5453` |
| 控制 Unix Socket | `/tmp/verge/verge-mihomo.sock` |

---

## 2. 全局设置

```yaml
mode: rule              # 规则模式：按规则匹配流量
mixed-port: 7897        # HTTP + SOCKS5 混合端口
allow-lan: false        # 不允许局域网设备使用此代理
log-level: info         # 日志级别
ipv6: true              # 启用 IPv6
```

---

## 3. TUN 模式（透明代理）— 当前关闭

> **2026-05-04 更新**：TUN 模式已关闭。clash meta 的 gvisor 栈在 Linux 上存在 DIRECT 连接路由死循环 bug——Clash 自己的直连请求也会被 TUN 路由回自身，导致所有国内直连站点超时。修复需 root 权限调整路由规则优先级。当前使用系统代理替代。

<!--
  原始配置（TUN 开启）:
  tun:
    enable: true
    stack: gvisor
    auto-route: true
    auto-detect-interface: true
    dns-hijack: [any:53]
  TUN 开启后所有流量自动走代理，无需设 http_proxy。关闭后需系统代理配合。
-->

```yaml
tun:
  enable: false                           # 关闭 TUN
  stack: gvisor
  auto-route: true
  strict-route: false
  auto-detect-interface: true
  dns-hijack:
    - any:53
```

### 当前代理方案

由于 TUN 关闭，需要配置应用层代理：

<!--
  此前系统代理 mode 被设为 'none'（2026-05-01 排障时清除残留），
  2026-05-04 恢复为 'manual'，指向 127.0.0.1:7897。
  火狐通过 gsettings 读取系统代理。
-->

| 应用 | 方式 |
|------|------|
| **火狐** | 系统代理 `127.0.0.1:7897`（gsettings） |
| **Codex** | `https_proxy=http://127.0.0.1:7897 codex` |
| **终端其他程序** | `export https_proxy=http://127.0.0.1:7897` |
| **国内AI (DeepSeek)** | 直连，已在 no_proxy 豁免 |

`http_proxy` 环境变量通过 `.zshrc` 管理，国内镜像和 DeepSeek API 在 `no_proxy` 中豁免。

---

## 4. DNS 配置

```yaml
dns:
  enable: true
  listen: 127.0.0.1:5453       # 避开 systemd-resolved 的 53 端口
  enhanced-mode: fake-ip
  fake-ip-range: 198.18.0.1/16
  respect-rules: false          # DNS 不遵循规则
```

### DNS 服务器层级

| 层级 | 服务器 | 用途 |
|------|--------|------|
| default-nameserver | `223.5.5.5`, `119.29.29.29` | 解析代理节点域名 |
| nameserver | `223.5.5.5`, `119.29.29.29`, `https://doh.pub/dns-query` | 国内域名默认用这些 |
| fallback | `8.8.8.8`, `1.1.1.1` | 非中国 IP 的域名回退 |
| proxy-server-nameserver | `https://doh.pub/dns-query`, `tls://223.5.5.5` | 解析代理节点域名 |

### fake-ip 模式

fake-ip 是 Clash 的高级 DNS 模式：当应用查询域名时，Clash 不返回真实 IP，而是返回 `198.18.0.x` 的假 IP。当流量到达 Clash 时，Clash 根据假 IP 找到原始域名，再按规则匹配。

**好处**：
- 支持域名规则匹配（即使应用使用了 IP 直连）
- 避免 DNS 污染

**注意**：`listen: :53` 会与 systemd-resolved 冲突（arch 默认运行 systemd-resolved），已改为 `127.0.0.1:5453`。
