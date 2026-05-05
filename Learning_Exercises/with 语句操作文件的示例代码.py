"""
Python文件操作与机器学习指标对照表生成器
功能：1. with语句操作文件示例 2. 生成ML评价指标对照表CSV
"""

import os
import csv
import json
import time
import pandas as pd
import numpy as np
from datetime import datetime
import contextlib

# ============================================================================
# 第一部分：with语句操作文件示例代码
# ============================================================================

def demonstrate_with_statement():
    """演示with语句操作文件的完整示例"""
    
    print("=" * 70)
    print("Python with语句操作文件示例演示")
    print("=" * 70)
    
    # 创建示例文件
    create_example_files()
    
    # 1. 基本文件读取示例
    print("\n1. 基本文件读取示例:")
    basic_file_reading()
    
    # 2. 文件写入示例
    print("\n2. 文件写入示例:")
    file_writing_examples()
    
    # 3. 二进制文件操作
    print("\n3. 二进制文件操作示例:")
    binary_file_operations()
    
    # 4. 多个文件同时操作
    print("\n4. 多个文件同时操作示例:")
    multiple_file_operations()
    
    # 5. 异常处理
    print("\n5. 异常处理示例:")
    exception_handling_example()
    
    # 6. 上下文管理器高级应用
    print("\n6. 上下文管理器高级应用:")
    advanced_context_manager()
    
    # 7. CSV和JSON文件处理
    print("\n7. CSV和JSON文件处理:")
    csv_json_examples()
    
    # 8. 实用函数示例
    print("\n8. 实用函数示例:")
    utility_functions()
    
    # 清理临时文件
    cleanup_example_files()
    
    print("\n" + "=" * 70)
    print("所有示例演示完成!")
    print("=" * 70)

def create_example_files():
    """创建示例文件"""
    examples = [
        ("example.txt", "这是第一行\n这是第二行\n这是第三行\nPython文件操作示例\n机器学习\n人工智能"),
        ("data.txt", "100\n200\n300\n400\n500"),
        ("students.txt", "张三\n李四\n王五\n赵六"),
        ("scores.txt", "85\n92\n78\n88"),
    ]
    
    for filename, content in examples:
        with open(filename, 'w', encoding='utf-8') as f:
            f.write(content)

def basic_file_reading():
    """基本文件读取示例"""
    
    # 1.1 读取整个文件
    print("1.1 读取整个文件:")
    with open('example.txt', 'r', encoding='utf-8') as file:
        content = file.read()
        print(f"  文件内容长度: {len(content)} 字符")
        print(f"  前50个字符: {content[:50]}...")
    
    # 1.2 逐行读取
    print("\n1.2 逐行读取:")
    with open('example.txt', 'r', encoding='utf-8') as file:
        print("  文件内容:")
        for i, line in enumerate(file, 1):
            print(f"  第{i}行: {line.strip()}")
    
    # 1.3 读取为列表
    print("\n1.3 读取为列表:")
    with open('example.txt', 'r', encoding='utf-8') as file:
        lines = file.readlines()
        print(f"  文件共有 {len(lines)} 行")
        for i, line in enumerate(lines[:3], 1):
            print(f"  前3行第{i}行: {line.strip()}")

