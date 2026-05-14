import pandas as pd
import numpy as np
from io import StringIO
import csv

# 创建机器学习三大类型对比数据
machine_learning_comparison = [
    {
        "类别": "监督学习 (Supervised Learning)",
        "中文名称": "监督学习",
        "定义": "从带有标签的训练数据中学习，建立输入到输出的映射关系",
        "核心目标": "学习输入特征与输出标签之间的函数关系",
        "训练数据": "带标签的数据 (X, y)",
        "数据要求": "需要大量标注数据",
        "算法示例": "线性回归、逻辑回归、决策树、SVM、神经网络",
        "主要任务": "分类、回归、预测",
        "评估指标": "准确率、精确率、召回率、F1分数、RMSE、MAE",
        "输出类型": "离散值（分类）或连续值（回归）",
        "是否需要标签": "是",
        "实时反馈": "训练时提供完整标签",
        "典型应用": "垃圾邮件分类、房价预测、图像识别、疾病诊断",
        "优点": "1. 目标明确，效果直接<br>2. 评估简单直接<br>3. 理论成熟，应用广泛",
        "缺点": "1. 依赖标注数据，成本高<br>2. 对未知数据泛化能力有限<br>3. 容易过拟合",
        "数据依赖程度": "高",
        "可解释性": "中等至高（取决于模型）",
        "训练阶段": "离线训练",
        "推断阶段": "离线或在线推断",
        "超参数调整": "需要",
        "是否需探索": "否",
        "损失函数": "交叉熵、均方误差等",
        "优化目标": "最小化预测误差"
    },
    {
        "类别": "无监督学习 (Unsupervised Learning)",
        "中文名称": "无监督学习",
        "定义": "从无标签的数据中发现内在结构、模式或分布",
        "核心目标": "发现数据中的隐藏结构、模式或关系",
        "训练数据": "无标签数据 (X)",
        "数据要求": "不需要标签，但需要足够的数据量",
        "算法示例": "K-Means、层次聚类、PCA、t-SNE、自编码器",
        "主要任务": "聚类、降维、关联规则、异常检测",
        "评估指标": "轮廓系数、Calinski-Harabasz指数、重建误差、互信息",
        "输出类型": "簇标签、低维表示、关联规则",
        "是否需要标签": "否",
        "实时反馈": "无直接反馈",
        "典型应用": "客户细分、推荐系统、数据可视化、异常检测",
        "优点": "1. 不需要标注数据<br>2. 可以发现人类未察觉的模式<br>3. 可处理高维数据",
        "缺点": "1. 结果难以评估和解释<br>2. 目标不明确，可能无实际意义<br>3. 对参数敏感",
        "数据依赖程度": "非常高",
        "可解释性": "低至中等",
        "训练阶段": "离线训练",
        "推断阶段": "离线推断",
        "超参数调整": "需要（如聚类数量）",
        "是否需探索": "是（探索数据内在结构）",
        "损失函数": "重建误差、簇内距离等",
        "优化目标": "优化内在结构指标"
    },
    {
        "类别": "强化学习 (Reinforcement Learning)",
        "中文名称": "强化学习",
        "定义": "智能体通过与环境交互，学习采取行动以最大化累积奖励",
        "核心目标": "学习最优策略以最大化长期回报",
        "训练数据": "状态、行动、奖励序列 (S, A, R, S')",
        "数据要求": "需要环境交互生成的数据",
        "算法示例": "Q-Learning、DQN、Policy Gradient、A3C、PPO",
        "主要任务": "决策制定、控制、游戏、机器人控制",
        "评估指标": "累积奖励、成功率、步数、收敛速度",
        "输出类型": "策略（状态到行动的映射）",
        "是否需要标签": "否（需要奖励信号）",
        "实时反馈": "延迟的奖励反馈",
        "典型应用": "游戏AI、机器人控制、自动驾驶、资源管理",
        "优点": "1. 适合序列决策问题<br>2. 可处理动态环境<br>3. 能学习长期策略",
        "缺点": "1. 训练成本高，样本效率低<br>2. 不稳定，难收敛<br>3. 奖励设计困难",
        "数据依赖程度": "中等（需要环境交互）",
        "可解释性": "低",
        "训练阶段": "在线交互学习",
        "推断阶段": "在线决策",
        "超参数调整": "需要且敏感",
        "是否需探索": "是（探索-利用权衡）",
        "损失函数": "TD误差、策略梯度等",
        "优化目标": "最大化期望累积奖励"
    }
]

# 创建DataFrame
df = pd.DataFrame(machine_learning_comparison)

# 显示完整的DataFrame（不截断）
pd.set_option('display.max_columns', None)
pd.set_option('display.width', None)
pd.set_option('display.max_colwidth', None)

print("=" * 120)
print("机器学习三大类型核心特点对比表")
print("=" * 120)
print(df.to_string(index=False))

# 保存为CSV文件
csv_filename = "machine_learning_comparison.csv"
df.to_csv(csv_filename, index=False, encoding='utf-8-sig')
print(f"\n对比表已保存为: {csv_filename}")

