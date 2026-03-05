import pandas as pd
import numpy as np
from datetime import datetime

def create_ml_metrics_csv():
    """
    生成机器学习分类与回归任务常用评价指标对照表的CSV文件
    """
    
    # 准备数据
    metrics_data = [
        # 分类任务指标
        {
            "维度": "分类任务指标",
            "评价指标": "准确率",
            "主要任务": "分类",
            "核心思想/公式": "正确预测样本数 / 总样本数",
            "取值范围": "[0, 1]",
            "特点与适用场景": "最直观，但不适用于类别不平衡的数据集"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "精确率",
            "主要任务": "分类（正类）",
            "核心思想/公式": "TP / (TP + FP)",
            "取值范围": "[0, 1]",
            "特点与适用场景": "关注模型预测为正的样本中，有多少真是正类。侧重降低误报（FP），如垃圾邮件检测"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "召回率",
            "主要任务": "分类（正类）",
            "核心思想/公式": "TP / (TP + FN)",
            "取值范围": "[0, 1]",
            "特点与适用场景": "关注所有真实正类中，被模型找出多少。侧重降低漏报（FN），如疾病诊断"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "F1-Score",
            "主要任务": "分类",
            "核心思想/公式": "2 × 精确率 × 召回率 / (精确率 + 召回率)",
            "取值范围": "[0, 1]",
            "特点与适用场景": "精确率与召回率的调和平均数，在两者间寻求平衡。适用于类别不平衡场景"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "AUC-ROC",
            "主要任务": "二分类",
            "核心思想/公式": "ROC曲线下的面积",
            "取值范围": "[0, 1]",
            "特点与适用场景": "模型综合排序能力的体现，衡量模型将正样本排在负样本前面的整体能力。与阈值无关，对类别不平衡相对稳健"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "混淆矩阵",
            "主要任务": "分类",
            "核心思想/公式": "展示 TP, FP, TN, FN 的矩阵",
            "取值范围": "-",
            "特点与适用场景": "所有分类指标的基础，用于可视化模型的详细预测结果，便于计算各类衍生指标"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "对数损失",
            "主要任务": "分类",
            "核心思想/公式": "-1/N × Σ[y_i·log(p_i) + (1-y_i)·log(1-p_i)]",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "衡量预测概率的准确性。惩罚预测概率远离真实标签的情况。值越小越好"
        },
        # 回归任务指标
        {
            "维度": "回归任务指标",
            "评价指标": "均方误差",
            "主要任务": "回归",
            "核心思想/公式": "1/n × Σ(y_i - ŷ_i)²",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "最常用，放大较大误差的影响，对异常值敏感。使优化目标可导，广泛用于模型训练"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "均方根误差",
            "主要任务": "回归",
            "核心思想/公式": "√MSE",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "MSE的平方根。与预测目标量纲一致，解释性更强。同样对异常值敏感"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "平均绝对误差",
            "主要任务": "回归",
            "核心思想/公式": "1/n × Σ|y_i - ŷ_i|",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "绝对误差的平均值。对异常值鲁棒性优于MSE/RMSE，解释性直观"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "R²决定系数",
            "主要任务": "回归",
            "核心思想/公式": "1 - Σ(y_i-ŷ_i)² / Σ(y_i-ȳ)²",
            "取值范围": "(-∞, 1]",
            "特点与适用场景": "衡量模型拟合优度，表示模型对目标变量方差的解释比例。越接近1越好"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "平均绝对百分比误差",
            "主要任务": "回归",
            "核心思想/公式": "100% × 1/n × Σ|(y_i-ŷ_i)/y_i|",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "相对误差，以百分比表示。适用于量纲不同或需要了解相对误差的场景。对真实值为0的情况不稳定"
        }
    ]
    
    # 创建DataFrame
    df = pd.DataFrame(metrics_data)
    
    # 添加生成时间信息
    current_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    # 保存为CSV文件
    filename = f"ml_evaluation_metrics_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
    df.to_csv(filename, index=False, encoding='utf-8-sig')
    
    print(f"已成功生成机器学习评价指标对照表：{filename}")
    print(f"生成时间：{current_time}")
    print(f"文件位置：当前工作目录")
    print(f"总指标数量：{len(df)} 个")
    print(f"分类指标：{len(df[df['维度'] == '分类任务指标'])} 个")
    print(f"回归指标：{len(df[df['维度'] == '回归任务指标'])} 个")
    
    # 显示前几行数据
    print("\n数据预览：")
    print(df.head())
    
    return filename, df

