# 实验题目：实验五  项目跟踪工具、HR管理、工作量估计
实验目的：

1.  练习项目跟踪工具的使用。
2.  研论项目人员特点、工作风格、组织结构。
3.  练习工作量估算。
4.  小组继续分工协作进行开源项目的分析、研究。

要求：
1. 每组组长（可轮换制），负责小组分工、讨论、汇总。
2. 小组工作方式，应合理分工，将每个人的工作日志和小组进度报告等文档保存到共享平台上。

实验内容：
1. 练习项目跟踪进展工具的使用
    选用适合的CASE工具，如用甘特图记录跟踪项目进展。
    研究Scrum简介中的燃尽图（BrunDown Chart），对比课本图3-8的不同。

2. 研讨人员特性，工作风格，组织结构。
    （1）分析小组的成员各自贴近课本所指的哪种风格？针对用户的工作风格，应如何沟通？
    （2）结合课本主程序员负责制小组、忘我方法两种组织结构和补充材料3-2，讨论结构化与创造性的关系。
    （3）分工调研国内与国外软件开发团队的管理方式对比（如打卡制，KPI等）。
    讨论：
    从个人角度，你最喜欢的工作方式、工作环境条件、管理方式是什么？
    从团队项目管理的角度，你认为最有效的项目组工作管理方式是什么？

3. 练习工作量估算 COCOMOII：
   研究课本3.7（P94)皮卡地里电视广告销售系统按COCOMOII的工作量模型计算阶段1的例子（结合P79-80表），
   设：数据表数目都<4,
   预约屏幕（视图总数目<4）、 刊例价格表(ratecard)屏幕（视图总数目>8）、时段可用性屏幕（视图总数目>8），销售报表（总数目>8）。开发人员经验和能力认为是一般，CASE成熟度和能力是低，计算阶段1的工作量估算值？

4. 继续团队协作继续进行开源项目的分析工作
   （a）分工协作，根据自己的项目完善泛读报告。 

## 选用适合的CASE工具，如用甘特图记录跟踪项目进展。

## 研究Scrum简介中的燃尽图（BrunDown Chart），对比课本图3-8的不同。

#### 完成人：蒋裕成 薛正康

燃尽图（Burndown Chart）是敏捷项目管理和Scrum框架中常用的一种图表，用于可视化项目进度和团队的工作速度。它可以用于跟踪团队在一个特定的时间段内（通常是一个迭代周期，如一个Sprint）完成任务的情况。通过燃尽图，团队成员、Scrum Master和产品负责人可以清晰地了解项目进度，发现问题并及时采取行动，从而更好地调整计划和资源。同时，燃尽图也能促进团队之间的沟通与协作，提高团队的透明度。

燃尽图有两种形式：**向下燃尽图（burn down chart）** 和 **向上燃尽图（burn up chart）**。向下燃尽图显示的是随着时间的推移，剩余工作量的减少情况，理想情况下是一条向下的曲线。向上燃尽图显示的是随着时间的推移，已完成工作量和总工作量的变化情况，理想情况下是两条平行的线。下面是一个向下燃尽图的示例

![image](https://user-images.githubusercontent.com/119791285/227778384-2323abff-efcb-49ea-ae8e-1b1da3d48a14.png)

燃尽图的基本结构如下：

1. 横轴（X轴）：表示时间，通常以迭代周期（Sprint）内的工作日为单位。从Sprint的第一天开始，一直到Sprint的最后一天。
2. 纵轴（Y轴）：表示剩余工作量，通常以故事点（Story Points）或者任务数量为单位。
3. 理想燃尽线：从左上角开始，斜率代表理想的工作速度。这条线表示在理想情况下，团队在每个工作日应该完成的工作量，使得在Sprint结束时，所有任务都能完成。
4. 实际燃尽线：从左上角开始，根据团队实际完成的任务，实际燃尽线表示在Sprint的每一天团队剩余的工作量。通常会看到这条线在图表中波动，因为团队可能在某些日子完成的任务多，而在其他日子完成的任务少。实际燃尽线可以帮助团队识别进度问题，了解是否需要采取措施来调整计划或者资源分配。

通过比较理想燃尽线和实际燃尽线，可以对团队的工作进度进行评估。以下是可能的情况：

1. 实际燃尽线低于理想燃尽线：表示团队进度超前，完成任务的速度比预期快。这种情况下，团队可以考虑在Sprint期间增加更多的任务，或者继续保持当前的工作速度。
2. 实际燃尽线高于理想燃尽线：表示团队进度滞后，完成任务的速度比预期慢。在这种情况下，团队需要分析可能的原因，如资源短缺、技术难题或需求不明确等，并采取相应措施来解决问题，以提高工作效率。
3. 实际燃尽线接近理想燃尽线：表示团队进度基本符合预期。这种情况下，团队可以继续保持当前的工作速度，确保按计划完成任务。

作为一个显示项目或迭代中剩余工作量的图表，其具有以下优点：

1. 能直观地展示项目或迭代的进展情况，让所有人都能看到当前的状态和趋势。
2. 能够帮助负责人及时发现问题和风险，比如进度滞后、工作量不饱和、需求变更等，并采取相应的措施进行调整。
3. 可以激励团队成员积极参与项目，并提高工作效率和质量。
4. 可以根据历史数据评估团队的工作能力和速度，为后续的计划制定提供参考。

当然，任何事物都有两面性，燃尽图也有其局限性：

1. 不能呈现所有信息，比如具体完成了哪些任务，任务之间的依赖关系，任务的优先级等。
2. 只能反映任务的数量和工时，而不能反映任务的质量和价值，
3. 只能作为一个量化的参考，不能保证计划的准确性和稳定性，因为计划可能受到需求变更、资源变动、风险事件等因素的影响。
4. 不能替代其他项目管理工具和方法，比如需求分析、风险管理、沟通协作等。

尽管如此，燃尽图仍有其用武之地：

1. 其适用于敏捷开发（agile）项目管理中，特别是Scrum方法中，作为一个重要的可视化工具。
2. 其适用于任何可以量化并随时间变化的项目或任务，比如软件开发、产品设计、学习计划等。
3. 其适用于需要频繁跟进和调整进度的项目或任务，比如有较高不确定性或变化性的项目或任务。

---

燃尽图与图3-8之间的不同：

- 关注的指标：燃尽图关注的是剩余工作量，以故事点（Story Points）或任务数量为单位。而图3-8的跟踪计划开销与实际开销对比图关注的是项目的预算和实际花费，以货币单位（美元）表示。
- 目的：燃尽图的主要目的是展示项目进度，以及团队在一个迭代周期（如Sprint）内的工作速度。图3-8的主要目的是监控项目预算，确保项目在财务方面的可控性。
- 图表结构：燃尽图的横轴表示时间（通常是一个迭代周期的工作日），纵轴表示剩余工作量。图3-8的横轴也表示时间，但纵轴表示项目的预算和实际花费。
- 应用场景：燃尽图主要应用于敏捷项目管理和Scrum框架，而图3-8可以应用于各种项目管理方法，如传统的瀑布式项目管理和敏捷项目管理等。
- 可视化效果：燃尽图通过理想燃尽线和实际燃尽线直观地展示项目进度，便于团队成员、Scrum Master和产品负责人了解进度情况。图3-8通过对比预算和实际花费，直观地展示项目在财务方面的执行情况，便于项目经理和相关人员了解项目的成本控制情况。

作业提交：
（1）每个人针对任务2、3，书写报告。
（2）组长将汇总后小组调研报告（注明每部分的贡献者）和开源项目分析工作进度等打包提交。
