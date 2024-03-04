## 软件安装

#### CCS6软件安装



#### 编译器手动安装

- 到TI官网找CGTwww.ti.com.cn上搜索CGT，即可找到C2000-CGT:C2000代码生成工具-编译器。

![img](https://img-blog.csdnimg.cn/0fa4db90435649839432595e50cd6b92.png)

- 点进去之后，点击“下载”=》“下载选项”

![image-20230304210913237](Readme.assets/image-20230304210913237.png)

- 可以“查看所有版本”

![image-20230304210955176](Readme.assets/image-20230304210955176.png)

-  找到所需版本后即可**下载**。

![image-20230304211021360](Readme.assets/image-20230304211021360.png)

 

- 安装编译器在CCS软件目录下的compile目录下，并按照编译器名给文件夹命名

![image-20230304211149437](Readme.assets/image-20230304211149437.png)

![img](https://img-blog.csdnimg.cn/0e4c0fcada044110acdd6bc4fc04c2be.png)



#### 配置编译器

- 打开项目属性

![img](https://img-blog.csdnimg.cn/f582f7867f234027b996c3a8706bf93b.png)

- 选择General，找到Compiler version,点击“more...”

![img](https://img-blog.csdnimg.cn/161c8d055adb4263a6de12966d77e2ef.png)

- 点击“刷新”，新版本的就出来了。



## 文件说明

### APP 文件夹

​		用于存放用户编写的应用文件，如 LED、蜂鸣器、数码管、ADC
等文件，这样可有效的管理工程文件和提高我们代码的移植性。

### User 文件夹

用于存放用户编写的 main.c 等文件，用户编写的应用程序起
始位置即从 main 函数开始执行。

### DSP2833x_Libraries 文件夹

用于存放 TI 公司提供给我们开发 DSP2833x所需要的头文件、外设、内核、DSP 库、CMD 等文件。