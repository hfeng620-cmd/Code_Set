"""
格式化文本文件 - 控制每行字数
"""
import textwrap

def format_text_file(input_file: str, output_file: str, width: int = 80):
    """
    格式化文本文件，控制每行字数
    
    Args:
        input_file: 输入文件路径
        output_file: 输出文件路径
        width: 每行最大字符数，默认80
    """
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        print(f"读取文件: {input_file}")
        print(f"原始内容长度: {len(content)} 字符")
        
        # 分割内容为不同部分
        lines = content.split('\n')
        formatted_lines = []
        
        for line in lines:
            if len(line.strip()) == 0:
                # 空行保留
                formatted_lines.append('')
            elif len(line) <= width:
                # 短行直接保留
                formatted_lines.append(line)
            else:
                # 长行需要换行
                # 检查是否是回答内容部分
                if line.startswith('这没什么不好承认的') or '。' in line or '，' in line:
                    # 这是回答内容，按中文标点进行换行
                    wrapped_lines = wrap_chinese_text(line, width)
                    formatted_lines.extend(wrapped_lines)
                else:
                    # 其他长行按普通方式换行
                    wrapped_lines = textwrap.wrap(line, width)
                    formatted_lines.extend(wrapped_lines)
        
        # 写入新文件
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write('\n'.join(formatted_lines))
        
        print(f"已保存格式化文件: {output_file}")
        print(f"每行最大字符数: {width}")
        
    except Exception as e:
        print(f"格式化过程中发生错误: {e}")
        import traceback
        traceback.print_exc()

def wrap_chinese_text(text: str, width: int) -> list:
    """
    包装中文文本，考虑中文标点
    
    Args:
        text: 中文文本
        width: 每行最大字符数
    
    Returns:
        list: 换行后的文本列表
    """
    result = []
    current_line = ""
    
    for char in text:
        current_line += char
        
        # 检查是否需要换行
        if len(current_line) >= width:
            # 尝试在标点处换行
            last_punctuation = -1
            for i in range(len(current_line)-1, -1, -1):
                if current_line[i] in '。，；！？、,.;!?':
                    last_punctuation = i
                    break
            
            if last_punctuation != -1 and last_punctuation > width * 0.7:
                # 在标点处换行
                result.append(current_line[:last_punctuation+1])
                current_line = current_line[last_punctuation+1:]
            else:
                # 强制换行
                result.append(current_line)
                current_line = ""
    
    if current_line:
        result.append(current_line)
    
    return result

def main():
    """主函数"""
    input_file = "D:\\VS_Code\\zhihu_final.txt"
    output_file = "D:\\VS_Code\\zhihu_formatted.txt"
    
    print("开始格式化文本文件...")
    print(f"输入文件: {input_file}")
    print(f"输出文件: {output_file}")
    
    # 格式化文件，每行80个字符
    format_text_file(input_file, output_file, width=80)
    
    print("\n格式化完成！")

if __name__ == "__main__":
    main()