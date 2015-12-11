# FileRebuild
文件分割加密，解密重组执行
    
    Rebuild目录下有三个程序和一个文件夹：

    1)FILE文件夹
      *******执行前请将待拆分的文件放入该目录，执行FileSplit.exe后拆分的文件会保存在该目录下*********

    2)Encryption.exe: 
      加解密程序，算法为用输入的key循环异或。如原数据为abc，输入的key为123，将a与1异或，b与2异或，c与3异或得到密文，以此类推直到所有数据加密完成。
      用法：Encryption.exe 源文件 输出目录 key
      例：  Encryption.exe a.exe D:\\b.doc 123

    3)FileSplit.exe:
      将文件拆分成多个文件并调用Encryption.exe加密拆分后的第一个文件（带文件头的）。
      用法： cmd到Rebuild目录下使用
      命令参数： PERebuild.exe 待拆分文件 拆分个数/拆分大小
      例：
          ..\\Rebuild >  FileSplit.exe FILE\\hellworld.exe (3)   //表示拆分成三个文件
          ..\\Rebuild >  FileSplit.exe FILE\\hellworld.exe 114kb   //表示按每个文件114kb拆分

    4)BldExe.exe:
      将经2)拆分加密后的文件（FILE目录下）解密重组执行，直接双击即可。


附上源码，有点乱。囧
