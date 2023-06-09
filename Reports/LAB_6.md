# 实验题目：实验六 项目风险管理、SDP

实验目的：

1. 练习项目风险管理。
2. 了解软件文档规范国标
3. 小组继续分工协作进行开源项目分析、研究。 

要求：
1. 每组组长（可轮换制），负责小组分工、讨论、汇总。
2. 小组工作方式，应合理分工，将每个人的工作日志和小组进度报告等文档保存到共享平台上。

实验内容：

## 1. 风险管理

针对课本P99，ch3 习题11，分析在开源项目分析任务中可能存在的风险。并分栏列出风险名称、风险暴露、风险控制方案（风险管理）。



## 2. 针对课本P99，ch3 习题12，小组成员讨论给出相应意见。

**完成人：冯潇宇，蒋裕成，薛正康**

很多项目经理根据过去项目中程序员的生产率来计划项目的进度，生产率通常根据单位时间的单位规模来测量。例如，一个组织机构可能每天生产300行代码或每月生产1200个应用点。用这种方法测量生产率合适吗?

根据下列事项讨论生产率的测度:

- 用不同的语言实现同样的设计,可能产生的代码行数不同。
- 在实现开始之前不能用基于代码行的生产率进行测量。
- 程序员可能为了达到生产率的目标而堆积代码。



我们认为使用这种方法测量生产率不合适。

一个组织机构肯定有负责不同部分的成员，如果采取一个统一的标准进行量化，难免有失偏颇。其次就算仅考虑代码部分，这样测试生产率也不合适。

如题目中所说，不同语言的语法不同，其代码行数有较大差距。而且，即使是同样语言的同样功能也有实现方式，有些方式简单但是耦合性强，难以拓展；有些方式虽然较为复杂，但是拓展性强。虽然其实现了相同功能，但是要根据具体使用场景来选择合适的方式，不能简单的用行数计算。另外，有时候同样的问题有着不同的解决方法，有经验的程序员可能使用开源库几行就解决了问题，新手程序员却可能从头造轮子，费时费力，难道说新手程序员的生产力更高吗？

除此之外，一旦生产率进行量化，就难免会出现浑水摸鱼的情况，使用冗余的代码行来做面子工程，实际上却对整个项目百害而无一利，不但降低了运行效率，还增加了维护成本。

接下来给出一些关于上述三个事项的详细例子：

### 1. 用不同的语言实现同样的设计，可能产生的代码行数不同。

以一个简单的"Hello, World!"程序为例。在Python中，仅需一行代码即可实现：

```python
print("Hello, World!")
```

而在Java中，实现相同的功能需要更多的代码行：

```java
public class HelloWorld {
    public static void main(String[] args) {
        System.out.println("Hello, World!");
    }
}
```

这个例子表明，仅仅通过代码行数来衡量生产率可能导致对不同编程语言的开发效率产生误导。

可能的改进方法：使用功能点分析（Function Point Analysis, FPA）来衡量生产率。功能点分析通过计算软件系统的功能大小（而非代码行数）来衡量生产率。因为功能点分析侧重于软件系统所提供的功能，而不是特定语言的代码实现，所以它可以更准确地比较使用不同编程语言开发的项目的生产率。

### 2. 在实现开始之前不能用基于代码行的生产率进行测量。

在项目的规划和设计阶段，项目经理和团队成员可能会进行需求分析、概念设计、详细设计等活动。在这些阶段，团队成员可能会创建用例图、活动图、类图等，而这些活动并不直接产生代码行数。基于代码行数的生产率无法反映这些阶段的工作质量和工作量。

可能的改进方法：使用预估技术，如专家估算、类比估算或参数模型（例如COCOMO模型）来预测项目的生产率。这些方法通过分析历史项目数据、考虑项目的规模、复杂性和其他因素，为项目在实现开始之前提供生产率预测。

### 3. 程序员可能为了达到生产率的目标而堆积代码。

假设一个程序员被要求在一个月内完成3000行代码。为了达到这一目标，他可能会编写一些不必要的代码，如冗余的注释、重复的函数或过度封装。例如，他可能会将一个简单的加法操作封装成一个独立的函数，从而增加代码行数，而实际上这样的封装可能并没有实际价值：

```python
def add(a, b):
    return a + b

result = add(1, 2)
```

相比之下，一个更为简洁和直接的实现如下：

```python
result = 1 + 2
```

在这种情况下，代码行数的增加并不代表生产率的提高，反而可能导致代码的可读性和可维护性降低。

可能的改进方法：实施代码审查和质量检查。代码审查可以让团队成员相互监督，确保代码质量。同时，通过引入代码质量检查工具（如SonarQube），可以自动检查潜在的质量问题，从而确保代码的可读性和可维护性。此外，可以将生产率指标与其他指标（如缺陷密度、测试覆盖率等）结合使用，以便更全面地评估项目的进度和质量。

## 3. 参照课程网站“教学资料”/“实验”/“参考资料”中“计算软件文档编制规范国家标准GB5687”和实例，对照课本P87 项目计划内容（15项），对比SDP(软件开发计划)，查找有什么不同？



## 4. 继续团队协作继续进行开源项目的分析工作

（a）分工协作，根据自己的项目完善泛读报告。 
（b）记录跟踪团队项目推进的工作进度。 



作业提交：
（1）每个人针对任务1、3，书写报告。
（2）组长将汇总后小组调研报告和开源项目分析工作进度等打包提交。实验任务2,4
