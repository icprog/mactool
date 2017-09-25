使用说明

修改 mac 步骤

1. 配置 wifimac.txt 和 btmac.txt 两个文件，里面存放了当前的 mac 地址

2. 机器连接 PC，并且确认机器打开了 usb adb 调试功能

3. 建议机器打开工厂测试工具（设备 wifi+bt 的 mac 必须保证 wifi 和蓝牙都打开）

4. 双击运行 setmac.exe 自动设置 mac 地址，如果成功 mac 会自动增加


wifimac.txt 和 btmac.txt 配置方法

第一行：起始 mac
第二行：结束 mac
第三行：当前 mac

已经设置过的 mac，会分别保存到 wifiused.log 和 btused.log 文件中




