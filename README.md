# MGServer
一个通用的服务器框架，已经实现了普通的TCP socket和 websocket，可以用于长链接的游戏开发，小程序开发等。

# 运行
请使用cmake 到build 下面生成工程，执行编译， 编译后在执行的目录添加 logs 文件夹，用于输出log， 并且拷贝 conf 文件夹内内容到执行目录下


# 编译：
ubuntu:
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
