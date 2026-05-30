# Talk with yourself 本地 Web MVP

这是一个无外部依赖的 Node.js 本地 Web MVP，用来验证“上传授权聊天记录 -> 蒸馏人格 -> 网页聊天 -> 导出人格包”的完整流程。应用通过 `node server.js` 启动，默认监听 `8787` 端口；运行数据保存在 `data/` 目录。

## 启动

### 方式一：双击启动

在 Windows 11 上双击 `start.cmd`。脚本会打开一个命令行窗口并启动服务。

启动后访问：

```text
http://localhost:8787
```

### 方式二：PowerShell 启动

在本目录打开 PowerShell：

```powershell
.\start.ps1
```

指定端口：

```powershell
.\start.ps1 -Port 9000
```

也可以使用环境变量：

```powershell
$env:PORT = "9000"
.\start.ps1
```

### 方式三：直接使用 Node.js

```powershell
node server.js
```

指定端口：

```powershell
$env:PORT = "9000"
node server.js
```

## 使用流程

1. 启动本地服务。
2. 在浏览器打开 `http://localhost:8787`，或打开你指定的端口地址。
3. 上传 TXT/CSV/JSON/MD 格式的聊天记录文本；也可以直接把聊天记录粘贴到页面文本框。
4. 填写人格名称，并勾选上传授权、人工查看授权、训练授权。
5. 系统会生成一个人格包，并保存原始记录、脱敏文本、授权记录和人格 JSON。
6. 在页面右侧选择人格，可以直接测试聊天。
7. 点击“导出 ZIP”可下载人格包，里面包含 `persona.json`、`system_prompt.md`、`memory.md`、`style_examples.jsonl` 和 `consent.json`。

页面里还有“识别本机微信记录”按钮。它会扫描 Windows 常见微信目录，找到可读的 `txt/csv/json/md` 文件时可以一键导入；如果发现的是 `MSG.db`、`MicroMsg.db` 等微信数据库文件，当前版本只提示位置，不直接解密。

如果扫描不到可导入文件，通常说明微信没有把聊天记录保存成明文文本。此时需要先通过微信自带备份、可信导出工具或手工复制，把记录转成文本格式，再上传或粘贴。

本机已接入 `@jackwener/wx-cli` 作为新版 Windows 微信数据库读取器。首次使用需要在微信已登录时运行：

```powershell
npx wx init
```

成功后页面可以读取本机会话列表，并把指定会话导出为文本后生成人格。该能力只用于本人本机授权数据，不适合做成无感采集功能。

可以先用示例数据验证流程：

```text
sample-data/wechat-sample.txt
```

## 上传与授权边界

当前版本定位为本地 MVP，默认只在本机运行，不主动连接对象存储、计费系统或第三方分析服务。聊天回复会优先检测 `ANTHROPIC_BASE_URL`、`ANTHROPIC_API_KEY`、`ANTHROPIC_MODEL` 等 `cc` 兼容环境变量并调用云端模型；如果未配置或调用失败，会自动退回本地规则 fallback。

如果后续加入文件上传或云端同步，需要明确以下边界：

- 上传前必须让用户知道上传内容、用途、保存位置和保留周期。
- 只上传用户主动选择或明确授权的数据，不扫描或批量上传本机其他目录。
- 本地 `data/` 中的个人内容默认视为用户私有数据。
- 接入云端模型前，应说明哪些文本会发送给模型服务，以及是否会用于训练或日志分析。
- 接入账号、计费或团队协作后，应补充隐私政策、数据删除入口和权限控制。

## 目录结构

```text
Install/
  README.md                  项目说明和本地运行指南
  start.cmd                  Windows 双击启动脚本
  start.ps1                  PowerShell 启动脚本
  server.js                  Node.js 本地 Web 服务入口和 API
  lib/
    distiller.js             聊天解析、脱敏、人格蒸馏、本地回复
    zip.js                   无依赖 ZIP 导出
  public/
    index.html               Web 操作台
    styles.css               页面样式
    app.js                   前端交互逻辑
  sample-data/
    wechat-sample.txt        示例聊天记录
  data/
    personas/                人格 JSON
    uploads/                 原始记录、脱敏文本、授权记录
    chatlogs/                本地聊天测试日志
```

## 已有 API

- `GET /api/status`：服务状态和模型配置检测。
- `POST /api/ingest`：上传聊天记录并生成人格。
- `GET /api/personas`：人格列表。
- `GET /api/personas/:id`：人格详情。
- `POST /api/chat`：和指定人格测试聊天。
- `GET /api/personas/:id/export`：导出人格 ZIP。
- `POST /api/personas/delete`：删除指定人格及本地数据。
- `GET /api/device/wechat-scan`：扫描本机常见微信目录。
- `POST /api/device/import-local`：导入扫描到的本机可读文本记录。
- `GET /api/wechat/sessions`：读取本机微信会话列表，需要先完成 `npx wx init`。
- `POST /api/wechat/import-session`：导出指定微信会话并生成人格。

## 下一步

- 接云端模型：当前已支持读取现有 `cc` 环境变量调用 Anthropic Messages 兼容接口；下一步可以补流式输出、模型选择和更细的错误提示。
- 微调：沉淀可导出的训练样本格式，区分用户私有数据、可训练数据和禁止训练数据。
- 计费：增加账号体系、额度统计、用量日志、支付状态同步和本地降级策略。
- 数据治理：补充导出、删除、备份、恢复和授权撤回流程。
- 部署形态：在本地 MVP 稳定后，再考虑桌面打包、局域网访问或云端部署。
