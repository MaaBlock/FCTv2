@echo off
setlocal enabledelayedexpansion

rem ���ð汾��Ϣ
set "VERSION=0.0.1"

rem ������Ŀ·����������
set "projects=FCT Tutorial"
set "FCT_DEPS=spirv-cross glslang freeimage freetype"
set "TUTORIAL_DEPS=spirv-cross glslang freeimage freetype"

for %%p in (%projects%) do (
    echo ���ڴ��� %%p ��Ŀ...

    rem �л�����ĿĿ¼
    pushd %%p

    rem ɾ�����е� vcpkg.json �� vcpkg-configuration.json
    if exist "vcpkg.json" del "vcpkg.json"
    if exist "vcpkg-configuration.json" del "vcpkg-configuration.json"

    rem ����Ŀ��ת��ΪСд
    set "lowername=%%p"
    for %%L in (a b c d e f g h i j k l m n o p q r s t u v w x y z) do set "lowername=!lowername:%%L=%%L!"
    rem �����µ� vcpkg.json
    vcpkg new --name=!lowername! --version=%VERSION%

    rem ���������
    set "DEPS=!%%p_DEPS!"
    for %%d in (!DEPS!) do (
        vcpkg add port %%d
    )

    echo %%p �� vcpkg.json �ѳɹ����ɲ����������

    rem ������һ��Ŀ¼
    popd
)

echo ������Ŀ�� vcpkg.json �ļ��Ѹ��¡�

endlocal

