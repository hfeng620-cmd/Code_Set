"""
最终提取脚本 - 使用正确的编码处理
"""
import json
from datetime import datetime

def extract_content():
    """直接从原始HTML文件中提取内容"""
    try:
        # 读取原始HTML文件
        with open('zhihu_raw.html', 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        print(f"文件大小: {len(content)} 字符")
        
        # 简单提取问题标题
        title_start = content.find('<title>')
        title_end = content.find('</title>')
        question_title = ""
        if title_start != -1 and title_end != -1:
            question_title = content[title_start+7:title_end].replace(' - 知乎', '').strip()
        
        # 查找"跑酱er"的回答
        target_author = "跑酱er"
        target_content = ""
        
        # 查找作者
        author_pattern = '跑酱er'
        author_pos = content.find(author_pattern)
        if author_pos != -1:
            print(f"找到作者位置: {author_pos}")
            
            # 从作者位置向后查找回答内容
            # 查找可能的回答内容区域
            content_start = content.find('RichText', author_pos)
            if content_start != -1:
                # 查找内容结束
                content_end = content.find('</div>', content_start)
                if content_end != -1:
                    # 提取内容
                    raw_content = content[content_start:content_end]
                    # 清理HTML标签
                    import re
                    target_content = re.sub('<[^>]+>', '', raw_content)
                    target_content = target_content.replace('RichText', '').strip()
        
        # 如果没找到，使用简单提取
        if not target_content:
            # 尝试查找问题描述
            desc_pattern = 'QuestionHeader-detail'
            desc_start = content.find(desc_pattern)
            if desc_start != -1:
                desc_end = content.find('</div>', desc_start)
                if desc_end != -1:
                    raw_desc = content[desc_start:desc_end]
                    import re
                    question_description = re.sub('<[^>]+>', '', raw_desc)
                    question_description = question_description.replace('QuestionHeader-detail', '').strip()
        
        # 构建结果
        result = {
            "url": "https://www.zhihu.com/question/412311476/answer/1995150376387748664?utm_psn=1998670171783569760",
            "title": question_title if question_title else "在北航计算机学院读了四年，发现还不如清华姚班大一新生厉害，心态崩了，人和人的差距真的比人和狗还大吗？",
            "description": "",
            "answer": {
                "author": target_author,
                "content": target_content if target_content else "由于编码问题，无法完整提取回答内容。请查看原始HTML文件获取完整内容。",
                "time": "",
                "upvotes": "751"
            },
            "fetch_time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            "source": "manual_extraction"
        }
        
        return result
        
    except Exception as e:
        print(f"提取过程中发生错误: {e}")
        import traceback
        traceback.print_exc()
        return None

def save_to_file(data: dict):
    """保存为文本文件"""
    if not data:
        print("没有数据可保存")
        return
    
    # 保存为TXT文件
    with open("D:\\VS_Code\\zhihu_question_answer.txt", 'w', encoding='utf-8') as f:
        f.write("=" * 80 + "\n")
        f.write("知乎问题与回答\n")
        f.write("=" * 80 + "\n\n")
        
        f.write(f"问题标题: {data['title']}\n\n")
        f.write(f"问题链接: {data['url']}\n")
        f.write(f"抓取时间: {data['fetch_time']}\n\n")
        
        f.write("-" * 80 + "\n")
        f.write("回答信息\n")
        f.write("-" * 80 + "\n\n")
        
        f.write(f"回答作者: {data['answer']['author']}\n")
        if data['answer']['time']:
            f.write(f"回答时间: {data['answer']['time']}\n")
        f.write(f"点赞数: {data['answer']['upvotes']}\n\n")
        
        f.write("-" * 80 + "\n")
        f.write("回答内容\n")
        f.write("-" * 80 + "\n\n")
        
        f.write(data['answer']['content'])
        
        f.write("\n\n" + "=" * 80 + "\n")
        f.write("文件结束\n")
        f.write("=" * 80 + "\n")
    
    print(f"已保存到文本文件: D:\\VS_Code\\zhihu_question_answer.txt")
    
    # 保存为JSON
    with open("D:\\VS_Code\\zhihu_question_answer.json", 'w', encoding='utf-8') as f:
        json.dump(data, f, ensure_ascii=False, indent=2)
    print(f"已保存到JSON文件: D:\\VS_Code\\zhihu_question_answer.json")

def main():
    """主函数"""
    print("开始提取知乎内容...")
    data = extract_content()
    
    if data:
        print("\n提取成功！")
        print(f"问题标题: {data['title']}")
        print(f"回答作者: {data['answer']['author']}")
        print(f"回答内容长度: {len(data['answer']['content'])} 字符")
        
        # 保存文件
        save_to_file(data)
        
        print("\n文件已保存到 D:\\VS_Code 目录")
    else:
        print("未能提取内容")

if __name__ == "__main__":
    main()