# centos 7 安装opencv

## 安装依赖包

```bash
sudo yum install -y gcc gtk2-devel numpy unzip qt5-qtbase-devel python-devel jasper-devel openexr-devel libwebp-devel libjpeg-turbo-devel freeglut-devel mesa-libGL mesa-libGL-devel libtiff-devel libdc1394-devel tbb-devel eigen3-devel boost boost-thread boost-devel libv4l-devel gstreamer-plugins-base-devel
```

## 安装cmake 3.5.2

由于centos7自带的cmake版本2.8太旧了，opencv要用3.5以上，所以先编译安装cmake 3.5.2

```bash
wget https://cmake.org/files/v3.5/cmake-3.5.2.tar.gz
tar xf cmake-3.5.2.tar.gz
cd cmake-3.5.2
./bootstrap --prefix=/opt/cmake3.5
make -j 4
make install
ln -s /opt/cmake3.5 /opt/cmake
echo 'export CMAKE_ROOT=/opt/cmake' > /etc/profile.d/cmake.sh
echo 'export PATH=$PATH:$CMAKE_ROOT/bin' >>/etc/profile.d/cmake.sh
```
测试一下
```bash
source /etc/profile
cmkae --version
```

屏幕打印

```
cmake version 3.5.2

CMake suite maintained and supported by Kitware (kitware.com/cmake).
```

## 编译安装opencv

```bash
wget https://codeload.github.com/opencv/opencv/zip/4.3.0
unzip opencv-4.3.0.zip
cd opencv-4.3.0
mkdir build
cd build
#命令后面一定要有'..' 表示源代码路径为父目录
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/opt/opencv4.3 ..
make -j 4
make install
ln -s /opt/opencv4.3 /opt/opencv
echo '/opt/opencv/lib64' >/etc/ld.so.conf.d/opencv.conf
echo 'export PATH=$PATH:/opt/opencv/bin' > /etc/profile.d/opencv.sh
ldconfig
```