# 读取并显示CSV文件内容
print(f"\n读取CSV文件内容:")
print("=" * 120)
with open(csv_filename, 'r', encoding='utf-8-sig') as f:
    reader = csv.reader(f)
    for i, row in enumerate(reader):
        if i == 0:  # 标题行
            print(" | ".join(row))
        else:
            # 对每列进行适当截断以便显示
            truncated_row = []
            for cell in row:
                if len(cell) > 30:
                    truncated_row.append(cell[:27] + "...")
                else:
                    truncated_row.append(cell)
            print(" | ".join(truncated_row))
        if i == 0:  # 在标题后添加分隔线
            print("-" * 120)

# 生成简化的横向对比表
print("\n" + "=" * 120)
print("机器学习三大类型简明对比表")
print("=" * 120)

# 选择关键特征进行对比
key_features = ["类别", "数据要求", "主要任务", "是否需要标签", "实时反馈", "典型应用", "优缺点摘要"]

# 创建简明对比数据
simplified_comparison = []
for item in machine_learning_comparison:
    simplified = {feature: item[feature] for feature in key_features}
    
    # 创建优缺点摘要
    if item["类别"] == "监督学习 (Supervised Learning)":
        simplified["优缺点摘要"] = "✓ 目标明确 ✗ 需要标注数据"
    elif item["类别"] == "无监督学习 (Unsupervised Learning)":
        simplified["优缺点摘要"] = "✓ 无需标注 ✗ 结果难评估"
    else:
        simplified["优缺点摘要"] = "✓ 适合决策 ✗ 训练成本高"
    
    simplified_comparison.append(simplified)

# 转换为DataFrame并显示
df_simple = pd.DataFrame(simplified_comparison)
print(df_simple.to_string(index=False))

# 保存简明版本
simple_csv_filename = "machine_learning_simple_comparison.csv"
df_simple.to_csv(simple_csv_filename, index=False, encoding='utf-8-sig')
print(f"\n简明对比表已保存为: {simple_csv_filename}")

# 生成按特征对比的表格（更适合横向比较）
print("\n" + "=" * 120)
print("按特征对比的机器学习类型表")
print("=" * 120)

# 定义要对比的特征
comparison_features = [
    "定义",
    "核心目标", 
    "数据要求",
    "主要任务",
    "典型算法",
    "评估指标",
    "典型应用",
    "关键优势",
    "主要挑战"
]

# 创建对比字典
comparison_dict = {
    "特征": comparison_features,
    "监督学习": [
        "从带有标签的训练数据中学习，建立输入到输出的映射关系",
        "学习输入特征与输出标签之间的函数关系",
        "需要大量标注数据",
        "分类、回归、预测",
        "线性回归、逻辑回归、SVM、神经网络",
        "准确率、精确率、召回率、F1分数",
        "垃圾邮件分类、房价预测、图像识别",
        "目标明确，效果可直接评估",
        "依赖标注数据，成本高"
    ],
    "无监督学习": [
        "从无标签的数据中发现内在结构、模式或分布",
        "发现数据中的隐藏结构、模式或关系",
        "不需要标签，但需要足够的数据量",
        "聚类、降维、异常检测",
        "K-Means、PCA、DBSCAN、自编码器",
        "轮廓系数、重建误差、互信息",
        "客户细分、推荐系统、数据可视化",
        "不需要标注数据，可发现未知模式",
        "结果难以评估和解释"
    ],
    "强化学习": [
        "智能体通过与环境交互，学习采取行动以最大化累积奖励",
        "学习最优策略以最大化长期回报",
        "需要环境交互生成的数据",
        "决策制定、控制、优化",
        "Q-Learning、DQN、Policy Gradient",
        "累积奖励、成功率、收敛速度",
        "游戏AI、机器人控制、自动驾驶",
        "适合序列决策，能学习长期策略",
        "训练成本高，样本效率低"
    ]
}

# 转换为DataFrame
df_feature_based = pd.DataFrame(comparison_dict)
print(df_feature_based.to_string(index=False))

# 保存特征对比表
feature_csv_filename = "ml_feature_comparison.csv"
df_feature_based.to_csv(feature_csv_filename, index=False, encoding='utf-8-sig')
print(f"\n特征对比表已保存为: {feature_csv_filename}")

# 创建用于导入Excel或数据库的格式化CSV
print("\n" + "=" * 120)
print("CSV文件生成完成！")
print("=" * 120)
print("已生成以下CSV文件：")
print(f"1. {csv_filename} - 完整对比表")
print(f"2. {simple_csv_filename} - 简明对比表")  
print(f"3. {feature_csv_filename} - 特征对比表")
print("\n您可以将这些文件导入到Excel、数据库或其他数据分析工具中。")

# 显示CSV文件示例内容
print("\n" + "=" * 120)
print("CSV文件示例（前3行）：")
print("=" * 120)

with open(csv_filename, 'r', encoding='utf-8-sig') as f:
    lines = f.readlines()[:4]
    for line in lines:
        # 分割并显示前几列
        cells = line.strip().split(',')
        # 只显示前5列以避免输出过长
        truncated_line = ','.join(cells[:5])
        if len(cells) > 5:
            truncated_line += ",..."
        print(truncated_line)