def file_writing_examples():
    """文件写入示例"""
    
    # 2.1 覆盖写入
    print("2.1 覆盖写入:")
    with open('output.txt', 'w', encoding='utf-8') as file:
        file.write("这是覆盖写入的第一行\n")
        file.write("这是第二行\n")
        file.write(f"当前时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
    print("  已创建 output.txt")
    
    # 2.2 追加写入
    print("\n2.2 追加写入:")
    with open('output.txt', 'a', encoding='utf-8') as file:
        file.write("这是追加的内容\n")
        file.write("文件操作完成\n")
    print("  已向 output.txt 追加内容")
    
    # 2.3 读取验证
    print("\n2.3 验证写入内容:")
    with open('output.txt', 'r', encoding='utf-8') as file:
        print("  output.txt 内容:")
        for line in file:
            print(f"  {line.strip()}")

def binary_file_operations():
    """二进制文件操作示例"""
    
    # 创建二进制文件示例
    print("3.1 创建和读取二进制文件:")
    binary_data = b'\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09'
    
    with open('binary_example.bin', 'wb') as file:
        file.write(binary_data)
    print("  已创建 binary_example.bin")
    
    with open('binary_example.bin', 'rb') as file:
        read_data = file.read()
        print(f"  读取的二进制数据: {read_data}")
        print(f"  数据长度: {len(read_data)} 字节")
    
    # 文本文件的二进制模式
    print("\n3.2 文本文件的二进制模式:")
    with open('example.txt', 'rb') as file:
        binary_content = file.read()
        print(f"  二进制读取 example.txt: 前50字节")
        print(f"  {binary_content[:50]}")

def multiple_file_operations():
    """多个文件同时操作示例"""
    
    print("4.1 同时打开多个文件:")
    with open('students.txt', 'r', encoding='utf-8') as students, \
         open('scores.txt', 'r', encoding='utf-8') as scores, \
         open('student_scores.txt', 'w', encoding='utf-8') as output:
        
        output.write("学生成绩报告\n")
        output.write("=" * 20 + "\n")
        
        for student_line, score_line in zip(students, scores):
            student = student_line.strip()
            score = score_line.strip()
            output.write(f"{student}: {score}分\n")
    
    print("  已创建 student_scores.txt")
    
    # 显示合并结果
    print("\n4.2 合并文件内容:")
    with open('student_scores.txt', 'r', encoding='utf-8') as file:
        for line in file:
            print(f"  {line.strip()}")

def exception_handling_example():
    """异常处理示例"""
    
    print("5.1 文件不存在异常:")
    try:
        with open('nonexistent.txt', 'r', encoding='utf-8') as file:
            content = file.read()
    except FileNotFoundError as e:
        print(f"  文件不存在错误: {e}")
    
    print("\n5.2 数据类型异常处理:")
    try:
        with open('data.txt', 'r', encoding='utf-8') as file:
            content = file.read()
            numbers = [int(line.strip()) for line in content.split('\n') if line.strip()]
            average = sum(numbers) / len(numbers)
            print(f"  数据平均值: {average:.2f}")
    except ValueError as e:
        print(f"  数据类型错误: {e}")
    except ZeroDivisionError:
        print("  错误: 数据文件为空")
    finally:
        print("  异常处理示例完成")

def advanced_context_manager():
    """上下文管理器高级应用"""
    
    # 自定义上下文管理器类
    class FileManager:
        """自定义文件管理器"""
        
        def __init__(self, filename, mode, encoding='utf-8'):
            self.filename = filename
            self.mode = mode
            self.encoding = encoding
            self.file = None
            self.start_time = None
        
        def __enter__(self):
            print(f"  打开文件: {self.filename}")
            self.start_time = time.time()
            self.file = open(self.filename, self.mode, encoding=self.encoding)
            return self.file
        
        def __exit__(self, exc_type, exc_val, exc_tb):
            if self.file:
                self.file.close()
            elapsed = time.time() - self.start_time
            print(f"  关闭文件: {self.filename}, 耗时: {elapsed:.4f}秒")
            if exc_type:
                print(f"  发生异常: {exc_type.__name__}: {exc_val}")
            return False  # 不抑制异常
    
    print("6.1 自定义上下文管理器:")
    with FileManager('example.txt', 'r') as file:
        content = file.read()
        print(f"  读取字符数: {len(content)}")
    
    # 使用contextlib创建上下文管理器
    print("\n6.2 使用contextlib创建上下文管理器:")
    
    @contextlib.contextmanager
    def timed_open(filename, mode, encoding='utf-8'):
        start = time.time()
        print(f"  开始处理: {filename}")
        file = open(filename, mode, encoding=encoding)
        try:
            yield file
        finally:
            file.close()
            elapsed = time.time() - start
            print(f"  处理完成: {filename}, 耗时: {elapsed:.4f}秒")
    
    with timed_open('example.txt', 'r') as file:
        lines = file.readlines()
        print(f"  行数: {len(lines)}")

def csv_json_examples():
    """CSV和JSON文件处理示例"""
    
    print("7.1 CSV文件操作:")
    
    # 创建CSV数据
    csv_data = [
        ['姓名', '年龄', '城市', '成绩'],
        ['张三', 25, '北京', 85.5],
        ['李四', 30, '上海', 92.0],
        ['王五', 28, '广州', 78.5],
        ['赵六', 22, '深圳', 88.0]
    ]
    
    # 写入CSV
    with open('students_data.csv', 'w', newline='', encoding='utf-8-sig') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerows(csv_data)
    print("  已创建 students_data.csv")
    
    # 读取CSV
    with open('students_data.csv', 'r', encoding='utf-8-sig') as csvfile:
        reader = csv.reader(csvfile)
        print("  CSV内容:")
        for row in reader:
            print(f"    {row}")
    
    print("\n7.2 JSON文件操作:")
    
    # JSON数据
    json_data = {
        "项目": "机器学习评价指标",
        "作者": "AI助手",
        "创建时间": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "分类指标": ["准确率", "精确率", "召回率", "F1-Score", "AUC-ROC"],
        "回归指标": ["MSE", "RMSE", "MAE", "R²", "MAPE"],
        "元数据": {
            "版本": "1.0",
            "更新": "2024"
        }
    }
    
    # 写入JSON
    with open('config.json', 'w', encoding='utf-8') as jsonfile:
        json.dump(json_data, jsonfile, ensure_ascii=False, indent=2)
    print("  已创建 config.json")
    
    # 读取JSON
    with open('config.json', 'r', encoding='utf-8') as jsonfile:
        loaded_data = json.load(jsonfile)
        print("  JSON内容摘要:")
        print(f"    项目: {loaded_data['项目']}")
        print(f"    分类指标数: {len(loaded_data['分类指标'])}")
        print(f"    回归指标数: {len(loaded_data['回归指标'])}")

def utility_functions():
    """实用函数示例"""
    
    print("8.1 统计文件信息:")
    
    def get_file_info(filename):
        """获取文件信息"""
        info = {}
        with open(filename, 'r', encoding='utf-8') as file:
            content = file.read()
            lines = content.split('\n')
            info['文件名'] = filename
            info['大小(字节)'] = len(content.encode('utf-8'))
            info['行数'] = len(lines)
            info['字符数'] = len(content)
            info['单词数'] = len(content.split())
        return info
    
    file_info = get_file_info('example.txt')
    for key, value in file_info.items():
        print(f"  {key}: {value}")
    
    print("\n8.2 文件搜索和替换:")
    
    def search_in_file(filename, keyword):
        """在文件中搜索关键词"""
        results = []
        with open(filename, 'r', encoding='utf-8') as file:
            for line_num, line in enumerate(file, 1):
                if keyword in line:
                    results.append((line_num, line.strip()))
        return results
    
    keyword = "Python"
    search_results = search_in_file('example.txt', keyword)
    if search_results:
        print(f"  在example.txt中找到'{keyword}':")
        for line_num, line in search_results:
            print(f"    第{line_num}行: {line}")
    else:
        print(f"  在example.txt中未找到'{keyword}'")

def cleanup_example_files():
    """清理示例文件"""
    files_to_cleanup = [
        'example.txt', 'data.txt', 'students.txt', 'scores.txt',
        'output.txt', 'binary_example.bin', 'student_scores.txt',
        'students_data.csv', 'config.json'
    ]
    
    for filename in files_to_cleanup:
        if os.path.exists(filename):
            os.remove(filename)

# ============================================================================
# 第二部分：机器学习评价指标对照表生成器
# ============================================================================

def generate_ml_metrics_csv():
    """生成机器学习评价指标对照表的CSV文件"""
    
    print("\n" + "=" * 70)
    print("生成机器学习评价指标对照表")
    print("=" * 70)
    
    # 准备数据
    metrics_data = [
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
    df = pd.DataFrame(metrics_data)
    
    # 生成文件名（带时间戳）
    timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
    filename = f"ml_evaluation_metrics_{timestamp}.csv"
    
    # 保存为CSV文件
    df.to_csv(filename, index=False, encoding='utf-8-sig')
    
    # 生成简化版本
    simple_filename = f"ml_metrics_simple_{timestamp}.csv"
    
    simple_data = [
        {
            "维度": "分类任务指标",
            "评价指标": "准确率",
            "主要任务": "分类",
            "核心思想/公式": "正确预测样本数/总样本数",
            "取值范围": "[0, 1]",
            "特点与适用场景": "最直观，但不适用于类别不平衡的数据集"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "精确率",
            "主要任务": "分类（正类）",
            "核心思想/公式": "TP/(TP+FP)",
            "取值范围": "[0, 1]",
            "特点与适用场景": "侧重降低误报，如垃圾邮件检测"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "召回率",
            "主要任务": "分类（正类）",
            "核心思想/公式": "TP/(TP+FN)",
            "取值范围": "[0, 1]",
            "特点与适用场景": "侧重降低漏报，如疾病诊断"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "F1-Score",
            "主要任务": "分类",
            "核心思想/公式": "2×精确率×召回率/(精确率+召回率)",
            "取值范围": "[0, 1]",
            "特点与适用场景": "精确率与召回率的调和平均数，寻求平衡"
        },
        {
            "维度": "分类任务指标",
            "评价指标": "AUC-ROC",
            "主要任务": "二分类",
            "核心思想/公式": "ROC曲线下的面积",
            "取值范围": "[0, 1]",
            "特点与适用场景": "模型综合排序能力，与阈值无关，对类别不平衡稳健"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "均方误差",
            "主要任务": "回归",
            "核心思想/公式": "1/n Σ(y_i-ŷ_i)²",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "最常用，放大较大误差的影响，对异常值敏感"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "均方根误差",
            "主要任务": "回归",
            "核心思想/公式": "√MSE",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "与预测目标量纲一致，解释性更强"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "平均绝对误差",
            "主要任务": "回归",
            "核心思想/公式": "1/n Σ|y_i-ŷ_i|",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "对异常值鲁棒性优于MSE/RMSE，解释性直观"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "R²决定系数",
            "主要任务": "回归",
            "核心思想/公式": "1 - Σ(y_i-ŷ_i)²/Σ(y_i-ȳ)²",
            "取值范围": "(-∞, 1]",
            "特点与适用场景": "衡量模型拟合优度，表示模型对方差的解释比例"
        },
        {
            "维度": "回归任务指标",
            "评价指标": "平均绝对百分比误差",
            "主要任务": "回归",
            "核心思想/公式": "100% × 1/n Σ|(y_i-ŷ_i)/y_i|",
            "取值范围": "[0, +∞)",
            "特点与适用场景": "相对误差，以百分比表示，适用于量纲不同的场景"
        }
    ]
    
    simple_df = pd.DataFrame(simple_data)
    simple_df.to_csv(simple_filename, index=False, encoding='utf-8-sig')
    
    print(f"\n已成功生成机器学习评价指标对照表:")
    print(f"1. 详细版本: {filename}")
    print(f"2. 简化版本: {simple_filename}")
    print(f"生成时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    print(f"文件位置: 当前工作目录 ({os.getcwd()})")
    print(f"详细版本指标数: {len(df)} 个")
    print(f"简化版本指标数: {len(simple_df)} 个")
    
    print("\n数据预览 (详细版本前5行):")
    print(df.head().to_string())
    
    print("\n使用说明:")
    print("1. CSV文件可用Excel、Numbers或文本编辑器打开")
    print("2. 详细版本包含更多技术细节和Python实现参考")
    print("3. 简化版本适合快速参考和打印")
    
    return filename, simple_filename, df, simple_df

def create_readme_file():
    """创建README文件，说明生成的文件内容"""
    
    readme_content = f"""# Python文件操作与机器学习指标对照表

## 文件说明

### 1. Python脚本 (当前文件)
- 文件名: file_operations_and_ml_metrics.py
- 功能: 
  - 演示Python中with语句操作文件的各种用法
  - 生成机器学习分类与回归任务评价指标对照表

### 2. 生成的CSV文件
- ml_evaluation_metrics_YYYYMMDD_HHMMSS.csv: 详细版评价指标对照表
- ml_metrics_simple_YYYYMMDD_HHMMSS.csv: 简化版评价指标对照表

### 3. 文件内容说明

#### 详细版CSV包含以下字段:
1. 任务类型: 分类/回归
2. 评价指标: 指标中文名称
3. 英文名称: 指标英文名称
4. 核心公式: 指标的数学公式
5. 取值范围: 指标的值域范围
6. 最佳值: 最优值
7. 最差值: 最差值
8. Python函数: scikit-learn中对应的函数
9. 适用场景: 具体使用场景
10. 注意事项: 使用时的注意事项

#### 简化版CSV包含以下字段:
1. 维度: 分类任务指标/回归任务指标
2. 评价指标: 指标名称
3. 主要任务: 适用的任务类型
4. 核心思想/公式: 指标的数学公式或核心思想
5. 取值范围: 指标的值域范围
6. 特点与适用场景: 简要说明指标的特点和使用场景

## with语句文件操作示例

本脚本演示了Python中with语句操作文件的多种用法，包括:
1. 基本文件读取 (读整个文件、逐行读取、读取为列表)
2. 文件写入 (覆盖写入、追加写入)
3. 二进制文件操作
4. 多个文件同时操作
5. 异常处理
6. 上下文管理器高级应用
7. CSV和JSON文件处理
8. 实用函数示例

## 运行说明

1. 确保已安装必要的Python库:
   ```bash
   pip install pandas numpy