def create_extended_ml_metrics_csv():
    """
    生成更详细的机器学习评价指标对照表，包含更多信息和Python计算示例
    """
    
    # 准备更详细的数据
    detailed_metrics = [
        # 分类任务指标
        {
            "任务类型": "分类",
            "评价指标": "准确率",
            "英文名称": "Accuracy",
            "核心公式": "(TP+TN)/(TP+TN+FP+FN)",
            "取值范围": "[0, 1]",
            "最佳值": "1",
            "最差值": "0",
            "Python函数": "sklearn.metrics.accuracy_score",
            "适用场景": "各类均衡的分类问题",
            "注意事项": "类别不平衡时不可靠"
        },
        {
            "任务类型": "分类",
            "评价指标": "精确率",
            "英文名称": "Precision",
            "核心公式": "TP/(TP+FP)",
            "取值范围": "[0, 1]",
            "最佳值": "1",
            "最差值": "0",
            "Python函数": "sklearn.metrics.precision_score",
            "适用场景": "误报成本高的场景（如垃圾邮件检测）",
            "注意事项": "可能牺牲召回率"
        },
        {
            "任务类型": "分类",
            "评价指标": "召回率",
            "英文名称": "Recall",
            "核心公式": "TP/(TP+FN)",
            "取值范围": "[0, 1]",
            "最佳值": "1",
            "最差值": "0",
            "Python函数": "sklearn.metrics.recall_score",
            "适用场景": "漏报成本高的场景（如疾病诊断）",
            "注意事项": "可能牺牲精确率"
        },
        {
            "任务类型": "分类",
            "评价指标": "F1-Score",
            "英文名称": "F1-Score",
            "核心公式": "2×P×R/(P+R)",
            "取值范围": "[0, 1]",
            "最佳值": "1",
            "最差值": "0",
            "Python函数": "sklearn.metrics.f1_score",
            "适用场景": "精确率和召回率需要平衡的场景",
            "注意事项": "调和平均数，对低值惩罚更大"
        },
        {
            "任务类型": "分类",
            "评价指标": "AUC-ROC",
            "英文名称": "AUC-ROC",
            "核心公式": "ROC曲线下面积",
            "取值范围": "[0, 1]",
            "最佳值": "1",
            "最差值": "0.5",
            "Python函数": "sklearn.metrics.roc_auc_score",
            "适用场景": "各类分类问题，尤其是不平衡数据",
            "注意事项": "衡量排序能力，与阈值无关"
        },
        {
            "任务类型": "分类",
            "评价指标": "对数损失",
            "英文名称": "Log Loss",
            "核心公式": "-1/N Σ[y_i·log(p_i)+(1-y_i)·log(1-p_i)]",
            "取值范围": "[0, +∞)",
            "最佳值": "0",
            "最差值": "+∞",
            "Python函数": "sklearn.metrics.log_loss",
            "适用场景": "概率输出模型评估",
            "注意事项": "对错误概率预测惩罚严重"
        },
        # 回归任务指标
        {
            "任务类型": "回归",
            "评价指标": "均方误差",
            "英文名称": "Mean Squared Error (MSE)",
            "核心公式": "1/n Σ(y_i-ŷ_i)²",
            "取值范围": "[0, +∞)",
            "最佳值": "0",
            "最差值": "+∞",
            "Python函数": "sklearn.metrics.mean_squared_error",
            "适用场景": "一般回归问题，模型训练",
            "注意事项": "对异常值敏感，量纲为原量纲平方"
        },
        {
            "任务类型": "回归",
            "评价指标": "均方根误差",
            "英文名称": "Root Mean Squared Error (RMSE)",
            "核心公式": "√MSE",
            "取值范围": "[0, +∞)",
            "最佳值": "0",
            "最差值": "+∞",
            "Python函数": "np.sqrt(mean_squared_error(...))",
            "适用场景": "需要与原目标相同量纲的场景",
            "注意事项": "对异常值敏感，解释性更好"
        },
        {
            "任务类型": "回归",
            "评价指标": "平均绝对误差",
            "英文名称": "Mean Absolute Error (MAE)",
            "核心公式": "1/n Σ|y_i-ŷ_i|",
            "取值范围": "[0, +∞)",
            "最佳值": "0",
            "最差值": "+∞",
            "Python函数": "sklearn.metrics.mean_absolute_error",
            "适用场景": "对异常值敏感度低的场景",
            "注意事项": "鲁棒性更好，不可导（训练时不常用）"
        },
        {
            "任务类型": "回归",
            "评价指标": "R²决定系数",
            "英文名称": "R-squared",
            "核心公式": "1 - Σ(y_i-ŷ_i)²/Σ(y_i-ȳ)²",
            "取值范围": "(-∞, 1]",
            "最佳值": "1",
            "最差值": "-∞",
            "Python函数": "sklearn.metrics.r2_score",
            "适用场景": "模型解释力评估，模型比较",
            "注意事项": "负数表示模型不如均值预测"
        },
        {
            "任务类型": "回归",
            "评价指标": "平均绝对百分比误差",
            "英文名称": "Mean Absolute Percentage Error (MAPE)",
            "核心公式": "100% × 1/n Σ|(y_i-ŷ_i)/y_i|",
            "取值范围": "[0, +∞)",
            "最佳值": "0",
            "最差值": "+∞",
            "Python函数": "自定义计算",
            "适用场景": "需要百分比误差评估的场景",
            "注意事项": "y_i=0时无法计算，对负值处理需谨慎"
        }
    ]
    
    # 创建DataFrame
    df = pd.DataFrame(detailed_metrics)
    
    # 保存为CSV文件
    filename = f"detailed_ml_metrics_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
    df.to_csv(filename, index=False, encoding='utf-8-sig')
    
    print(f"\n已成功生成详细机器学习评价指标对照表：{filename}")
    print(f"文件位置：当前工作目录")
    print(f"总指标数量：{len(df)} 个")
    
    return filename, df

