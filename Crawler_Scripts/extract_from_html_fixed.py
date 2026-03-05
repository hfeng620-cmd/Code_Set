"""
从已保存的HTML文件中提取知乎内容 - 修复编码问题
"""
from bs4 import BeautifulSoup
import json
import re
from datetime import datetime
import chardet

def detect_encoding(file_path: str) -> str:
    """检测文件编码"""
    with open(file_path, 'rb') as f:
        raw_data = f.read()
    result = chardet.detect(raw_data)
    return result['encoding']

def extract_from_html_file(html_file: str) -> dict:
    """
    从HTML文件中提取知乎内容
    
    Args:
        html_file: HTML文件路径
        
    Returns:
        dict: 包含提取内容的字典
    """
    try:
        print(f"正在从文件 {html_file} 中提取内容...")
        
        # 检测文件编码
        encoding = detect_encoding(html_file)
        print(f"检测到文件编码: {encoding}")
        
        with open(html_file, 'r', encoding=encoding, errors='ignore') as f:
            html_content = f.read()
        
        print(f"文件大小: {len(html_content)} 字符")
        
        soup = BeautifulSoup(html_content, 'html.parser')
        
        # 提取问题标题
        question_title = ""
        title_selectors = [
            'h1.QuestionHeader-title',
            'h1[data-za-detail-view-element_name="Title"]',
            '.QuestionHeader-main h1',
            'h1'
        ]
        
        for selector in title_selectors:
            element = soup.select_one(selector)
            if element:
                question_title = element.get_text(strip=True)
                print(f"找到问题标题: {question_title}")
                break
        
        # 提取问题描述
        question_description = ""
        desc_selectors = [
            '.QuestionRichText .RichText',
            '.QuestionHeader-detail .RichText',
            '.QuestionHeader-detail',
            '.QuestionHeader .RichText'
        ]
        
        for selector in desc_selectors:
            element = soup.select_one(selector)
            if element:
                question_description = element.get_text(strip=True)
                print(f"找到问题描述: {question_description[:100]}...")
                break
        
        # 提取回答
        answers = soup.select('.List .List-item, .AnswerItem, .ContentItem.AnswerItem')
        print(f"找到 {len(answers)} 个回答")
        
        target_author = ""
        target_content = ""
        
        # 查找作者为"跑酱er"的回答
        for answer in answers:
            # 查找作者信息
            author_element = answer.select_one('.AuthorInfo .AuthorInfo-name, .AuthorInfo-name, .UserLink.AuthorInfo-name')
            if author_element:
                author = author_element.get_text(strip=True)
                if "跑酱er" in author:
                    target_author = author
                    print(f"找到目标作者: {target_author}")
                    
                    # 提取回答内容
                    content_element = answer.select_one('.RichContent .RichText, .RichContent-inner, .AnswerItem .RichText')
                    if content_element:
                        target_content = content_element.get_text(strip=True)
                        print(f"找到回答内容，长度: {len(target_content)} 字符")
                        break
        
        # 如果没找到特定作者，取第一个回答
        if not target_content and answers:
            print("未找到'跑酱er'的回答，取第一个回答")
            first_answer = answers[0]
            
            # 获取作者
            author_element = first_answer.select_one('.AuthorInfo .AuthorInfo-name')
            if author_element:
                target_author = author_element.get_text(strip=True)
                print(f"第一个回答作者: {target_author}")
            
            # 获取内容
            content_element = first_answer.select_one('.RichContent .RichText')
            if content_element:
                target_content = content_element.get_text(strip=True)
                print(f"第一个回答内容，长度: {len(target_content)} 字符")
        
        # 提取回答时间
        answer_time = ""
        time_element = soup.select_one('.ContentItem-time, .AnswerItem .ContentItem-time')
        if time_element:
            answer_time = time_element.get_text(strip=True)
            print(f"回答时间: {answer_time}")
        
        # 提取点赞数
        upvote_count = ""
        upvote_element = soup.select_one('.VoteButton--up, .AnswerItem .VoteButton')
        if upvote_element:
            upvote_count = upvote_element.get_text(strip=True)
            print(f"点赞数: {upvote_count}")
        
        # 构建结果
        result = {
            "url": "https://www.zhihu.com/question/412311476/answer/1995150376387748664?utm_psn=1998670171783569760",
            "title": question_title,
            "description": question_description,
            "answer": {
                "author": target_author,
                "content": target_content,
                "time": answer_time,
                "upvotes": upvote_count
            },
            "fetch_time": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
            "source": "html_file"
        }
        
        return result
        
    except Exception as e:
        print(f"提取过程中发生错误: {e}")
        import traceback
        traceback.print_exc()
        return None

