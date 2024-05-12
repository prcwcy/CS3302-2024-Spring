# CS3302 Project 3

## 要求

开发一个内核模块，实现用户内存池（`umem`），满足以下要求：

1. 当 `umem` 内存池模块加载时，将自动初始化 `UMEM_NUM_POOL` 个内存池，并向每个内存池添加 `UMEM_POOL_SIZE` 个物理页。当模块被卸载时，将内存池中的页面归还给系统。假定在模块卸载时，没有进程使用内存池内存。

2. 用户空间进程可以使用接口 `void *umem_malloc(unsigned int umem_size, unsigned int umem_pool)` 从指定编号为 `umem_pool` 的内存池申请内存；使用接口 `void umem_free(void *umem_addr)` 将从内存池中申请的内存释放。

3. 内存池中的内存采用按需分页的方式提供，即只有应用真正读写该内存地址时，才真正分配物理页。

4. 可能会有多个进程同时使用多个内存池，请考虑并发情况，在代码适当位置添加同步原语以确保模块在并行条件下的正确性。

5. 验证传递给模块的参数的正确性，避免恶意用户读取不属于其的物理内存。

6. 对于没有使用 `void umem_free(void *umem_addr)` 接口释放内存的进程，在进程退出时，模块应当回收其曾经使用的内存池页面。

7. 当应用**实际**使用了超过内存池容量的内存时（即允许应用通过 `umem_malloc` 申请超过内存池容量的内存，只要应用最后没有读写它们），应向应用程序报错（在内核模块中返回 -EINVAL）。

8. 强制要求大家使用 6.0 及以上版本的内核。推荐直接使用 `Ubuntu 22.04` 发行版自带的内核完成本次实验（本文编写时的版本号为 `6.5.0-28`）。本项目不允许修改内核。必须以内核模块的方式完成本次实验。

## 提示

本项目主要涉及对 `umem.c` 文件的修改，重点关注标记为 `TODO` 的地方。

本项目涉及字符设备相关内容，包括物理页和 slab 的申请和释放。可以参考 [https://lwn.net/Kernel/LDD3/](https://lwn.net/Kernel/LDD3/) 了解这些细节。

本项目需要为每个进程维护相关信息，可能会使用到 [https://elixir.bootlin.com/linux/latest/source/include/linux/list.h](https://elixir.bootlin.com/linux/latest/source/include/linux/list.h) 中定义的链表。

本项目涉及虚拟地址空间的申请，页表项的创建和删除，可能会使用到 [https://elixir.bootlin.com/linux/latest/source/include/linux/mm.h](https://elixir.bootlin.com/linux/latest/source/include/linux/mm.h) 中提供的有关内存操作的 API。

本项目提供的测试程序 `tests/main.c` 不是最终的验收程序。此外，不应修改提供给用户态进程的接口文件 `tests/umem-user.c` 和 `tests/umem-user.h`。如果进行了修改，请在报告中说明原因。

## 提交

实验报告和所有源代码打包成 `zip` 文件。源代码文件夹命名为 `[SJTU_ID]_src`，实验报告命名为 `[SJTU_ID]_report.pdf`。