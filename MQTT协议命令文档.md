# MQTT 通讯命令协议文档

本项目使用 **中国移动 OneNet** MQTT 平台，通过 MN316 NB-IoT 模块进行通讯。

## 连接信息

| 参数 | 值 |
|------|------|
| MQTT 服务器 | mqtts.heclouds.com:1883 |
| 客户端 ID | MN316 |
| 用户名 | 382J6WcAh6 |
| 订阅话题 | `$sys/382J6WcAh6/MN316/thing/property/post/reply` |
| 发布话题 | `$sys/382J6WcAh6/MN316/thing/property/post` |

---

## 上行命令（设备 → 云端）

设备通过 MQTT Publish 向云端发送传感器数据，每秒发送一次。

### JSON 格式

因 MN316 模块限制每次最多发送 3 个参数，设备将 5 个传感器参数分 **2 次 Publish** 上报：

**第一包（3个参数）：Temperature, Humidity, Lightness**

```json
{"id":"123","version":"1.0","params":{"Temperature":{"value":<水温>},"Humidity":{"value":<水位>},"Lightness":{"value":<光照>}}}
```

**第二包（2个参数）：Turbidity, pHValue**

```json
{"id":"123","version":"1.0","params":{"Turbidity":{"value":<浊度>},"pHValue":{"value":<PH值>}}}
```

### 参数字段

| 字段 | 说明 | 类型 | 取值范围 | 分包 |
|------|------|------|----------|------|
| Temperature | 水温 | int (℃) | 0~100 | 第一包 |
| Humidity | 水位 | int | 0~50 | 第一包 |
| Lightness | 光照强度 | int (Lux) | 0~65535 | 第一包 |
| Turbidity | 浊度 | int | 0~1200 | 第二包 |
| pHValue | PH值 | float | 0.0~14.0 | 第二包 |

> 字段名使用 Temperature/Humidity 以适配 OneNet 平台物模型定义，实际分别代表水温和水位。

### 实际传输（HEX）

MN316 模块要求以 HEX 编码发送 JSON，例如：

```
AT+MQTTPUB="$sys/382J6WcAh6/MN316/thing/property/post",0,0,0,<长度>,<HEX数据>
```

示例：`{"id":"123","version":"1.0","params":{"Temperature":{"value":25},"Humidity":{"value":30}}}` 转换为 HEX 后发送。

---

## 下行命令（云端 → 设备）

云端通过 MQTT Publish 向设备发送控制指令，设备解析 JSON 中的字符串关键词执行对应操作。

### 一、模式切换

| 关键词 | 功能 | 说明 |
|--------|------|------|
| `Automatic` | 自动模式 | 进入自动控制逻辑 |
| `TimeFlag` | 定时模式 | 进入定时控制逻辑 |
| `ThresholdMODE` | 阈值模式 | 进入阈值控制逻辑 |
| `Manual` | 手动模式 | 进入手动控制，同时关闭所有7路开关 |

### 二、继电器/负载开关控制

| 关键词 | 功能 | 硬件对应 |
|--------|------|----------|
| `Switch1ON` | 加热开 | 加热继电器 |
| `Switch1OFF` | 加热关 | 加热继电器 |
| `Switch2ON` | 排水开 | 排水继电器 |
| `Switch2OFF` | 排水关 | 排水继电器 |
| `Switch3ON` | 照明开 | 照明继电器 |
| `Switch3OFF` | 照明关 | 照明继电器 |
| `Switch4ON` | 排水2开 | 排水2继电器 |
| `Switch4OFF` | 排水2关 | 排水2继电器 |
| `Switch5ON` | 投喂开 | 投喂舵机 |
| `Switch5OFF` | 投喂关 | 投喂舵机 |
| `Switch6ON` | 灯光开 | 灯光继电器 |
| `Switch6OFF` | 灯光关 | 灯光继电器 |
| `Switch7ON` | 水泵开 | 水泵继电器 |
| `Switch7OFF` | 水泵关 | 水泵继电器 |

### 三、阈值参数调整

#### 水温阈值

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `WtrTempThresholdDown` | 水温阈值减1 | -1℃ |
| `WtrTempThresholdAdd` | 水温阈值加1 | +1℃ |

#### 水位阈值

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `WtrLevelThresholdDown` | 水位阈值减1 | -1 |
| `WtrLevelThresholdAdd` | 水位阈值加1 | +1 |

#### 光照阈值

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `LightThresholdDown` | 光照阈值减10 | -10 Lux |
| `LightThresholdAdd` | 光照阈值加10 | +10 Lux |

#### 浊度阈值

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `TurbThersholdDown` | 浊度阈值减10 | -10 |
| `TurbThersholdAdd` | 浊度阈值加10 | +10 |

#### PH 值阈值

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `PHMaxDown` | PH上限减1 | -1 |
| `PHMaxAdd` | PH上限加1 | +1 |
| `PHMinDown` | PH下限减1 | -1 |
| `PHMinAdd` | PH下限加1 | +1 |

### 四、定时参数调整

#### 定时类型切换

| 关键词 | 功能 | 说明 |
|--------|------|------|
| `FeedMode` | 切换为投喂定时模式 | 对应 TimeMode_Selection=0 |
| `OxMode` | 切换为增氧定时模式 | 对应 TimeMode_Selection=1 |

#### 时

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `HourDown` | 小时减1 | 当前模式对应 FeedHour 或 OxHour |
| `HourAdd` | 小时加1 | 当前模式对应 FeedHour 或 OxHour |

#### 分

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `MinuteDown` | 分钟减1 | 当前模式对应 FeedMinute 或 OxMinute |
| `MinuteAdd` | 分钟加1 | 当前模式对应 FeedMinute 或 OxMinute |

#### 执行时长

| 关键词 | 功能 | 变化量 |
|--------|------|--------|
| `TimeDown` | 执行时长减1秒 | 当前模式对应 FeedTime 或 OxTime |
| `TimeAdd` | 执行时长加1秒 | 当前模式对应 FeedTime 或 OxTime |

#### 定时开关

| 关键词 | 功能 | 说明 |
|--------|------|------|
| `TimeON` | 开启定时任务 | 开启当前模式的定时投喂或增氧 |
| `TimeOFF` | 关闭定时任务 | 关闭当前模式的定时投喂或增氧 |

---

## 汇总统计

| 类别 | 命令数量 |
|------|----------|
| 上行 JSON 参数 | 5 个（Temperature/Humidity/Lightness + Turbidity/pHValue，分2包发送） |
| 下行模式切换 | 4 个 |
| 下行开关控制 | 14 个（7路 × ON/OFF） |
| 下行阈值调整 | 12 个（6项 × +/-） |
| 下行定时参数 | 8 个 |
| **下行命令合计** | **38 个** |
