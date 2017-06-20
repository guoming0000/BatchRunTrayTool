[![Github Releases](https://img.shields.io/badge/download-32_bit-0078d7.svg)](https://github.com/guoming0000/BatchRunTrayTool/releases/download/v17.6.20/2017-06-20.BatchRunTrayToolSetup.exe) [![license](https://img.shields.io/badge/license-MIT-lightgray.svg)](https://opensource.org/licenses/MIT) [![Qt](https://img.shields.io/badge/Qt-qmake-green.svg)](https://www.qt.io)



# BatchRunTrayTool

# 软件快捷访问工具

这是一款可以快捷启动各种文件（包括可执行文件）的软件，是快捷方式的集合。将快捷方式拖到软件目录下的config文件夹归类使用，可以让桌面洁净，配合软件的“重试模式”可以进一步减少日常工作中的反复点击。

![demo](doc/demo.gif)

## 本软件适合哪些人

* 工作中经常要打开某些文件、文件夹。
* 工作中经常要跑某些脚本或者程序。
* 桌面洁癖者。

## 使用该软件你可以

* 把项目文档归到一起，快捷访问。（推荐将快捷方式放到config目录下）

```
BatchRunTrayTool/BatchRunTrayTool.exe
BatchRunTrayTool/config/
BatchRunTrayTool/config/X项目
BatchRunTrayTool/config/X项目/X项目功能.word
BatchRunTrayTool/config/X项目/X项目时间计划.xls
BatchRunTrayTool/config/X项目/X项目脑图.xmind
BatchRunTrayTool/config/X项目/X项目图片.png
BatchRunTrayTool/config/X项目/X项目地址.html
```

* 把常用的工具放到一起，如开发软件、绘图软件
* 可以自己DIY，写bat脚本跑一些任务
* 可以为每一个文件夹或文件设置图标（不设置就显示系统默认图标）

## 软件运行截图：

![batch_run_tray_tool](doc/batch_run_tray_tool.png)

![batch_run_tray_tool1](doc/batch_run_tray_tool1.png)

## 简要说下“关于”中的几个功能：

1. 打开配置目录：打开程序下的config目录，将快捷方式拖到这里，并重新加载即可看到软件的快捷入口。
2. 重新加载配置：软件自动重启，重新加载目录下的config配置
3. 软件自启动（可勾选）：勾选则添加本软件为开机自启动项，推荐大家勾选。默认关闭。
4. 重试模式（可勾选）：勾选则启动重试模式，打开某一个文件后，下来再需要重新打开只需要左键单击软件图标。取消勾选，可关闭该模式。默认关闭。

## 如何使用

安装后，您最好打开BatchRunTrayTool.exe同级目录下的"__config-simple__"文件夹，浏览下一个简单的配置(config)是如何构成的。然后你可以将任何文件、快捷方式拖放到__config__文件夹中，重新加载后，这个软件可使你仅需要一二次点击就可打开它。

了解__config-simple__是什么回事后，打开__config__可以了解更多的配置技巧：

1. **\_\_logo\_\_.png**是文件夹图标。
2. **程序\_\_logo\_\_.png**是程序的图标。（大部分程序有自己的图标）
3. **程序\_\_show\_\_**强调程序以可见方式运行（一般不需设置），对于.bat脚本可以看到命令行界面。
4. **文件夹\_\_seperator\_\_**使得菜单产生分隔符。




## 已知软件限制

* 最多支持三层，config/第一层/第二层/第三层
* 不支持文件夹快捷方式
* 目前仅支持windows平台



## License

Copyright (c) BrightGuo.com. All rights reserved.

Licensed under the [MIT](LICENSE.txt) License.
