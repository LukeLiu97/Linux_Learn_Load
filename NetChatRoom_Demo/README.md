# Net Chat Room

============================

## Contents

----------------------------
[TOC]

## 程序流程图

----------------------------

```flow
St=>start: 开始，识别参数
ArgmentCond=>operation: 识别命令，并初始化IP&Port
ThreadCreateSub=>subroutine: 依次创建子线程
```

- 创建Tcp线程
- 创建从机

