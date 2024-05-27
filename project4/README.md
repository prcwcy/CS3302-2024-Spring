# CS3302 Project 4

## 内容

以[FUSE文件系统](https://zh.wikipedia.org/zh-hans/FUSE)`passthrough`为基础, 实现一个自定义文件系统, 实现以下功能:

* `./passthrough`接受三个参数： `hidden_file_name`, `encrypted_file_name`和`exec_file_name`:

  * `hidden_file_name=xxx`：需要被隐藏的文件路径

  * `encrypted_file_name=xxx`：需要被加密的文件路径

  * `exec_file_name=xxx`：需要修改权限为可执行的文件路径

通过命令行`./passthrough --hidden_file_name=xxx --encrypted_file_name=xxx --exec_file_name=xxx mount_point`挂载`passthrough`文件系统到`mount_point`文件夹下. 通过命令行`umount mount_point`卸载该文件系统.

## 思路 & 提示

本项目只需要修改`passthrough.c`中的内容.

隐藏文件的位置:

```c
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi,
		       enum fuse_readdir_flags flags)
```

修改文件内容的位置:

```c
static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
```

修改文件权限的位置:

```c
static int xmp_getattr(const char *path, struct stat *stbuf,
		       struct fuse_file_info *fi)
```

加密的方法随意, 你可以直接对文件的每个字节单独进行$f(x)=(x+k) \mod 255$的操作, 其中$k$为任取的非零常数.

`readdir`, `lstat`等函数的具体功能, 你可以查阅[https://man7.org/linux/man-pages/man3/readdir.3.html](https://man7.org/linux/man-pages/man3/readdir.3.html), [https://man7.org/linux/man-pages/man2/stat.2.html](https://man7.org/linux/man-pages/man2/stat.2.html).

## 前置

以`Ubuntu 22.04`为例, 安装`libfuse3-dev`, `pkg-config`.

```sh
sudo apt install libfuse3-dev pkg-config
```

## 提交

实验报告和所有源代码打包成 `zip` 文件。源代码文件夹命名为 `[SJTU_ID]_src`，实验报告命名为 `[SJTU_ID]_report.pdf`。
