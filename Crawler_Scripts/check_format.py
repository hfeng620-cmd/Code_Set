"""
检查文件格式
"""
def check_file_format():
    """检查文件格式"""
    try:
        with open('D:\\VS_Code\\zhihu_final.txt', 'r', encoding='utf-8') as f:
            lines = f.readlines()
        
        print(f"文件总行数: {len(lines)}")
        
        # 检查长行
        long_lines = []
        for i, line in enumerate(lines):
            line_length = len(line.rstrip())
            if line_length > 80:
                long_lines.append((i+1, line_length, line[:50]))
        
        print(f"超过80字符的行数: {len(long_lines)}")
        
        if long_lines:
            print("前5个长行:")
            for i, length, preview in long_lines[:5]:
                print(f"  行{i}: {length}字符, 预览: {preview}...")
        else:
            print("所有行都在80字符以内，格式良好！")
        
        # 显示文件结构
        print("\n文件结构:")
        for i, line in enumerate(lines[:10]):
            print(f"行{i+1}: {len(line.rstrip())}字符 - {line[:50]}...")
        
        # 检查回答内容部分
        print("\n回答内容部分示例:")
        in_answer = False
        answer_lines = []
        for i, line in enumerate(lines):
            if '回答内容' in line:
                in_answer = True
                continue
            if in_answer and line.strip() and '===' not in line:
                answer_lines.append((i+1, line))
                if len(answer_lines) >= 5:
                    break
        
        for line_num, content in answer_lines:
            print(f"行{line_num}: {len(content.rstrip())}字符 - {content[:60]}...")
        
    except Exception as e:
        print(f"检查过程中发生错误: {e}")
        import traceback
        traceback.print_exc()

if __name__ == "__main__":
    check_file_format()