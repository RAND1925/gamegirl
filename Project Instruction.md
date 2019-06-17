# GameBoy Emulator 项目手册
****
## 简介

本GameBoy模拟器名为`GameGirl`
项目地址为 https://github.com/RAND1925/gamegirl

###  成员
-  1753764	韩嵩     30.0%
-  1751047	蒋金峰   27.5%
-  1851623  陈睿     27.5%
-  1751044	李申羽   15%
###  时间线
- 4.20 完成CPU指令
- 4.28 仓库混乱，重建
- 5.1  完成Interrupt部分
- 5.20 CPU指令调试完成
- 5.28 Timer初步完成
- 6.4  MBC1初步完成
- 6.9  GPU初步完成
- 6.10 添加boot
- 6.13 JoyPad功能正常
- 6.15 CPU和Timer功能完全正常
- 6.15 Sprite功能正常
- 6.16 命令行参数，单例化，规范代码


## 各部分功能

| 部件             | 功能          
| ------------- |:-------------:
|  CPU          | 进⾏行行命令的执行、中断的判断 |
|GPU            |  画面显示部分模式转换及读写|
|  Cartige Driver| 游戏ROM的读取和卡带模式的转换   |
|Timer          |   各部分cycle（循环计数器）的同步 |  
|MMU            | 执行各部分对内存的调用
|SDL Manager    |  负责调用各类SDL库中函数|

目前**该模拟器**依然存在一些bug以及一些功能并没有实现（例如声音和其他MBC模式）,但此模拟器已可以实现选关、移动人物、显示标题等基本功能。
具体已实现功能如下：
- 实现Sharp LR35902 CPU大部分模拟
- 实现内存读写、卡带读取与切换
- 实现按键输入的模拟
- 实现cycle counter和timer的模拟
- 图形显示的模拟
以及以下附加功能：
- BGP(Background Window Graphic)的显示
- Sprite的显示

目前此模拟器已经可以在Windows 10和Arch Linux下运行：


Windows 1
<img src ="https://github.com/NorthernLights-1/ImageRepositry/blob/master/gamegirl/Windows.png" width="800" hegiht="450" align=center >

Linux
<img src="https://github.com/NorthernLights-1/ImageRepositry/blob/master/gamegirl/Linux1.png"  width="800" hegiht="450" align=center />



*****
## 使用方法

### 载入游戏
在exe文件夹内，有GameGirl.exe/gamegirl.out的可执行程序

#### 1. 附带几款游戏的方法
exe/rom文件下有几款游戏的.bat批处理文件，对应几个游戏ROM

#### 2. 其他游戏载入
用参数传入文件名，在程序所在文件路径下，用命令行输入
````
Windows10:
    gamegirl.exe bgbtest.gb
Linux:

    ./gamegirl.out bgbtest.gb
````
#### 3. 其他参数
| 部件             | 功能          
| ------------- |:-------------:
|        b    | 输入b,启用任天堂boot开机画面|
|       f| 输入"f60"，启用特定帧数（60fps）|
|  z       | 输入"z2",启用缩放（放大2倍）|
| x,y       |输入"x10""y10",在桌面特定区域出现窗口|
|S            |输入"s",启用sprite功能|
|c|             输入"c"，启用~~CGB功能~~自定义颜色 |
c1----Classical   ;     c2 ---- ~~Good For Eve~~  Green ;  c3----Pink
</br>
<img src="https://github.com/NorthernLights-1/ImageRepositry/tree/master/gamegirl/Tetris3.png" width="200" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="https://github.com/NorthernLights-1/ImageRepositry/tree/master/gamegirl/Tetris5.png" width="200" >&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="https://github.com/NorthernLights-1/ImageRepositry/tree/master/gamegirl/Tetris6.png" width="200" ></br>

### 按键输入
方向键：  <kbd>&uarr;</kbd>, <kbd>&darr;</kbd>, <kbd>&larr;</kbd>, <kbd>&rarr;</kbd>
选择键：  <kbd>X</kbd> for B, <kbd>Z</kbd> for A, <kbd>Enter</kbd> for START , <kbd>Backspace</kbd> for SELECT.
**********************
## 项目过程
### 难点
<font size="4">
 1. 缺少计组相关知识，需要额外学习相应的汇编指令<br/>
 2. CPU调试困难，大多数时候需要通过文件输出比对各个寄存器的状态，而在莫名其妙 的卡死之后往往会面临近亿条输出，导致找到错误操作所用时间较长<br/>
 3. Timer理解困难，有时分不清机械周期和时钟周期<br/>
 4. 图像显示部分拥有四种模式，须在cpu运转的同时，接受cycle之后     进行模式转换，从而进行不同工作<br/>
 5. 对于Sprite数据的转换存在误区，导致DMA出现不工作的情况<br/>
</font>


### 踩过的坑
<font size="3">
 1. <I> GameBoy CPU manual </I>文档中有一些地方没有描述清楚，有一些地方直接是<B><font color="#dd0000">错</font></B>的，只好参考其他多份资料进行对比。<br/>
 同时此CPU不是<font color="#dd0000">Z80 CPU</font>而是<font color="#dd0000">Sharp LR35902</font>。<br/>
   <B><font color="#dd0000"> (ps：助教大大求求您，下一届改个参考文档吧，巨坑啊！！！)</font></B></br></br>
 2. 许多汇编指令没有读明白，如<B>RLCA</B>和<B>RLC</B>的区别没有看懂<br/><br/>
 3. opcode中使用了lamda表达式，对捕获变量处理出错导致在一些情况下无法读取立即数，后来将立即数 读取改为函数<br/>
 </font>

### 设计模式
#### 单例化：
确保每个类只有一个对象，封装了构造函数，使得程序任何其他地方不能构造该类对象，这个设计借鉴了GPU硬件部分

### 内存设计
###### 包含内存的部分都继承AddressSpace, 并同时定义accepts,getByte,setByte 三个虚方法，对于一些特定寄存器分别定义特定的写入。这样避免了每加入一个部分就必须在 mmu读取中多加一次else if，使mmu 部分不那么臃肿，也避免之后debug 过程需要专门去读mmu来确认是否有bug。
###### 在不同的内存区域定义内存的方式也不相同，如 Cartridge区域就是定义一整块数组，而Timer 区域则是改为了四个寄存器分别定义，返回时也 只是依据输入的地址分别返回各寄存器的值，
###### 一是为了方便在像是Timer中std::array<Byte, 0x8000> 读写 MMU *AddressSpace这样内存中有特殊寄存器的部分进行赋值，
###### 二是进一步模拟gameboy硬件上分离的实际情况

