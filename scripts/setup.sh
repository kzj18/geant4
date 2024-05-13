#!/bin/bash

# 用于编译Geant4的脚本
ShellScriptFolder=$(cd $(dirname "$0"); pwd)
WorkspaceFolder=$(cd $ShellScriptFolder/..; pwd)
echo $WorkspaceFolder

DownloadsFolder=$WorkspaceFolder/downloads
DataFolder=$WorkspaceFolder/data
BuildFolder=$WorkspaceFolder/build

while true; do
    echo "Do you want to debug the shell script? [y/N]"
    read -r response
    if [[ -z "$response" ]]; then
       response="N"
    fi
    response=${response,,}    # 转换为小写
    if [[ "$response" =~ ^(yes|y|no|n)$ ]]; then
        break
    fi
done

if [[ "$response" =~ ^(yes|y)$ ]]; then
    set -x # 打印执行的命令
fi

# 下载Geant4的数据文件
while true; do
    echo "Do you want to download Geant4 data files? [Y/n]"
    read -r response
    if [[ -z "$response" ]]; then
       response="Y"
    fi
    response=${response,,}    # 转换为小写
    if [[ "$response" =~ ^(yes|y|no|n)$ ]]; then
        break
    fi
done

if [[ "$response" =~ ^(yes|y)$ ]]; then
    mkdir -p $DownloadsFolder
    urls=(
        "https://cern.ch/geant4-data/datasets/G4NDL.4.6.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4EMLOW.7.13.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4PhotonEvaporation.5.7.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4RadioactiveDecay.5.6.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4PARTICLEXS.3.1.1.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4PII.1.3.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4RealSurface.2.2.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4SAIDDATA.2.0.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4ABLA.3.1.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4INCL.1.0.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4ENSDFSTATE.2.3.tar.gz"
        "https://cern.ch/geant4-data/datasets/G4TENDL.1.3.2.tar.gz"
    )

    for url in "${urls[@]}"; do
        wget -cv "$url" -P $DownloadsFolder
    done
fi

# 解压Geant4的数据文件
while true; do
    echo "Do you want to extract Geant4 data files? [Y/n]"
    read -r response
    if [[ -z "$response" ]]; then
       response="Y"
    fi
    response=${response,,}    # 转换为小写
    if [[ "$response" =~ ^(yes|y|no|n)$ ]]; then
        break
    fi
done

if [[ "$response" =~ ^(yes|y)$ ]]; then
    mkdir -p $DataFolder
    for file in $DownloadsFolder/*.tar.gz; do
        tar -zxvf $file -C $DataFolder
    done
fi

# 自定义变量
PythonExec=$(which python)
CMakeInstallPythonFolder=$($PythonExec -c "import site; print(site.getsitepackages()[0])")
ExpatLib=/usr/lib/x86_64-linux-gnu/libexpat.so

CMakeInstallPrefixString=\$HOME/Installed/geant4-v10.7.4
CMakeInstallPrefix=$(eval echo $CMakeInstallPrefixString)
CMakePrefixPath=$HOME/Installed/boost-1.85.0/lib/cmake

# cmake
while true; do
    echo "Do you want to cmake? [Y/n]"
    read -r response
    if [[ -z "$response" ]]; then
       response="Y"
    fi
    response=${response,,}    # 转换为小写
    if [[ "$response" =~ ^(yes|y|no|n)$ ]]; then
        break
    fi
done

if [[ "$response" =~ ^(yes|y)$ ]]; then
    # 判断build文件夹是否存在，不存在则创建，存在则删除
    cd $WorkspaceFolder
    if [ ! -d $BuildFolder ]; then
        echo "build dir not exist, create it"
    else
        while true; do
            echo "Do you want to clean the build directory? [y/n]"
            read -r response
            response=${response,,}    # 转换为小写
            if [[ "$response" =~ ^(yes|y|no|n)$ ]]; then
                break
            fi
        done

        if [[ "$response" =~ ^(yes|y)$ ]]; then
            echo "build dir exist, clean it"
            cd $BuildFolder
            make uninstall
            rm -rf $CMakeInstallPythonFolder/Geant4*
            rm -rf $CMakeInstallPrefix
            cd $WorkspaceFolder
            rm -rf $BuildFolder
        fi
    fi
    mkdir -p $BuildFolder

    cd $BuildFolder
    cmake $WorkspaceFolder \
        -DPYTHON_EXECUTABLE=$PythonExec \
        -DCMAKE_INSTALL_PREFIX=$CMakeInstallPrefix \
        -DCMAKE_BUILD_TYPE=RelWithDebInfo \
        -DGEANT4_BUILD_MULTITHREADED=ON \
        -DGEANT4_INSTALL_DATA=ON \
        -DGEANT4_INSTALL_DATADIR=$DataFolder \
        -DGEANT4_USE_OPENGL_X11=ON \
        -DGEANT4_USE_PYTHON=ON \
        -DGEANT4_USE_QT=ON \
        -DGEANT4_USE_RAYTRACER_X11=ON \
        -DCMAKE_INSTALL_PYTHONDIR=$CMakeInstallPythonFolder \
        -DGEANT4_INSTALL_EXAMPLES=OFF \
        -DGEANT4_BUILD_TLS_MODEL=global-dynamic \
        -DCMAKE_PREFIX_PATH=$CMakePrefixPath \
        -DEXPAT_LIBRARY=$ExpatLib
fi

# make
while true; do
    echo "Do you want to make? [Y/n]"
    read -r response
    if [[ -z "$response" ]]; then
       response="Y"
    fi
    response=${response,,}    # 转换为小写
    if [[ "$response" =~ ^(yes|y|no|n)$ ]]; then
        break
    fi
done

if [[ "$response" =~ ^(yes|y)$ ]]; then
    cd $BuildFolder
    while true; do
        echo "How many threads do you want to use to make? [all]"
        read -r response
        if [[ -z "$response" ]]; then
            make -j
            break
        elif [[ "$response" =~ ^[0-9]+$ ]]; then
            make -j $response
            break
        fi
    done
    make install
fi

# 判断$HOME/.bashrc文件中是否已经添加了Geant4的环境变量
Geant4String="source $CMakeInstallPrefixString/bin/geant4.sh"
if ! grep -q "$Geant4String" $HOME/.bashrc; then
    echo "Add Geant4 environment variables to $HOME/.bashrc"
    echo -e "$Geant4String" >> $HOME/.bashrc
fi
Geant4String="source $CMakeInstallPrefixString/share/Geant4-10.7.4/geant4make/geant4make.sh"
if ! grep -q "$Geant4String" $HOME/.bashrc; then
    echo "Add Geant4 environment variables to $HOME/.bashrc"
    echo -e "$Geant4String" >> $HOME/.bashrc
fi

