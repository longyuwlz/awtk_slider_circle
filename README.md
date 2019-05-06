# AWTK 圆形滑块扩展控件

slider_circle 是 awtk 中的圆形滑块，是 awtk 的一个扩展组件实例。

## 目录结构

| 目录 | 说明 |
| -------- | ----- | 
| awtk | awtk 源码 |
| src | slider_circle 源码 |
| docs | 相关文档  |
| demos | demo 源码及资源文件|
| scripts | 资源文件生成脚本 |
| tests | gtest 相关源码　|


## 准备

### Ubuntu

如果没有安装 scons 和依赖的软件包，请在终端运行下面的命令：

```sh
sudo apt-get install scons libsndio-dev libgtk-3-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev
```
## 编译

#### 克隆项目并编译 awtk

```
git clone --recursive git@git.zhaoqi.info:awtk/awtk-slider-circle.git
cd awtk && scons -j4; cd - 
```
#### 更新资源文件编译工程

```
python scripts/update_res.py all
scons -j4
```
## 运行

#### 运行演示 demo

```sh
./bin/demo_slider_circle
```

### 预览 xml 文件

```sh
./bin/preview_ui ./demos/assets/raw/ui/slider_circle.xml
```

## 相关文档

* [awtk 扩展组件－以圆形滑块为例](http://git.zhaoqi.info:9998/awtk/awtk-slider-circle/blob/master/docs/awtk-slider-circle.md)
* [圆形滑块 manual 文档](http://git.zhaoqi.info:9998/awtk/awtk-slider-circle/blob/master/docs/slider_circle_t.md)