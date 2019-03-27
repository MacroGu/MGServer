# MGServer
一个通用的服务器框架，已经实现了普通的TCP socket和 websocket，可以用于长链接的游戏开发，小程序开发等。

# 运行
请使用cmake 到build 下面生成工程，执行编译， 编译后在执行的目录添加 logs 文件夹，用于输出log， 并且拷贝 conf 文件夹内内容到执行目录下


# 编译：
在编译前需要安装 gcc 4.8.5 或以上版本，cmake 3.10 或以上版本， 已在 centos 7 ，ubuntu 18.10 的裸机上测试通过
直接运行命令
./linux_build.sh 
即可以编译整个服务器程序