def save_to_markdown(data: dict, output_path: str):
    """保存为Markdown格式"""
    if not data:
        print("没有数据可保存")
        return
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write(f"# {data['title']}\n\n")
        f.write(f"**来源**: [知乎链接]({data['url']})\n\n")
        f.write(f"**抓取时间**: {data['fetch_time']}\n\n")
        
        if data['description']:
            f.write("## 问题描述\n\n")
            f.write(f"{data['description']}\n\n")
        
        f.write("## 回答\n\n")
        f.write(f"**作者**: {data['answer']['author']}\n\n")
        
        if data['answer']['time']:
            f.write(f"**回答时间**: {data['answer']['time']}\n\n")
        
        if data['answer']['upvotes']:
            f.write(f"**点赞数**: {data['answer']['upvotes']}\n\n")
        
        f.write("### 回答内容\n\n")
        f.write(f"{data['answer']['content']}\n\n")
        
        f.write("---\n\n")
        f.write("*本文件由知乎内容提取工具自动生成*\n")
    
    print(f"已保存到Markdown文件: {output_path}")

def save_to_html(data: dict, output_path: str):
    """保存为HTML格式"""
    if not data:
        print("没有数据可保存")
        return
    
    with open(output_path, 'w', encoding='utf-8') as f:
        f.write("""<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>知乎内容提取 - {title}</title>
    <style>
        body {{ font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'PingFang SC', 'Hiragino Sans GB', 'Microsoft YaHei', 'Helvetica Neue', Helvetica, Arial, sans-serif; line-height: 1.6; color: #1a1a1a; max-width: 800px; margin: 0 auto; padding: 20px; }}
        h1 {{ color: #0084ff; border-bottom: 1px solid #ebebeb; padding-bottom: 10px; }}
        h2 {{ color: #262626; margin-top: 30px; }}
        h3 {{ color: #595959; }}
        .meta {{ color: #8590a6; font-size: 14px; margin-bottom: 20px; }}
        .answer {{ background-color: #f6f6f6; padding: 20px; border-radius: 8px; margin: 20px 0; }}
        .author {{ color: #175199; font-weight: bold; }}
        .content {{ white-space: pre-wrap; word-wrap: break-word; }}
        .footer {{ margin-top: 40px; padding-top: 20px; border-top: 1px solid #ebebeb; color: #8590a6; font-size: 12px; text-align: center; }}
    </style>
</head>
<body>
""".format(title=data['title']))
        
        f.write(f"    <h1>{data['title']}</h1>\n")
        f.write(f'    <div class="meta">\n')
        f.write(f'        <p><strong>来源</strong>: <a href="{data["url"]}" target="_blank">知乎链接</a></p>\n')
        f.write(f'        <p><strong>抓取时间</strong>: {data["fetch_time"]}</p>\n')
        f.write(f'    </div>\n')
        
        if data['description']:
            f.write(f'    <h2>问题描述</h2>\n')
            f.write(f'    <div class="content">{data["description"]}</div>\n')
        
        f.write(f'    <h2>回答</h2>\n')
        f.write(f'    <div class="answer">\n')
        f.write(f'        <div class="author">{data["answer"]["author"]}</div>\n')
        
        if data['answer']['time']:
            f.write(f'        <div class="meta">回答时间: {data["answer"]["time"]}</div>\n')
        
        if data['answer']['upvotes']:
            f.write(f'        <div class="meta">点赞数: {data["answer"]["upvotes"]}</div>\n')
        
        f.write(f'        <h3>回答内容</h3>\n')
        f.write(f'        <div class="content">{data["answer"]["content"]}</div>\n')
        f.write(f'    </div>\n')
        
        f.write("""    <div class="footer">
        本文件由知乎内容提取工具自动生成
    </div>
</body>
</html>""")
    
    print(f"已保存到HTML文件: {output_path}")

def main():
    """主函数"""
    html_file = "zhihu_raw.html"
    
    print("开始从HTML文件中提取知乎内容...")
    data = extract_from_html_file(html_file)
    
    if data:
        print("\n提取成功！")
        print(f"问题标题: {data['title']}")
        print(f"回答作者: {data['answer']['author']}")
        print(f"回答内容长度: {len(data['answer']['content'])} 字符")
        
        # 保存文件
        save_to_markdown(data, "D:\\VS_Code\\zhihu_content_fixed.md")
        save_to_html(data, "D:\\VS_Code\\zhihu_content_fixed.html")
        
        # 保存JSON
        with open("D:\\VS_Code\\zhihu_content_fixed.json", 'w', encoding='utf-8') as f:
            json.dump(data, f, ensure_ascii=False, indent=2)
        print("已保存到JSON文件: D:\\VS_Code\\zhihu_content_fixed.json")
        
        print("\n所有文件已保存到 D:\\VS_Code 目录")
    else:
        print("未能提取内容，请检查HTML文件结构")

if __name__ == "__main__":
    main()