def create_python_example_code():
    """
    创建包含Python计算示例的代码文件
    """
    code_content = '''"""
机器学习评价指标Python计算示例
需要安装：pip install scikit-learn numpy pandas
"""

import numpy as np
import pandas as pd
from sklearn.metrics import (
    accuracy_score, precision_score, recall_score, f1_score,
    roc_auc_score, log_loss, mean_squared_error, mean_absolute_error, r2_score
)

# 示例数据
y_true_class = np.array([1, 0, 1, 1, 0, 1, 0, 0, 1, 0])
y_pred_class = np.array([1, 0, 0, 1, 0, 1, 1, 0, 1, 0])
y_pred_proba = np.array([0.9, 0.1, 0.4, 0.8, 0.2, 0.7, 0.6, 0.3, 0.85, 0.15])

y_true_reg = np.array([3.0, -0.5, 2.0, 7.0, 5.2])
y_pred_reg = np.array([2.5, 0.0, 2.1, 8.0, 5.0])

print("="*60)
print("分类指标计算示例:")
print("="*60)

# 准确率
accuracy = accuracy_score(y_true_class, y_pred_class)
print(f"准确率 (Accuracy): {accuracy:.4f}")

# 精确率
precision = precision_score(y_true_class, y_pred_class)
print(f"精确率 (Precision): {precision:.4f}")

# 召回率
recall = recall_score(y_true_class, y_pred_class)
print(f"召回率 (Recall): {recall:.4f}")

# F1-Score
f1 = f1_score(y_true_class, y_pred_class)
print(f"F1-Score: {f1:.4f}")

# AUC-ROC
auc = roc_auc_score(y_true_class, y_pred_proba)
print(f"AUC-ROC: {auc:.4f}")

# 对数损失
logloss = log_loss(y_true_class, y_pred_proba)
print(f"对数损失 (Log Loss): {logloss:.4f}")

print("\\n" + "="*60)
print("回归指标计算示例:")
print("="*60)

# 均方误差
mse = mean_squared_error(y_true_reg, y_pred_reg)
print(f"均方误差 (MSE): {mse:.4f}")

# 均方根误差
rmse = np.sqrt(mse)
print(f"均方根误差 (RMSE): {rmse:.4f}")

# 平均绝对误差
mae = mean_absolute_error(y_true_reg, y_pred_reg)
print(f"平均绝对误差 (MAE): {mae:.4f}")

# R²决定系数
r2 = r2_score(y_true_reg, y_pred_reg)
print(f"R²决定系数: {r2:.4f}")

# 平均绝对百分比误差 (自定义实现)
def mean_absolute_percentage_error(y_true, y_pred):
    """计算平均绝对百分比误差"""
    y_true, y_pred = np.array(y_true), np.array(y_pred)
    # 避免除以0
    mask = y_true != 0
    return np.mean(np.abs((y_true[mask] - y_pred[mask]) / y_true[mask])) * 100

mape = mean_absolute_percentage_error(y_true_reg, y_pred_reg)
print(f"平均绝对百分比误差 (MAPE): {mape:.2f}%")

print("\\n" + "="*60)
print("指标选择建议:")
print("="*60)
print("1. 分类任务：")
print("   - 类别均衡：准确率 + F1-Score")
print("   - 类别不平衡：精确率 + 召回率 + F1-Score + AUC-ROC")
print("   - 概率模型评估：对数损失 + AUC-ROC")
print("\\n2. 回归任务：")
print("   - 一般评估：RMSE + MAE + R²")
print("   - 异常值多：MAE + R²")
print("   - 百分比评估：MAPE")
'''

    # 保存为Python文件
    filename = "ml_metrics_examples.py"
    with open(filename, 'w', encoding='utf-8') as f:
        f.write(code_content)
    
    print(f"\n已生成Python计算示例文件：{filename}")
    print("运行此文件查看各指标的计算示例：python ml_metrics_examples.py")
    
    return filename

if __name__ == "__main__":
    print("开始生成机器学习评价指标对照表...")
    print("-" * 60)
    
    # 生成基础对照表
    csv_file1, df1 = create_ml_metrics_csv()
    
    print("\n" + "-" * 60)
    
    # 生成详细对照表
    csv_file2, df2 = create_extended_ml_metrics_csv()
    
    print("\n" + "-" * 60)
    
    # 生成Python示例代码
    py_file = create_python_example_code()
    
    print("\n" + "=" * 60)
    print("生成完成！")
    print("=" * 60)
    print(f"生成的CSV文件：")
    print(f"1. {csv_file1} (基础对照表)")
    print(f"2. {csv_file2} (详细对照表)")
    print(f"生成的Python示例文件：{py_file}")
    print("\n使用建议：")
    print("1. CSV文件可用Excel或文本编辑器打开")
    print("2. 详细对照表包含更多技术细节和Python实现参考")
    print("3. Python示例文件展示了各指标的具体计算方法")