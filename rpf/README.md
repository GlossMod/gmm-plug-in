# RPF 文件编辑工具

作者: 小莫


这是为 [Gloss Mod Manager](https://mod.3dmgame.com/mod/197445) 制作的 一个RPF 文件处理工具, 以为GMM实现自动安装.

### 使用方法

```sh

# 读取文件内容 | 从 dlc.rpf 文件中读取 setup2.xml 的内容
rpf read "dlc.rpf" "setup2.xml"

# 写入文件到 RPF | 将 dlclist.xml 文件写入到 update.rpf 中的 common\data\dlclist.xml 文件
rpf write "update.rpf" "dlclist.xml" "common\data\dlclist.xml"

# 通过文件夹创建 rpf 文件 | 将 inputFolder 目录中的所有文件打包为 dlc.rpf 并保存到 output 目录中
rpf create "inputFolder" "output" "dlc"

```

### 快捷 bat 

为了处理相对路径相关的问题, 顺便写了几个 bat 文件, 
一方面是为了简化参数, 另一方面是为了处理相对目录引发的一些bug

```sh
# 读取文件内容 | 从 dlc.rpf 文件中读取 setup2.xml 的内容
_read "dlc.rpf" "setup2.xml"

# 写入文件到 RPF | 将 dlclist.xml 文件写入到 update.rpf 中的 common\data\dlclist.xml 文件
_write "update.rpf" "dlclist.xml" "common\data\dlclist.xml"

# 通过文件夹创建 rpf 文件 | 将 inputFolder 目录中的所有文件打包为 dlc.rpf 并保存到 output 目录中
_create "inputFolder" "output" "dlc"

# 初始化gmm的dlc.rpf | 参数是你的游戏目录中的 dlcpacks 目录
_buidGMM "F:\steam\steamapps\common\Grand Theft Auto V\mods\update\x64\dlcpacks"

# 获取 dlc 列表 | 参数是你 update.rpf 所在位置
_getDlcList "F:\steam\steamapps\common\Grand Theft Auto V\mods\update\update.rpf"

```
