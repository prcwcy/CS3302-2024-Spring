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


## 模板代码解析

`umem`模块在`/proc/`文件夹下创建了文件`/proc/umem`。用户进程在编译时链接的`umem-user`库通过对该文件使用`ioctl`来控制`umem`模块。当用户进程调用`umem-user`库中的函数`umem_malloc`时，`umem-user`库会打开`/proc/umem`文件，并随后利用该文件对`umem`模块进行相关的控制。

当`umem`模块接收到用户进程的`umem_malloc`请求后，会向用户进程返回一个虚拟地址。然而，由于`umem`采用了按需分页的设计，该虚拟地址在未被访问时并不会有对应的物理页，因此访问该地址会导致段错误。当用户进程首次读写该地址时，将会触发段错误异常。`umem-user`库捕获用户进程触发的段错误异常，并将捕获的异常信息（包含触发段错误的地址）发送给`umem`库。`umem`库在收到该地址时，会判断该地址是否为`umem`内存池申请的虚拟地址空间。如果是，并且`umem`中对应的内存池仍有剩余的物理页，则`umem`模块会将该内存池中的一个物理页填充在用户进程触发段错误的虚拟地址处。

## 提示

本项目主要涉及对 `umem.c` 文件的修改，重点关注标记为 `TODO` 的地方。

本项目涉及字符设备相关内容，包括物理页和 slab 的申请和释放。可以参考 [https://lwn.net/Kernel/LDD3/](https://lwn.net/Kernel/LDD3/) 了解这些细节。

本项目需要为每个进程维护相关信息，可能会使用到 [https://elixir.bootlin.com/linux/latest/source/include/linux/list.h](https://elixir.bootlin.com/linux/latest/source/include/linux/list.h) 中定义的链表。

本项目涉及虚拟地址空间的申请，页表项的创建和删除，可能会使用到 [https://elixir.bootlin.com/linux/latest/source/include/linux/mm.h](https://elixir.bootlin.com/linux/latest/source/include/linux/mm.h) 中提供的有关内存操作的 API。

本项目提供的测试程序 `tests/main.c` 不是最终的验收程序。此外，不应修改提供给用户态进程的接口文件 `tests/umem-user.c` 和 `tests/umem-user.h`。如果进行了修改，请在报告中说明原因。

用户进程在打开`/proc/umem`文件时，可以为该进程保存自定义的数据。具体地，可以通过`umem_open`函数和`umem_release`函数中的`struct file *filp`参数的`filp->private_data`字段来实现。可以使用`kmalloc`函数创建一个结构体来保存进程相关的信息，例如进程当前使用了`umem`模块提供的内存池的哪些虚拟地址以及使用了哪些物理页。在进程退出时，需要使用`kfree`释放该结构体，以避免内核内存泄漏。

`umem_ioctl`函数已经提供了获取触发段错误的地址的方法，请注意`switch (cmd)`语句中的`case UMEM_IOC_PAGE_FAULT:`分支。在`get_user`调用后，`kern_umem_info.umem_addr`中将保存触发段错误的虚拟地址信息。需要增加判断来验证该地址是否为`umem`模块为进程分配的虚拟地址空间，并为其分配`umem`模块的内存池中的物理页。

`umem_ioctl`函数还提供了处理用户的`umem_malloc`和`umem_free`请求的分支，分别对应`switch (cmd)`语句中的`case UMEM_IOC_MALLOC:`和`case UMEM_IOC_FREE:`分支。参数的获取已经包含在模板中。需要实现用户进程虚拟地址空间的申请和释放。

函数`umem_ioctl`返回0表示一切正常，没有错误。返回负值`-EINVAL`表示发生了错误。错误号的定义可以参考[https://elixir.bootlin.com/linux/latest/source/include/linux/errno.h](https://elixir.bootlin.com/linux/latest/source/include/linux/errno.h)，本项目对此没有要求。

`current`是内核中经常使用的宏，用于表示当前进程。在本项目中，`current`即为当前使用`umem`模块申请内存的进程。

`page_to_pfn`是内核中常用的宏，用于将一个物理页的`struct page *`结构体指针转换为其物理页号。

关于Linux内核中的内存申请，请参考[https://static.lwn.net/images/pdf/LDD3/ch08.pdf](https://static.lwn.net/images/pdf/LDD3/ch08.pdf)。请注意，`vmalloc`函数申请的是**内核的**虚拟地址空间的内存，而不是**用户进程的**虚拟地址空间的内存。本项目一般不会使用到这个调用。对于本项目来说，申请物理页、释放物理页，使用`alloc_page`和`__free_page`函数即已足够满足要求。如果对Linux内核中丰富的内存管理API不感兴趣，也可以直接使用这两个调用来管理`umem`内存池中的物理页。

链表是数据结构课程中常用的经典数据结构。Linux内核采用了基于宏的优雅实现。一个用户自定义的类型，只需包含`struct list_head`字段，即可将该类型作为链表节点使用。你可以查阅网络上的相关材料，学习Linux内核中链表的创建和修改方式。本项目不要求使用进阶的数据结构，例如内核中的哈希表，但你也可以使用以提高代码的运行效率。

可以使用`vm_mmap`函数在内核模块中申请虚拟地址空间。请注意使用参数`PROT_NONE`以避免OS自动分配系统中其他来源的物理页，让`umem`模块能够捕获用户进程触发的段错误。类似地，可以使用`vm_munmap`函数释放虚拟地址空间。[https://man7.org/linux/man-pages/man2/mmap.2.html](https://man7.org/linux/man-pages/man2/mmap.2.html)

要修改页表，让虚拟地址对应某个物理页，可以使用`remap_pfn_range`函数实现。当然，也可以使用其他方案。本项目对实现方式没有严格要求。[https://elixir.bootlin.com/linux/v6.9.1/source/mm/memory.c#L2651](https://elixir.bootlin.com/linux/v6.9.1/source/mm/memory.c#L2651)

对于Linux内核中的内存管理，建议大家复习操作系统课程中相关知识，例如按需分页（demand paging）、页表、分页错误（page fault）等。操作系统负责维护进程的虚拟地址和物理地址之间的映射，这涉及修改用户进程的页表。一般来说，用户进程无法直接修改自身的页表，页表的修改通常由操作系统完成。

在实践本项目之前，建议大家先复习相关知识。此外，对于Linux内核中丰富的内存管理API，建议查阅相关文档和资料，以便更好地理解和应用这些API。

在完成项目时，要注意遵循Linux内核的设计原则和最佳实践，确保代码的可读性、稳定性和安全性。同时，注重代码的注释和文档，以便他人阅读和理解你的代码。

## 提交

实验报告和所有源代码打包成 `zip` 文件。源代码文件夹命名为 `[SJTU_ID]_src`，实验报告命名为 `[SJTU_ID]_report.pdf`。