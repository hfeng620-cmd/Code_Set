"""
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

print("\n" + "="*60)
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

print("\n" + "="*60)
print("指标选择建议:")
print("="*60)
print("1. 分类任务：")
print("   - 类别均衡：准确率 + F1-Score")
print("   - 类别不平衡：精确率 + 召回率 + F1-Score + AUC-ROC")
print("   - 概率模型评估：对数损失 + AUC-ROC")
print("\n2. 回归任务：")
print("   - 一般评估：RMSE + MAE + R²")
print("   - 异常值多：MAE + R²")
print("   - 百分比评估：MAPE")
