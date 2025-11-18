# GitHub 发布指南

## 📦 上传到 GitHub 的步骤

### 1. 初始化 Git 仓库

在项目目录中打开终端/PowerShell：

```bash
cd MIDI_Joystick
git init
```

### 2. 添加文件

```bash
git add .
```

`.gitignore` 会自动排除编译产物，只添加源代码和文档。

### 3. 创建第一次提交

```bash
git commit -m "Initial commit: CH552T Dual-Mode Controller (HID/MIDI)"
```

### 4. 在 GitHub 上创建仓库

1. 登录 GitHub
2. 点击右上角 "+" → "New repository"
3. 填写仓库信息：
   - **Repository name**: `CH552T-Dual-Mode-Controller`（或你喜欢的名字）
   - **Description**: `A dual-mode USB controller based on CH552T, switchable between HID game controller and MIDI controller`
   - **Public** 或 **Private**（推荐选Public）
   - **不要**勾选 "Initialize with README"（我们已经有了）

### 5. 连接远程仓库

复制 GitHub 提供的仓库 URL，然后执行：

```bash
git remote add origin https://github.com/你的用户名/仓库名.git
git branch -M main
git push -u origin main
```

### 6. 添加标签（可选）

为第一个版本打标签：

```bash
git tag -a v1.0.0 -m "First release: Dual-mode controller"
git push origin v1.0.0
```

---

## 📋 包含的文件清单

### ✅ 会上传的文件

**源代码**:
- `dual_mode_controller.c` - 主程序
- `src/*.c` - 所有 C 源文件
- `src/*.h` - 所有头文件

**脚本**:
- `compile.bat` - 编译脚本
- `start_midi_tester.bat` - MIDI 测试工具启动脚本

**Python 工具**:
- `midi_tester.py` - MIDI 测试程序
- `requirements.txt` - Python 依赖

**文档**:
- `README.md` - 完整使用文档
- `QUICK_REFERENCE.md` - 快速参考卡
- `MIDI_TESTER_README.md` - MIDI 测试工具说明
- `NEW_FEATURES.md` - 新功能说明
- `MODE_SELECTION_UPDATE.md` - 模式选择更新说明
- `CH552T SuperMini USB 开发板引脚功能.txt` - 引脚说明

**配置**:
- `.gitignore` - Git 忽略规则
- `LICENSE` - MIT 许可证

### ❌ 不会上传的文件（被 .gitignore 排除）

**编译产物**:
- `*.asm` - 汇编文件
- `*.lst` - 列表文件
- `*.rel` - 重定位文件
- `*.rst` - 重置文件
- `*.sym` - 符号文件
- `*.lk` - 链接文件
- `*.map` - 映射文件
- `*.mem` - 内存文件
- `*.ihx` - Intel HEX 中间文件
- `*.hex` - 最终固件文件（可选择上传）

---

## 🎯 发布检查清单

在上传前请确认：

- [ ] 已删除所有临时文件和编译产物
- [ ] README.md 内容完整且准确
- [ ] LICENSE 文件已填写您的名字和年份
- [ ] .gitignore 文件已创建
- [ ] 代码可以成功编译
- [ ] 所有功能已测试通过
- [ ] 文档中没有个人敏感信息

---

## 🌟 提升项目可见度的建议

### 1. 完善 README.md

确保包含：
- 项目演示图片或 GIF
- 清晰的硬件连接图
- 详细的功能说明
- 编译和烧录步骤

### 2. 添加 Topics（标签）

在 GitHub 仓库页面添加相关标签：
- `ch552`
- `ch552t`
- `usb-hid`
- `usb-midi`
- `midi-controller`
- `game-controller`
- `diy-electronics`
- `embedded`
- `sdcc`

### 3. 添加项目图片

在 README.md 开头添加：
```markdown
![Project Photo](docs/images/controller.jpg)
```

上传实物照片到 `docs/images/` 目录。

### 4. 创建 Release

在 GitHub 上创建正式的 Release：
1. 进入仓库的 "Releases" 页面
2. 点击 "Create a new release"
3. 选择标签 v1.0.0
4. 填写 Release notes
5. **可以附加编译好的 .hex 文件**，方便其他人直接使用

---

## 📝 推荐的 Release Notes 模板

```markdown
## CH552T 双模式控制器 v1.0.0

### ✨ 功能特性

**HID 模式**:
- 2 个旋钮（油门/倒车）
- 4 个按钮
- 即插即用，无需驱动

**MIDI 模式**:
- 2 个旋钮（CC13 + Pitch Bend）
  - ✅ CC13完整行程 0-127（已校准）
  - ✅ Pitch Bend完整行程 -8192到+8191（已校准）
- 2 个音符按键（C4, A4，中央C组）
- 2 个功能键（八度移调±12半音）
- 精确的ADC校准算法

**模式切换**:
- 按住按钮1上电 → MIDI 模式
- 不按按钮上电 → HID 模式
- 按住按钮3+4上电 → ISP 烧录模式

**技术亮点**:
- 解决电位器行程不满问题
- Pitch Bend精确居中
- Python MIDI测试工具

### 📦 下载

- **源代码**: Source code (zip)
- **固件文件**: dual_mode_controller.hex（可直接烧录）

### 🔧 使用方法

详见项目 README.md

### 🌍 跨平台支持

- ✅ Windows
- ✅ macOS
- ✅ Linux

### 🐛 已知问题

目前无已知问题

### 📄 许可证

MIT License
```

---

## 🤝 社区互动建议

1. **鼓励反馈**: 在 README 中添加 Issues 链接
2. **欢迎贡献**: 添加 CONTRIBUTING.md（可选）
3. **回应问题**: 及时回复 GitHub Issues
4. **分享项目**: 在相关论坛分享（EEVblog, Reddit 等）

---

## 📊 后续维护

### 定期更新

```bash
# 修改代码后
git add .
git commit -m "描述你的修改"
git push
```

### 发布新版本

```bash
# 打新标签
git tag -a v1.1.0 -m "版本 1.1.0: 添加新功能"
git push origin v1.1.0

# 在 GitHub 上创建对应的 Release
```

---

**祝您的项目获得更多 ⭐ Star！**

