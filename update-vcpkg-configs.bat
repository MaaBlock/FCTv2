@echo off
setlocal enabledelayedexpansion

rem 设置版本信息
set "VERSION=0.0.1"

rem 定义项目路径和依赖库
set "projects=FCT Tutorial"
set "FCT_DEPS=spirv-cross glslang freeimage freetype"
set "TUTORIAL_DEPS=spirv-cross glslang freeimage freetype"

for %%p in (%projects%) do (
    echo 正在处理 %%p 项目...

    rem 切换到项目目录
    pushd %%p

    rem 删除现有的 vcpkg.json 和 vcpkg-configuration.json
    if exist "vcpkg.json" del "vcpkg.json"
    if exist "vcpkg-configuration.json" del "vcpkg-configuration.json"

    rem 将项目名转换为小写
    set "lowername=%%p"
    for %%L in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do set "lowername=!lowername:%%L=%%L!"
    rem 创建新的 vcpkg.json
    vcpkg new --name=!lowername! --version=%VERSION%

    rem 添加依赖库
    set "DEPS=!%%p_DEPS!"
    for %%d in (!DEPS!) do (
        vcpkg add port %%d
    )

    echo %%p 的 vcpkg.json 已成功生成并添加依赖。

    rem 返回上一级目录
    popd
)

echo 所有项目的 vcpkg.json 文件已更新。

endlocal

