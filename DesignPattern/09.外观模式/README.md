# 外观模式

一个程序需要操作一些外部设备，有的使用TCP进行控制，有的使用串口进行控制，还有的使用PLC或者Modbus控制

对于这些外部设备，可以统一封装一个设备控制类，将这些设备封装在内部，软件只需要调用统一的接口就可以
