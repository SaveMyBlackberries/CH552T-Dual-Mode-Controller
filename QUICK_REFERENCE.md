# 🚀 快速参考卡

## 模式切换速查

| 操作 | LED指示 | 结果 |
|------|---------|------|
| **不按任何键**上电 | 快速闪3次 | HID模式 |
| **按住按钮1**上电 | 慢速闪5次 | MIDI模式 |
| **按住按钮3+4**上电 | 快速闪10次 | ISP烧录模式 ⭐ |

---

## 引脚速查表

| 引脚 | HID功能 | MIDI功能 |
|------|---------|----------|
| **P1.4** | 油门轴(Z) | CC13(Effect Control 1) |
| **P1.1** | 反推轴(Slider) | Pitch Bend(弯音轮) |
| **P3.1** | 按钮1 / **模式选择** | 音符C4(60,中央C) / **模式选择** |
| **P3.2** | 按钮2 | 音符A4(69) |
| **P1.6** | 按钮3 | 功能键(+12半音) |
| **P1.7** | 按钮4 | 功能键(-12半音) |
| **P3.0** | LED指示 | LED指示 |

**注意**：按钮1(P3.1)在上电时用于模式选择，进入系统后作为普通按钮使用。

---

## MIDI音符速查

| 按键组合 | 音符 | MIDI值 | 频率 |
|---------|------|--------|------|
| 按钮1 | **C4 (中央C)** | 60 | 262Hz |
| 按钮2 | A4 | 69 | 440Hz |
| **按钮3+按钮1** | C5 | 72 | 523Hz |
| **按钮3+按钮2** | A5 | 81 | 880Hz |
| **按钮4+按钮1** | C3 | 48 | 131Hz |
| **按钮4+按钮2** | A3 | 57 | 220Hz |

---

## 常用命令

### 编译固件
```bash
cd MIDI_Joystick
compile.bat
```

### 启动MIDI测试工具
```bash
# 方法1: 使用启动器
start_midi_tester.bat

# 方法2: 直接运行
python midi_tester.py
```

### 安装Python依赖
```bash
pip install -r requirements.txt
```

---

## 烧录固件流程

### 软件方式（推荐）
```
1. 同时按住按钮3+4
2. 插入USB
3. LED闪10次
4. 松开按钮
5. WCH ISP Tool烧录
```

### 硬件方式
```
1. 按住P36
2. 插入USB
3. 松开P36
4. WCH ISP Tool烧录
```

---

## 测试检查清单

### HID模式 ✓
- [ ] LED快速闪3次
- [ ] joy.cpl识别设备
- [ ] 2个旋钮正常
- [ ] 4个按钮正常

### MIDI模式 ✓
- [ ] LED慢速闪5次
- [ ] 测试工具识别设备
- [ ] CC13值0-127（✅ 满行程，已校准）
- [ ] Pitch Bend值-8192到+8191（✅ 满行程，已校准）
- [ ] 6个音符正常（C4/A4组）
- [ ] 功能键移调正常（±12半音）

### ISP模式 ✓
- [ ] LED快速闪10次
- [ ] 设备管理器显示USB Module
- [ ] WCH ISP Tool能烧录

---

## 故障速查

| 问题 | 解决方法 |
|------|---------|
| LED不闪烁 | 检查固件烧录 |
| 找不到设备 | 重新插拔USB |
| 旋钮不工作 | 检查P1.1/P1.4接线 |
| 按钮不响应 | 检查按钮接地 |
| MIDI测试工具打不开 | `pip install pygame` |
| 无法进入ISP | 同时按住按钮3+4再插USB |

---

## 文件清单

```
MIDI_Joystick/
├── dual_mode_controller.c       主程序
├── compile.bat                  编译脚本
├── midi_tester.py              MIDI测试工具
├── start_midi_tester.bat       测试工具启动器
├── requirements.txt            Python依赖
├── README.md                   完整说明
├── MIDI_TESTER_README.md       测试工具说明
├── NEW_FEATURES.md             新功能说明
└── QUICK_REFERENCE.md          本文档
```

---

## 联系信息

- **项目目录**: `F:\Documents\开发板\Throttle\MIDI_Joystick`
- **固件输出**: `dual_mode_controller.hex`
- **开发板**: CH552T SuperMini USB

---

**快速开始**: 编译 → 烧录 → 测试 → 享受！🎉

