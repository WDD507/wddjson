@echo off
setlocal enabledelayedexpansion

REM Git 全局设置
git config --global user.name "WDDwthclj"
git config --global user.email "1751398270@qq.com"
git config --global user.name "WDD507"
git config --global user.email "1751398270@qq.com"

REM 创建 git 仓库
git init 
touch README.md
git add README.md
git commit -m "first commit"
git remote add origin https://gitee.com/user-wddwthclj/wddmcl.git
git push -u origin "master"

REM 已有仓库
REM cd existing_git_repo
git remote add origin https://gitee.com/user-wddwthclj/wddmcl.git
git push -u origin "master"


git -C E:\Apps\wddjson init
git -C E:\Apps\wddjson add .
git -C E:\Apps\wddjson commit -m "初始提交：WDD's JSON v0.0.5.1 Beta"

git -C E:\Apps\wddjson remote add gitee https://gitee.com/user-wddwthclj/wddmcl.git
git -C E:\Apps\wddjson remote set-url gitee https://gitee.com/user-wddwthclj/wddmcl.git
git -C E:\Apps\wddjson branch -M main
git -C E:\Apps\wddjson push -u gitee main

git -C E:\Apps\wddjson remote add origin https://github.com/WDD507/wddmcl.git
git -C E:\Apps\wddjson remote set-url origin https://github.com/WDD507/wddmcl.git
git -C E:\Apps\wddjson branch -M main
git -C E:\Apps\wddjson push -u origin main

git -C E:\Apps\wddjson commit -m ""

REM 记事本必须在“关闭”而不是“最小化”时才算你写完了，Git 是等编辑器进程退出。
git config --global core.editor notepad

git rm ./utils/wddwxy_json.h
git rm ./utils/wddwxy_json.cpp 


REM 每次提交时详细调用的命令
git -C E:\Apps\wddjson commit -m "（初始提交）完成阶段一" ^
    -m "阶段一搭建出了基础的框架，并添加了9个类型判断函数、get函数及其两个变种、dump和转义等"
